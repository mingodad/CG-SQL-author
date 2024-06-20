/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#if defined(CQL_AMALGAM_LEAN) && !defined(CQL_AMALGAM_CG_COMMON)

// minimal stubs to avoid link errors

cql_noexport void cg_common_cleanup(CS) {}
void cql_exit_on_semantic_errors(CS, ast_node *head) {}

//cql_data_defn( cg_blob_mappings_t *_Nullable cg_blob_mappings );

#else

#include "cg_common.h"
#include "ast.h"
#include "sem.h"
#include "symtab.h"
#include "encoders.h"
#include "cql_state.h"

// Storage declarations
//cql_data_defn( symtab *_Nullable cg_stmts );
//cql_data_defn( symtab *_Nullable cg_funcs );
//cql_data_defn( symtab *_Nullable cg_exprs );
//cql_data_defn( charbuf *_Nullable cg_header_output );
//cql_data_defn( charbuf *_Nullable cg_main_output );
//cql_data_defn( charbuf *_Nullable cg_fwd_ref_output );
//cql_data_defn( charbuf *_Nullable cg_constants_output );
//cql_data_defn( charbuf *_Nullable cg_declarations_output );
//cql_data_defn( charbuf *_Nullable cg_scratch_vars_output );
//cql_data_defn( charbuf *_Nullable cg_cleanup_output );
//cql_data_defn( charbuf *_Nullable cg_pieces_output );
//cql_data_defn( cg_blob_mappings_t *_Nullable cg_blob_mappings );


// Prints a symbol name, along with any configured prefix, to the specified buffer.
// Multiple CSTRs may be supplied to build the name, which will be concatenated
// together.  The configured symbol case will be applied to the full symbol name.
// The prefix will be included as specified.
//
// All input names are assumed to be in snake case already.
cql_noexport void cg_sym_name(CqlState* CS, cg_symbol_case symbol_case, charbuf *_Nonnull output, CSTR _Nonnull symbol_prefix, CSTR _Nonnull name, ...)
{
  // Print the prefix first
  bprintf(output, "%s", symbol_prefix);

  // Setup the arg list
  va_list args;
  va_start(args, name);

  CSTR name_component = name;

  // Check the case configuration
  switch (symbol_case) {
    case cg_symbol_case_snake:{
      // No need to modify it, everything in here is already snake case.
      do {
        bprintf(output, "%s", name_component);
      } while ((name_component = va_arg(args, CSTR)));
      break;
    }
    case cg_symbol_case_camel:
    case cg_symbol_case_pascal:{
      // Remove all underscores and uppercase each next character, along with the first if pascal case.
      bool should_upper = (symbol_case != cg_symbol_case_camel);
      do {
        const size_t len = strlen(name_component);
        for (size_t i = 0; i != len; ++i) {
          if (name_component[i] == '_') {
            should_upper = true;
          } else if (should_upper) {
            bputc(output, Toupper(name_component[i]));
            should_upper = false;
          } else {
            bputc(output, name_component[i]);
          }
        }
      } while ((name_component = va_arg(args, CSTR)));
      break;
    }
  }
  va_end(args);
}

// normal charbuf allocation and open goes on the stack
// this macro is for the case where we want a durable buffer
// that will be used across multiple functions
#define ALLOC_AND_OPEN_CHARBUF_REF(x) \
  (x) = (charbuf *)calloc(1, sizeof(charbuf)); \
  bopen(CS, x);

// and here's the cleanup for the durable buffer
#define CLEANUP_CHARBUF_REF(x) if (x) { bclose(CS, x); free(x);  x = NULL; }

cql_noexport void cg_common_init(CqlState* CS)
{
  // All of these will leak, but we don't care.  The tool will shut down after running cg, so it is pointless to clean
  // up after ourselves here.
  CS->cg_stmts = symtab_new();
  CS->cg_funcs = symtab_new();
  CS->cg_exprs = symtab_new();

  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_header_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_main_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_fwd_ref_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_constants_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_declarations_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_scratch_vars_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_cleanup_output);
  ALLOC_AND_OPEN_CHARBUF_REF(CS->cg_pieces_output);

  CS->cg_blob_mappings = calloc(1, sizeof(cg_blob_mappings_t));

  // Default mappings, these are going to be overwritten almost certainly
  // but we need some not null default or else everywhere the mapping code
  // will have to deal with nulls.
  CS->cg_blob_mappings->blob_get_key_type = "bgetkey_type";
  CS->cg_blob_mappings->blob_get_val_type = "bgetval_type";
  CS->cg_blob_mappings->blob_get_key = "bgetkey";
  CS->cg_blob_mappings->blob_get_key_use_offsets = true;
  CS->cg_blob_mappings->blob_get_val = "bgetval";
  CS->cg_blob_mappings->blob_create_key = "bcreatekey";
  CS->cg_blob_mappings->blob_create_key_use_offsets = true;
  CS->cg_blob_mappings->blob_create_val = "bcreateval";
  CS->cg_blob_mappings->blob_update_key = "bupdatekey";
  CS->cg_blob_mappings->blob_update_key_use_offsets = true;
  CS->cg_blob_mappings->blob_update_val = "bupdateval";

  if (CS->rt->cql_post_common_init) CS->rt->cql_post_common_init();
}

