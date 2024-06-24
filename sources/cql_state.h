/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "symtab.h"
#include "cg_common.h"
#include <setjmp.h>
#include "cql.y.h"

//From ast.c
typedef struct ast_macro_state_t {
  CSTR name;
  CSTR file;
  int32_t line;
  struct ast_macro_state_t *parent;
  symtab *args;
} ast_macro_state_t;

//From sem.c
typedef struct {
  bool_t strict_fk_update;            // indicates there must be some "ON UPDATE" action in every FK
  bool_t strict_fk_delete;            // indicates there must be some "ON DELETE" action in every FK
  bool_t strict_join;                 // only ANSI style joins may be used, "from A,B" is rejected
  bool_t strict_upsert_stmt;          // no upsert statement may be used
  bool_t strict_window_func;          // no window functions may be used
  bool_t strict_without_rowid;        // no WITHOUT ROWID may be used.
  bool_t strict_transaction;          // no transactions may be started, commited, aborted etc.
  bool_t strict_if_nothing;           // (select ..) expressions must include the if nothing form
  bool_t strict_insert_select;        // insert with select may not include joins
  bool_t strict_table_function;       // table valued functions cannot be used on left/right joins (avoiding SQLite bug)
  bool_t strict_encode_context;       // encode context must be specified in @vault_sensitive
  bool_t strict_encode_context_type;  // the specified vault context column must be the specified data type
  bool_t strict_is_true;              // IS TRUE, IS FALSE, etc. may not be used because of downlevel issues
  bool_t strict_cast;                 // NO-OP casts result in errors
  bool_t strict_sign_function;        // the SQLite sign function may not be used (as it is absent in <3.35.0)
  bool_t strict_cursor_has_row;       // auto cursors require a has-row check before certain fields are accessed
  bool_t strict_update_from;          // the UPDATE statement may not include a FROM clause (absent in <3.33.0)
  bool_t strict_and_or_not_null_check; // nullability analysis on AND/OR logical expressions
} enforcement_options;
// The analysis of loops like LOOP and WHILE is done in two passes. First, we
// analyze the loop to conservatively figure out every improvement that the loop
// could possibly unset. After that, then we reanalyze it with said improvements
// unset to ensure that everything is safe. See `sem_stmt_list_within_loop` for
// more information on why this is necessary.
typedef enum {
  LOOP_ANALYSIS_STATE_NONE,
  LOOP_ANALYSIS_STATE_ANALYZE,
  LOOP_ANALYSIS_STATE_REANALYZE
} loop_analysis_state;


