/* Copyright 2010 IPB, INRIA & CNRS
**
** This file originally comes from the Scotch software package for
** static mapping, graph partitioning and sparse matrix ordering.
**
** This software is governed by the CeCILL-B license under French law
** and abiding by the rules of distribution of free software. You can
** use, modify and/or redistribute the software under the terms of the
** CeCILL-B license as circulated by CEA, CNRS and INRIA at the following
** URL: "http://www.cecill.info".
** 
** As a counterpart to the access to the source code and rights to copy,
** modify and redistribute granted by the license, users are provided
** only with a limited warranty and the software's author, the holder of
** the economic rights, and the successive licensors have only limited
** liability.
** 
** In this respect, the user's attention is drawn to the risks associated
** with loading, using, modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean that it is complicated to manipulate, and that also
** therefore means that it is reserved for developers and experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards
** their requirements in conditions enabling the security of their
** systems and/or data to be ensured and, more generally, to use and
** operate it in the same conditions as regards security.
** 
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-B license and that you accept its terms.
*/
/************************************************************/
/**                                                        **/
/**   NAME       : fibo.h                                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is a sample test program    **/
/**                for the generic Fibonacci trees module. **/
/**                                                        **/
/**   DATES      : # Version 1.0  : from : 01 may 2010     **/
/**                                 to     12 may 2010     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define TEST_FIBO

#include <malloc.h>
#include <stdio.h>
#include <sys/time.h>

#include "fibo.h"

/*
**  The type and structure definitions.
*/

/* Sample data structure which uses the Fibonacci
   tree module to sort some of its properties.    */

typedef struct Data_ {
  double              dummy1;
  FiboNode            node;                       /* Can be everywhere in the data structure */
  int                 dummy2;
  int                 data;                       /* Can be everywhere in the data structure */
  float               dummy3;
} Data;

Data                  dummy;                      /* Dummy data for pointer offset computations */

/* The comparison function which is used to sort
** "Data" structures. Since only "FiboNode" pointers
** are passed, pointer offset computations are
** necessary to access the whole "Data" structures,
** with all their fields. Since this Fibonacci tree
** implementation sorts data by ascending order,
** the comparison function must return a negative
** number if "node1" has to be returned before
** "node2" when extracting sorted data from the
** Fibonacci tree structure.
*/

static
int
cmpFunc (
const FiboNode * const      node1,
const FiboNode * const      node2)
{
  const Data * const  data1 = (Data *) ((char *) node1 - (((char *) &dummy.node) - ((char *) &dummy)));
  const Data * const  data2 = (Data *) ((char *) node2 - (((char *) &dummy.node) - ((char *) &dummy)));

/* Any kind of criteria can be used for sorting, provided
   that the comparison function guarantees a total order
   on the elements.                                       */

  return ((data1->data < data2->data) ? -1 : 1);
}

/* Sample insertion and extraction sequence. */

static int            sequence[] = { 2, 5, 12, 3, 6, -1, 2, 34, -1, -1, 6, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2 };

/*********************/
/*                   */
/* The main routine. */
/*                   */
/*********************/

int
main (
int                 argc,
char *              argv[])
{
  FiboTree            treedat;
  FiboNode *          nodeptr;
  Data *              dataptr;
  int                 i;

  fprintf (stderr, "Initializing tree\n");
  if (fiboTreeInit (&treedat, cmpFunc) != 0) {
    fprintf (stderr, "Cannot initialize tree\n");
    return  (1);
  }

  for (i = 0; sequence[i] != -2; i ++) {
    if (sequence[i] >= 0) {

/* In this example, we allocate "Data" elements one
   by one by means of individual malloc's, but they
   could have been allocated as a whole as a single
   array of "Data" structures.                      */

      fprintf (stderr, "Inserting : \"%d\"\n", sequence[i]);
      dataptr = malloc (sizeof (Data));
      dataptr->data = sequence[i];
      fiboTreeAdd (&treedat, &dataptr->node);
    }
    else {
      fprintf (stderr, "Getting min : ");
      nodeptr = fiboTreeMin (&treedat);
      if (nodeptr == NULL)
        fprintf (stderr, "TREE IS EMPTY\n");
      else {
        fiboTreeDel (&treedat, nodeptr);

/* We always need this sort of data offset computations
   in order to get the pointer to the "Data" structure
   from the pointer to the "FiboNode" structure. A way
   to simplify it is to place the "FiboNode" structure
   at the very beginning of the "Data" structure. In
   this case, only a pointer type cast is required.     */

        dataptr = (Data *) ((char *) nodeptr - (((char *) &dummy.node) - ((char *) &dummy)));
        fprintf (stderr, "\"%d\"\n", dataptr->data);
        free (dataptr);
      }
    }
  }

  fiboTreeExit (&treedat);                        /* Necessary to free the internal consolidation array */

  return (0);
}
