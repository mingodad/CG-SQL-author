/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "cql.h"

cql_noexport void cg_c_main(CqlState* _Nonnull CS, struct ast_node *_Nonnull root);
cql_noexport void cg_c_cleanup(CqlState* _Nonnull CS);
