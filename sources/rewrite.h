/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#if defined(CQL_AMALGAM_LEAN) && !defined(CQL_AMALGAM_SEM)

// minimal stuff goes here

#else

#include "ast.h"
#include "bytebuf.h"
#include "symtab.h"
#include "charbuf.h"
#include "list.h"
#include "sem.h"

cql_noexport void rewrite_proclit(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_insert_list_from_shape(CqlState* _Nonnull CS, ast_node *_Nonnull ast, ast_node *_Nonnull from_shape, uint32_t count);
cql_noexport void rewrite_like_column_spec_if_needed(CqlState* _Nonnull CS, ast_node *_Nonnull columns_values);
cql_noexport void rewrite_from_shape_if_needed(CqlState* _Nonnull CS, ast_node *_Nonnull ast_stmt, ast_node *_Nonnull columns_values);
cql_noexport bool_t rewrite_col_key_list(CqlState* _Nonnull CS, ast_node *_Nullable ast);
cql_noexport CSTR _Nullable process_proclit(CqlState* _Nonnull CS, ast_node *_Nullable ast, CSTR _Nonnull name);
cql_noexport ast_node *_Nonnull rewrite_gen_data_type(CqlState* _Nonnull CS, sem_t sem_type, CSTR _Nullable kind);
cql_noexport ast_node *_Nonnull rewrite_gen_full_column_list(CqlState* _Nonnull CS, sem_struct *_Nonnull sptr);
cql_noexport void rewrite_expr_names_to_columns_values(CqlState* _Nonnull CS, ast_node *_Nonnull columns_values);
cql_noexport void rewrite_select_stmt_to_columns_values(CqlState* _Nonnull CS, ast_node *_Nonnull columns_values);
cql_noexport void rewrite_empty_column_list(CqlState* _Nonnull CS, ast_node *_Nonnull columns_values, sem_struct *_Nonnull sptr);
cql_noexport void rewrite_cql_cursor_diff(CqlState* _Nonnull CS, ast_node *_Nonnull ast, bool_t report_column_name);
cql_noexport void rewrite_iif(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport bool_t rewrite_ast_star_if_needed(CqlState* _Nonnull CS, ast_node *_Nullable arg_list, ast_node *_Nonnull proc_name_ast);
cql_noexport bool_t rewrite_shape_forms_in_list_if_needed(CqlState* _Nonnull CS, ast_node *_Nullable arg_list);
cql_noexport void rewrite_cte_name_list_from_columns(CqlState* _Nonnull CS, ast_node *_Nonnull ast, ast_node *_Nonnull select_core);
cql_noexport void rewrite_params(CqlState* _Nonnull CS, ast_node *_Nullable head, bytebuf *_Nullable args_info);
cql_noexport void rewrite_typed_names(CqlState* _Nonnull CS, ast_node *_Nullable head);
cql_noexport void rewrite_data_type_if_needed(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_right_col_def_type_attrs_if_needed(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_nullable_to_notnull(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_guard_stmt_to_if_stmt(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_printf_inserting_casts_as_needed(CqlState* _Nonnull CS, ast_node *_Nonnull ast, CSTR _Nonnull format_string);
cql_noexport void rewrite_select_expr_list(CqlState* _Nonnull CS, ast_node *_Nonnull ast, sem_join *_Nullable jptr_from);
cql_noexport bool_t try_rewrite_blob_fetch_forms(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_out_union_parent_child_stmt(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_shared_fragment_from_backed_table(CqlState* _Nonnull CS, ast_node *_Nonnull backed_table);
cql_noexport void rewrite_select_for_backed_tables(CqlState* _Nonnull CS, ast_node *_Nonnull stmt, list_item *_Nonnull backed_tables_list);
cql_noexport void rewrite_reverse_apply(CqlState* _Nonnull CS, ast_node *_Nonnull head, CSTR _Nonnull op);
cql_noexport void rewrite_insert_statement_for_backed_table(CqlState* _Nonnull CS, ast_node *_Nonnull ast, list_item *_Nullable backed_tables_list);
cql_noexport void rewrite_delete_statement_for_backed_table(CqlState* _Nonnull CS, ast_node *_Nonnull ast, list_item *_Nullable backed_tables_list);
cql_noexport void rewrite_update_statement_for_backed_table(CqlState* _Nonnull CS, ast_node *_Nonnull ast, list_item *_Nullable backed_tables_list);
cql_noexport void rewrite_upsert_statement_for_backed_table(CqlState* _Nonnull CS, ast_node *_Nonnull ast, list_item *_Nullable backed_tables_list);
cql_noexport void rewrite_func_call_as_proc_call(CqlState* _Nonnull CS, ast_node *_Nonnull ast);
cql_noexport void rewrite_array_as_call(CqlState* _Nonnull CS, ast_node *_Nonnull expr, CSTR _Nonnull new_name);
cql_noexport void rewrite_op_equals_assignment_if_needed(CqlState* _Nonnull CS, ast_node *_Nonnull expr, CSTR _Nonnull op);
cql_noexport void rewrite_append_arg(CqlState* _Nonnull CS, ast_node *_Nonnull call, ast_node *_Nonnull arg);
cql_noexport CSTR _Nonnull rewrite_type_suffix(sem_t sem_type);
cql_noexport void rewrite_dot_as_call(CqlState* _Nonnull CS, ast_node *_Nonnull dot, CSTR _Nonnull new_name);
cql_noexport ast_node *_Nonnull rewrite_column_values_as_update_list(CqlState* _Nonnull CS, ast_node *_Nonnull columns_values);
#endif
