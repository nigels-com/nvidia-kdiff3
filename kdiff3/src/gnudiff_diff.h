/* Shared definitions for GNU DIFF

   Modified for KDiff3 by Joachim Eibl 2003.
   The original file was part of GNU DIFF.

   Copyright (C) 1988, 1989, 1991, 1992, 1993, 1994, 1995, 1998, 2001,
   2002 Free Software Foundation, Inc.

   GNU DIFF is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU DIFF is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.
   If not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef GNUDIFF_DIFF_H
#define GNUDIFF_DIFF_H

#include "gnudiff_system.h"

#include <stdio.h>

#define TAB_WIDTH 8

class GnuDiff
{
public:
/* What kind of changes a hunk contains.  */
enum changes
{
  /* No changes: lines common to both files.  */
  UNCHANGED,

  /* Deletes only: lines taken from just the first file.  */
  OLD,

  /* Inserts only: lines taken from just the second file.  */
  NEW,

  /* Both deletes and inserts: a hunk containing both old and new lines.  */
  CHANGED
};

/* Variables for command line options */

enum output_style
{
  /* No output style specified.  */
  OUTPUT_UNSPECIFIED,

  /* Default output style.  */
  OUTPUT_NORMAL,

  /* Output the differences with lines of context before and after (-c).  */
  OUTPUT_CONTEXT,

  /* Output the differences in a unified context diff format (-u).  */
  OUTPUT_UNIFIED,

  /* Output the differences as commands suitable for `ed' (-e).  */
  OUTPUT_ED,

  /* Output the diff as a forward ed script (-f).  */
  OUTPUT_FORWARD_ED,

  /* Like -f, but output a count of changed lines in each "command" (-n).  */
  OUTPUT_RCS,

  /* Output merged #ifdef'd file (-D).  */
  OUTPUT_IFDEF,

  /* Output sdiff style (-y).  */
  OUTPUT_SDIFF
};

/* True for output styles that are robust,
   i.e. can handle a file that ends in a non-newline.  */
#define ROBUST_OUTPUT_STYLE(S) ((S) != OUTPUT_ED && (S) != OUTPUT_FORWARD_ED)

enum output_style output_style;

/* Nonzero if output cannot be generated for identical files.  */
bool no_diff_means_no_output;

/* Number of lines of context to show in each set of diffs.
   This is zero when context is not to be shown.  */
lin context;

/* Consider all files as text files (-a).
   Don't interpret codes over 0177 as implying a "binary file".  */
bool text;

/* Number of lines to keep in identical prefix and suffix.  */
lin horizon_lines;

/* The significance of white space during comparisons.  */
enum
{
  /* All white space is significant (the default).  */
  IGNORE_NO_WHITE_SPACE,

  /* Ignore changes due to tab expansion (-E).  */
  IGNORE_TAB_EXPANSION,

  /* Ignore changes in horizontal white space (-b).  */
  IGNORE_SPACE_CHANGE,

  /* Ignore all horizontal white space (-w).  */
  IGNORE_ALL_SPACE
} ignore_white_space;

/* Ignore changes that affect only blank lines (-B).  */
bool ignore_blank_lines;

/* Ignore changes that affect only numbers. (J. Eibl)  */
bool bIgnoreNumbers;
bool bIgnoreWhiteSpace;

/* Files can be compared byte-by-byte, as if they were binary.
   This depends on various options.  */
bool files_can_be_treated_as_binary;

/* Ignore differences in case of letters (-i).  */
bool ignore_case;

/* Ignore differences in case of letters in file names.  */
bool ignore_file_name_case;

/* File labels for `-c' output headers (--label).  */
char *file_label[2];

/* Regexp to identify function-header lines (-F).  */
//struct re_pattern_buffer function_regexp;

/* Ignore changes that affect only lines matching this regexp (-I).  */
//struct re_pattern_buffer ignore_regexp;

/* Say only whether files differ, not how (-q).  */
bool brief;

/* Expand tabs in the output so the text lines up properly
   despite the characters added to the front of each line (-t).  */
bool expand_tabs;

/* Use a tab in the output, rather than a space, before the text of an
   input line, so as to keep the proper alignment in the input line
   without changing the characters in it (-T).  */
bool initial_tab;

/* Remove trailing carriage returns from input.  */
bool strip_trailing_cr;

/* In directory comparison, specify file to start with (-S).
   This is used for resuming an aborted comparison.
   All file names less than this name are ignored.  */
