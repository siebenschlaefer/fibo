# Fibonacci heap (or Fibonacci trees)

In the course of the development of my main research project, [Scotch](https://web.archive.org/web/20190723125631/http://www.labri.fr/~pelegrin/scotch/), I needed to sort elements according to several criteria, which might not always be single integer values. Consequently, the bucket sorting algorithms that I used before were not adequate. After looking at the [Red-Black tree](http://en.wikipedia.org/wiki/Red-black_tree) data structure (a thoughtful description and implementation of which can be found [here](https://web.archive.org/web/20190723125631/http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx)), I turned to the [Fibonacci heap](http://en.wikipedia.org/wiki/Fibonacci_heap) (also called «Fibonacci trees», since it is technically a forest of trees with given degree properties), which better fitted my needs. Indeed, in the context of graph partitioning, I planned to use this data structure to select the best vertex to move from one part to another, depending on the improvement such a move may bring to the current partition. Such local optimisation algorithms are classically referred to as «Fiduccia-Mattheyses» or «Kernighan-Lin», depending on the way vertices are moved or swapped between parts. In such algorithms, once a vertex of best gain value has been selected for moving, the gains of all its neighbors have to be recomputed. Consequently, the data structure will be subject to much more deletion and insertion operations of «unsorted» arbitrary elements than to searches for, and deletions of, elements of best gain value. By adding and removing arbitrary vertices quickly (in constant time for addition), and by postponing the expensive sorting operations up to the moment when searching for the element of extremal value, Fibonacci trees are just what I needed when list buckets cannot be used.

However, all of the existing free/libre implementations that I could find on the Internet had two drawbacks. First, all of them were recursive, incurring a heavy recursion cost for a few useful instructions per function call. Second, all of them treated elements as independent data structures, which were internally allocated and freed at insertion and deletion time. Both of these features made them completely inappropriate for the kind of heavy use that I had in mind, for which such routines have to be called millions of times, and vertex elements are already embodied within data structures such as hash table arrays (yes, high performance computing can be real fun in terms of data structures!).

Consequently, I wrote my own, de-recursived, implementation of the Fibonacci trees data structure. The code is small, as this is really a very elegant data structure. Save for the pointer array internally used by the consolidation routine, no memory allocation is performed by this module. The allocation and freeing of the individual elements is left to the users of the module, who can opt for the kind of memory handling that they want. Unlike most implementations, the element of minimum value is not internally recorded, as I had no need to preserve it; however, module users can easily implement this feature on top of the existing routines. All module routines, except the consolidation routine which computes the current extremal value, are written as macros, which can be used as is or embodied within regular functions, depending on the pre-definition of symbolic constants.

Fibonacci trees are handled by means of a `FiboTree` data structure. Elements to be inserted and searched for must comprise a `FiboNode` substructure. Elements can belong to several tree structures, provided they possess as many `FiboNode` substructures as needed, and provided that users have a means for determining the offset of the element structure from the pointer to the `FiboNode` which is returned by the module routines. The module routines are the following:

<dl>
<dt><code>fiboTreeInit</code></dt>
<dd>Initializes a <code>FiboTree</code> data structure, by allocating the internal consolidation array.</dd>

<dt><code>fiboTreeExit</code></dt>
<dd>Destroys a <code>FiboTree</code> data structure, by freeing the internal consolidation array. Tree elements are not modified in any way. If elements have to be individually processed at freeing time, for instance to be freed themselves, then they have to be removed one by one from the tree before <code>fiboTreeExit</code> is called, or be handled by means of external iterators (for instance by traversing their support array if they were all allocated together).</dd>

<dt><code>fiboTreeFree</code></dt>
<dd>Makes the <code>FiboTree</code> data structure become free, by removing all of its elements. Like for <code>fiboTreeExit</code>, tree elements are not themselves considered nor modified in any way. Their chaining pointers just become meaningless, so that removing any previously inserted <code>FiboNode</code> element from a freed tree may lead to segmentation violation errors.</dd>

<dt><code>fiboTreeAdd</code></dt>
<dd>Adds a <code>FiboNode</code> to a <code>FiboTree</code>.</dd>

<dt><code>fiboTreeDel</code></dt>
<dd>Removes the given <code>FiboNode</code> from its <code>FiboTree</code>.</dd>

<dt><code>fiboTreeConsolidate</code></dt>
<dd>Consolidates the current tree and returns a pointer to its extremal element. This element itself is not modified, and still belongs to the <code>FiboTree</code>.</dd>

<dt><code>fiboTreeMin</code></dt>
<dd>Returns the tree element of extremal value (assumed to be «minimal» in my original context), without removing it from the <code>FiboTree</code>. At the time being, this routine just calls <code>fiboTreeConsolidate</code>. Module users willing to keep track of the element of extremal gain can redefine this routine, as well as the <code>fiboTreeAdd</code> and <code>fiboTreeDel</code> routines, such that a pointer to the element of extremal gain is preserved in the <code>FiboTree</code> data structure. The recorded element must be compared against elements which are removed and deleted, and updated whenever necessary (which may require a call to <code>fiboTreeConsolidate</code> if the extremal element is the one which is being removed from the <code>FiboTree</code>).</dd>

<dt><code>fiboTreeCheck</code></dt>
<dd>When the <code>FIBO_DEBUG</code> debugging flag has been set at compile time, this routine checks the consistency of the given <code>FiboTree</code> structure.</dd>
</dl>

The tarball of my implementation of the Fibonacci tree module can be obtained [here](https://web.archive.org/web/20190723125631/http://www.labri.fr/perso/pelegrin/code/fibo-1.0.tgz). It contains files [fibo.c](fibo.c), [fibo.h](fibo.h), and [test\_fibo.c](test_fibo.c), as a sample use. I have placed this version of my module under a [CeCILL-B license](http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html) (equivalent in its principle to a «BSD» license), for maximal re-use.

François Pellegrini

---

Apparently this explanation and the code were originally published at [www.labri.fr/perso/pelegrin/code](http://www.labri.fr/perso/pelegrin/code/) but that's no longer available. I've downloaded an archived copy from [archive.org](https://web.archive.org/web/20190723125631/http://www.labri.fr/perso/pelegrin/code/) and decided to put it on GitHub for easier access.