// lots of AST nodes require no action -- this guy is very good at that.
cql_noexport void cg_no_op(CqlState* CS, ast_node * ast) {
}

// If there is a semantic error, we should not proceed with code generation.
// We find such an error at the root of the AST.  Note its important
// to be pristine in memory usage because the amalgam version of the compiler
// does not necessarily exit when it's done. It might be used in a long running
// process, like VSCode, and we don't want to leak memory.
cql_noexport void cql_exit_on_semantic_errors(CqlState* CS, ast_node *head) {
  if (head && is_error(head)) {
    cql_error(CS, "semantic errors present; no code gen.\n");
    cql_cleanup_and_exit(CS, 1);
  }
}

// If we are here then we've already determined that there is global code to emit.
// This is code that is outside of any procedure.  This is actually pretty common
// for instance the CQL unit test pattern relies on global code to make the tests run.
// If there is no procedure name for the global code then we have a problem.  We
// can't proceed.
cql_noexport void exit_on_no_global_proc(CqlState* CS) {
  if (!CS->global_proc_name) {
    cql_error(CS, "There are global statements but no global proc name was specified (use --global_proc)\n");
    cql_cleanup_and_exit(CS, 1);
  }
}

// This callback causes select * to be expanded into the appropriate literal
// column names.  We don't do this expansion for exists or not exists because
// the columns don't matter.
//
// The tree shape is:
//
// |       | | {exists_expr}: bool notnull
// |       |   | {select_stmt}: select: { id: integer notnull }
// |       |     | {select_core}: select: { id: integer notnull }
// |       |     | | {select_expr_list_con}: select: { id: integer notnull }
// |       |     |   | {select_expr_list}: select: { id: integer notnull }
// --
// or
// --
// {sem_select_stmt}: union: { A: integer }
// | {select_core_list}: union: { A: integer }
// | | {select_core}: select: { A: integer variable }
// | | | {select_expr_list_con}: select: { A: integer variable }
// | |   | {select_expr_list}: select: { A: integer variable }
// | |   | | ...
// | | {select_core_compound}
// |   | {int 2}
// |   | {select_core_list}: union: { A: integer }
// |     | {select_core}: select: { A: integer variable }
// |     | | {select_expr_list_con}: select: { A: integer variable }
// |     |   | {select_expr_list}: select: { A: integer variable }
// |     |   | | ...
// |     | {select_core_compound}
// |       | {int 2}
// |       | {select_core}: select: { A: integer variable }
// |         | {select_expr_list_con}: select: { A: integer variable }
// |           | {select_expr_list}: select: { A: integer variable }
// |           | | ...
bool_t cg_expand_star(CqlState* CS, ast_node *_Nonnull ast, void *_Nullable context, charbuf *_Nonnull buffer) {
  Contract(is_ast_star(ast) || is_ast_table_star(ast));
  Contract(ast->sem);
  Contract(!is_error(ast));  // already "ok" at least
  Contract(is_struct(ast->sem->sem_type));

  if (is_ast_star(ast)) {
    EXTRACT(select_expr_list, ast->parent);
    EXTRACT(select_expr_list_con, select_expr_list->parent);
    EXTRACT(select_core, select_expr_list_con->parent);
    EXTRACT_ANY(any_select_core, select_core->parent);

    while (!is_ast_select_stmt(any_select_core)) {
      any_select_core = any_select_core->parent;
    }
    EXTRACT_ANY_NOTNULL(select_context, any_select_core->parent);

    if (is_ast_exists_expr(select_context)) {
      return false;
    }

    sem_struct *sptr = ast->sem->sptr;
    uint32_t count = sptr->count;

    bool_t first = true;
    for (uint32_t i = 0; i < count; i++) {
      if (!(sptr->semtypes[i] & SEM_TYPE_HIDDEN_COL)) {
        if (!first) {
          bprintf(buffer, ", ");
        }
        first = false;
        if (!(sptr->semtypes[i] & SEM_TYPE_QID)) {
          bprintf(buffer, "%s", sptr->names[i]);
        }
        else {
          bprintf(buffer, "[");
          cg_unquote_encoded_qstr(CS, buffer, sptr->names[i]);
          bprintf(buffer, "]");
        }
      }
    }
  }
  else {
    Invariant(is_ast_table_star(ast));
    sem_struct *sptr = ast->sem->sptr;
    uint32_t count = sptr->count;

    bool_t first = true;
    for (uint32_t i = 0; i < count; i++) {
      if (!(sptr->semtypes[i] & SEM_TYPE_HIDDEN_COL)) {
        if (!first) {
          bprintf(buffer, ", ");
        }
        first = false;

        if (CS->sem.keep_table_name_in_aliases && get_inserted_table_alias_string_override(ast)) {
          // In case of QID the table alias is already encoded and ready to use
          // It's like that because it is always of the from "[table table_name as some_alias]"
          // so we decode the QID stuff when we make the alias
          CSTR table_alias_name = get_inserted_table_alias_string_override(ast);
          bprintf(buffer, "%s", table_alias_name);
        } else {
          if (is_qid(ast->left)) {
            bprintf(buffer, "[");
            cg_unquote_encoded_qstr(CS, buffer, ast->sem->name);
            bprintf(buffer, "]");
          }
          else {
            bprintf(buffer, "%s", ast->sem->name);
          }
        }

        bprintf(buffer, ".");

        if (!(sptr->semtypes[i] & SEM_TYPE_QID)) {
          bprintf(buffer, "%s", sptr->names[i]);
        }
        else {
          bprintf(buffer, "[");
          cg_unquote_encoded_qstr(CS, buffer, sptr->names[i]);
          bprintf(buffer, "]");
        }
      }
    }
  }

  return true;
}

