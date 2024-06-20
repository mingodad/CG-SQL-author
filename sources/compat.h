/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

cql_noexport char *_Nonnull Strdup(CqlState* CS, const char *_Nonnull s);
cql_noexport int32_t Strcasecmp(const char *_Nonnull s1, const char *_Nonnull s2);
cql_noexport int32_t Strncasecmp(const char *_Nonnull s1, const char *_Nonnull s2, size_t n);
cql_noexport int32_t Strendswith(const char *_Nonnull haystack, const char *_Nonnull needle);

// On Windows, the normal versions of some of these function assert on non-ASCII
// characters when using a debug CRT library. These alternative versions allow
// us to avoid "ctype.h" entirely.
cql_noexport bool_t Isalpha(char c);
cql_noexport bool_t Isdigit(char c);
cql_noexport bool_t Islower(char c);
cql_noexport bool_t Isupper(char c);
cql_noexport bool_t Isxdigit(char c);
cql_noexport char Tolower(char c);
cql_noexport char Toupper(char c);
