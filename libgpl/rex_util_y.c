/* $Id: rex_util_y.c,v 1.5 2011/07/18 17:48:17 alex Exp $ */

/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with rr or RR, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define RRBISON 1

/* Skeleton name.  */
#define RRSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define RRPURE 0

/* Using locations.  */
#define RRLSP_NEEDED 0



/* Tokens.  */
#ifndef RRTOKENTYPE
# define RRTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum rrtokentype {
     ANY_CHARACTER = 258,
     ASSIGNMENT = 259,
     BOL_ANCHOR = 260,
     CARAT = 261,
     COMMA = 262,
     CTYPE = 263,
     DIGIT = 264,
     EOL_ANCHOR = 265,
     _ERROR = 266,
     HYPHEN = 267,
     LEFT_BRACE = 268,
     LEFT_BRACKET = 269,
     LEFT_PAREN = 270,
     ONE_OR_MORE = 271,
     OR = 272,
     RIGHT_BRACE = 273,
     RIGHT_BRACKET = 274,
     RIGHT_PAREN = 275,
     SINGLE_CHARACTER = 276,
     ZERO_OR_MORE = 277,
     ZERO_OR_ONE = 278
   };
#endif
#define ANY_CHARACTER 258
#define ASSIGNMENT 259
#define BOL_ANCHOR 260
#define CARAT 261
#define COMMA 262
#define CTYPE 263
#define DIGIT 264
#define EOL_ANCHOR 265
#define _ERROR 266
#define HYPHEN 267
#define LEFT_BRACE 268
#define LEFT_BRACKET 269
#define LEFT_PAREN 270
#define ONE_OR_MORE 271
#define OR 272
#define RIGHT_BRACE 273
#define RIGHT_BRACKET 274
#define RIGHT_PAREN 275
#define SINGLE_CHARACTER 276
#define ZERO_OR_MORE 277
#define ZERO_OR_ONE 278




/* Copy the first part of user declarations.  */
#line 1 "rex_util_y.y"

/* $Id: rex_util_y.c,v 1.5 2011/07/18 17:48:17 alex Exp $ */
/*******************************************************************************

Procedure:

    rrparse ()


Author:    Alex Measday


Purpose:

    Function RRPARSE is a YACC-generated routine that parses a regular
    expression (RE) and builds a non-deterministic finite state machine
    that will recognize the RE.  The grammar for regular expressions
    is derived from the grammar presented by Robert Sedgewick in his
    book, ALGORITHMS (Chapter 21: Parsing).


    Invocation:

        status = rrparse () ;

    where

        <status>
            returns the status of parsing the input, zero if no errors occurred
            and a non-zero value otherwise.


    Public Variables:

        REX_UTIL_ERROR_TEXT - is a (CHAR *) pointer to a short error message
            explaining why RRPARSE failed, if it does.


Development History:
                                               Description
Author         Change ID       Build/Date      of Change
-------------  --------------  --------------  ---------------------
A. Measday     Port from TPOCC Release 6.1 to XSAR Release 0
    Added conditional inclusion of VMS-specific header files.  Eliminated
    external function declarations, since the required function prototypes
    are available in the "#include"d system and TPOCC header files.

A. Measday     Port from TPOCC Release 7 to XSAR Pre-Release 1
    Converted function declarations to ANSI C and added function prototypes
    for static, internal functions.

*******************************************************************************/


#include  "pragmatics.h"		/* Compiler, OS, logging definitions. */
#include  <ctype.h>			/* Character classification macros. */
#include  <stdio.h>			/* Standard I/O definitions. */
#include  <stdlib.h>			/* Standard C Library definitions. */
#include  <string.h>			/* Standard C string functions. */
#include  "str_util.h"			/* String manipulation functions. */
#include  "rex_util.h"			/* Regular expression definitions. */
#include  "rex_internals.h"		/* Internal definitions. */

#define  NEW_STATE \
  { if (allocate_state ()) { \
        rex_error_text = "error increasing size of state list" ; \
        return (errno) ; \
    } \
  }

#undef  I_DEFAULT_GUARD
#define  I_DEFAULT_GUARD  rex_util_debug

#define  display_production(left, right) \
    LGI "%s ::== %s\n", left, right) ;

#define  display_token(name, text) \
    { LGI "-- Token %s = \"%c\"\n", name, text) ; }

#define  beginning_of_input()  (rtx->scan == rtx->expression)
#define  input()  ((*rtx->scan == '\0') ? 0 : *rtx->scan++)
#define  unput(c)  (((rtx->scan == rtx->expression) || (c == '\0')) ? -1 : (rtx->scan--, 0))

/*******************************************************************************

    Internal Non-Local Variables.  These flags give the parser (RRPARSE) some
        control over how the lexer (RRLEX) interprets characters in the input
        string.  The flags are automatically reset when RRLEX is positioned at
        the beginning of the input string.

        RRPARSE_AFTER_BRACKET - controls the interpretation of characters
            within a character class specification (i.e., "[...]").  A value
            of zero indicates that the parser is not in the midst of a bracket
            expression.  Once the left bracket is encountered, RRPARSE sets
            this variable to 1; as each character in the bracket is scanned,
            RRPARSE_AFTER_BRACKET is incremented.

        RRPARSE_EXPECTING_ASSIGNMENT - controls the interpretation of a '$'
            that follows a right parenthesis: "...)$...".  RRPARSE sets this
            variable to 1 after reading the right parenthesis.  If RRLEX then
            encounters a '$' followed by a digit ('0' - '9'), it returns the
            '$' as an ASSIGNMENT token; otherwise, the '$' is returned as an
            EOL_ANCHOR token.

        RRPARSE_HYPHEN_AS_LITERAL - controls the interpretation of hyphens
            within a bracketed character class expression.

*******************************************************************************/

static  int  rrparse_after_bracket  OCD ("rex_util")  = 0 ;
static  int  rrparse_expecting_assignment  OCD ("rex_util")  = 0 ;
static  int  rrparse_hyphen_as_literal  OCD ("rex_util")  = 0 ;


/*******************************************************************************
    Private Functions
*******************************************************************************/

static  int  rrerror (
#    if PROTOTYPES
        char  *s
#    endif
    )  OCD ("rex_util") ;

static  int  rrlex (
#    if PROTOTYPES && !defined(__cplusplus)
        void
#    endif
    )  OCD ("rex_util") ;

static  int  allocate_state (
#    if PROTOTYPES && !defined(__cplusplus)
        void
#    endif
    )  OCD ("rex_util") ;

static  int  first_char_of (
#    if PROTOTYPES
        int  state,
        cs_set  *first_set
#    endif
    )  OCD ("rex_util") ;

static  int  last_of (
#    if PROTOTYPES
        int  state
#    endif
    )  OCD ("rex_util") ;

static  int  longest_path (
#    if PROTOTYPES
        int  state
#    endif
    )  OCD ("rex_util") ;

static  int  shortest_path (
#    if PROTOTYPES
        int  state
#    endif
    )  OCD ("rex_util") ;
#line 197 "rex_util_y.y"

/*******************************************************************************
    Miscellaneous declarations.
*******************************************************************************/

    static  char  buffer[8] ;
    static  cs_set  char_class ;
    static  int  i ;

/* Character classification names.  The ordering of these names is important
   to RRPARSE; see the "character_class => CTYPE" rule before making any
   changes. */

    static  char  *class_name[]  OCD ("rex_util")  = {
        "alnum",
        "alpha",
        "cntrl",
        "digit",
        "graph",
        "lower",
        "print",
        "punct",
        "space",
        "upper",
        "xdigit",
        NULL
    } ;



/* Enabling traces.  */
#ifndef RRDEBUG
# define RRDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef RRERROR_VERBOSE
# undef RRERROR_VERBOSE
# define RRERROR_VERBOSE 1
#else
# define RRERROR_VERBOSE 0
#endif

#if ! defined (RRSTYPE) && ! defined (RRSTYPE_IS_DECLARED)
typedef int RRSTYPE;
# define rrstype RRSTYPE /* obsolescent; will be withdrawn */
# define RRSTYPE_IS_DECLARED 1
# define RRSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 324 "y.tab.c"