// Produce a crc of a given charbuf using the CRC helpers.
cql_noexport crc_t crc_charbuf(charbuf *input) {
  crc_t crc = crc_init();
  crc = crc_update(crc, (const unsigned char *)input->ptr, input->used);
  return crc_finalize(crc);
}

// Produce a sha256 reduced to 64 bits using the SHA256 helpers
cql_noexport int64_t sha256_charbuf(charbuf *input) {
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, (const SHA256_BYTE *)input->ptr, input->used - 1);
  SHA256_BYTE hash_bytes[64];
  sha256_final(&ctx, hash_bytes);
  int64_t *h = (int64_t *)hash_bytes;
  int64_t hash = h[0] ^ h[1] ^h[2] ^ h[3];
  return hash;
}

// See cg_find_first_line for more details on why this is what it is.
// All that's going on here is we recursively visit the tree and find the smallest
// line number that matches the given file in that branch.
int32_t cg_find_first_line_recursive(ast_node *ast, CSTR filename) {
  int32_t line = INT32_MAX;
  int32_t lleft = INT32_MAX;
  int32_t lright = INT32_MAX;

  // file name is usually the same actual string but not always
  if (ast->filename == filename || !strcmp(filename, ast->filename)) {
   line = ast->lineno;
  }

  if (ast_has_left(ast)) {
   lleft = cg_find_first_line_recursive(ast->left, filename);
   if (lleft < line) line = lleft;
  }

  if (ast_has_right(ast)) {
   lright = cg_find_first_line_recursive(ast->right, filename);
   if (lright < line) line = lright;
  }

  return line;
}

cql_noexport void cg_common_blob_get_key_type_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_get_key_type_stmt(ast));
  EXTRACT_STRING(name, ast->left);

  CS->cg_blob_mappings->blob_get_key_type = name;
}

cql_noexport void cg_common_blob_get_val_type_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_get_val_type_stmt(ast));
  EXTRACT_STRING(name, ast->left);

  CS->cg_blob_mappings->blob_get_val_type = name;
}

cql_noexport void cg_common_blob_get_key_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_get_key_stmt(ast));
  EXTRACT_STRING(name, ast->left);
  EXTRACT_OPTION(offset, ast->right);

  CS->cg_blob_mappings->blob_get_key = name;
  CS->cg_blob_mappings->blob_get_key_use_offsets = !!offset;
}

