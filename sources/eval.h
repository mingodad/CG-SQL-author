/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#if defined(CQL_AMALGAM_LEAN) && !defined(CQL_AMALGAM_SEM)

// minimal stuff goes here (none at this point)

#else

#include "ast.h"
#include "bytebuf.h"
#include "symtab.h"
#include "charbuf.h"
#include "list.h"
#include "sem.h"

#define EVAL_NIL {0, 0, 0, 0, false}

#define EVAL_FORMAT_NORMAL 1
#define EVAL_FORMAT_FOR_C 2
#define EVAL_FORMAT_FOR_LUA 3

typedef struct eval_node {
  sem_t sem_type;
  int64_t int64_value;
  int32_t int32_value;
  double  real_value;
  bool_t  bool_value;
} eval_node;

cql_noexport void eval_init(CqlState* _Nonnull CS);
cql_noexport void eval_cleanup(CqlState* _Nonnull CS);
cql_noexport void eval(CqlState* _Nonnull CS, ast_node *_Nonnull expr, eval_node *_Nonnull result);
cql_noexport ast_node *_Nonnull eval_set(CqlState* _Nonnull CS, ast_node *_Nonnull expr, eval_node *_Nonnull result);
cql_noexport void eval_cast_to(eval_node *_Nonnull result, sem_t sem_type);
cql_noexport void eval_add_one(eval_node *_Nonnull result);
cql_noexport void eval_format_number(CqlState* _Nonnull CS, eval_node *_Nonnull result, int32_t format_mode, charbuf *_Nonnull output);

#endif