#if ! defined (rroverflow) || RRERROR_VERBOSE

# ifndef RRFREE
#  define RRFREE free
# endif
# ifndef RRMALLOC
#  define RRMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef RRSTACK_USE_ALLOCA
#  if RRSTACK_USE_ALLOCA
#   define RRSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define RRSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define RRSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef RRSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define RRSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define RRSIZE_T size_t
#  endif
#  define RRSTACK_ALLOC RRMALLOC
#  define RRSTACK_FREE RRFREE
# endif
#endif /* ! defined (rroverflow) || RRERROR_VERBOSE */


#if (! defined (rroverflow) \
     && (! defined (__cplusplus) \
	 || (defined (RRSTYPE_IS_TRIVIAL) && RRSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union rralloc
{
  short rrss;
  RRSTYPE rrvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define RRSTACK_GAP_MAXIMUM (sizeof (union rralloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define RRSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (RRSTYPE))				\
      + RRSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef RRCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define RRCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define RRCOPY(To, From, Count)		\
      do					\
	{					\
	  register RRSIZE_T rri;		\
	  for (rri = 0; rri < (Count); rri++)	\
	    (To)[rri] = (From)[rri];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables RRSIZE and RRSTACKSIZE give the old and new number of
   elements in the stack, and RRPTR gives the new location of the
   stack.  Advance RRPTR to a properly aligned location for the next
   stack.  */
# define RRSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	RRSIZE_T rrnewbytes;						\
	RRCOPY (&rrptr->Stack, Stack, rrsize);				\
	Stack = &rrptr->Stack;						\
	rrnewbytes = rrstacksize * sizeof (*Stack) + RRSTACK_GAP_MAXIMUM; \
	rrptr += rrnewbytes / sizeof (*rrptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char rrsigned_char;
#else
   typedef short rrsigned_char;
#endif

/* RRFINAL -- State number of the termination state. */
#define RRFINAL  15
/* RRLAST -- Last index in RRTABLE.  */
#define RRLAST   43

/* RRNTOKENS -- Number of terminals. */
#define RRNTOKENS  24
/* RRNNTS -- Number of nonterminals. */
#define RRNNTS  16
/* RRNRULES -- Number of rules. */
#define RRNRULES  38
/* RRNRULES -- Number of states. */
#define RRNSTATES  48

/* RRTRANSLATE(RRLEX) -- Bison symbol number corresponding to RRLEX.  */
#define RRUNDEFTOK  2
#define RRMAXUTOK   278

#define RRTRANSLATE(RRX) 						\
  ((unsigned int) (RRX) <= RRMAXUTOK ? rrtranslate[RRX] : RRUNDEFTOK)

/* RRTRANSLATE[RRLEX] -- Bison symbol number corresponding to RRLEX.  */
static const unsigned char rrtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if RRDEBUG
/* RRPRHS[RRN] -- Index of the first RHS symbol of rule number RRN in
   RRRHS.  */
static const unsigned char rrprhs[] =
{
       0,     0,     3,     5,     6,     8,    12,    14,    17,    19,
      21,    27,    28,    34,    37,    40,    43,    44,    50,    52,
      54,    56,    58,    59,    62,    63,    65,    66,    68,    69,
      72,    74,    77,    79,    82,    84,    85,    90,    92
};

/* RRRHS -- A `-1'-separated list of the rules' RHS. */
static const rrsigned_char rrrhs[] =
{
      25,     0,    -1,    26,    -1,    -1,    27,    -1,    26,    17,
      26,    -1,    28,    -1,    28,    27,    -1,     5,    -1,    10,
      -1,    28,    13,    33,    34,    18,    -1,    -1,    15,    26,
      20,    29,    31,    -1,    28,    22,    -1,    28,    16,    -1,
      28,    23,    -1,    -1,    14,    30,    32,    36,    19,    -1,
      21,    -1,     7,    -1,     9,    -1,     3,    -1,    -1,     4,
       9,    -1,    -1,     6,    -1,    -1,    35,    -1,    -1,     7,
      33,    -1,     9,    -1,    35,     9,    -1,    37,    -1,    36,
      37,    -1,    39,    -1,    -1,    39,    12,    38,    39,    -1,
       8,    -1,    21,    -1
};

/* RRRLINE[RRN] -- source line where rule number RRN was defined.  */
static const unsigned short rrrline[] =
{
       0,   235,   235,   266,   275,   277,   302,   304,   315,   325,
     335,   366,   365,   398,   423,   448,   474,   473,   512,   522,
     532,   542,   557,   558,   567,   568,   575,   576,   583,   586,
     597,   601,   609,   611,   617,   622,   621,   635,   657
};
#endif

#if RRDEBUG || RRERROR_VERBOSE
/* RRTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at RRNTOKENS, nonterminals. */
static const char *const rrtname[] =
{
  "$end", "error", "$undefined", "ANY_CHARACTER", "ASSIGNMENT",
  "BOL_ANCHOR", "CARAT", "COMMA", "CTYPE", "DIGIT", "EOL_ANCHOR", "_ERROR",
  "HYPHEN", "LEFT_BRACE", "LEFT_BRACKET", "LEFT_PAREN", "ONE_OR_MORE",
  "OR", "RIGHT_BRACE", "RIGHT_BRACKET", "RIGHT_PAREN", "SINGLE_CHARACTER",
  "ZERO_OR_MORE", "ZERO_OR_ONE", "$accept", "complete_re",
  "regular_expression", "term", "factor", "@1", "@2", "assignment",
  "complement", "number", "upper_bound", "digits", "character_classes",
  "character_class", "@3", "character", 0
};
#endif

# ifdef RRPRINT
/* RRTOKNUM[RRLEX-NUM] -- Internal token number corresponding to
   token RRLEX-NUM.  */
static const unsigned short rrtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278
};
# endif

/* RRR1[RRN] -- Symbol number of symbol that rule RRN derives.  */
static const unsigned char rrr1[] =
{
       0,    24,    25,    26,    26,    26,    27,    27,    28,    28,
      28,    29,    28,    28,    28,    28,    30,    28,    28,    28,
      28,    28,    31,    31,    32,    32,    33,    33,    34,    34,
      35,    35,    36,    36,    37,    38,    37,    37,    39
};

/* RRR2[RRN] -- Number of symbols composing right hand side of rule RRN.  */
static const unsigned char rrr2[] =
{
       0,     2,     1,     0,     1,     3,     1,     2,     1,     1,
       5,     0,     5,     2,     2,     2,     0,     5,     1,     1,
       1,     1,     0,     2,     0,     1,     0,     1,     0,     2,
       1,     2,     1,     2,     1,     0,     4,     1,     1
};

/* RRDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when RRTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char rrdefact[] =
{
       3,    21,     8,    19,    20,     9,    16,     3,    18,     0,
       2,     4,     6,    24,     0,     1,     3,    26,    14,    13,
      15,     7,    25,     0,    11,     5,    30,    28,    27,    37,
      38,     0,    32,    34,    22,    26,     0,    31,    17,    33,
      35,     0,    12,    29,    10,     0,    23,    36
};

/* RRDEFGOTO[NTERM-NUM]. */
static const rrsigned_char rrdefgoto[] =
{
      -1,     9,    10,    11,    12,    34,    13,    42,    23,    27,
      36,    28,    31,    32,    45,    33
};

/* RRPACT[STATE-NUM] -- Index in RRTABLE of the portion describing
   STATE-NUM.  */
#define RRPACT_NINF -17
static const rrsigned_char rrpact[] =
{
      18,   -17,   -17,   -17,   -17,   -17,   -17,    18,   -17,    17,
     -16,   -17,    -3,    20,   -12,   -17,    18,    21,   -17,   -17,
     -17,   -17,   -17,     1,   -17,   -17,   -17,    22,    25,   -17,
     -17,    -5,   -17,    19,    31,    21,    23,   -17,   -17,   -17,
     -17,    27,   -17,   -17,   -17,    16,   -17,   -17
};

/* RRPGOTO[NTERM-NUM].  */
static const rrsigned_char rrpgoto[] =
{
     -17,   -17,     8,    26,   -17,   -17,   -17,   -17,   -17,     5,
     -17,   -17,   -17,    11,   -17,    -2
};

/* RRTABLE[RRPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what RRDEFACT says.
   If RRTABLE_NINF, syntax error.  */
#define RRTABLE_NINF -1
static const unsigned char rrtable[] =
{
       1,    16,     2,    29,     3,    16,     4,     5,    24,    29,
      17,     6,     7,    18,    38,    14,    30,    15,     8,    19,
      20,     1,    30,     2,    25,     3,    22,     4,     5,    35,
      26,    40,     6,     7,    37,    41,    46,    30,    21,     8,
      43,    44,    39,    47
};

static const unsigned char rrcheck[] =
{
       3,    17,     5,     8,     7,    17,     9,    10,    20,     8,
      13,    14,    15,    16,    19,     7,    21,     0,    21,    22,
      23,     3,    21,     5,    16,     7,     6,     9,    10,     7,
       9,    12,    14,    15,     9,     4,     9,    21,    12,    21,
      35,    18,    31,    45
};

/* RRSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char rrstos[] =
{
       0,     3,     5,     7,     9,    10,    14,    15,    21,    25,
      26,    27,    28,    30,    26,     0,    17,    13,    16,    22,
      23,    27,     6,    32,    20,    26,     9,    33,    35,     8,
      21,    36,    37,    39,    29,     7,    34,     9,    19,    37,
      12,     4,    31,    33,    18,    38,     9,    39
};

#if ! defined (RRSIZE_T) && defined (__SIZE_TYPE__)
# define RRSIZE_T __SIZE_TYPE__
#endif
#if ! defined (RRSIZE_T) && defined (size_t)
# define RRSIZE_T size_t
#endif
#if ! defined (RRSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define RRSIZE_T size_t
# endif
#endif
#if ! defined (RRSIZE_T)
# define RRSIZE_T unsigned int
#endif

#define rrerrok		(rrerrstatus = 0)
#define rrclearin	(rrchar = RREMPTY)
#define RREMPTY		(-2)
#define RREOF		0

#define RRACCEPT	goto rracceptlab
#define RRABORT		goto rrabortlab
#define RRERROR		goto rrerrorlab


/* Like RRERROR except do call rrerror.  This remains here temporarily
   to ease the transition to the new meaning of RRERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define RRFAIL		goto rrerrlab

#define RRRECOVERING()  (!!rrerrstatus)

#define RRBACKUP(Token, Value)					\
do								\
  if (rrchar == RREMPTY && rrlen == 1)				\
    {								\
      rrchar = (Token);						\
      rrlval = (Value);						\
      rrtoken = RRTRANSLATE (rrchar);				\
      RRPOPSTACK;						\
      goto rrbackup;						\
    }								\
  else								\
    { 								\
      rrerror ("syntax error: cannot back up");\
      RRERROR;							\
    }								\
while (0)

#define RRTERROR	1
#define RRERRCODE	256

/* RRLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef RRLLOC_DEFAULT
# define RRLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* RRLEX -- calling `rrlex' with the right arguments.  */

#ifdef RRLEX_PARAM
# define RRLEX rrlex (RRLEX_PARAM)
#else
# define RRLEX rrlex ()
#endif

/* Enable debugging if requested.  */
#if RRDEBUG

# ifndef RRFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define RRFPRINTF fprintf
# endif

# define RRDPRINTF(Args)			\
do {						\
  if (rrdebug)					\
    RRFPRINTF Args;				\
} while (0)

# define RRDSYMPRINT(Args)			\
do {						\
  if (rrdebug)					\
    rrsymprint Args;				\
} while (0)

# define RRDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (rrdebug)							\
    {								\
      RRFPRINTF (stderr, "%s ", Title);				\
      rrsymprint (stderr, 					\
                  Token, Value);	\
      RRFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| rr_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
rr_stack_print (short *bottom, short *top)
#else
static void
rr_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  RRFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    RRFPRINTF (stderr, " %d", *bottom);
  RRFPRINTF (stderr, "\n");
}

# define RR_STACK_PRINT(Bottom, Top)				\
do {								\
  if (rrdebug)							\
    rr_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the RRRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
rr_reduce_print (int rrrule)
#else
static void
rr_reduce_print (rrrule)
    int rrrule;
#endif
{
  int rri;
  unsigned int rrlno = rrrline[rrrule];
  RRFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             rrrule - 1, rrlno);
  /* Print the symbols being reduced, and their result.  */
  for (rri = rrprhs[rrrule]; 0 <= rrrhs[rri]; rri++)
    RRFPRINTF (stderr, "%s ", rrtname [rrrhs[rri]]);
  RRFPRINTF (stderr, "-> %s\n", rrtname [rrr1[rrrule]]);
}

# define RR_REDUCE_PRINT(Rule)		\
do {					\
  if (rrdebug)				\
    rr_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int rrdebug;
#else /* !RRDEBUG */
# define RRDPRINTF(Args)
# define RRDSYMPRINT(Args)
# define RRDSYMPRINTF(Title, Token, Value, Location)
# define RR_STACK_PRINT(Bottom, Top)
# define RR_REDUCE_PRINT(Rule)
#endif /* !RRDEBUG */


/* RRINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	RRINITDEPTH
# define RRINITDEPTH 200
#endif

/* RRMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < RRSTACK_BYTES (RRMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (RRMAXDEPTH) && RRMAXDEPTH == 0
# undef RRMAXDEPTH
#endif

#ifndef RRMAXDEPTH
# define RRMAXDEPTH 10000
#endif



#if RRERROR_VERBOSE

# ifndef rrstrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define rrstrlen strlen
#  else
/* Return the length of RRSTR.  */
static RRSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
rrstrlen (const char *rrstr)
#   else
rrstrlen (rrstr)
     const char *rrstr;
#   endif
{
  register const char *rrs = rrstr;

  while (*rrs++ != '\0')
    continue;

  return rrs - rrstr - 1;
}
#  endif
# endif

# ifndef rrstpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define rrstpcpy stpcpy
#  else
/* Copy RRSRC to RRDEST, returning the address of the terminating '\0' in
   RRDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
rrstpcpy (char *rrdest, const char *rrsrc)
#   else
rrstpcpy (rrdest, rrsrc)
     char *rrdest;
     const char *rrsrc;
#   endif
{
  register char *rrd = rrdest;
  register const char *rrs = rrsrc;

  while ((*rrd++ = *rrs++) != '\0')
    continue;

  return rrd - 1;
}
#  endif
# endif

#endif /* !RRERROR_VERBOSE */



#if RRDEBUG
/*--------------------------------.
| Print this symbol on RROUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
rrsymprint (FILE *rroutput, int rrtype, RRSTYPE *rrvaluep)
#else
static void
rrsymprint (rroutput, rrtype, rrvaluep)
    FILE *rroutput;
    int rrtype;
    RRSTYPE *rrvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) rrvaluep;

  if (rrtype < RRNTOKENS)
    {
      RRFPRINTF (rroutput, "token %s (", rrtname[rrtype]);
# ifdef RRPRINT
      RRPRINT (rroutput, rrtoknum[rrtype], *rrvaluep);
# endif
    }
  else
    RRFPRINTF (rroutput, "nterm %s (", rrtname[rrtype]);

  switch (rrtype)
    {
      default:
        break;
    }
  RRFPRINTF (rroutput, ")");
}

#endif /* ! RRDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
rrdestruct (int rrtype, RRSTYPE *rrvaluep)
#else
static void
rrdestruct (rrtype, rrvaluep)
    int rrtype;
    RRSTYPE *rrvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) rrvaluep;

  switch (rrtype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef RRPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int rrparse (void *RRPARSE_PARAM);
# else
int rrparse ();
# endif
#else /* ! RRPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int rrparse (void);
#else
int rrparse ();
#endif
#endif /* ! RRPARSE_PARAM */



/* The lookahead symbol.  */
int rrchar;

/* The semantic value of the lookahead symbol.  */
RRSTYPE rrlval;

/* Number of syntax errors so far.  */
int rrnerrs;



/*----------.
| rrparse.  |
`----------*/

#ifdef RRPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int rrparse (void *RRPARSE_PARAM)
# else
int rrparse (RRPARSE_PARAM)
  void *RRPARSE_PARAM;
# endif
#else /* ! RRPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
rrparse (void)
#else
int
rrparse ()

#endif
#endif
{

  register int rrstate;
  register int rrn;
  int rrresult;
  /* Number of tokens to shift before error messages enabled.  */
  int rrerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int rrtoken = 0;

  /* Three stacks and their tools:
     `rrss': related to states,
     `rrvs': related to semantic values,
     `rrls': related to locations.

     Refer to the stacks thru separate pointers, to allow rroverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	rrssa[RRINITDEPTH];
  short *rrss = rrssa;
  register short *rrssp;

  /* The semantic value stack.  */
  RRSTYPE rrvsa[RRINITDEPTH];
  RRSTYPE *rrvs = rrvsa;
  register RRSTYPE *rrvsp;



#define RRPOPSTACK   (rrvsp--, rrssp--)

  RRSIZE_T rrstacksize = RRINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  RRSTYPE rrval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int rrlen;

  RRDPRINTF ((stderr, "Starting parse\n"));

  rrstate = 0;
  rrerrstatus = 0;
  rrnerrs = 0;
  rrchar = RREMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  rrssp = rrss;
  rrvsp = rrvs;

  goto rrsetstate;

/*------------------------------------------------------------.
| rrnewstate -- Push a new state, which is found in rrstate.  |
`------------------------------------------------------------*/
 rrnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  rrssp++;

 rrsetstate:
  *rrssp = rrstate;

  if (rrss + rrstacksize - 1 <= rrssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      RRSIZE_T rrsize = rrssp - rrss + 1;

#ifdef rroverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	RRSTYPE *rrvs1 = rrvs;
	short *rrss1 = rrss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if rroverflow is a macro.  */
	rroverflow ("parser stack overflow",
		    &rrss1, rrsize * sizeof (*rrssp),
		    &rrvs1, rrsize * sizeof (*rrvsp),

		    &rrstacksize);

	rrss = rrss1;
	rrvs = rrvs1;
      }
#else /* no rroverflow */
# ifndef RRSTACK_RELOCATE
      goto rroverflowlab;
# else
      /* Extend the stack our own way.  */
      if (RRMAXDEPTH <= rrstacksize)
	goto rroverflowlab;
      rrstacksize *= 2;
      if (RRMAXDEPTH < rrstacksize)
	rrstacksize = RRMAXDEPTH;

      {
	short *rrss1 = rrss;
	union rralloc *rrptr =
	  (union rralloc *) RRSTACK_ALLOC (RRSTACK_BYTES (rrstacksize));
	if (! rrptr)
	  goto rroverflowlab;
	RRSTACK_RELOCATE (rrss);
	RRSTACK_RELOCATE (rrvs);

#  undef RRSTACK_RELOCATE
	if (rrss1 != rrssa)
	  RRSTACK_FREE (rrss1);
      }
# endif
#endif /* no rroverflow */

      rrssp = rrss + rrsize - 1;
      rrvsp = rrvs + rrsize - 1;


      RRDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) rrstacksize));

      if (rrss + rrstacksize - 1 <= rrssp)
	RRABORT;
    }

  RRDPRINTF ((stderr, "Entering state %d\n", rrstate));

  goto rrbackup;

/*-----------.
| rrbackup.  |
`-----------*/
rrbackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* rrresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  rrn = rrpact[rrstate];
  if (rrn == RRPACT_NINF)
    goto rrdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* RRCHAR is either RREMPTY or RREOF or a valid lookahead symbol.  */
  if (rrchar == RREMPTY)
    {
      RRDPRINTF ((stderr, "Reading a token: "));
      rrchar = RRLEX;
    }

  if (rrchar <= RREOF)
    {
      rrchar = rrtoken = RREOF;
      RRDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      rrtoken = RRTRANSLATE (rrchar);
      RRDSYMPRINTF ("Next token is", rrtoken, &rrlval, &rrlloc);
    }

  /* If the proper action on seeing token RRTOKEN is to reduce or to
     detect an error, take that action.  */
  rrn += rrtoken;
  if (rrn < 0 || RRLAST < rrn || rrcheck[rrn] != rrtoken)
    goto rrdefault;
  rrn = rrtable[rrn];
  if (rrn <= 0)
    {
      if (rrn == 0 || rrn == RRTABLE_NINF)
	goto rrerrlab;
      rrn = -rrn;
      goto rrreduce;
    }

  if (rrn == RRFINAL)
    RRACCEPT;

  /* Shift the lookahead token.  */
  RRDPRINTF ((stderr, "Shifting token %s, ", rrtname[rrtoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (rrchar != RREOF)
    rrchar = RREMPTY;

  *++rrvsp = rrlval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (rrerrstatus)
    rrerrstatus--;

  rrstate = rrn;
  goto rrnewstate;


/*-----------------------------------------------------------.
| rrdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
rrdefault:
  rrn = rrdefact[rrstate];
  if (rrn == 0)
    goto rrerrlab;
  goto rrreduce;


/*-----------------------------.
| rrreduce -- Do a reduction.  |
`-----------------------------*/
rrreduce:
  /* rrn is the number of a rule to reduce with.  */
  rrlen = rrr2[rrn];

  /* If RRLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets RRVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to RRVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that RRVAL may be used uninitialized.  */
  rrval = rrvsp[1-rrlen];


  RR_REDUCE_PRINT (rrn);
  switch (rrn)
    {
        case 2:
#line 236 "rex_util_y.y"
    { display_production ("complete_re", "regular_expression") ;
            /* The initial state equals the first state in RE.  Add a final
               state and link the last state in RE to the final state. */
            rtx->start_state = rrvsp[0] ;
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[0])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = final ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->num_states++ ;
            /* Estimate the length of the longest path through the complete
               RE.  The length is used to size the stack in the iterative
               version of REX_SEARCH. */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            rtx->longest_path = longest_path (rrvsp[0]) ;
            /* Compute the set of first characters that may appear at the
               beginning of a target string matched by this RE.  This set
               allows REX_MATCH to avoid calling REX_SEARCH when the first
               character of the target string is not in the set of first
               characters. */
            CS_ZERO (&rtx->first_set) ;
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            first_char_of (rtx->start_state, &rtx->first_set) ;
          }
    break;

  case 3:
#line 266 "rex_util_y.y"
    { display_production ("regular_expression", "<empty>") ;
            /* Add an EMPTY state. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = empty ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 4:
#line 276 "rex_util_y.y"
    { display_production ("regular_expression", "term") ;  rrval = rrvsp[0] ; }
    break;

  case 5:
#line 278 "rex_util_y.y"
    { display_production ("regular_expression", "regular_expression | regular_expression") ;
            /* Add an alternation state E1 and an empty state E2.  Link E1 to
               the first state in RE1 and the first state in RE2.  Link the
               last state in RE1 to E2; do the same for the last state in RE2.
               (The first state in "RE|RE" is E1.) */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = alternation ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->state_list[rtx->num_states].next1 = rrvsp[-2] ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[0] ;
            rrval = rtx->num_states++ ;
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-2])].next1 = rtx->num_states ;
            rtx->state_list[last_of (rrvsp[0])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = empty ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rtx->num_states++ ;
          }
    break;

  case 6:
#line 303 "rex_util_y.y"
    { display_production ("term", "factor") ;  rrval = rrvsp[0] ; }
    break;

  case 7:
#line 305 "rex_util_y.y"
    { display_production ("term", "factor term") ;
            /* Link the last state in RE1 to the first state in RE2.  (The
               first state in "RE1 RE2" is the first state in RE1.) */
            rtx->state_list[last_of (rrvsp[-1])].next1 = rrvsp[0] ;
            rrval = rrvsp[-1] ;
          }
    break;

  case 8:
#line 316 "rex_util_y.y"
    { display_production ("factor", "^") ;
            /* Add an anchor state and set its match character to "^". */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = anchor ;
            rtx->state_list[rtx->num_states].x.match_char = '^' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 9:
#line 326 "rex_util_y.y"
    { display_production ("factor", "$") ;
            /* Add an anchor state and set its match character to "$". */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = anchor ;
            rtx->state_list[rtx->num_states].x.match_char = '$' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 10:
#line 336 "rex_util_y.y"
    { display_production ("factor", "factor{[min][,[max]]}") ;
            /* Check for nested closures that might cause the matching
               algorithm to endlessly loop without consuming any input
               from the target string; e.g., "(a*){0,10}".  Actually,
               the maximum field puts a cap on the number of loops, but
               we'll disallow it anyway. */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-4]) ;
            LGI "(rrparse) Shortest path from state %d = %d\n", rrvsp[-4], i) ;
            if (i <= 0) {
                SET_ERRNO (EINVAL) ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state and set its minimum and maximum fields.
               Link the last state in RE to the closure state; link the closure
               state backwards to the first state in RE.  (The first state in
               "RE{min,max}" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-4])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            if (rrvsp[-2] < 0)  rrvsp[-2] = 0 ;
            rtx->state_list[rtx->num_states].x.min_closure = rrvsp[-2] ;
            if (rrvsp[-1] < 0)  rrvsp[-1] = rrvsp[-2] ;
            rtx->state_list[rtx->num_states].y.max_closure = rrvsp[-1] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-4] ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 11:
#line 366 "rex_util_y.y"
    { rrparse_expecting_assignment = -1 ; }
    break;

  case 12:
#line 368 "rex_util_y.y"
    { display_production ("factor", "(regular_expression) <assignment>") ;
            /* If an assignment clause ("$<digit>") was specified, add two
               parenthesis states, P1 and P2.  Link P1 to the first state
               in RE; link the last state in RE to P2.  Store the argument
               index (<digit>) in P1 and store the index of P1 in P2.  If an
               assignment clause was NOT specified, nothing needs to be done.
               (The first state in "(RE)" is the first state in RE; the first
                state in "(RE)$n" is P1.) */
            if (rrvsp[0] < 0) {				/* "(RE)" */
                rrval = rrvsp[-3] ;
            } else {					/* "(RE)$n" */
                NEW_STATE ;
                rtx->state_list[rtx->num_states].type = left_paren ;	/* P1 */
                rtx->state_list[rtx->num_states].x.subexp_index = rrvsp[0] ;
                rtx->state_list[rtx->num_states].y.subexp_state =
                    rtx->num_states + 1 ;
                rtx->state_list[rtx->num_states].next1 = rrvsp[-3] ;
                rtx->state_list[rtx->num_states].next2 = -1 ;
                if (rtx->num_args < (rrvsp[0]+1))  rtx->num_args = rrvsp[0] + 1 ;
                rrval = rtx->num_states++ ;
                NEW_STATE ;
                rtx->state_list[last_of (rrvsp[-3])].next1 = rtx->num_states ;
                rtx->state_list[rtx->num_states].type = right_paren ;	/* P2 */
                rtx->state_list[rtx->num_states].x.subexp_index = rrvsp[0] ;
                rtx->state_list[rtx->num_states].y.subexp_state = rrval ;
                rtx->state_list[rtx->num_states].next1 = -1 ;
                rtx->state_list[rtx->num_states].next2 = -1 ;
                rtx->num_states++ ;
            }
          }
    break;

  case 13:
#line 399 "rex_util_y.y"
    { display_production ("factor", "factor*") ;
            /* Check for nested closures that might cause the matching
               algorithm to endlessly loop without consuming any input
               from the target string; e.g., "(a*)*". */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-1]) ;
            LGI "(rrparse) Shortest path from state %d = %d\n", rrvsp[-1], i) ;
            if (i <= 0) {
                SET_ERRNO (EINVAL) ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state.  Link the last state in RE to the closure
               state; link the closure state backwards to the first state in
               RE.  (The first state in "RE*" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-1])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            rtx->state_list[rtx->num_states].x.min_closure = 0 ;
            rtx->state_list[rtx->num_states].y.max_closure = INT_MAX ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-1] ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 14:
