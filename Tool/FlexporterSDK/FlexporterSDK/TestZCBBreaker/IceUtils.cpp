///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains misc. useful macros & defines.
 *	\file		IceUtils.cpp
 *	\author		Pierre Terdiman (collected from various sources)
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceCore;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Returns the alignment of the input address.
 *	\fn			Alignment()
 *	\param		address	[in] address to check
 *	\return		the best alignment (e.g. 1 for odd addresses, etc)
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword IceCore::Alignment(udword address)
{
	// Returns 0 for null addresses
	if(!address) return 0;

	// Test all bits
	udword Align = 1;
	for(udword i=1;i<32;i++)
	{
		// Returns as soon as the alignment is broken
		if(address&Align)	return Align;
		Align<<=1;
	}
	// Here all bits are null, except the highest one (else the address would be null)
	return Align;
}






#ifdef TOSEE

x & ~(x-1) 
= extracts the lowest set bit of x (all others are clear). Pretty patterns when applied to a linear sequence. 
x & (x+(1<<n)) 
= x, with the run of set bits (possibly length 0) starting at bit n cleared. 
x & ~(x+(1<<n)) 
= the run of set bits (possibly length 0) in x, starting at bit n. 
x | (x+1) 
= x with the lowest cleared bit set. 
x | ~(x+1) 
= extracts the lowest cleared bit of x (all others are set). 
x | (x-(1<<n)) 
= x, with the run of cleared bits (possibly length 0) starting at bit n set. 
x | ~(x-(1<<n)) 
= the lowest run of cleared bits (possibly length 0) in x, starting at bit n are the only clear bits. 
Gray code 
x ^ (x >> 1) 
= The standard (binary-reflected) Gray code for x. 
z = y = x; do { y = y >> 1; z ^= y } while (y); return z; 
= decoder for the above Gray code. 
NUL finding 
from "newlib": Note the odd use of NULL for the NUL character. 

/*
#if LONG_MAX == 2147483647L
	#define DETECTNULL(X) (((X) - 0x01010101) & ~(X) & 0x80808080)
#else
#if LONG_MAX == 9223372036854775807L
// Nonzero if X (a long int) contains a NULL byte.
#define DETECTNULL(X) (((X) - 0x0101010101010101) & ~(X) & 0x8080808080808080)
#else
#error long int is not a 32bit or 64bit type.
#endif
#endif
*/

Take the case of the one byte equivalent:
(X - 0x01) & ~(X) & 0x80 
== ~(x | ~(x - 1)) & 0x80 


From above we can see that the the first part of the expression is the lowest run of clearedbits is set to 1, above that is 0. So the high bit, tested by & 0x80 can only be set when the entire byte is cleared. 

For multiple bytes we can see that this works fine if we ignore borrow from one byte to another. Borrow can only happen if one byte started out as zero anyway though, in which case the lower byte will properly register as non-zero. 

Benefit from hardware implementation: 
Some architectures have both a population count instruction, and a find most significant bit AKA log2. If yours doesn't, these might help: 

Population count 
Naive: 

unsigned int iterative_popcount(unsigned int b) {
	unsigned int n;
	for (n = 0; b != 0; n++, b &= (b - 1));
	return n;
}

works in O(number of set bits) 

#define m1 ((unsigned_64) 0x5555555555555555)
#define m2 ((unsigned_64) 0x3333333333333333)

unsigned int non_iterative_popcount(const unsigned_64 b) {
	unsigned_32 n;
	const unsigned_64 a = b - ((b >> 1) & m1);
	const unsigned_64 c = (a & m2) + ((a >> 2) & m2);
	n = ((unsigned_32) c) + ((unsigned_32) (c >> 32));
	n = (n & 0x0F0F0F0F) + ((n >> 4) & 0x0F0F0F0F);
	n = (n & 0xFFFF) + (n >> 16);
	n = (n & 0xFF) + (n >> 8);
	return n;
}

can be extended to any size, and is then O(log n), of course, for this case it is O(1), since the number of bits are constant. 

find MSB, or log2
naive. If you know more about your distribution, you can do stuff like check bytes (or words) at a time. For 8-bit bytes you should probably use a lookup table. 

unsigned int iterative_log2(unsigned int b) {
	unsigned int n;
	for (n = 0; b != 0; n++, b >>= 1));
	return n;
}

}

#endif


#ifdef TOSEE


--------------------------------------------------------------------------------

Alignment of Pointers
Alignment of pointers is a pretty common problem, and there are several relevant tricks, so, at the suggestion of Jean-Charles Meyrignac
(who also provided an example of the upward alignment described below), I've added a little description here. 