char const *starting_file;

/* Pipe each file's output through pr (-l).  */
bool paginate;

/* Line group formats for unchanged, old, new, and changed groups.  */
char const *group_format[CHANGED + 1];

/* Line formats for unchanged, old, and new lines.  */
char const *line_format[NEW + 1];

/* If using OUTPUT_SDIFF print extra information to help the sdiff filter.  */
bool sdiff_merge_assist;

/* Tell OUTPUT_SDIFF to show only the left version of common lines.  */
bool left_column;

/* Tell OUTPUT_SDIFF to not show common lines.  */
bool suppress_common_lines;

/* The half line width and column 2 offset for OUTPUT_SDIFF.  */
unsigned int sdiff_half_width;
unsigned int sdiff_column2_offset;

/* String containing all the command options diff received,
   with spaces between and at the beginning but none at the end.
   If there were no options given, this string is empty.  */
char *switch_string;

/* Use heuristics for better speed with large files with a small
   density of changes.  */
bool speed_large_files;

/* Patterns that match file names to be excluded.  */
struct exclude *excluded;

/* Don't discard lines.  This makes things slower (sometimes much
   slower) but will find a guaranteed minimal set of changes.  */
bool minimal;

/* Name of program the user invoked (for error messages).  */
char *program_name;

/* The strftime format to use for time strings.  */
char const *time_format;

/* The result of comparison is an "edit script": a chain of `struct change'.
   Each `struct change' represents one place where some lines are deleted
   and some are inserted.

   LINE0 and LINE1 are the first affected lines in the two files (origin 0).
   DELETED is the number of lines deleted here from file 0.
   INSERTED is the number of lines inserted here in file 1.

   If DELETED is 0 then LINE0 is the number of the line before
   which the insertion was done; vice versa for INSERTED and LINE1.  */

struct change
{
  struct change *link;		/* Previous or next edit command  */
  lin inserted;			/* # lines of file 1 changed here.  */
  lin deleted;			/* # lines of file 0 changed here.  */
  lin line0;			/* Line number of 1st deleted line.  */
  lin line1;			/* Line number of 1st inserted line.  */
  bool ignore;			/* Flag used in context.c.  */
};

/* Structures that describe the input files.  */

/* Data on one input file being compared.  */

struct file_data {
#if 0
    int             desc;	/* File descriptor  */
    char const      *name;	/* File name  */
    struct stat     stat;	/* File status */
#endif
    /* Buffer in which text of file is read.  */
    word *buffer;

    /* Allocated size of buffer, in bytes.  Always a multiple of
       sizeof *buffer.  */
    size_t bufsize;

    /* Number of valid bytes now in the buffer.  */
    size_t buffered;

    /* Array of pointers to lines in the file.  */
    char const **linbuf;

    /* linbuf_base <= buffered_lines <= valid_lines <= alloc_lines.
       linebuf[linbuf_base ... buffered_lines - 1] are possibly differing.
       linebuf[linbuf_base ... valid_lines - 1] contain valid data.
       linebuf[linbuf_base ... alloc_lines - 1] are allocated.  */
    lin linbuf_base, buffered_lines, valid_lines, alloc_lines;

    /* Pointer to end of prefix of this file to ignore when hashing.  */
    char const *prefix_end;

    /* Count of lines in the prefix.
       There are this many lines in the file before linbuf[0].  */
    lin prefix_lines;

    /* Pointer to start of suffix of this file to ignore when hashing.  */
    char const *suffix_begin;

    /* Vector, indexed by line number, containing an equivalence code for
       each line.  It is this vector that is actually compared with that
       of another file to generate differences.  */
    lin *equivs;

    /* Vector, like the previous one except that
       the elements for discarded lines have been squeezed out.  */
    lin *undiscarded;

    /* Vector mapping virtual line numbers (not counting discarded lines)
       to real ones (counting those lines).  Both are origin-0.  */
    lin *realindexes;

    /* Total number of nondiscarded lines.  */
    lin nondiscarded_lines;

    /* Vector, indexed by real origin-0 line number,
       containing TRUE for a line that is an insertion or a deletion.
       The results of comparison are stored here.  */
    bool *changed;

    /* 1 if file ends in a line with no final newline.  */
    bool missing_newline;

    /* 1 if at end of file.  */
    bool eof;

    /* 1 more than the maximum equivalence value used for this or its
       sibling file.  */
    lin equiv_max;
};