#line 424 "rex_util_y.y"
    { display_production ("factor", "factor+") ;
            /* Check for nested closures that might cause the matching
               algorithm to endlessly loop without consuming any input
               from the target string; e.g., "(a*)+". */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-1]) ;
            LGI "(rrparse) Shortest path from state %d = %d\n", rrvsp[-1], i) ;
            if (i <= 0) {
                SET_ERRNO (EINVAL) ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state.  Link the last state in RE to the closure
               state; link the closure state backwards to the first state in
               RE.  (The first state in "RE+" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-1])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            rtx->state_list[rtx->num_states].x.min_closure = 1 ;
            rtx->state_list[rtx->num_states].y.max_closure = INT_MAX ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-1] ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 15:
#line 449 "rex_util_y.y"
    { display_production ("factor", "factor?") ;
            /* Check for nested closures.  Even though nested closures are
               not a major threat in a zero-or-one closure, we check anyway,
               just to be consistent with the other forms of closure. */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-1]) ;
            LGI "(rrparse) Shortest path from state %d = %d\n", rrvsp[-1], i) ;
            if (i <= 0) {
                SET_ERRNO (EINVAL) ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state.  Link the last state in RE to the closure
               state; link the closure state backwards to the first state in
               RE.  (The first state in "RE?" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-1])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            rtx->state_list[rtx->num_states].x.min_closure = 0 ;
            rtx->state_list[rtx->num_states].y.max_closure = 1 ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-1] ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 16:
#line 474 "rex_util_y.y"
    { CS_ZERO (&char_class) ;
            rrparse_after_bracket = 1 ;
            rrparse_hyphen_as_literal = 0 ;
          }
    break;

  case 17:
#line 479 "rex_util_y.y"
    { display_production ("factor", "[character_classes]") ;
            rrparse_after_bracket = 0 ;
            if (rrvsp[-2]) {
                for (i = 0 ;  i < CS_SETSIZE ;  i++) {
                    if (CS_ISSET (i, &char_class))
                        CS_CLR (i, &char_class) ;
                    else
                        CS_SET (i, &char_class) ;
                }
            }
            LGI "Character Class:\n") ;
            if (I_DEFAULT_GUARD) {
                rex_dump_class (stdout, "    Matches: ", &char_class) ;
            }
            /* Add a multi-character state for the character class. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = 0 ;
            rtx->state_list[rtx->num_states].y.match_charset =
                (cs_set *) malloc (sizeof (cs_set)) ;
            if (rtx->state_list[rtx->num_states].y.match_charset == NULL) {
                LGE "(rrparse) Error allocating character class set.\nmalloc: ") ;
                rex_error_text = "error allocating character class set" ;
                return (errno) ;
            }
            CS_ZERO (rtx->state_list[rtx->num_states].y.match_charset) ;
            for (i = 0 ;  i < CS_SETSIZE ;  i++)
                if (CS_ISSET (i, &char_class))
                    CS_SET (i, rtx->state_list[rtx->num_states].y.match_charset) ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 18:
#line 513 "rex_util_y.y"
    { display_production ("factor", "SINGLE_CHARACTER") ;
            /* Add a one-character state for the character. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = rrvsp[0] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 19:
#line 523 "rex_util_y.y"
    { display_production ("factor", "COMMA") ;
            /* Add a one-character state for ",". */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = rrvsp[0] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 20:
