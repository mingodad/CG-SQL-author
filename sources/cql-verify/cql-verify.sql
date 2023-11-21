/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* This file generates cql-verify.h and cql-verify.c */

@echo c, '
//
// This file is auto-generated by cql-verify.sql, it is checked in just
// in case CQL is broken by a change.  The Last Known Good Verifier
// can be used to verify the tests pass again, or report failures
// while things are still otherwise broken.  Rebuild with regen.sh
//

// enable detailed error tracing
#undef cql_error_trace
#define cql_error_trace() fprintf(stderr, "SQL Failure %d %s: %s %d\n", _rc_, sqlite3_errmsg(_db_), __FILE__, __LINE__)
';

declare proc printf no check;

declare function cql_fopen(name text!, mode text!) create object<file>;
declare function readline_object_file(f object<file>!) create text;
declare function atoi_at_text(str text, `offset` int!) int!;
declare function len_text(self text) int!;
declare function octet_text(self text, `offset` int!) int!;
declare function after_text(self text, `offset` int!) create text;
declare function starts_with_text(haystack text!, needle text!) bool!;
declare function index_of_text(haystack text!, needle text!) int!;
declare function contains_at_text(haystack text!, needle text!, `offset` int!) bool!;

var sql_name text;
var result_name text;
var attempts int!;
var errors int!;
var tests int!;
var last_rowid long!;

-- setup the table and the index
[[private]]
proc setup()
begin
  -- this is for the test results
  create table test_results(
     line int!,
     data text!
  );

  create index __idx__test_results on test_results (line);

  -- this is for the test cases
  create table test_input(
     line int!,
     data text!
  );

  create index __idx__test_input on test_input (line);
end;

-- find the line number of the statement that came after line_
[[private]]
proc find_test_output_line(expectation_line int!, out test_output_line int!)
begin
  /* the pattern match line is before the statement that generates the output like so:

     1:  -- TEST : something
     2:  -- + foo                           <-- we have this line number, e.g. 2
     3:  -- - bar
     4:  select something where something;  <-- we need this line number, e.g. 4

     We need to find the number of a line in the test output that has been charged
     to an input line greater than the one we are on.
  */
  begin try
    set test_output_line := (select line from test_results where line >= expectation_line limit 1);
  end try;
  begin catch
    printf("no lines come after %d\n", expectation_line);
    printf("available test output lines: %d\n", (select count(*) from test_results));
    printf("max line number: %d\n", (select max(line) from test_results));
    throw;
  end catch;
end;

-- find the next match, the matches have to be in order
[[private]]
proc find_next(pattern text!, test_output_line int!, out found int!)
begin
  -- once we have it, search for matches on that line and return the number we found
  declare C cursor for
    select rowid
      from test_results
      where line = test_output_line and data like ("%" || pattern || "%") and rowid > last_rowid;

  fetch C;
  if C then
    set last_rowid := C.rowid;
    set found := 1;
  else
    set found := 0;
  end if;
end;

-- search for a match on the same rowid as the last match we found
[[private]]
proc find_same(pattern text!, out found int!)
begin
  set found := (
    select data like ("%" || pattern || "%")
      from test_results
      where rowid = last_rowid
      if nothing false);
end;


-- find the statement that came after line_
-- search the results of that statement for the indicated pattern
[[private]]
proc find_count(pattern text!, test_output_line int!, out found int!)
begin
  -- once we have it, search for matches on that line and return the number we found
  set found := (select count(*) from test_results where line = test_output_line and data like ("%" || pattern || "%"));
end;

-- This code uses The Price Is Right algorithm, i.e.,  get
-- as close to the line number specified without going over.
-- Or in this case, actually staying under.  We need this to
-- find the end of the test before the one we are
-- working on.
[[private]]
proc prev_line(test_output_line int!, out prev int!)
begin
  set prev := (
    select line
    from test_results
    where line < test_output_line
    order by line desc
    limit 1
    if nothing 0);
end;

-- dump all of the input lines starting from line1 up to but not including line2
[[private]]
proc dump_source(line1 int!, line2 int!, current_line int!)
begin
  declare C cursor for
    select line, data
      from test_input
      where line > line1 and line <= line2;

  loop fetch C
  begin
    printf("%5s %05d: %s\n",
      case when C.line == current_line then "FAIL" else "" end,
      C.line,
      C.data);
  end;
end;

