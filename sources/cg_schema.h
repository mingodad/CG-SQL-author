/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ast.h"

cql_noexport void cg_schema_main(CqlState* CS, struct ast_node *root);
cql_noexport void cg_schema_upgrade_main(CqlState* CS, struct ast_node *root);
cql_noexport void cg_schema_sqlite_main(CqlState* CS, struct ast_node *root);