#line 533 "rex_util_y.y"
    { display_production ("factor", "DIGIT") ;
            /* Add a one-character state for the digit. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = rrvsp[0] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 21:
#line 543 "rex_util_y.y"
    { display_production ("factor", "ANY_CHARACTER") ;
            /* Add a one-character state for any-character-matches. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = -1 ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
    break;

  case 22:
#line 557 "rex_util_y.y"
    { display_production ("assignment", "<empty>") ;  rrval = -1 ; }
    break;

  case 23:
#line 559 "rex_util_y.y"
    { display_production ("assignment", "$<0-9>") ;
            sprintf (buffer, "%c", rrvsp[0]) ;  rrval = atoi (buffer) ;
          }
    break;

  case 24:
#line 567 "rex_util_y.y"
    { display_production ("complement", "<empty>") ;  rrval = 0 ; }
    break;

  case 25:
#line 569 "rex_util_y.y"
    { display_production ("complement", "^") ;  rrval = -1 ; }
    break;

  case 26:
#line 575 "rex_util_y.y"
    { display_production ("number", "<empty>") ;  rrval = -1 ; }
    break;

  case 27:
#line 577 "rex_util_y.y"
    { display_production ("number", "digits") ;  rrval = rrvsp[0] ; }
    break;

  case 28:
#line 583 "rex_util_y.y"
    { display_production ("upper_bound", "<empty>") ;
            rrval = -1 ;			/* Upper bound = lower bound. */
          }
    break;

  case 29:
#line 587 "rex_util_y.y"
    { display_production ("lower_bound", "digits") ;
            if (rrvsp[0] < 0)
                rrval = INT_MAX ;		/* Upper bound = infinity. */
            else
                rrval = rrvsp[0] ;		/* Normal upper bound. */
          }
    break;

  case 30:
#line 598 "rex_util_y.y"
    { display_production ("digits", "DIGIT") ;
            sprintf (buffer, "%c", rrvsp[0]) ;  rrval = atoi (buffer) ;
          }
    break;

  case 31:
#line 602 "rex_util_y.y"
    { display_production ("digits", "digits DIGIT") ;
            sprintf (buffer, "%c", rrvsp[0]) ;  rrval = (rrvsp[-1] * 10) + atoi (buffer) ;
          }
    break;

  case 32:
#line 610 "rex_util_y.y"
    { display_production ("character_classes", "character_class") ; }
    break;

  case 33:
#line 612 "rex_util_y.y"
    { display_production ("character_classes", "character_classes character_class") ; }
    break;

  case 34:
#line 618 "rex_util_y.y"
    { display_production ("character_class", "character") ;
            CS_SET (rrvsp[0], &char_class) ;
          }
    break;

  case 35:
#line 622 "rex_util_y.y"
    { rrparse_hyphen_as_literal = 1 ; }
    break;

  case 36:
#line 624 "rex_util_y.y"
    { display_production ("character_class", "character-character") ;
            rrparse_hyphen_as_literal = 0 ;
            if (rrvsp[-3] > rrvsp[0]) {
                LGI "(rrparse) Start character exceeds end character in character class range \"%c-%c\".\n", rrvsp[-3], rrvsp[0]) ;
                SET_ERRNO (EINVAL) ;
                rex_error_text = "inverted character class range" ;
                return (errno) ;
            }
            for (i = rrvsp[-3] ;  i <= rrvsp[0] ;  i++)
                CS_SET (i, &char_class) ;
          }
    break;

  case 37:
#line 636 "rex_util_y.y"
    { display_production ("character_class", ":<class>:") ;
            for (i = 0 ;  i < CS_SETSIZE ;  i++) {
                switch (rrvsp[0]) {
                case  0:  if (isalnum (i))  CS_SET (i, &char_class) ;  break ;
                case  1:  if (isalpha (i))  CS_SET (i, &char_class) ;  break ;
                case  2:  if (iscntrl (i))  CS_SET (i, &char_class) ;  break ;
                case  3:  if (isdigit (i))  CS_SET (i, &char_class) ;  break ;
                case  4:  if (isgraph (i))  CS_SET (i, &char_class) ;  break ;
                case  5:  if (islower (i))  CS_SET (i, &char_class) ;  break ;
                case  6:  if (isprint (i))  CS_SET (i, &char_class) ;  break ;
                case  7:  if (ispunct (i))  CS_SET (i, &char_class) ;  break ;
                case  8:  if (isspace (i))  CS_SET (i, &char_class) ;  break ;
                case  9:  if (isupper (i))  CS_SET (i, &char_class) ;  break ;
                case 10:  if (isxdigit (i)) CS_SET (i, &char_class) ;  break ;
                }
            }
          }
    break;

  case 38:
#line 658 "rex_util_y.y"
    { display_production ("character", "SINGLE_CHARACTER") ;  rrval = rrvsp[0] ; }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 1736 "y.tab.c"

  rrvsp -= rrlen;
  rrssp -= rrlen;


  RR_STACK_PRINT (rrss, rrssp);

  *++rrvsp = rrval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  rrn = rrr1[rrn];

  rrstate = rrpgoto[rrn - RRNTOKENS] + *rrssp;
  if (0 <= rrstate && rrstate <= RRLAST && rrcheck[rrstate] == *rrssp)
    rrstate = rrtable[rrstate];
  else
    rrstate = rrdefgoto[rrn - RRNTOKENS];

  goto rrnewstate;