It is fairly obvious that the downward alignment of an address a to a multiple-of-b boundary, where b is a power of 2, is simply (a & ~(b-1)).
Of course, ~(b-1) is also -b, so (a & -b) also works (the difference is usually nothing; if b is a constant, most compilers will fold the first
into the second form). 

For upward alignment, we simply add b-1: ((a + (b-1)) & -b). 

Of course, there are a few complications. First, languages like C, which allow pointer arithmetic, generally scale pointer offsets by the size of
the target object -- which would keep our math from working. It used to be that casting a pointer as a (char *) would turn-off this scaling, but
with long char and such out there, a cast as (void *) is probably a safer bet. Unfortunately, C doesn't define bitwise operations on pointers of
any flavor, so you'll need to cast to an appropriately-large integer type before doing a bitwise AND. 

Secondly, aligning an address doesn't help unless you allocated a large enough chunk of memory so that you can treat your data structure as starting
at the aligned address. In general, if you wish to create a b-aligned data structure with c bytes, you would do something like:
a=((typeof(a))(((int)(((void *)malloc(c+(b-1)))+(b-1)))&-b)). Please excuse my use of the GCC typeof(). Anyway, this is particularly useful
for cache-line alignment of data structures. One little annoyance: you can't call free(a); you'll need to keep a copy of the original block address
for that. 

--------------------------------------------------------------------------------

Average of Integers
This is actually an extension of the "well known" fact that for binary integer values x and y, (x+y) equals ((x&y)+(x|y)) equals ((x^y)+2*(x&y)). 

Given two integer values x and y, the (floor of the) average normally would be computed by (x+y)/2; unfortunately, this can yield incorrect results
due to overflow. A very sneaky alternative is to use (x&y)+((x^y)/2). If we are aware of the potential non-portability due to the fact that C does
not specify if shifts are signed, this can be simplified to (x&y)+((x^y)>>1). In either case, the benefit is that this code sequence cannot overflow. 

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------

Comparison of Float Values
IEEE floating point has a number of nice properties, including the ability to use 2's complement integer comparisons to compare floating point values,
provided the native byte order is consistent between float and integer values. The only complication is the use of sign+magnitude representation in
floats. The AMD Athlon Processor x86 Code Optimization Guide gives a nice summary on Page 43. Here's a set of C routines that embody the same logic: 

#define FasI(f)  (*((int *) &(f)))
#define FasUI(f) (*((unsigned int *) &(f)))

#define	lt0(f)	(FasUI(f) > 0x80000000U)
#define	le0(f)	(FasI(f) <= 0)
#define	gt0(f)	(FasI(f) > 0)
#define	ge0(f)	(FasUI(f) <= 0x80000000U)

--------------------------------------------------------------------------------

Comparison to Mask Conversion
In many cases, it is useful to convert the result of a comparison, which is either 0 or some non-zero bit pattern, into either a "clean" 0 or -1 bit mask.
If the messy non-negative integer value is x, the sanitized version is: 

(((int) (-x)) >> (WORDBITS - 1))

To remove the constraint that the messy value be non-negative, use: 

(((int) (x | -x)) >> (WORDBITS - 1))

Logically, this works because the shift by (WORDBITS-1) replicates the sign bit to create a mask -- be aware, however, that the C language does not
require that shifts are signed even if their operands are signed, so there is a potential portability problem. Additionally, one might think that a
shift by any number greater than or equal to WORDBITS would have the same effect, but many instruction sets have shifts that behave strangely when
such shift distances are specified. 

Of course, the opposite condition can be tested using: 

(((int) ~(x | -x)) >> (WORDBITS - 1))

If you prefer the C-standard 0 or 1 comparison result, simply use the unsigned shift: 

(((unsigned int) (x | -x)) >> (WORDBITS - 1))

The opposite condition can be obtained using: 

(((unsigned int) ~(x | -x)) >> (WORDBITS - 1))


--------------------------------------------------------------------------------

Dual-Linked List with One Pointer Field
Normally, a dual-linked circular list would contain both previous and next pointer fields and the current position in the list would be identified
by a single pointer. By using two current pointers, one to the node in question and the other to the one just before/after it, it becomes possible
to store only a single pointer value in each node. The value stored in each node is the XOR of the next and previous pointers that normally would
have been stored in each node. Decoding is obvious. 

Unfortunately, using this trick in C is awkward because the XOR operation is not defined for pointers. 


--------------------------------------------------------------------------------