typedef struct CqlState {

//From cg_common.c
// Storage declarations
symtab *_Nullable cg_stmts;
symtab *_Nullable cg_funcs;
symtab *_Nullable cg_exprs;
charbuf *_Nullable cg_header_output;
charbuf *_Nullable cg_main_output;
charbuf *_Nullable cg_fwd_ref_output;
charbuf *_Nullable cg_constants_output;
charbuf *_Nullable cg_declarations_output;
charbuf *_Nullable cg_scratch_vars_output;
charbuf *_Nullable cg_cleanup_output;
charbuf *_Nullable cg_pieces_output;
cg_blob_mappings_t *_Nullable cg_blob_mappings;

//From cg_c.c
int32_t stmt_nesting_level;

//From ast.c
char *_Nullable current_file;
bool_t macro_expansion_errors;
minipool *ast_pool;
minipool *str_pool;
/*static*/ symtab *macro_table;
/*static*/ symtab *macro_arg_table;
/*static*/ ast_macro_state_t macro_state;
// for indenting, it just holds spaces.
/*static*/ char padbuffer[4096];



//From charbuf.c
int32_t charbuf_open_count;
pending_charbuf *__charbufs_in_flight;

//From cql.y
const char *global_proc_name;
/*static*/ jmp_buf cql_for_exit;
/*static*/ int32_t cql_exit_code;
// Set to true upon a call to `yyerror`.
/*static*/ bool_t parse_error_occurred;
/*static*/ CSTR table_comment_saved;
/*static*/ symtab *defines;
/*static*/ uint64_t next_id; // = 0;
cmd_options options;
const rtdata *rt;

yyscan_t scanner;

//From cg_objc.c
// Whether a text column in the result set of a proc is encoded
/*static*/ bool_t is_string_column_encoded;

//From gg_stats.c
/*static*/ CSTR cg_stats_current_proc;
/*static*/ symtab *stats_table;
/*static*/ charbuf *stats_output;
/*static*/ symtab *stats_stoplist;

//From eval.c
// This is the symbol table with the ast dispatch when we get to an ast node
// we look it up here and call the appropriate function whose name matches the ast
// node type.
/*static*/ symtab *evals;

//From minipool.c
/*static*/ lazy_free *_Nullable lazy_frees;

//From cg_schema.c
/*static*/ symtab *full_drop_funcs;
// Set to keep track of which functions we have emitted group_drops functions for
/*static*/ symtab *group_drop_funcs;
/*static*/ int32_t max_group_ordinal; // = 0;

//From rewrite.c
/*static*/ int32_t cursor_base;

//From cql.l
/*static*/ bool cql_builtins_processing; //= false;
/*static*/ bool cql_delete_main_buffer; // = false;
/*static*/ int cql_include_index; // = 0;
/*static*/ symtab *processed_files;
/*static*/ CSTR last_doc_comment; // = NULL;
/*static YY_BUFFER_STATE*/void* cql_main_buffer;
/*static cql_ifdef_state_t **/void *cql_ifdef_state;
/*static cql_include_state*/void *cql_includes; //[MAX_INCLUDES];

//From cqlrt.c
//int32_t cql_outstanding_refs; // = 0;

//From json_schema.c
/*static*/ bool_t found_shared_fragment;
// These are the main output buffers for the various forms of statements we support
// we build these up as we encounter them, redirecting the local 'output' to one of these
/*static*/ charbuf *queries;
/*static*/ charbuf *deletes;
/*static*/ charbuf *inserts;
/*static*/ charbuf *updates;
/*static*/ charbuf *general;
/*static*/ charbuf *general_inserts;

// We use this to track every table we've ever seen and we remember what stored procedures use it
/*static*/ symtab *tables_to_procs;

//From cg_query_plan.c
/*static*/ charbuf *schema_stmts;
/*static*/ charbuf *backed_tables;
/*static*/ charbuf *query_plans;
/*static*/ CSTR current_procedure_name;
/*static*/ charbuf *current_ok_table_scan;
/*static*/ symtab *virtual_tables;

// Count sql statement found in ast
/*static*/ uint32_t sql_stmt_count; // = 0;

/*static gen_sql_callbacks **/void *cg_qp_callbacks;// = NULL;

//From gen_sql.c
int32_t gen_stmt_level;
// the current primary output buffer for the closure of declares
/*static*/ charbuf *closure_output;

// The declares we have already emitted, if NULL we are emitting
// everything every time -- useful for --test output but otherwise
// just redundant at best.  Note cycles are not possible.
// even with no checking because declares form a partial order.
/*static*/ symtab *closure_emitted;

/*static*/ symtab *gen_stmts;
/*static*/ symtab *gen_exprs;
/*static*/ charbuf *gen_output;
/*static gen_sql_callbacks**/void *gen_callbacks; // = NULL;
/*static*/ symtab *used_alias_syms; // = NULL;
/*static*/ int32_t gen_indent; // = 0;
/*static*/ int32_t pending_indent; // = 0;

//From flow.c
/*static flow_context**/void *current_context;
/*static flow_context**/void *top_jump_context;

//From cg_c.c
struct {
// Each prepared statement in a proc gets a unique index
/*static*/ int32_t prepared_statement_index;

/*static*/ int32_t case_statement_count;
/*static*/ int32_t catch_block_count;
/*static cg_scratch_masks*/void *current_masks;
/*static*/ bool_t in_loop;
/*static*/ int32_t cur_bound_statement;
/*static*/ int32_t cur_fragment_predicate;
/*static*/ int32_t cur_variable_count;
/*static*/ symtab *emitted_proc_decls;
/*static*/ bool_t error_target_used;
/*static*/ charbuf *exports_output;
/*static*/ bool_t has_conditional_fragments;
/*static*/ bool_t has_shared_fragments;
/*static*/ bool_t has_variables;
/*static*/ bool_t in_inline_function_fragment;
/*static*/ bool_t in_proc;
/*static*/ bool_t in_var_group_decl;
/*static*/ bool_t in_var_group_emit;
/*static*/ uint32_t max_fragment_predicate;
/*static*/ int32_t piece_last_offset;
/*static*/ int32_t prev_variable_count;
/*static*/ symtab *proc_arg_aliases;
/*static*/ symtab *proc_cte_aliases;
/*static*/ int32_t proc_cte_index;
/*static*/ int32_t rcthrown_index;
/*static*/ bool_t rcthrown_used;
/*static*/ bool_t return_used;
/*static*/ bool_t seed_declared;
/*static*/ int32_t stack_level;
/*static*/ symtab *string_literals;
/*static*/ int32_t string_literals_count;
/*static*/ int32_t temp_cstr_count;
/*static*/ bool_t temp_statement_emitted;
/*static*/ symtab *text_pieces;
/*static*/ CSTR error_target; // = CQL_CLEANUP_DEFAULT_LABEL;
/*static*/ CSTR rcthrown_current; // = CQL_RCTHROWN_DEFAULT;
/*static*/ bytebuf shared_fragment_strings; // = {NULL, 0, 0};

} cg_c;

struct {
// Case statements might need to generate a unique label for their "else" code
// We count the statements to make an easy label
/*static*/ int32_t case_statement_count; // = 0;

/*static cg_lua_scratch_masks**/void *current_masks;
/*static*/ int32_t catch_block_count;
/*static*/ bool_t continue_label_needed;
/*static*/ int32_t continue_label_next;
/*static*/ int32_t continue_label_number;
/*static*/ int32_t cur_bound_statement;
/*static*/ int32_t cur_fragment_predicate;
/*static*/ int32_t cur_variable_count;
/*static*/ bool_t error_target_used;
/*static*/ charbuf *exports_output;
/*static*/ bool_t has_conditional_fragments;
/*static*/ bool_t has_shared_fragments;
/*static*/ bool_t has_variables;
/*static*/ bool_t in_inline_function_fragment;
/*static*/ bool_t in_loop;
/*static*/ bool_t in_proc;
/*static*/ bool_t in_var_group_emit;
/*static*/ uint32_t max_fragment_predicate;
/*static*/ symtab *named_temporaries;
/*static*/ int32_t prepared_statement_index;
/*static*/ int32_t prev_variable_count;
/*static*/ int32_t rcthrown_index;
/*static*/ bool_t rcthrown_used;
/*static*/ bool_t return_used;
/*static*/ bool_t seed_declared;
/*static*/ int32_t stack_level;
/*static*/ int32_t temp_cstr_count;
/*static*/ bool_t temp_statement_emitted;
/*static*/ symtab *proc_arg_aliases;
/*static*/ symtab *proc_cte_aliases;
/*static*/ int32_t proc_cte_index;
/*static*/ CSTR error_target; // = CQL_CLEANUP_DEFAULT_LABEL;
/*static*/ CSTR rcthrown_current; // = CQL_LUA_RCTHROWN_DEFAULT;
/*static*/ bytebuf shared_fragment_strings; // = {NULL, 0, 0};

} cg_lua;

struct {

symtab *ad_hoc_recreate_actions;
struct list_item *all_ad_hoc_list;
struct list_item *all_constant_groups_list;
struct list_item *all_enums_list;
struct list_item *all_functions_list;
struct list_item *all_indices_list;
struct list_item *all_regions_list;
struct list_item *all_select_functions_list;
struct list_item *all_subscriptions_list;
struct list_item *all_tables_list;
struct list_item *all_triggers_list;
struct list_item *all_views_list;
cte_state *cte_cur;
ast_node *current_proc;
ast_node *current_upsert_table_ast;
symtab *encode_columns;
CSTR encode_context_column;
charbuf *error_capture;
symtab *excluded_regions;
sem_t global_proc_flags;
bool_t in_upsert;
bool_t in_upsert_rewrite;
symtab *included_regions;
bool_t keep_table_name_in_aliases;
bytebuf *recreate_annotations;
symtab *recreate_group_deps;
symtab *ref_sources_for_target_table;
symtab *ref_targets_for_source_table;
bytebuf *schema_annotations;
symtab *schema_regions;
bool_t use_encode;

// As we walk sql expressions we note the ast nodes that hold table names that
// are backed tables so that we can swap them out later
/*static*/ list_item *backed_tables_list;
/*static*/ bool_t in_backing_rewrite;

/*static*/ bytebuf *deployable_validations;

/*static*/ bytebuf *unitary_locals;

// for making unique names of between temporaries
/*static*/ int32_t between_count;
// These are the various symbol tables we need, they are stored super dumbly.
/*static*/ symtab *interfaces;
/*static*/ symtab *procs;
/*static*/ symtab *unchecked_procs;
/*static*/ symtab *proc_arg_info;
/*static*/ symtab *triggers;
/*static*/ symtab *upgrade_procs;
/*static*/ symtab *ad_hoc_migrates;
/*static*/ symtab *builtin_funcs;
/*static*/ symtab *builtin_special_funcs;
/*static*/ symtab *funcs;
/*static*/ symtab *unchecked_funcs;
/*static*/ symtab *exprs;
/*static*/ symtab *tables;
/*static*/ symtab *table_default_values;
/*static*/ symtab *indices;
/*static*/ symtab *globals;
/*static*/ symtab *locals;
/*static*/ symtab *enums;
/*static*/ symtab *constant_groups;
/*static*/ symtab *variable_groups;
/*static*/ symtab *constants;
/*static*/ symtab *current_variables;
/*static*/ symtab *savepoints;
/*static*/ symtab *table_items;  // assorted things that go into a table
/*static*/ symtab *builtin_aggregated_funcs;
/*static*/ symtab *arg_bundles;
/*static*/ symtab *global_types;
/*static*/ symtab *local_types;
/*static*/ symtab *misc_attributes;

// These are the symbol tables with the ast dispatch when we get to an ast node
// we look it up here and call the appropriate function whose name matches the ast
// node type.

/*static*/ symtab *non_sql_stmts;
/*static*/ symtab *sql_stmts;

// Note: initialized statics are moot because in amalgam mode the code
// will not be reloaded... you have to re-initialize all statics in the cleanup function

// We have to do extra checks against tables that transitioned from the @recreate plan
// to the strongly managed plan
/*static*/ list_item *all_prev_recreate_tables;

// When validating against the previous schema all newly @create columns must
// have a schema version >= the max in the previous schema.
/*static*/ list_item *created_columns;
/*static*/ int32_t max_previous_schema_version;

/*static*/ CSTR annotation_target;
/*static*/ ast_node *current_explain_stmt;
/*static*/ uint32_t current_expr_context;
/*static sem_joinscope**/void *current_joinscope;
/*static*/ loop_analysis_state current_loop_analysis_state; // = LOOP_ANALYSIS_STATE_NONE;
/*static*/ bool_t current_proc_contains_try_is_proc_body;
/*static*/ CSTR current_region;
/*static*/ symtab *current_region_image;
/*static*/ ast_node *current_table_ast;
/*static*/ CSTR current_table_name;
/*static*/ sem_t encode_context_type;
/*static*/ enforcement_options enforcement;
/*static struct enforcement_stack_record**/void *enforcement_stack;
/*static*/ bool_t found_subscription_error;
/*static global_notnull_improvement_item**/void *global_notnull_improvements;
/*static*/ bool_t has_dml;
///*static*/ const uintptr_t id_out_tag_bit;
/*static*/ bool_t in_proc_savepoint;
/*static*/ bool_t in_shared_fragment;
/*static*/ bool_t in_shared_fragment_call;
/*static*/ bool_t in_switch;
/*static*/ bool_t in_trigger;
/*static*/ bool_t in_trigger_when_expr;
/*static*/ bool_t is_analyzing_notnull_rewrite;
/*static*/ int32_t loop_depth;
/*static*/ sem_join *monitor_jptr;
/*static*/ symtab *monitor_symtab;
/*static*/ symtab *new_enums;
/*static*/ symtab *new_regions;
/*static*/ list_item *next_subscription;
/*static pending_table_validation**/void *pending_table_validations_head;
/*static*/ int32_t recreates;
/*static*/ bool_t schema_upgrade_script;
/*static*/ int32_t schema_upgrade_version;
/*static*/ int32_t select_level;
/*static*/ sem_node *sem_ok;
/*static*/ int32_t sem_stmt_level;
/*static*/ bool_t validating_previous_schema;

} sem;

//From cg_test_helpers.c
struct {
/*static*/ charbuf *cg_th_output;
/*static*/ charbuf *cg_th_decls;
/*static*/ charbuf* cg_th_procs;

// dummy_test utility variable used to emit statements.
/*static*/ charbuf *gen_create_triggers;
/*static*/ charbuf *gen_drop_triggers;

// All triggers per tables. This is used as part of dummy_test to help look up
// all the triggers to emit
/*static*/ symtab *all_tables_with_triggers;

// All indexes per tables. This is used as part of dummy_test to help look up
// all the indexes to emit
/*static*/ symtab *all_tables_with_indexes;

// We use this table to track which proc declarations we've already emitted
/*static*/ symtab *test_helper_decls_emitted;

// Record the autotest attribute processed. This is used to figure out if there
// will be code gen to write to the output file
/*static*/ int32_t helper_flags; // = 0;

// hold all the table name, column name and column values provided by dummy_test node
/*static*/ symtab *dummy_test_infos; // = NULL;

} th;

} CqlState;