-- Dump the test output for the test that just failed
-- the line number in the test_results table is exactly
-- the test we need.
[[private]]
proc dump_output(test_output_line int!, pat text!)
begin
  let p := (select "%" || pat || "%");
  declare C cursor for
    select rowid, line, data
    from test_results where line = test_output_line;
  loop fetch C
  begin
    printf(
      "%3s%s\n",
      case when last_rowid == C.rowid
           then ">  "
           when C.data like p then "!  "
           else "" end,
      C.data
    );
  end;
end;

-- Give some helpful text that describes why the expectation failed
-- Tthis is a bunch of string literals so it's easier to look at the
-- code than explain it in english.
[[private]]
proc print_fail_details(pat text!, test_output_line int!, expected int!)
begin
  let found := find_count(pat, test_output_line);

  let details := case
    when expected == -2 then
      case when found > 0 then
        "pattern found but not on the same line (see lines marked with !)"
      else
        "pattern exists nowhere in test output"
      end
    when expected == -1 then
      case when found > 0 then
        "pattern exists but only earlier in the results where + doesn't match it"
      else
        "pattern exists nowhere in test output"
      end
    else
      printf("pattern occurrences found: %d, expecting: %d (see lines marked with !)",
        found, expected)
    end;

  printf("\n%s\n\n", details);
end;

-- If an error is found, we need to print a diagnostic message to help fix it
-- Here is where we do that.  The output is going to be:
-- * the actual results of the test
--   * we add some helpful markers in that output so you can see how the matching had progressed
-- * the test case
--   * we emit the line numbers and mark the failed line
[[private]]
proc print_error_block(test_output_line int!, pat text!, expectation_line int!, expected int!)
begin
  printf("test results:\n");

  -- dump all the text associated with this expectation_line (this could be many lines)
  -- it's all the output associated with this test case
  dump_output(test_output_line, pat);

  printf("Line Markings:\n");
  printf("> : the location of the last successful + match.\n");
  printf("! : any lines that match the pattern; count or location is wrong.\n\n");

  -- find the line that ended the previous test block
  let prev := prev_line(test_output_line);

  -- dump everything from there to here, that's the test case
  printf("\nThe corresponding test case is:\n");
  dump_source(prev, test_output_line, expectation_line);

  print_fail_details(pat, expectation_line, expected);
end;

-- looks for lines of the form "-- +1"  -- any single digit
[[private]]
proc match_multiline(buffer text!, out result bool!)
begin
  result := false;

  if buffer::len() < 7 return;
  if not buffer::starts_with("-- +") return;
  let digit := buffer::octet(4);
  let space := buffer::octet(5);
  if space != 32 return;
  if digit < 48 or digit > 48+9 return;

  result := true;
end;

-- Given a line in the test input file do the matching that it demands if any
-- match_actual is so called because it really does the match.  The match
-- function is just a try/catch wrapper.
proc match_actual(buffer text!, expectation_line int!)
begin
  var found int!;
  var expected int!;
  var pattern text;

  -- the comments encode the matches, early out if that fails
  if not buffer::starts_with("-- ") then
    -- lines can be out of order or re-visited
    -- we don't want a segment of rows that is later in the file
    -- to prevent later matching.  This can happen with
    -- # line directives in the stream and it does
    last_rowid := 0;
    return;
  end if;

  -- the standard test prefix it just counts tests, this doesn't mean anything
  -- but it's a useful statistic
  if buffer::starts_with("-- TEST:") then
    tests += 1;
  end if;

  if buffer::starts_with("-- - ") then
    -- found -- - foo
    -- negation, none expected
    pattern := buffer::after(5);
    expected := 0;
  else if buffer::starts_with("-- * ") then
    -- -- * foo
    -- at least one is expected, any number will do, any buffer order
    pattern := buffer::after(5);
    expected := 1;
  else if buffer::starts_with("-- + ") then
    -- -- + foo
    -- at least one is expected, matches have to be in order!
    pattern := buffer::after(5);
    expected := -1;
  else if buffer::starts_with("-- = ") then
    -- -- + foo
    -- at least one is expected, matches have to be in order!
    pattern := buffer::after(5);
    expected := -2;
  else if match_multiline(buffer) then
    -- -- +7 foo
    -- an exact match (single digit matches)
    pattern := buffer::after(6);
    expected := buffer::octet(4) - 48;
  else
    -- any other line is just a normal comment, ignore it
    return;
  end if;

  attempts += 1;

  let pat := ifnull_throw(pattern);

  let test_output_line := find_test_output_line(expectation_line);

  if expected == -1 then
    found := find_next(pat, test_output_line);
    if found == 1 return;
  else if expected == -2 then
    found := find_same(pat);
    if found == 1 return;
  else
    -- search among all the matching lines
    found := find_count(pat, test_output_line);
    if expected == found return;
  end if;

  -- print error corresponding to the pattern
  errors += 1;

  print_error_block(test_output_line, pat, expectation_line, expected);

  printf("test file %s:%d\n", sql_name, expectation_line);
  printf("result file: %s\n", result_name);
  printf("\n");