Gray Code Conversion
A Gray code is any binary coding sequence in which only a single bit position changes as we move from one value to the next. There are many such codes,
but the traditional one is computed such that the Kth Gray code is K^(K>>1). 

The well-known algorithm for conversion from Gray to binary is a linear sequence of XORs that makes it seem each bit must be dealt with separately.
Fortunately, that is equivalent to a parallel prefix XOR that can be computed using SWAR techniques in log time. For 32-bit Gray code values produced
as described above, the conversion from Gray code back to unsigned binary is: 

unsigned int
g2b(unsigned int gray)
{
        gray ^= (gray >> 16);
        gray ^= (gray >> 8);
        gray ^= (gray >> 4);
        gray ^= (gray >> 2);
        gray ^= (gray >> 1);
        return(gray);
}

--------------------------------------------------------------------------------

Integer Constant Multiply
Given an integer value x and an integer or floating point value y, the value of x*y can be computed efficiently using a sequence derived from the
binary value of x. For example, if x is 5 (4 + 1): 

y2 = y + y;
y4 = y2 + y2;
result = y + y4;

In the special case that y is an integer, this can be done with shifts: 

y4 = (y << 4);
result = y + y4;


--------------------------------------------------------------------------------

Integer Minimum or Maximum
Given 2's complement integer values x and y, the minimum can be computed without any branches as x+(((y-x)>>(WORDBITS-1))&(y-x)). Logically,
this works because the shift by (WORDBITS-1) replicates the sign bit to create a mask -- be aware, however, that the C language does not require
 that shifts are signed even if their operands are signed, so there is a potential portability problem. Additionally, one might think that a shift
 by any number greater than or equal to WORDBITS would have the same effect, but many instruction sets have shifts that behave strangely when such
 shift distances are specified. 

Of course, maximum can be computed using the same trick: x-(((x-y)>>(WORDBITS-1))&(x-y)). 

Actually, the Integer Selection coding trick is just as efficient in encoding minimum and maximum.... 


--------------------------------------------------------------------------------

Integer Power
Given an integer value x and an integer or floating point value y, the value of y to the x power can be computed efficiently using a sequence
derived from the binary value of x. For example, if x is 5 (4 + 1): 

y2 = y * y;
y4 = y2 * y2;
result = y * y4;


--------------------------------------------------------------------------------

Integer Selection
A branchless, lookup-free, alternative to code like if (a<b) x=c; else x=d; is ((((a-b) >> (WORDBITS-1)) & (c^d)) ^ d). This code assumes that
the shift is signed, which, of course, C does not promise. 


--------------------------------------------------------------------------------

Leading Zero Count
Some machines have had single instructions that count the number of leading zero bits in an integer; such an operation can be an artifact of having
floating point normalization hardware around. Clearly, floor of base 2 log of x is (WORDBITS-lzc(x)). In any case, this operation has found its way
into quite a few algorithms, so it is useful to have an efficient implementation: 

unsigned int
lzc(register unsigned int x)
{
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return(WORDBITS - ones(x));
}


--------------------------------------------------------------------------------

Least Significant 1 Bit
This can be useful for extracting the lowest numbered element of a bit set. Given a 2's complement binary integer value x, (x&-x) is the least
significant 1 bit. (This was pointed-out by Tom May.) The reason this works is that it is equivalent to (x & ((~x) + 1)); any trailing zero bits
in x become ones in ~x, adding 1 to that carries into the following bit, and AND with x yields only the flipped bit... the original position of the
least significant 1 bit. 

Alternatively, since (x&(x-1)) is actually x stripped of its least significant 1 bit, the least significant 1 bit is also (x^(x&(x-1))). 


--------------------------------------------------------------------------------

Log2 of an Integer
Given a binary integer value x, the floor of the base 2 log of that number efficiently can be computed by the application of two variable-precision
SWAR algorithms. The first "folds" the upper bits into the lower bits to construct a bit vector with the same most significant 1 as x, but all 1's
below it. The second SWAR algorithm is population count, defined elsewhere in this document. However, we must consider the issue of what the log2(0)
should be; the log of 0 is undefined, so how that value should be handled is unclear. The following code for handling a 32-bit value gives two options:
if LOG0UNDEFINED, this code returns -1 for log2(0); otherwise, it returns 0 for log2(0). For a 32-bit value: 

unsigned int
floor_log2(register unsigned int x)
{
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
#ifdef	LOG0UNDEFINED
        return(ones32(x) - 1);
#else
	return(ones32(x >> 1));
#endif
}

Suppose instead that you want the ceiling of the base 2 log. The floor and ceiling are identical if x is a power of two; otherwise, the result is 1
too small. This can be corrected using the power of 2 test followed with the comparison-to-mask shift used in integer minimum/maximum. The result is: 