cql_noexport void cg_common_blob_get_val_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_get_val_stmt(ast));
  EXTRACT_STRING(name, ast->left);
  EXTRACT_OPTION(offset, ast->right);

  CS->cg_blob_mappings->blob_get_val = name;
  CS->cg_blob_mappings->blob_get_val_use_offsets = !!offset;
}

cql_noexport void cg_common_blob_create_key_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_create_key_stmt(ast));
  EXTRACT_STRING(name, ast->left);
  EXTRACT_OPTION(offset, ast->right);

  CS->cg_blob_mappings->blob_create_key = name;
  CS->cg_blob_mappings->blob_create_key_use_offsets = !!offset;
}

cql_noexport void cg_common_blob_create_val_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_create_val_stmt(ast));
  EXTRACT_STRING(name, ast->left);
  EXTRACT_OPTION(offset, ast->right);

  CS->cg_blob_mappings->blob_create_val = name;
  CS->cg_blob_mappings->blob_create_val_use_offsets = !!offset;
}

cql_noexport void cg_common_blob_update_key_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_update_key_stmt(ast));
  EXTRACT_STRING(name, ast->left);
  EXTRACT_OPTION(offset, ast->right);

  CS->cg_blob_mappings->blob_update_key = name;
  CS->cg_blob_mappings->blob_update_key_use_offsets = !!offset;
}

cql_noexport void cg_common_blob_update_val_stmt(CqlState* CS, ast_node *ast) {
  Contract(is_ast_blob_update_val_stmt(ast));
  EXTRACT_STRING(name, ast->left);
  EXTRACT_OPTION(offset, ast->right);

  CS->cg_blob_mappings->blob_update_val = name;
  CS->cg_blob_mappings->blob_update_val_use_offsets = !!offset;
}

// What's going on here is that the AST is generated on REDUCE operations.
// that means the line number at the time any AST node was generated is
// the largest line number anywhere in that AST.  But if we're looking for
// the line number for a statement we want the line number where it started.
// The way to get that is to recurse through the tree and choose the smallest
// line number anywhere in the tree.  But, we must only use line numbers
// from the same file as the one we ended on.  If (e.g.) a procedure spans files
// this will cause jumping around but that's not really avoidable.
cql_noexport int32_t cg_find_first_line(ast_node *ast) {
  return cg_find_first_line_recursive(ast, ast->filename);
}

cql_noexport void cg_emit_name(CqlState* CS, charbuf *output, CSTR name, bool_t qid) {
  if (qid) {
    cg_decode_qstr(CS, output, name);
  }
  else {
    bprintf(output, "%s", name);
  }
}

// emit a name or a quoted name as needed
cql_noexport void cg_emit_name_ast(CqlState* CS, charbuf *output, ast_node *name_ast) {
  EXTRACT_STRING(name, name_ast);
  cg_emit_name(CS, output, name, is_qid(name_ast));
}

cql_noexport void cg_emit_sptr_index(CqlState* CS, charbuf *output, sem_struct *sptr, uint32_t i) {
  cg_emit_name(CS, output, sptr->names[i], !!(sptr->semtypes[i] & SEM_TYPE_QID));
}

cql_noexport void cg_common_cleanup(CqlState* CS) {
  SYMTAB_CLEANUP(CS->cg_stmts);
  SYMTAB_CLEANUP(CS->cg_funcs);
  SYMTAB_CLEANUP(CS->cg_exprs);

  CLEANUP_CHARBUF_REF(CS->cg_header_output);
  CLEANUP_CHARBUF_REF(CS->cg_main_output);
  CLEANUP_CHARBUF_REF(CS->cg_fwd_ref_output);
  CLEANUP_CHARBUF_REF(CS->cg_constants_output);
  CLEANUP_CHARBUF_REF(CS->cg_declarations_output);
  CLEANUP_CHARBUF_REF(CS->cg_scratch_vars_output);
  CLEANUP_CHARBUF_REF(CS->cg_cleanup_output);
  CLEANUP_CHARBUF_REF(CS->cg_pieces_output)

  free(CS->cg_blob_mappings);
  CS->cg_blob_mappings = 0;
}

CqlState* cql_new_state() {
    CqlState *cs = _new(CqlState);
    memset(cs, 0, sizeof(CqlState));
    return cs; 
}

void cql_free_state(CqlState* CS) {
    free(CS);
}

#endif
