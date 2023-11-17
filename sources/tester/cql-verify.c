/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "cql-verify.h"

#ifndef _MSC_VER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wbitwise-op-parentheses"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#pragma clang diagnostic ignored "-Wlogical-not-parentheses"
#pragma clang diagnostic ignored "-Wlogical-op-parentheses"
#pragma clang diagnostic ignored "-Wliteral-conversion"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#pragma clang diagnostic ignored "-Wunused-function"
#endif
extern cql_object_ref _Nonnull cql_partition_create(void);
extern cql_bool cql_partition_cursor(cql_object_ref _Nonnull p, cql_dynamic_cursor *_Nonnull key, cql_dynamic_cursor *_Nonnull value);
extern cql_object_ref _Nonnull cql_extract_partition(cql_object_ref _Nonnull p, cql_dynamic_cursor *_Nonnull key);
extern cql_object_ref _Nonnull cql_string_dictionary_create(void);
extern cql_bool cql_string_dictionary_add(cql_object_ref _Nonnull dict, cql_string_ref _Nonnull key, cql_string_ref _Nonnull value);
extern cql_string_ref _Nullable cql_string_dictionary_find(cql_object_ref _Nonnull dict, cql_string_ref _Nullable key);
extern cql_string_ref _Nonnull cql_cursor_format(cql_dynamic_cursor *_Nonnull C);
extern cql_object_ref _Nonnull create_cql_string_list(void);
extern cql_object_ref _Nonnull set_in_object_cql_string_list(cql_object_ref _Nonnull list, cql_int32 index_, cql_string_ref _Nonnull value_);
extern cql_string_ref _Nullable get_from_object_cql_string_list(cql_object_ref _Nonnull list, cql_int32 index_);
extern cql_int32 get_object_cql_string_list_count(cql_object_ref _Nonnull list);
extern cql_object_ref _Nonnull add_object_cql_string_list(cql_object_ref _Nonnull list, cql_string_ref _Nonnull string);
extern cql_object_ref _Nullable cql_fopen(cql_string_ref _Nonnull name, cql_string_ref _Nonnull mode);
extern cql_string_ref _Nullable readline_object_file(cql_object_ref _Nonnull f);
extern cql_int32 atoi_at_text(cql_string_ref _Nullable str, cql_int32 offset);
extern cql_int32 len_text(cql_string_ref _Nullable self);
extern cql_int32 octet_text(cql_string_ref _Nullable self, cql_int32 offset);
extern cql_string_ref _Nullable after_text(cql_string_ref _Nullable self, cql_int32 offset);
extern cql_bool starts_with_text(cql_string_ref _Nonnull haystack, cql_string_ref _Nonnull needle);
extern cql_int32 index_of_text(cql_string_ref _Nonnull haystack, cql_string_ref _Nonnull needle);
extern cql_bool contains_at_text(cql_string_ref _Nonnull haystack, cql_string_ref _Nonnull needle, cql_int32 offset);
cql_string_literal(_literal_1_print_error_message, "");
cql_string_literal(_literal_2_not_print_error_message, "not ");
cql_string_literal(_literal_3_at_least_print_error_message, "at least ");
cql_string_literal(_literal_4_match_multiline, "-- +");
cql_string_literal(_literal_5_match_actual, "-- ");
cql_string_literal(_literal_6_TEST_match_actual, "-- TEST:");
cql_string_literal(_literal_7_match_actual, "-- - ");
cql_string_literal(_literal_8_match_actual, "-- + ");
cql_string_literal(_literal_9_r_read_test_results, "r");
cql_string_literal(_literal_10_The_statement_ending_at_line_read_test_results, "The statement ending at line ");


//
// This file (dbhelp.c) is auto-generated by dbhelp.sql, it is checked in just
// in case CQL is broken by a change.  The Last Known Good Verifier
// can be used to verify the tests pass again, or report failures
// while things are still otherwise broken.  Rebuild with 'regen.sh'
//

#undef cql_error_trace
#define cql_error_trace() fprintf(stderr, "SQL Failure %d %s: %s %d\n", _rc_, sqlite3_errmsg(_db_), __FILE__, __LINE__)

// Generated from cql-verify.sql:38

/*
DECLARE sql_name TEXT;
*/
cql_string_ref sql_name = NULL;

// Generated from cql-verify.sql:39

/*
DECLARE result_name TEXT;
*/
cql_string_ref result_name = NULL;

// Generated from cql-verify.sql:40