/* The file buffer, considered as an array of bytes rather than
   as an array of words.  */
#define FILE_BUFFER(f) ((char *) (f)->buffer)

/* Data on two input files being compared.  */

struct comparison
  {
    struct file_data file[2];
    struct comparison const *parent;  /* parent, if a recursive comparison */
  };

/* Describe the two files currently being compared.  */

struct file_data files[2];

/* Stdio stream to output diffs to.  */

FILE *outfile;

/* Declare various functions.  */

/* analyze.c */
struct change* diff_2_files (struct comparison *);

/* context.c */
void print_context_header (struct file_data[], bool);
void print_context_script (struct change *, bool);

/* dir.c */
int diff_dirs (struct comparison const *, int (*) (struct comparison const *, char const *, char const *));

/* ed.c */
void print_ed_script (struct change *);
void pr_forward_ed_script (struct change *);

/* ifdef.c */
void print_ifdef_script (struct change *);

/* io.c */
void file_block_read (struct file_data *, size_t);
bool read_files (struct file_data[], bool);

/* normal.c */
void print_normal_script (struct change *);

/* rcs.c */
void print_rcs_script (struct change *);

/* side.c */
void print_sdiff_script (struct change *);

/* util.c */
//extern char const change_letter[4];
//extern char const pr_program[];
char *concat (char const *, char const *, char const *);
char *dir_file_pathname (char const *, char const *);
bool lines_differ (char const *, char const *);
lin translate_line_number (struct file_data const *, lin);
struct change *find_change (struct change *);
struct change *find_reverse_change (struct change *);
void *zalloc (size_t);
enum changes analyze_hunk (struct change *, lin *, lin *, lin *, lin *);
void begin_output (void);
void debug_script (struct change *);
void fatal (char const *) __attribute__((noreturn));
void finish_output (void);
void message (char const *, char const *, char const *);
void message5 (char const *, char const *, char const *, char const *, char const *);
void output_1_line (char const *, char const *, char const *, char const *);
void perror_with_name (char const *);
void pfatal_with_name (char const *) __attribute__((noreturn));
void print_1_line (char const *, char const * const *);
void print_message_queue (void);
void print_number_range (char, struct file_data *, lin, lin);
void print_script (struct change *, struct change * (*) (struct change *), void (*) (struct change *));
void setup_output (char const *, char const *, bool);
void translate_range (struct file_data const *, lin, lin, long *, long *);

/* version.c */
//extern char const version_string[];

private:
   // gnudiff_analyze.cpp
   lin diag (lin xoff, lin xlim, lin yoff, lin ylim, bool find_minimal, struct partition *part);
   void compareseq (lin xoff, lin xlim, lin yoff, lin ylim, bool find_minimal);
   void discard_confusing_lines (struct file_data filevec[]);
   void shift_boundaries (struct file_data filevec[]);
   struct change * add_change (lin line0, lin line1, lin deleted, lin inserted, struct change *old);
   struct change * build_reverse_script (struct file_data const filevec[]);
   struct change* build_script (struct file_data const filevec[]);
   
   // gnudiff_io.cpp
   void find_and_hash_each_line (struct file_data *current);
   void prepare_text (struct file_data *current);
   void find_identical_ends (struct file_data filevec[]);

   // gnudiff_xmalloc.cpp
   void *xmalloc (size_t n);
   void *xrealloc(void *p, size_t n);
   void xalloc_die (void);
   
   inline bool isWhite( char c )
   {
      return c==' ' || c=='\t' ||  c=='\r';
   }
}; // class GnuDiff

# define XMALLOC(Type, N_items) ((Type *) xmalloc (sizeof (Type) * (N_items)))
# define XREALLOC(Ptr, Type, N_items) \
  ((Type *) xrealloc ((void *) (Ptr), sizeof (Type) * (N_items)))

/* Declare and alloc memory for VAR of type TYPE. */
# define NEW(Type, Var)  Type *(Var) = XMALLOC (Type, 1)

/* Free VAR only if non NULL. */
# define XFREE(Var)	\
   do {                 \
      if (Var)          \
        free (Var);     \
   } while (0)

/* Return a pointer to a malloc'ed copy of the array SRC of NUM elements. */
# define CCLONE(Src, Num) \
  (memcpy (xmalloc (sizeof (*Src) * (Num)), (Src), sizeof (*Src) * (Num)))

/* Return a malloc'ed copy of SRC. */
# define CLONE(Src) CCLONE (Src, 1)

#endif