unsigned int
log2(register unsigned int x)
{
	register int y = (x & (x - 1));

	y |= -y;
	y >>= (WORDBITS - 1);
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
#ifdef	LOG0UNDEFINED
        return(ones(x) - 1 - y);
#else
	return(ones(x >> 1) - y);
#endif
}

--------------------------------------------------------------------------------




--------------------------------------------------------------------------------

Natural Data Type Precision Conversions
For integers used to represent natural data types, simply shifting right works well for conversion to a lower precision, but shifting left is not very
 effective for converting to a higher precision. The problem is simply that if the "new" bits are taken to be 0s, the maximum value will never be attained.
 Likewise, if taken to be any fixed non-0 value, the value zero will never be obtained. A good answer to this problem is to replicate the existing bit
 pattern in the "new" bits, truncating or repeating the pattern if more bits are needed. 

For example, a 10-bit raw pixel value (e.g., from my Canon G1) called x can be extended to a 16-bit value by the expression ((x<<6)|(x>>4)). This way,
 both the maximum and minimum values are reachable, with good linearity throughout the entire range. 


--------------------------------------------------------------------------------

Polynomials
It is fairly obvious, but x0+x1*x+x2*x*x+x3*x*x*x+... always can be rewritten as the usually faster equivalent x0+x*(x1+x*(x2+x*(x3+x*(...)))). There are various accuracy and other issues, but this sort of obvious transformation should not be overlooked. 


--------------------------------------------------------------------------------

Population Count (Ones Count)
The population count of a binary integer value x is the number of one bits in the value. Although many machines have single instructions for this, the single instructions are usually microcoded loops that test a bit per cycle; a log-time algorithm coded in C is often faster. The following code uses a variable-precision SWAR algorithm to perform a tree reduction adding the bits in a 32-bit value: 

unsigned int
ones32(register unsigned int x)
{
        /* 32-bit recursive reduction using SWAR...
	   but first step is mapping 2-bit values
	   into sum of 2 1-bit values in sneaky way
	*/
        x -= ((x >> 1) & 0x55555555);
        x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
        x = (((x >> 4) + x) & 0x0f0f0f0f);
        x += (x >> 8);
        x += (x >> 16);
        return(x & 0x0000003f);
}

It is worthwhile noting that the SWAR population count algorithm given above can be improved upon for the case of counting the population of multi-word bit sets. How? The last few steps in the reduction are using only a portion of the SWAR width to produce their results; thus, it would be possible to combine these steps across multiple words being reduced. 

One additional note: the AMD Athlon optimization guidelines suggest a very similar algorithm that replaces the last three lines with return((x * 0x01010101) >> 24);. For the Athlon (which has a very fast integer multiply), I would have expected AMD's code to be faster... but it is actually 6% slower according to my benchmarks using a 1.2GHz Athlon (a Thunderbird). Why? Well, it so happens that GCC doesn't use a multiply instruction - it writes out the equivalent shift and add sequence! 


--------------------------------------------------------------------------------

Shift-and-Add Optimization
Rather obviously, if an integer multiply can be implemented by a shift-and-add sequence, then a shift-and-add sequence can be implemented by multiplying by the appropriate constant... with some speedup on processors like the AMD Athlon. Unfortunately, GCC seems to believe constant multiplies should always be converted into shift-and-add sequences, so there is a problem in using this optimization in C source code. 


--------------------------------------------------------------------------------

Sign Extension
Although many instruction sets provide single machine instructions that implement sign extension of 2's-complement integers, Jean-Charles Meyrignac
suggested including a couple of tricks for sign extension. I've included them here because sign extension instructions generally work only on the data
sizes directly understood by the processor, whereas these methods work on any bit precisions. Before his suggestion, I had often used the first trick,
but the second is probably better in most cases. 

The most obvious method assumes that you have a signed right shift: to extend an a-bit number x to b bits, shift left by b-a, then signed shift that
value right by b-a bits. A more interesting variation that doesn't use shifts (shifters are often a limited resource) basically does a 1-bit add to
flip high bits: if n is 2 to the a, simply compute (((x | -n) + (n/2)) ^ (n/2)). 

--------------------------------------------------------------------------------

Trailing Zero Count
Given the Least Significant 1 Bit and Population Count (Ones Count) algorithms, it is trivial to combine them to construct a trailing zero count
(as pointed-out by Joe Bowbeer): 

unsigned int
tzc(register int x)
{
        return(ones((x & -x) - 1));
}

#endif