/*
DECLARE attempts INTEGER NOT NULL;
*/
cql_int32 attempts = 0;

// Generated from cql-verify.sql:41

/*
DECLARE errors INTEGER NOT NULL;
*/
cql_int32 errors = 0;

// Generated from cql-verify.sql:42

/*
DECLARE tests INTEGER NOT NULL;
*/
cql_int32 tests = 0;

// Generated from cql-verify.sql:61

/*
@ATTRIBUTE(cql:private)
CREATE PROC setup ()
BEGIN
  CREATE TABLE test_output(
    line INTEGER NOT NULL,
    data TEXT NOT NULL
  );
  CREATE INDEX __idx__test_lines ON test_output (line);
  CREATE TABLE source_input(
    line INTEGER NOT NULL,
    data TEXT NOT NULL
  );
  CREATE INDEX __idx__source_lines ON source_input (line);
END;
*/

#define _PROC_ "setup"
static CQL_WARN_UNUSED cql_code setup(sqlite3 *_Nonnull _db_) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();

  _rc_ = cql_exec(_db_,
    "CREATE TABLE test_output( "
      "line INTEGER NOT NULL, "
      "data TEXT NOT NULL "
    ")");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = cql_exec(_db_,
    "CREATE INDEX __idx__test_lines ON test_output (line)");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = cql_exec(_db_,
    "CREATE TABLE source_input( "
      "line INTEGER NOT NULL, "
      "data TEXT NOT NULL "
    ")");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = cql_exec(_db_,
    "CREATE INDEX __idx__source_lines ON source_input (line)");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:72

/*
@ATTRIBUTE(cql:private)
CREATE PROC prev_line (line_ INTEGER NOT NULL, OUT prev INTEGER NOT NULL)
BEGIN
  BEGIN TRY
    SET prev := ( SELECT ifnull(max(line), -1)
      FROM test_output
      WHERE line < line_ );
  END TRY;
  BEGIN CATCH
    SET prev := 0;
  END CATCH;
END;
*/

