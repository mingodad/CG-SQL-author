/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define CHARBUF_INTERNAL_SIZE 1024
#define CHARBUF_GROWTH_SIZE 1024

typedef struct charbuf
{
  char *ptr;      // pointer to stored data, if any
  uint32_t used;  // bytes used in current buffer
  uint32_t max;   // max bytes in current buffer

  // builtin buffer storage
  char internal[CHARBUF_INTERNAL_SIZE];
} charbuf;

typedef struct pending_charbuf {
  charbuf *buf;
  struct pending_charbuf *prev;
} pending_charbuf;

//cql_data_decl( int32_t CS->charbuf_open_count );
//cql_data_decl( pending_charbuf *CS->__charbufs_in_flight );

cql_noexport void bopen(CqlState* CS, charbuf* b);
cql_noexport void bclose(CqlState* CS, charbuf *b);
cql_noexport void bclear(charbuf *b);
cql_noexport void release_open_charbufs(CqlState* CS);
cql_noexport void vbprintf(charbuf *b, const char *format, va_list args);
cql_noexport void bprintf(charbuf *b, const char *format, ...) __attribute__ (( format( printf, 2, 3 ) ));
cql_noexport CSTR dup_printf(CqlState* CS, const char *format, ...) __attribute__ (( format( printf, 2, 3 ) ));
cql_noexport void bputc(charbuf *b, char c);
cql_noexport void bindent(CqlState* CS, charbuf *output, charbuf *input, int32_t indent);
cql_noexport bool_t breadline(charbuf *output, CSTR *data);

#define CHARBUF_OPEN(x) \
  int32_t __saved_charbuf_count##x = CS->charbuf_open_count; \
  charbuf x; \
  pending_charbuf pending_charbuf_##x; \
  pending_charbuf_##x.prev = CS->__charbufs_in_flight; \
  pending_charbuf_##x.buf = &x; \
  CS->__charbufs_in_flight = &pending_charbuf_##x; \
  bopen(CS, &x)

#define CHARBUF_CLOSE(x) \
  bclose(CS, &x); \
  Invariant(__saved_charbuf_count##x == CS->charbuf_open_count); \
  CS->__charbufs_in_flight = CS->__charbufs_in_flight->prev \

// These helpers push a buffer and use it for the output temporarily.
// When the buffer is finished (at END_INDENT) bindent is used to
// indent it by the indicated amount.  They assume the output buffer is called
// "output".
#define BEGIN_INDENT(name, level) \
  charbuf *name##_saved = output; \
  int32_t name##_level = level; \
  CHARBUF_OPEN(name); \
  output = &name;

#define END_INDENT(name) \
  output = name##_saved; \
  bindent(CS, output, &name, name##_level); \
  CHARBUF_CLOSE(name);
