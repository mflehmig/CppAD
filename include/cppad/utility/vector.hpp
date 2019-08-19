# ifndef CPPAD_UTILITY_VECTOR_HPP
# define CPPAD_UTILITY_VECTOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */


# include <cstddef>
# include <iostream>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// ==========================================================================
template <class Type> class vector {
// ==========================================================================
/*
$begin cppad_vector_member$$

$section Vector Class: Member Data $$

$head Type$$
is the type of the elements in the array.

$head capacity_$$
Number of $icode Type$$ elements in $code data_$$ that have been allocated
(and constructor has been called).

$head length_$$
Number of $icode Type$$ elements currently in this vector.

$head data_$$
Pointer to the first element of the vector
(not defined and should not be used when $code capacity_$$ is  0).

$srccode%hpp% */
private:
    size_t capacity_;
    size_t length_;
    Type*  data_;
public:
    size_t capacity(void) const
    {   return capacity_; }
    size_t size(void) const
    {   return length_; }
    const Type* data(void) const
    {   return data_; }
    Type* data(void)
    {   return data_; }
/* %$$
$end
-----------------------------------------------------------------------------
$begin cppad_vector_typedef$$

$section Vector Class: Type definitions$$

$head value_type$$
Type corresponding to an element of the vector.

$srccode%hpp% */
public:
    typedef Type value_type;
/* %$$
$end
-----------------------------------------------------------------------------
$begin cppad_vector_ctor$$
$spell
    vec
$$

$section Vector Class: Constructors and Destructor$$

$head Default$$
The syntax
$codei%
    vector<%Type%> %vec%
%$$
creates an empty vector no elements and no capacity.

$head Sizing$$
The syntax
$codei%
    vector<%Type%> %vec%(%n%)
%$$
where $icode n$$ is a $code size_t$$,
creates the vector $icode vec$$ with $icode n$$ elements and capacity
greater than or equal $icode n$$.

$head Copy$$
The syntax
$codei%
    vector<%Type%> %vec%(%other%)
%$$
where $icode other$$ is a $codei%vector<%Type%>%$$,
creates the vector $icode vec$$
with $icode%n% = %other%.size()%$$ elements and capacity
greater than or equal $icode n$$.

$head Destructor$$
If $code capacity_$$ is non-zero, call the destructor
for all the corresponding elements and then frees the corresponding memory.

$head delete_data$$
Call destructor and free all the allocated elements
(there are $code capacity_$$ such elements).

$srccode%hpp% */
public:
    vector(void) : capacity_(0), length_(0), data_(CPPAD_NULL)
    { }
    vector(size_t n) : capacity_(0), length_(0), data_(CPPAD_NULL)
    {   resize(n); }
    vector(const vector& other) : capacity_(0), length_(0), data_(CPPAD_NULL)
    {   resize(other.length_);
        for(size_t i = 0; i < length_; i++)
            data_[i] = other.data_[i];
    }
    ~vector(void)
    {   if( capacity_ > 0 ) delete_data(data_); }
private:
    void delete_data(Type* data_ptr)
    {   thread_alloc::delete_array(data_ptr); }
/* %$$
$end
-----------------------------------------------------------------------------
$begin cppad_vector_size$$
$spell
    resize
    vec
$$

$section Vector Class: Change Size$$

$head Syntax$$
$icode%vec%.resize(%n%)
%$$
$icode%vec%.clear()%$$

$head Prototype$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_RESIZE%// END_RESIZE%1
%$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_CLEAR%// END_CLEAR%1
%$$

$head n$$
is the number of elements in the new version of the vector.

$head resize$$
If $icode n$$ is less than or equal the input value of
$icode%vec%.capacity_%$$,
the only change is that $icode%vec%.length_%$$ is set to $icode n$$.
Otherwise the old elements are deleted and a new vector is created
with $icode%vec%.length_%$$ equal to $icode n$$.

$head clear$$
The destructor is called for all the elements of $icode vec$$
and then $icode%vec.length_%$$ and $icode%vec%.capacity_%$$ are set to zero.

$end
------------------------------------------------------------------------------
*/
// BEGIN_RESIZE
public:
    void resize(size_t n)
// END_RESIZE
    {   length_ = n;
        if( capacity_ < length_ )
        {   // we must allocate new memory

            // free old memory
            if( capacity_ > 0 )
                delete_data(data_);

            // get new memory and set capacity
            data_ = thread_alloc::create_array<Type>(length_, capacity_);
        }
    }
// BEGIN_CLEAR
    void clear(void)
// END_CLEAR
    {   length_ = 0;
        // check if there is old memory to be freed
        if( capacity_ > 0 )
            delete_data(data_);
        capacity_ = 0;
    }
/*
-------------------------------------------------------------------------------
$begin cppad_vector_assign$$
$spell
    resize
    vec
$$

$section Vector Class: Assignment Operators$$

$head Syntax$$
$icode%vec%.swap(%other%)
%$$
$icode%vec% = %other%$$

$head Prototype$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_SWAP%// END_SWAP%1
%$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_ASSIGN%// END_ASSIGN%1
%$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_MOVE_SEMANTICS%// END_SEMANTICS%1
%$$

$head swap$$
Swaps $code length_$$, $code capacity_$$ and $code data_$$
between $icode vec$$ and $icode other$$.

$head Assignment$$
If the input value of $icode%vec%.length_%$$ is zero,
$cref/resize/cppad_vector_size/resize/$$ is used to change its size to
be the same as other.
The size of $icode vec$$ and $icode other$$ are then compared and if
different, an assert with a know cause is generated.
The elements of $icode vec$$ are then individually assigned
to have the value of the corresponding elements of $icode other$$.

$head Move Semantics$$
If $code CPPAD_USE_CPLUSPLUS_2011$$ is $code 1$$
the move semantics version of the assignment operator, implemented using
$code swap$$, is defined.

$end
-------------------------------------------------------------------------------
*/
// BEGIN_SWAP
public:
    void swap(vector& other)
// END_SWAP
    {  // special case where vec and other are the same vector
       if( this == &other )
            return;
        //
        std::swap(length_,   other.length_   );
        std::swap(capacity_, other.capacity_ );
        std::swap(data_,     other.data_     );
        return;
    }
// BEGIN_ASSIGN
    vector& operator=(const vector& other)
// END_ASSIGN
    { if( length_ == 0 )
            resize( other.length_ );
        CPPAD_ASSERT_KNOWN(
            length_ == other.length_ ,
            "vector: size miss match in assignment operation"
        );
        for(size_t i = 0; i < length_; i++)
            data_[i] = other.data_[i];
        return *this;
    }
# if CPPAD_USE_CPLUSPLUS_2011
// BEGIN_MOVE_SEMANTICS
    vector& operator=(vector&& other)
// END_SEMANTICS
    {   CPPAD_ASSERT_KNOWN(
            length_ == other.length_ || (length_ == 0),
            "vector: size miss match in assignment operation"
        );
        swap(other);
        return *this;
    }
# endif
/*
-------------------------------------------------------------------------------
$begin cppad_vector_subscript$$
$spell
    vec
$$

$section Vector Class: Subscript Operator$$

$head Syntax$$
$icode%element% = %vec%[%i%]
%$$
$icode%vec%[%i%] = %element%
%$$

$srccode%hpp% */
    const Type& operator[]( size_t i) const
    {   CPPAD_ASSERT_KNOWN( i < length_,
            "vector: index greater than or equal vector size"
        );
        return data_[i];
    }
    Type& operator[](size_t i)
    {   CPPAD_ASSERT_KNOWN(i < length_,
            "vector: index greater than or equal vector size"
        );
        return data_[i];
    }
    template <class Index> const Type& operator[]( Index i) const
    {   return (*this)[size_t(i)]; }
    template <class Index> Type& operator[](Index i)
    {   return (*this)[size_t(i)]; }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_push_back$$
$spell
    vec
$$

$section Vector Class: push_back$$

$head Syntax$$
$icode%vec%.push_back(%element%)%$$

$head Prototype$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_PUSH_BACK%// END_PUSH_BACK%1
%$$

$end
*/
// BEGIN_PUSH_BACK
    void push_back(const Type& element)
// END_PUSH_BACK
    {   // case where no allocation is necessary
        if( length_ < capacity_ )
        {   data_[length_++] = element;
            return;
        }
        CPPAD_ASSERT_UNKNOWN( length_ == capacity_ );

        // create new vector with required size
        vector vec(length_ + 1);

        // copy old data values
        for(size_t i = 0; i < length_; ++i)
            vec.data_[i] = data_[i];

        // put the new element in the new vector
        CPPAD_ASSERT_UNKNOWN( vec.length_ == length_ + 1);
        vec.data_[length_] = element;

        // swap old and new vectors
        swap(vec);
    }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_push_vector$$
$spell
    vec
$$

$section Vector Class: push_vector$$

$head Syntax$$
$icode%vec%.push_vector(%other%)%$$

$head Prototype$$
$srcfile%include/cppad/utility/vector.hpp%
    0%// BEGIN_PUSH_VECTOR%// END_PUSH_VECTOR%1
%$$

$head other$$
is a $cref SimpleVector$$ with elements of type $icode Type$$.

$head vec$$
The output value of $icode vec$$ has its input elements
following by the elements in other.
It output size is its input size plus the size of $icode other$$.

$end
*/
// BEGIN_PUSH_VECTOR
    template <class Vector> void push_vector(const Vector& other)
// END_PUSH_VECTOR
    {   // can not use push_back because MS V++ 7.1 did not resolve
        // to non-template member function when scalar is used.
        //
        CheckSimpleVector<Type, Vector>();
        size_t m = other.size();

        // case where no allcoation is necessary
        if( length_ + m <= capacity_ )
        {   for(size_t i = 0; i < m; i++)
                data_[length_++] = other[i];
            return;
        }

        // create new vector with required size
        vector vec(length_ + m);

        // copy old data values
        for(size_t i = 0; i < length_; ++i)
            vec.data_[i] = data_[i];

        // put the new elements in the new vector
        CPPAD_ASSERT_UNKNOWN( vec.length_ == length_ + m );
        for(size_t i = 0; i < m; i++)
            vec.data_[length_ + i] = other[i];

        // swap old and new vectors
        swap(vec);
    }

// =========================================================================
};  // END_TEMPLATE_CLASS_VECTOR
// =========================================================================

/*
$begin cppad_vector_output$$
$spell
    vec
$$

$section Vector Class: Output$$

$head Syntax$$
$icode%os% << vec%$$

$srccode%hpp% */
template <class Type>
std::ostream& operator << (std::ostream&  os , const CppAD::vector<Type>& vec )
{   os << "{ ";
    for(size_t i = 0; i < vec.size(); ++i)
    {   os << vec[i];
        if( i + 1 < vec.size() )
            os << ", ";
    }
    os << " }";
    return os;
}
/* %$$
$end
*/

} // END_CPPAD_NAMESPACE

// user API specifies that vector_bool.hpp is included by vector.hpp
# include <cppad/utility/vector_bool.hpp>

# endif