#define _PROC_ "prev_line"
static CQL_WARN_UNUSED cql_code prev_line(sqlite3 *_Nonnull _db_, cql_int32 line_, cql_int32 *_Nonnull prev) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *_temp_stmt = NULL;

  *prev = 0; // set out arg to non-garbage
  // try
  {
    _rc_ = cql_prepare(_db_, &_temp_stmt,
      "SELECT ifnull(max(line), -1) "
        "FROM test_output "
        "WHERE line < ?");
    cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line_);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto catch_start_1; }
    _rc_ = sqlite3_step(_temp_stmt);
    if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto catch_start_1; }
      *prev = sqlite3_column_int(_temp_stmt, 0);
    cql_finalize_stmt(&_temp_stmt);
    goto catch_end_1;
  }
  catch_start_1: {
    *prev = 0;
  }
  catch_end_1:;
  _rc_ = SQLITE_OK;

  cql_error_report();
  cql_finalize_stmt(&_temp_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:83

/*
@ATTRIBUTE(cql:private)
CREATE PROC dump_output (line_ INTEGER NOT NULL)
BEGIN
  DECLARE C CURSOR FOR
    SELECT *
      FROM test_output
      WHERE line = line_;
  LOOP FETCH C
  BEGIN
    CALL printf("%s\n", C.data);
  END;
END;
*/

#define _PROC_ "dump_output"

typedef struct dump_output_C_row {
  cql_bool _has_row_;
  cql_uint16 _refs_count_;
  cql_uint16 _refs_offset_;
  cql_int32 line;
  cql_string_ref _Nonnull data;
} dump_output_C_row;

#define dump_output_C_refs_offset cql_offsetof(dump_output_C_row, data) // count = 1
static CQL_WARN_UNUSED cql_code dump_output(sqlite3 *_Nonnull _db_, cql_int32 line_) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *C_stmt = NULL;
  dump_output_C_row C = { ._refs_count_ = 1, ._refs_offset_ = dump_output_C_refs_offset };

  _rc_ = cql_prepare(_db_, &C_stmt,
    "SELECT line, data "
      "FROM test_output "
      "WHERE line = ?");
  cql_multibind(&_rc_, _db_, &C_stmt, 1,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line_);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  for (;;) {
    _rc_ = sqlite3_step(C_stmt);
    C._has_row_ = _rc_ == SQLITE_ROW;
    cql_multifetch(_rc_, C_stmt, 2,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.line,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, &C.data);
    if (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    if (!C._has_row_) break;
    cql_alloc_cstr(_cstr_1, C.data);
    printf("%s\n", _cstr_1);
    cql_free_cstr(_cstr_1, C.data);
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&C_stmt);
  cql_teardown_row(C);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:111

/*
@ATTRIBUTE(cql:private)
CREATE PROC find (line_ INTEGER NOT NULL, pattern TEXT NOT NULL, OUT search_line INTEGER NOT NULL, OUT found INTEGER NOT NULL)
BEGIN
  BEGIN TRY
    SET search_line := ( SELECT line
      FROM test_output
      WHERE line >= line_
      LIMIT 1 );
  END TRY;
  BEGIN CATCH
    CALL printf("no lines come after %d\n", line_);
    CALL printf("available test output lines: %d\n", ( SELECT count(*)
      FROM test_output ));
    CALL printf("max line number: %d\n", ( SELECT max(line)
      FROM test_output ));
    THROW;
  END CATCH;
  SET found := ( SELECT count(*)
    FROM test_output
    WHERE line = search_line AND data LIKE "%" || pattern || "%" );
END;
*/

#define _PROC_ "find"
static CQL_WARN_UNUSED cql_code find(sqlite3 *_Nonnull _db_, cql_int32 line_, cql_string_ref _Nonnull pattern, cql_int32 *_Nonnull search_line, cql_int32 *_Nonnull found) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  cql_int32 _tmp_int_0 = 0;
  cql_nullable_int32 _tmp_n_int_0 = { .is_null = 1 };
  sqlite3_stmt *_temp_stmt = NULL;

  *search_line = 0; // set out arg to non-garbage
  *found = 0; // set out arg to non-garbage
  // try
  {
    _rc_ = cql_prepare(_db_, &_temp_stmt,
      "SELECT line "
        "FROM test_output "
        "WHERE line >= ? "
        "LIMIT 1");
    cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line_);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto catch_start_2; }
    _rc_ = sqlite3_step(_temp_stmt);
    if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto catch_start_2; }
      *search_line = sqlite3_column_int(_temp_stmt, 0);
    cql_finalize_stmt(&_temp_stmt);
    goto catch_end_2;
  }
  catch_start_2: {
    int32_t _rc_thrown_1 = _rc_;
    printf("no lines come after %d\n", line_);
    _rc_ = cql_prepare(_db_, &_temp_stmt,
      "SELECT count(*) "
        "FROM test_output");
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
    _rc_ = sqlite3_step(_temp_stmt);
    if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
      _tmp_int_0 = sqlite3_column_int(_temp_stmt, 0);
    cql_finalize_stmt(&_temp_stmt);
    printf("available test output lines: %d\n", _tmp_int_0);
    _rc_ = cql_prepare(_db_, &_temp_stmt,
      "SELECT max(line) "
        "FROM test_output");
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
    _rc_ = sqlite3_step(_temp_stmt);
    if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
      cql_column_nullable_int32(_temp_stmt, 0, &_tmp_n_int_0);
    cql_finalize_stmt(&_temp_stmt);
    printf("max line number: %d\n", _tmp_n_int_0.value);
    _rc_ = cql_best_error(_rc_thrown_1);
    cql_error_trace();
    goto cql_cleanup;
  }
  catch_end_2:;
  _rc_ = cql_prepare(_db_, &_temp_stmt,
    "SELECT count(*) "
      "FROM test_output "
      "WHERE line = ? AND data LIKE '%' || ? || '%'");
  cql_multibind(&_rc_, _db_, &_temp_stmt, 2,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, *search_line,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, pattern);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = sqlite3_step(_temp_stmt);
  if (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
    *found = sqlite3_column_int(_temp_stmt, 0);
  cql_finalize_stmt(&_temp_stmt);
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&_temp_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:122

/*
@ATTRIBUTE(cql:private)
CREATE PROC dump_source (line1 INTEGER NOT NULL, line2 INTEGER NOT NULL)
BEGIN
  DECLARE C CURSOR FOR
    SELECT *
      FROM source_input
      WHERE line > line1 AND line <= line2;
  LOOP FETCH C
  BEGIN
    CALL printf("%s\n", C.data);
  END;
END;
*/

#define _PROC_ "dump_source"

typedef struct dump_source_C_row {
  cql_bool _has_row_;
  cql_uint16 _refs_count_;
  cql_uint16 _refs_offset_;
  cql_int32 line;
  cql_string_ref _Nonnull data;
} dump_source_C_row;

#define dump_source_C_refs_offset cql_offsetof(dump_source_C_row, data) // count = 1
static CQL_WARN_UNUSED cql_code dump_source(sqlite3 *_Nonnull _db_, cql_int32 line1, cql_int32 line2) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *C_stmt = NULL;
  dump_source_C_row C = { ._refs_count_ = 1, ._refs_offset_ = dump_source_C_refs_offset };

  _rc_ = cql_prepare(_db_, &C_stmt,
    "SELECT line, data "
      "FROM source_input "
      "WHERE line > ? AND line <= ?");
  cql_multibind(&_rc_, _db_, &C_stmt, 2,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line1,
                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line2);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  for (;;) {
    _rc_ = sqlite3_step(C_stmt);
    C._has_row_ = _rc_ == SQLITE_ROW;
    cql_multifetch(_rc_, C_stmt, 2,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.line,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, &C.data);
    if (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    if (!C._has_row_) break;
    cql_alloc_cstr(_cstr_2, C.data);
    printf("%s\n", _cstr_2);
    cql_free_cstr(_cstr_2, C.data);
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&C_stmt);
  cql_teardown_row(C);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:141

/*
@ATTRIBUTE(cql:private)
CREATE PROC print_error_message (buffer TEXT NOT NULL, line INTEGER NOT NULL, expected INTEGER NOT NULL)
BEGIN
  CALL printf("\n%s:%d error: expected '%s' %spresent", sql_name, line, buffer, CASE WHEN expected <> 0 THEN ""
  ELSE "not "
  END);
  IF expected <> 0 THEN
    CALL printf(" %s%d times\n", CASE WHEN expected = -1 THEN "at least "
    ELSE ""
    END, CASE WHEN expected = -1 THEN 1
    ELSE expected
    END);
  END IF;
  CALL printf("\n");
END;
*/

#define _PROC_ "print_error_message"
static void print_error_message(cql_string_ref _Nonnull buffer, cql_int32 line, cql_int32 expected) {
  cql_string_ref _tmp_text_0 = NULL;
  cql_int32 _tmp_int_1 = 0;

  do {
    if (expected != 0) {
      cql_set_string_ref(&_tmp_text_0, _literal_1_print_error_message);
      break;
    }
    cql_set_string_ref(&_tmp_text_0, _literal_2_not_print_error_message);
  } while (0);
  cql_alloc_cstr(_cstr_3, sql_name);
  cql_alloc_cstr(_cstr_4, buffer);
  cql_alloc_cstr(_cstr_5, _tmp_text_0);
  printf("\n%s:%d error: expected '%s' %spresent", _cstr_3, line, _cstr_4, _cstr_5);
  cql_free_cstr(_cstr_3, sql_name);
  cql_free_cstr(_cstr_4, buffer);
  cql_free_cstr(_cstr_5, _tmp_text_0);
  if (expected != 0) {
    do {
      if (expected == - 1) {
        cql_set_string_ref(&_tmp_text_0, _literal_3_at_least_print_error_message);
        break;
      }
      cql_set_string_ref(&_tmp_text_0, _literal_1_print_error_message);
    } while (0);
    do {
      if (expected == - 1) {
        _tmp_int_1 = 1;
        break;
      }
      _tmp_int_1 = expected;
    } while (0);
    cql_alloc_cstr(_cstr_6, _tmp_text_0);
    printf(" %s%d times\n", _cstr_6, _tmp_int_1);
    cql_free_cstr(_cstr_6, _tmp_text_0);
  }
  printf("\n");

  cql_string_release(_tmp_text_0);
}
#undef _PROC_

// Generated from cql-verify.sql:156

/*
@ATTRIBUTE(cql:private)
CREATE PROC match_multiline (buffer TEXT NOT NULL, OUT result BOOL NOT NULL)
BEGIN
  SET result := FALSE;
  IF len_text(buffer) < 7 THEN
    RETURN;
  END IF;
  IF NOT starts_with_text(buffer, "-- +") THEN
    RETURN;
  END IF;
  LET digit := octet_text(buffer, 4);
  LET space := octet_text(buffer, 5);
  IF space <> 32 THEN
    RETURN;
  END IF;
  IF digit < 48 OR digit > 48 + 9 THEN
    RETURN;
  END IF;
  SET result := TRUE;
END;
*/

#define _PROC_ "match_multiline"
static void match_multiline(cql_string_ref _Nonnull buffer, cql_bool *_Nonnull result) {
  cql_int32 _tmp_int_1 = 0;
  cql_bool _tmp_bool_1 = 0;
  cql_int32 digit = 0;
  cql_int32 space = 0;

  *result = 0; // set out arg to non-garbage
  *result = 0;
  _tmp_int_1 = len_text(buffer);
  if (_tmp_int_1 < 7) {
    goto cql_cleanup; // return
  }
  _tmp_bool_1 = starts_with_text(buffer, _literal_4_match_multiline);
  if (! _tmp_bool_1) {
    goto cql_cleanup; // return
  }
  digit = octet_text(buffer, 4);
  space = octet_text(buffer, 5);
  if (space != 32) {
    goto cql_cleanup; // return
  }
  if (digit < 48 || digit > 48 + 9) {
    goto cql_cleanup; // return
  }
  *result = 1;

cql_cleanup:
  ; // label requires some statement
}
#undef _PROC_

// Generated from cql-verify.sql:223

/*
CREATE PROC match_actual (buffer TEXT NOT NULL, line INTEGER NOT NULL)
BEGIN
  DECLARE search_line INTEGER NOT NULL;
  DECLARE count INTEGER NOT NULL;
  DECLARE expected INTEGER NOT NULL;
  DECLARE pattern TEXT;
  IF NOT starts_with_text(buffer, "-- ") THEN
    RETURN;
  END IF;
  IF starts_with_text(buffer, "-- TEST:") THEN
    SET tests := tests + 1;
  END IF;
  IF starts_with_text(buffer, "-- - ") THEN
    SET pattern := after_text(buffer, 5);
    SET expected := 0;
  ELSE IF starts_with_text(buffer, "-- + ") THEN
    SET pattern := after_text(buffer, 5);
    SET expected := -1;
  ELSE IF match_multiline(buffer) THEN
    SET expected := octet_text(buffer, 4) - 48;
    SET pattern := after_text(buffer, 6);
  ELSE
    RETURN;
  END IF;
  SET attempts := attempts + 1;
  CALL find(line, ifnull_throw(pattern), search_line, count);
  IF expected = count OR expected = -1 AND count > 0 THEN
    RETURN;
  END IF;
  SET errors := errors + 1;
  CALL print_error_message(buffer, line, expected);
  CALL printf("found:\n");
  CALL dump_output(search_line);
  DECLARE prev INTEGER NOT NULL;
  CALL prev_line(search_line, prev);
  CALL printf("\nThe corresponding test case is:\n");
  CALL dump_source(prev, search_line);
  CALL print_error_message(buffer, line, expected);
  CALL printf("test file: %s\n", sql_name);
  CALL printf("result file: %s\n", result_name);
  CALL printf("\n");
END;
*/

#define _PROC_ "match_actual"
CQL_WARN_UNUSED cql_code match_actual(sqlite3 *_Nonnull _db_, cql_string_ref _Nonnull buffer, cql_int32 line) {
  cql_contract_argument_notnull((void *)buffer, 1);

  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  cql_int32 search_line = 0;
  cql_int32 count = 0;
  cql_int32 expected = 0;
  cql_string_ref pattern = NULL;
  cql_bool _tmp_bool_1 = 0;
  cql_bool _tmp_bool_0 = 0;
  cql_int32 _tmp_int_1 = 0;
  cql_int32 prev = 0;

  _tmp_bool_1 = starts_with_text(buffer, _literal_5_match_actual);
  if (! _tmp_bool_1) {
    _rc_ = SQLITE_OK; // clean up any SQLITE_ROW value or other non-error
    goto cql_cleanup; // return
  }
  _tmp_bool_0 = starts_with_text(buffer, _literal_6_TEST_match_actual);
  if (_tmp_bool_0) {
    tests = tests + 1;
  }
  _tmp_bool_0 = starts_with_text(buffer, _literal_7_match_actual);
  if (_tmp_bool_0) {
    cql_string_release(pattern);
    pattern = after_text(buffer, 5);
    expected = 0;
  }
  else {
    _tmp_bool_0 = starts_with_text(buffer, _literal_8_match_actual);
    if (_tmp_bool_0) {
      cql_string_release(pattern);
      pattern = after_text(buffer, 5);
      expected = - 1;
    }
    else {
      match_multiline(buffer, &_tmp_bool_1);
      if (_tmp_bool_1) {
        _tmp_int_1 = octet_text(buffer, 4);
        expected = _tmp_int_1 - 48;
        cql_string_release(pattern);
        pattern = after_text(buffer, 6);
      }
      else {
        _rc_ = SQLITE_OK; // clean up any SQLITE_ROW value or other non-error
        goto cql_cleanup; // return
      }
    }
  }
  attempts = attempts + 1;
  if (!pattern) {
    _rc_ = SQLITE_ERROR;
    cql_error_trace();
    goto cql_cleanup;
  }
  _rc_ = find(_db_, line, pattern, &search_line, &count);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  if (expected == count || expected == - 1 && count > 0) {
    _rc_ = SQLITE_OK; // clean up any SQLITE_ROW value or other non-error
    goto cql_cleanup; // return
  }
  errors = errors + 1;
  print_error_message(buffer, line, expected);
  printf("found:\n");
  _rc_ = dump_output(_db_, search_line);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = prev_line(_db_, search_line, &prev);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  printf("\nThe corresponding test case is:\n");
  _rc_ = dump_source(_db_, prev, search_line);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  print_error_message(buffer, line, expected);
  cql_alloc_cstr(_cstr_7, sql_name);
  printf("test file: %s\n", _cstr_7);
  cql_free_cstr(_cstr_7, sql_name);
  cql_alloc_cstr(_cstr_8, result_name);
  printf("result file: %s\n", _cstr_8);
  cql_free_cstr(_cstr_8, result_name);
  printf("\n");
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_string_release(pattern);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:235

/*
@ATTRIBUTE(cql:private)
CREATE PROC do_match (buffer TEXT NOT NULL, line INTEGER NOT NULL)
BEGIN
  BEGIN TRY
    CALL match_actual(buffer, line);
  END TRY;
  BEGIN CATCH
    CALL printf("unexpected sqlite error\n");
    THROW;
  END CATCH;
END;
*/

#define _PROC_ "do_match"
static CQL_WARN_UNUSED cql_code do_match(sqlite3 *_Nonnull _db_, cql_string_ref _Nonnull buffer, cql_int32 line) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();

  // try
  {
    _rc_ = match_actual(_db_, buffer, line);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto catch_start_3; }
    goto catch_end_3;
  }
  catch_start_3: {
    int32_t _rc_thrown_1 = _rc_;
    printf("unexpected sqlite error\n");
    _rc_ = cql_best_error(_rc_thrown_1);
    cql_error_trace();
    goto cql_cleanup;
  }
  catch_end_3:;
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:250

/*
@ATTRIBUTE(cql:private)
CREATE PROC process ()
BEGIN
  DECLARE C CURSOR FOR
    SELECT *
      FROM source_input;
  LOOP FETCH C
  BEGIN
    CALL do_match(C.data, C.line);
  END;
  CALL printf("Verification results: %d tests matched %d patterns of which %d were errors.\n", tests, attempts, errors);
END;
*/

#define _PROC_ "process"

typedef struct process_C_row {
  cql_bool _has_row_;
  cql_uint16 _refs_count_;
  cql_uint16 _refs_offset_;
  cql_int32 line;
  cql_string_ref _Nonnull data;
} process_C_row;

#define process_C_refs_offset cql_offsetof(process_C_row, data) // count = 1
static CQL_WARN_UNUSED cql_code process(sqlite3 *_Nonnull _db_) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  sqlite3_stmt *C_stmt = NULL;
  process_C_row C = { ._refs_count_ = 1, ._refs_offset_ = process_C_refs_offset };

  _rc_ = cql_prepare(_db_, &C_stmt,
    "SELECT line, data "
      "FROM source_input");
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  for (;;) {
    _rc_ = sqlite3_step(C_stmt);
    C._has_row_ = _rc_ == SQLITE_ROW;
    cql_multifetch(_rc_, C_stmt, 2,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.line,
                   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, &C.data);
    if (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    if (!C._has_row_) break;
    _rc_ = do_match(_db_, C.data, C.line);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  }
  printf("Verification results: %d tests matched %d patterns of which %d were errors.\n", tests, attempts, errors);
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_finalize_stmt(&C_stmt);
  cql_teardown_row(C);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:288

/*
@ATTRIBUTE(cql:private)
CREATE PROC read_test_results (result_name TEXT NOT NULL)
BEGIN
  LET result_file := cql_fopen(result_name, "r");
  IF result_file IS NULL THEN
    CALL printf("unable to open file '%s'\n", result_name);
    THROW;
  END IF;
  LET line := 0;
  LET key_string := "The statement ending at line ";
  LET len := len_text(key_string);
  WHILE TRUE
  BEGIN
    LET data := readline_object_file(result_file);
    IF data IS NULL THEN
      LEAVE;
    END IF;
    LET loc := index_of_text(data, key_string);
    IF loc >= 0 THEN
      SET line := atoi_at_text(data, loc + len);
    END IF;
    INSERT INTO test_output(line, data)
      VALUES(line, data);
  END;
END;
*/

#define _PROC_ "read_test_results"
static CQL_WARN_UNUSED cql_code read_test_results(sqlite3 *_Nonnull _db_, cql_string_ref _Nonnull result_name) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  cql_object_ref result_file = NULL;
  cql_int32 line = 0;
  cql_string_ref key_string = NULL;
  cql_int32 len = 0;
  cql_string_ref data = NULL;
  cql_int32 loc = 0;
  sqlite3_stmt *_temp1_stmt = NULL;

  cql_object_release(result_file);
  result_file = cql_fopen(result_name, _literal_9_r_read_test_results);
  if (!result_file) {
    cql_alloc_cstr(_cstr_9, result_name);
    printf("unable to open file '%s'\n", _cstr_9);
    cql_free_cstr(_cstr_9, result_name);
    _rc_ = cql_best_error(SQLITE_OK);
    cql_error_trace();
    goto cql_cleanup;
  }
  line = 0;
  cql_set_string_ref(&key_string, _literal_10_The_statement_ending_at_line_read_test_results);
  len = len_text(key_string);
  for (;;) {
    if (!(1)) break;
    cql_string_release(data);
    data = readline_object_file(result_file);
    if (!data) {
      break;
    }
    loc = index_of_text(data, key_string);
    if (loc >= 0) {
      line = atoi_at_text(data, loc + len);
    }
    if (!_temp1_stmt) {
      _rc_ = cql_prepare(_db_, &_temp1_stmt,
      "INSERT INTO test_output(line, data) "
        "VALUES(?, ?)");
    }
    else {
      _rc_ = SQLITE_OK;
    }
    cql_multibind(&_rc_, _db_, &_temp1_stmt, 2,
                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line,
                  CQL_DATA_TYPE_STRING, data);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
    _rc_ = sqlite3_step(_temp1_stmt);
    if (_rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    sqlite3_reset(_temp1_stmt);
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_object_release(result_file);
  cql_string_release(key_string);
  cql_string_release(data);
  cql_finalize_stmt(&_temp1_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:313

/*
@ATTRIBUTE(cql:private)
CREATE PROC read_test_file (sql_name TEXT NOT NULL)
BEGIN
  LET sql_file := cql_fopen(sql_name, "r");
  IF sql_file IS NULL THEN
    CALL printf("unable to open file '%s'\n", sql_name);
    THROW;
  END IF;
  LET line := 1;
  WHILE TRUE
  BEGIN
    LET data := readline_object_file(sql_file);
    IF data IS NULL THEN
      LEAVE;
    END IF;
    INSERT INTO source_input(line, data)
      VALUES(line, data);
    SET line := line + 1;
  END;
END;
*/

#define _PROC_ "read_test_file"
static CQL_WARN_UNUSED cql_code read_test_file(sqlite3 *_Nonnull _db_, cql_string_ref _Nonnull sql_name) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  cql_object_ref sql_file = NULL;
  cql_int32 line = 0;
  cql_string_ref data = NULL;
  sqlite3_stmt *_temp1_stmt = NULL;

  cql_object_release(sql_file);
  sql_file = cql_fopen(sql_name, _literal_9_r_read_test_results);
  if (!sql_file) {
    cql_alloc_cstr(_cstr_10, sql_name);
    printf("unable to open file '%s'\n", _cstr_10);
    cql_free_cstr(_cstr_10, sql_name);
    _rc_ = cql_best_error(SQLITE_OK);
    cql_error_trace();
    goto cql_cleanup;
  }
  line = 1;
  for (;;) {
    if (!(1)) break;
    cql_string_release(data);
    data = readline_object_file(sql_file);
    if (!data) {
      break;
    }
    if (!_temp1_stmt) {
      _rc_ = cql_prepare(_db_, &_temp1_stmt,
      "INSERT INTO source_input(line, data) "
        "VALUES(?, ?)");
    }
    else {
      _rc_ = SQLITE_OK;
    }
    cql_multibind(&_rc_, _db_, &_temp1_stmt, 2,
                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, line,
                  CQL_DATA_TYPE_STRING, data);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
    _rc_ = sqlite3_step(_temp1_stmt);
    if (_rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
    sqlite3_reset(_temp1_stmt);
    line = line + 1;
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_object_release(sql_file);
  cql_string_release(data);
  cql_finalize_stmt(&_temp1_stmt);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:320

/*
@ATTRIBUTE(cql:private)
CREATE PROC load_data (sql_name TEXT NOT NULL, result_name TEXT NOT NULL)
BEGIN
  CALL read_test_results(result_name);
  CALL read_test_file(sql_name);
END;
*/

#define _PROC_ "load_data"
static CQL_WARN_UNUSED cql_code load_data(sqlite3 *_Nonnull _db_, cql_string_ref _Nonnull sql_name, cql_string_ref _Nonnull result_name) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();

  _rc_ = read_test_results(_db_, result_name);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = read_test_file(_db_, sql_name);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:337

/*
@ATTRIBUTE(cql:private)
CREATE PROC parse_args (args OBJECT<cql_string_list> NOT NULL)
BEGIN
  LET argc := get_object_cql_string_list_count(args);
  IF argc <> 3 THEN
    CALL printf("usage cql-verify foo.sql foo.out\n");
    CALL printf("cql-verify is a test tool.  It processes the input foo.sql\n");
    CALL printf("looking for patterns to match in the CQL output foo.out\n");
    RETURN;
  END IF;
  SET sql_name := ifnull_throw(get_from_object_cql_string_list(args, 1));
  SET result_name := ifnull_throw(get_from_object_cql_string_list(args, 2));
END;
*/

#define _PROC_ "parse_args"
static CQL_WARN_UNUSED cql_code parse_args(sqlite3 *_Nonnull _db_, cql_object_ref _Nonnull args) {
  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();
  cql_int32 argc = 0;
  cql_string_ref _tmp_n_text_0 = NULL;

  argc = get_object_cql_string_list_count(args);
  if (argc != 3) {
    printf("usage cql-verify foo.sql foo.out\n");
    printf("cql-verify is a test tool.  It processes the input foo.sql\n");
    printf("looking for patterns to match in the CQL output foo.out\n");
    _rc_ = SQLITE_OK; // clean up any SQLITE_ROW value or other non-error
    goto cql_cleanup; // return
  }
  cql_set_string_ref(&_tmp_n_text_0, get_from_object_cql_string_list(args, 1));
  if (!_tmp_n_text_0) {
    _rc_ = SQLITE_ERROR;
    cql_error_trace();
    goto cql_cleanup;
  }
  cql_set_string_ref(&sql_name, _tmp_n_text_0);
  cql_set_string_ref(&_tmp_n_text_0, get_from_object_cql_string_list(args, 2));
  if (!_tmp_n_text_0) {
    _rc_ = SQLITE_ERROR;
    cql_error_trace();
    goto cql_cleanup;
  }
  cql_set_string_ref(&result_name, _tmp_n_text_0);
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  cql_string_release(_tmp_n_text_0);
  return _rc_;
}
#undef _PROC_

// Generated from cql-verify.sql:349

/*
CREATE PROC dbhelp_main (args OBJECT<cql_string_list> NOT NULL)
BEGIN
  CALL setup();
  CALL parse_args(args);
  IF sql_name IS NOT NULL AND result_name IS NOT NULL THEN
    CALL load_data(sql_name, result_name);
    CALL process();
  END IF;
END;
*/

#define _PROC_ "dbhelp_main"
CQL_WARN_UNUSED cql_code dbhelp_main(sqlite3 *_Nonnull _db_, cql_object_ref _Nonnull args) {
  cql_contract_argument_notnull((void *)args, 1);

  cql_code _rc_ = SQLITE_OK;
  cql_error_prepare();

  _rc_ = setup(_db_);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  _rc_ = parse_args(_db_, args);
  if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  if (!!sql_name && !!result_name) {
    _rc_ = load_data(_db_, sql_name, result_name);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
    _rc_ = process(_db_);
    if (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
  }
  _rc_ = SQLITE_OK;

cql_cleanup:
  cql_error_report();
  return _rc_;
}
#undef _PROC_

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
  exit(errors);
}
#pragma clang diagnostic pop