/*------------------------------------.
| rrerrlab -- here on detecting error |
`------------------------------------*/
rrerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!rrerrstatus)
    {
      ++rrnerrs;
#if RRERROR_VERBOSE
      rrn = rrpact[rrstate];

      if (RRPACT_NINF < rrn && rrn < RRLAST)
	{
	  RRSIZE_T rrsize = 0;
	  int rrtype = RRTRANSLATE (rrchar);
	  const char* rrprefix;
	  char *rrmsg;
	  int rrx;

	  /* Start RRX at -RRN if negative to avoid negative indexes in
	     RRCHECK.  */
	  int rrxbegin = rrn < 0 ? -rrn : 0;

	  /* Stay within bounds of both rrcheck and rrtname.  */
	  int rrchecklim = RRLAST - rrn;
	  int rrxend = rrchecklim < RRNTOKENS ? rrchecklim : RRNTOKENS;
	  int rrcount = 0;

	  rrprefix = ", expecting ";
	  for (rrx = rrxbegin; rrx < rrxend; ++rrx)
	    if (rrcheck[rrx + rrn] == rrx && rrx != RRTERROR)
	      {
		rrsize += rrstrlen (rrprefix) + rrstrlen (rrtname [rrx]);
		rrcount += 1;
		if (rrcount == 5)
		  {
		    rrsize = 0;
		    break;
		  }
	      }
	  rrsize += (sizeof ("syntax error, unexpected ")
		     + rrstrlen (rrtname[rrtype]));
	  rrmsg = (char *) RRSTACK_ALLOC (rrsize);
	  if (rrmsg != 0)
	    {
	      char *rrp = rrstpcpy (rrmsg, "syntax error, unexpected ");
	      rrp = rrstpcpy (rrp, rrtname[rrtype]);

	      if (rrcount < 5)
		{
		  rrprefix = ", expecting ";
		  for (rrx = rrxbegin; rrx < rrxend; ++rrx)
		    if (rrcheck[rrx + rrn] == rrx && rrx != RRTERROR)
		      {
			rrp = rrstpcpy (rrp, rrprefix);
			rrp = rrstpcpy (rrp, rrtname[rrx]);
			rrprefix = " or ";
		      }
		}
	      rrerror (rrmsg);
	      RRSTACK_FREE (rrmsg);
	    }
	  else
	    rrerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* RRERROR_VERBOSE */
	rrerror ("syntax error");
    }



  if (rrerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (rrchar <= RREOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (rrchar == RREOF)
	     for (;;)
	       {
		 RRPOPSTACK;
		 if (rrssp == rrss)
		   RRABORT;
		 RRDSYMPRINTF ("Error: popping", rrstos[*rrssp], rrvsp, rrlsp);
		 rrdestruct (rrstos[*rrssp], rrvsp);
	       }
        }
      else
	{
	  RRDSYMPRINTF ("Error: discarding", rrtoken, &rrlval, &rrlloc);
	  rrdestruct (rrtoken, &rrlval);
	  rrchar = RREMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto rrerrlab1;


/*---------------------------------------------------.
| rrerrorlab -- error raised explicitly by RRERROR.  |
`---------------------------------------------------*/
rrerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes RRERROR and the label
     rrerrorlab therefore never appears in user code.  */
  if (0)
     goto rrerrorlab;
#endif

  rrvsp -= rrlen;
  rrssp -= rrlen;
  rrstate = *rrssp;
  goto rrerrlab1;


/*-------------------------------------------------------------.
| rrerrlab1 -- common code for both syntax error and RRERROR.  |
`-------------------------------------------------------------*/
rrerrlab1:
  rrerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      rrn = rrpact[rrstate];
      if (rrn != RRPACT_NINF)
	{
	  rrn += RRTERROR;
	  if (0 <= rrn && rrn <= RRLAST && rrcheck[rrn] == RRTERROR)
	    {
	      rrn = rrtable[rrn];
	      if (0 < rrn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (rrssp == rrss)
	RRABORT;

      RRDSYMPRINTF ("Error: popping", rrstos[*rrssp], rrvsp, rrlsp);
      rrdestruct (rrstos[rrstate], rrvsp);
      RRPOPSTACK;
      rrstate = *rrssp;
      RR_STACK_PRINT (rrss, rrssp);
    }

  if (rrn == RRFINAL)
    RRACCEPT;

  RRDPRINTF ((stderr, "Shifting error token, "));

  *++rrvsp = rrlval;


  rrstate = rrn;
  goto rrnewstate;


/*-------------------------------------.
| rracceptlab -- RRACCEPT comes here.  |
`-------------------------------------*/
rracceptlab:
  rrresult = 0;
  goto rrreturn;

/*-----------------------------------.
| rrabortlab -- RRABORT comes here.  |
`-----------------------------------*/
rrabortlab:
  rrresult = 1;
  goto rrreturn;

#ifndef rroverflow
/*----------------------------------------------.
| rroverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
rroverflowlab:
  rrerror ("parser stack overflow");
  rrresult = 2;
  /* Fall through.  */
#endif

rrreturn:
#ifndef rroverflow
  if (rrss != rrssa)
    RRSTACK_FREE (rrss);
#endif
  return rrresult;
}


#line 662 "rex_util_y.y"


/*!*****************************************************************************
    Function RRERROR is invoked automatically by RRPARSE when an error is
    detected.  RRERROR simply prints out the error message passed to it by
    RRPARSE.
*******************************************************************************/

static  int  rrerror (

#    if PROTOTYPES
        char  *s)
#    else
        s)

        char  *s ;
#    endif

{

    LGI "(rrparse) %s\n", s) ;
    SET_ERRNO (EINVAL) ;  rex_error_text = s ;
    return (errno) ;

}

/*!*****************************************************************************
    Function RRLEX returns the next token of input.  This function is normally
    generated by LEX, but, for this parser, it's simple enough to do by hand.
*******************************************************************************/


static  int  rrlex (

#    if PROTOTYPES && !defined(__cplusplus)
        void)
#    else
        )
#    endif

{    /* Local variables. */
    char  buffer[8] ;
    unsigned  char  c ;
    unsigned  int  i ;




    if (beginning_of_input ()) {		/* Reset lexical flags. */
        rrparse_after_bracket = 0 ;
        rrparse_expecting_assignment = 0 ;
        rrparse_hyphen_as_literal = 0 ;
    }

    c = (unsigned char) input () ;  rrlval = c ;

/* At the end of the string, return an end marker. */

    if (c == '\0') {
        display_token ("END_MARKER", '0') ;  return (0) ;
    }

/* Within a bracket expression ("[...]"), most characters are interpreted
   literally.  When the left bracket is encountered, RRPARSE sets AFTER_BRACKET
   to 1.  A "^" immediately after the left bracket is returned as the character
   class complement indicator and AFTER_BRACKET is set to -1.  A right bracket
   or hyphen immediately after the left bracket (AFTER_BRACKET == 1) or the
   complement character (AFTER_BRACKET == -1) is returned as a literal
   character.  A hyphen following a hyphen ("[a--]") or preceding the right
   bracket ("[a-]") is returned as a literal character.  A colon (":")
   immediately after the left bracket or the complement character is the
   start of a character class name specification (":<class>:"); the class
   name is extracted, looked up in the class name table, and its index is
   returned to RRPARSE.  "\<c>" constructs are handled by the non-bracket
   expression SWITCH statement further down below. */

    if (rrparse_after_bracket) {
        switch (c) {
        case '^':
            if (rrparse_after_bracket == 1) {
                rrparse_after_bracket = -1 ;
                display_token ("CARAT", c) ;  return (CARAT) ;
            }
            if (rrparse_after_bracket == -1)  rrparse_after_bracket = 1 ;
            rrparse_after_bracket++ ;
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        case ']':
            if (rrparse_after_bracket++ > 1) {
                display_token ("RIGHT_BRACKET", c) ;  return (RIGHT_BRACKET) ;
            }
            rrparse_after_bracket = 2 ;
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        case '-':				/* Is "-" followed by a "]"? */
            c = (unsigned char) input () ;  unput (c) ;
            if ((rrparse_after_bracket++ > 1) &&
                (!rrparse_hyphen_as_literal) && (c != ']')) {
                display_token ("HYPHEN", c) ;  return (HYPHEN) ;
            }
            rrparse_after_bracket = 2 ;
            display_token ("SINGLE_CHARACTER", '-') ;
            return (SINGLE_CHARACTER) ;
        case ':':
            if (rrparse_after_bracket++ > 1) {
                display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
            }
            rrparse_after_bracket = 2 ;
				/* Extract class name from ":<class>:". */
            for (i = 0 ;  (c = (unsigned char) input ()) && (c != ':') ;  )
                if (i < (sizeof buffer - 1))  buffer[i++] = c ;
            buffer[i] = '\0' ;  strToLower (buffer, -1) ;
				/* Lookup name in class name table. */
            for (i = 0 ;  class_name[i] != NULL ;  i++)
                if (strcmp (buffer, class_name[i]) == 0)  break ;
            rrlval = i ;  return (CTYPE) ;
        case '\\':
            if (rrparse_after_bracket == -1)  rrparse_after_bracket = 1 ;
            rrparse_after_bracket++ ;  rrparse_after_bracket++ ;
            break ;
        default:
            if (rrparse_after_bracket == -1)  rrparse_after_bracket = 1 ;
            rrparse_after_bracket++ ;
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        }
    }


/* Outside of a bracket expression, characters receive the standard regular
   expression interpretation. */

    switch (c) {
    case '.':
        display_token ("ANY_CHARACTER", c) ;  return (ANY_CHARACTER) ;
    case '^':
        display_token ("BOL_ANCHOR", c) ;  return (BOL_ANCHOR) ;
    case ',':
        display_token ("COMMA", c) ;  return (COMMA) ;
    case '$':
        c = (unsigned char) input () ;  unput (c) ;
        if (rrparse_expecting_assignment && isdigit (c)) {
            rrparse_expecting_assignment = 0 ;
            display_token ("ASSIGNMENT", '$') ;  return (ASSIGNMENT) ;
        } else {
            rrparse_expecting_assignment = 0 ;
            display_token ("EOL_ANCHOR", '$') ;  return (EOL_ANCHOR) ;
        }
    case '|':
        display_token ("OR", c) ;  return (OR) ;
    case '*':
        display_token ("ZERO_OR_MORE", c) ;  return (ZERO_OR_MORE) ;
    case '+':
        display_token ("ONE_OR_MORE", c) ;  return (ONE_OR_MORE) ;
    case '?':
        display_token ("ZERO_OR_ONE", c) ;  return (ZERO_OR_ONE) ;
    case '(':
        display_token ("LEFT_PAREN", c) ;  return (LEFT_PAREN) ;
    case ')':
        display_token ("RIGHT_PAREN", c) ;  return (RIGHT_PAREN) ;
    case '{':
        display_token ("LEFT_BRACE", c) ;  return (LEFT_BRACE) ;
    case '}':
        display_token ("RIGHT_BRACE", c) ;  return (RIGHT_BRACE) ;
    case '[':
        display_token ("LEFT_BRACKET", c) ;  return (LEFT_BRACKET) ;
    case '\\':
        c = input () ;  rrlval = c ;
        switch (c) {
        case 'n':  rrlval = '\n' ;  break ;
        case 'r':  rrlval = '\n' ;  break ;
        case 't':  rrlval = '\t' ;  break ;
        default:   break ;
        }
        if (c == '\0') {
            unput (c) ;  display_token ("_ERROR", '0') ;  return (_ERROR) ;
        } else {
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        }
    default:
        if (isdigit (c)) {
            display_token ("DIGIT", c) ;  return (DIGIT) ;
        } else {
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        }
    }

}

/*!*****************************************************************************
    Function ALLOCATE_STATE simply checks if there is enough room to add a new
    state to the state list.  If there is enough room, nothing is done; the
    calling routine is responsible for incrementing the NUM_STATES pointer.
    If there is not enough room, the size of the state list is increased.
    Zero is returned if the calling routine can now add a state; ERRNO is
    returned if ALLOCATE_STATE could not resize the state list.
*******************************************************************************/

#define  INCREMENT  4

static  int  allocate_state (

#    if PROTOTYPES && !defined(__cplusplus)
        void)
#    else
        )