end;

[[private]]
proc do_match(buffer text!, expectation_line int!)
begin
  begin try
     match_actual(buffer, expectation_line);
  end try;
  begin catch
    printf("unexpected sqlite error\n");
    throw;
  end catch;
end;

[[private]]
create proc process()
begin
  -- this procedure gets us all of the lines and the data on those lines in order
  declare C cursor for select * from test_input;

  -- get the count of rows (lines) and start looping
  loop fetch C
  begin
    do_match(C.data, C.line);
  end;

  printf("Verification results: %d tests matched %d patterns of which %d were errors.\n", tests, attempts, errors);
end;

-- first we read the test results, we're looking for sentinel lines
-- that tell us where in the input these results came from
-- each result will be charged to the input line it is associated
-- with.  The key_string introduces those lines
[[private]]
proc read_test_results(result_name text!)
begin
  let result_file := cql_fopen(result_name, "r");
  if result_file is null then
    printf("unable to open file '%s'\n", result_name);
    throw;
  end if;

  let line := 0;

  let key_string := "The statement ending at line ";

  let len := key_string::len();

  while true
  begin
    let data := result_file:::readline();
    if data is null leave;

    -- lines in the output that start with the key_string demark
    -- output that corresponds to the given input line

    let loc := data::index_of(key_string);

    if loc >= 0 then
      line := data::atoi_at(loc + len);
    end if;

    -- add the indicated text to the database indexed by the line it was on
    insert into test_results values (line, data);
  end;
end;

-- now we're going to read the entire test file and store it in
-- the database indexed by line.  We're going to do this so that
-- we can go backwards for forwards in the lines easily using
-- the database.  We can select ranges of lines, that sort of thing.
[[private]]
proc read_test_file(sql_name text!)
begin
  let sql_file := cql_fopen(sql_name, "r");
  if sql_file is null then
    printf("unable to open file '%s'\n", sql_name);
    throw;
  end if;

  let line := 1;

  while true
  begin
    let data := sql_file:::readline();
    if data is null leave;

    insert into test_input values (line, data);
    line += 1;
  end;
end;

[[private]]
proc load_data(sql_name text!, result_name text!)
begin
  read_test_results(result_name);
  read_test_file(sql_name);
end;

[[private]]
proc parse_args(args cql_string_list!)
begin
  let argc := args.count;

  if argc != 3 then
    printf("usage cql-verify foo.sql foo.out\n");
    printf("cql-verify is a test tool.  It processes the input foo.sql\n");
    printf("looking for patterns to match in the CQL output foo.out\n");
    return;
  end if;

  -- store the test and output file names
  set sql_name := ifnull_throw(args[1]);
  set result_name := ifnull_throw(args[2]);
end;

-- main entry point
proc dbhelp_main(args cql_string_list!)
begin
  setup();
  parse_args(args);

  if sql_name is not null and result_name is not null then
    load_data(sql_name, result_name);
    process();
  end if;
end;

@echo c, '
#include "cqlhelp.h"

// super cheesy error handling

#define E(x) \
  if (SQLITE_OK != (x)) { \
   fprintf(stderr, "error encountered at: %s (%s:%d)\n", #x, __FILE__, __LINE__); \
   fprintf(stderr, "sqlite3_errmsg: %s\n", sqlite3_errmsg(db)); \
   errors = -1; \
   goto error; \
  }

int main(int argc, char **argv) {
  cql_object_ref args = create_arglist(argc, argv);

  sqlite3 *db = NULL;
  E(sqlite3_open(":memory:", &db));
  E(dbhelp_main(db, args));

error:
  if (db) sqlite3_close(db);
  cql_object_release(args);
  exit(errors ? 1 : 0);
}
';