#    endif

{    /* Local variables. */
    char  *s ;
    int  size ;



    if (rtx->num_states >= rtx->max_states) {
        size = (rtx->max_states + INCREMENT) * sizeof (StateNode) ;
        if (rtx->state_list == NULL)
            s = (char *) malloc (size) ;
        else
            s = (char *) realloc ((char *) rtx->state_list, size) ;
        if (s == NULL) {
            LGE "(allocate_state) Error reallocating the state list.\nrealloc: ") ;
            return (errno) ;
        }
        rtx->state_list = (StateNode *) s ;
        rtx->max_states = rtx->max_states + INCREMENT ;
    }

    return (0) ;

}

/*!*****************************************************************************
    Function FIRST_CHAR_OF computes the set of characters that may appear at
    the beginning of a string that could be matched by a regular expression.
    For example, the set of first characters for "abc" is simply "a"; the set
    for "[A-Za-z][A-Za-z0-9]*" contains "A" through "Z" and "a" through "z".
    The set of first characters for "a*" is the set of all characters, since
    the regular expression could still match a zero-length string if the
    target string begins with any character other than "a".  FIRST_CHAR_OF
    returns a function value of zero if the computed set does not contain
    all characters; a non-zero value is returned if the computed set does
    contain all characters.
*******************************************************************************/


static  int  first_char_of (

#    if PROTOTYPES
        int  state,
        cs_set  *first_set)
#    else
        state, first_set)

        int  state ;
        cs_set  *first_set ;
#    endif

{    /* Local variables. */
    int  i ;



    LGI "(first_char_of) State = %d\n", state) ;
    if ((state < 0) || (rtx->state_list[state].z.visited))  return (0) ;

/* Mark the state as visited during the traversal.  The VISITED flags keep
   the program from looping endlessly on cycles in the RE's graph (e.g.,
   closure states). */

    rtx->state_list[state].z.visited = 1 ;

/* Compute the set of first characters, starting with this state and proceeding
   to the end of the RE's graph. */

    switch (rtx->state_list[state].type) {
    case empty:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case anchor:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case alternation:
        if (first_char_of (rtx->state_list[state].next1, first_set))
            return (1) ;
        return (first_char_of (rtx->state_list[state].next2, first_set)) ;
    case closure:
        first_char_of (rtx->state_list[state].next2, first_set) ;
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case final:
        for (i = 0 ;  i < CS_SETSIZE ;  i++)
            CS_SET (i, first_set) ;
        return (1) ;
    case match:
        if (rtx->state_list[state].x.match_char < 0) {
            for (i = 0 ;  i < CS_SETSIZE ;  i++)
                CS_SET (i, first_set) ;
            return (1) ;
        } else if (rtx->state_list[state].x.match_char == 0) {
            for (i = 0 ;  i < CS_SETSIZE ;  i++) {
                if (CS_ISSET (i, rtx->state_list[state].y.match_charset))
                    CS_SET (i, first_set) ;
            }
        } else {
            CS_SET (rtx->state_list[state].x.match_char, first_set) ;
        }
        return (0) ;
    case left_paren:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case right_paren:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    default:
        return (0) ;
    }

}

/*!*****************************************************************************
    Function LAST_OF locates the last state in a regular expression graph.
    LAST_OF, passed the index of the first state in the graph, simply traverses
    the NEXT1 links until a null link (index = -1) is found.  Note that this
    technique assumes that the NEXT1 links lead to the last state in the
    graph; RRPARSE adheres to this convention and uses NEXT2 links only for
    backtracking in closure (RE* and RE+) expressions.
*******************************************************************************/

static  int  last_of (

#    if PROTOTYPES
        int  state)		/* The first state in the RE graph. */
#    else
        state)

        int  state ;		/* The first state in the RE graph. */
#    endif

{

    while (rtx->state_list[state].next1 >= 0)
        state = rtx->state_list[state].next1 ;

    return (state) ;

}

/*!*****************************************************************************
    Function LONGEST_PATH computes the longest path through an RE.  The
    longest path is an estimate of the maximum number of states that would
    be visited during an attempt to match a target string.
*******************************************************************************/


static  int  longest_path (

#    if PROTOTYPES
        int  state)
#    else
        state)

        int  state ;
#    endif

{    /* Local variables. */
    int  i, j ;



    LGI "(longest_path) State = %d\n", state) ;
    if ((state < 0) || (rtx->state_list[state].z.visited))  return (0) ;

/* Mark the state as visited during the traversal.  The VISITED flags keep
   the program from looping endlessly on cycles in the RE's graph (e.g.,
   closure states). */

    rtx->state_list[state].z.visited = 1 ;

/* Compute the longest path from this state to the end of the RE's graph. */

    switch (rtx->state_list[state].type) {
    case empty:
        return (longest_path (rtx->state_list[state].next1)) ;
    case anchor:
        return (longest_path (rtx->state_list[state].next1)) ;
    case alternation:
        i = 1 + longest_path (rtx->state_list[state].next1) ;
        j = 1 + longest_path (rtx->state_list[state].next2) ;
        return ((i > j) ? i : j) ;
    case closure:
        return (longest_path (rtx->state_list[state].next2) + 2 +
                longest_path (rtx->state_list[state].next1)) ;
    case final:
        return (0) ;
    case match:
        return (1 + longest_path (rtx->state_list[state].next1)) ;
    case left_paren:
        return (1 + longest_path (rtx->state_list[state].next1)) ;
    case right_paren:
        return (1 + longest_path (rtx->state_list[state].next1)) ;
    default:
        return (0) ;
    }

}

/*!*****************************************************************************
    Function SHORTEST_PATH computes the shortest path through an RE.  The
    shortest path equals the minimum length of a target string that would
    be matched by the RE.
*******************************************************************************/


static  int  shortest_path (

#    if PROTOTYPES
        int  state)
#    else
        state)

        int  state ;
#    endif

{    /* Local variables. */
    int  i, j ;



    LGI "(shortest_path) State = %d\n", state) ;
    if ((state < 0) || (rtx->state_list[state].z.visited))  return (0) ;

/* Mark the state as visited during the traversal.  The VISITED flags keep
   the program from looping endlessly on cycles in the RE's graph (e.g.,
   closure states). */

    rtx->state_list[state].z.visited = 1 ;

/* Compute the shortest path from this state to the end of the RE's graph. */

    switch (rtx->state_list[state].type) {
    case empty:
        return (shortest_path (rtx->state_list[state].next1)) ;
    case anchor:
        return (shortest_path (rtx->state_list[state].next1)) ;
    case alternation:
        i = shortest_path (rtx->state_list[state].next1) ;
        j = shortest_path (rtx->state_list[state].next2) ;
        return ((i < j) ? i : j) ;
    case closure:
        return ((shortest_path (rtx->state_list[state].next2) *
                 rtx->state_list[state].x.min_closure)  +
                shortest_path (rtx->state_list[state].next1)) ;
    case final:
        return (0) ;
    case match:
        return (1 + shortest_path (rtx->state_list[state].next1)) ;
    case left_paren:
        return (shortest_path (rtx->state_list[state].next1)) ;
    case right_paren:
        return (shortest_path (rtx->state_list[state].next1)) ;
    default:
        return (0) ;
    }

}
