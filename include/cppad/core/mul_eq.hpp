# ifndef CPPAD_CORE_MUL_EQ_HPP
# define CPPAD_CORE_MUL_EQ_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base>& AD<Base>::operator *= (const AD<Base> &right)
{
    // compute the Base part
    Base left;
    left    = value_;
    value_ *= right.value_;

    // check if there is a recording in progress
    local::ADTape<Base>* tape = AD<Base>::tape_ptr();
    if( tape == CPPAD_NULL )
        return *this;
    tape_id_t tape_id = tape->id_;
    // tape_id cannot match the default value for tape_id_; i.e., 0
    CPPAD_ASSERT_UNKNOWN( tape_id > 0 );

    // check if left and right tapes match
    bool match_left  = tape_id_       == tape_id;
    bool match_right = right.tape_id_ == tape_id;

    // check if left and right are dynamic parameters
    bool dyn_left  = match_left  & (ad_type_ == local::dyn_ad_type);
    bool dyn_right = match_right & (right.ad_type_ == local::dyn_ad_type);

    // check if left and right are variables
    bool var_left  = match_left  & (ad_type_ != local::dyn_ad_type);
    bool var_right = match_right & (right.ad_type_ != local::dyn_ad_type);

    CPPAD_ASSERT_KNOWN(
        tape_id_ == right.tape_id_ || ! match_left || ! match_right ,
        "*= : AD variables or dynamic parameters on different threads."
    );
    if( var_left )
    {   if( var_right )
        {   // this = variable * variable
            CPPAD_ASSERT_UNKNOWN( local::NumRes(local::MulvvOp) == 1 );
            CPPAD_ASSERT_UNKNOWN( local::NumArg(local::MulvvOp) == 2 );

            // put operand addresses in tape
            tape->Rec_.PutArg(taddr_, right.taddr_);
            // put operator in the tape
            taddr_ = tape->Rec_.PutOp(local::MulvvOp);
            // check that this is a variable
            CPPAD_ASSERT_UNKNOWN( tape_id_ == tape_id );
            CPPAD_ASSERT_UNKNOWN( ad_type_ == local::var_ad_type);
        }
        else if( (! dyn_right) & IdenticalOne(right.value_) )
        {   // this = variable * 1
        }
        else if( (! dyn_right) & IdenticalZero(right.value_) )
        {   // this = variable * 0
            tape_id_ = 0; // not in current tape
        }
        else
        {   // this = variable  * parameter
            //      = parameter * variable
            CPPAD_ASSERT_UNKNOWN( local::NumRes(local::MulpvOp) == 1 );
            CPPAD_ASSERT_UNKNOWN( local::NumArg(local::MulpvOp) == 2 );

            // put operand addresses in tape
            addr_t p = right.taddr_;
            if( ! dyn_right )
                p = tape->Rec_.put_con_par(right.value_);
            tape->Rec_.PutArg(p, taddr_);
            // put operator in the tape
            taddr_ = tape->Rec_.PutOp(local::MulpvOp);
            // check that this is a variable
            CPPAD_ASSERT_UNKNOWN( tape_id_ == tape_id );
            CPPAD_ASSERT_UNKNOWN( ad_type_ == local::var_ad_type);
        }
    }
    else if( var_right  )
    {   if( (! dyn_left) & IdenticalZero(left) )
        {   // this = 0 * right
        }
        else if( (! dyn_left) & IdenticalOne(left) )
        {   // this = 1 * right
            make_variable(right.tape_id_, right.taddr_);
        }
        else
        {   // this = parameter * variable
            CPPAD_ASSERT_UNKNOWN( local::NumRes(local::MulpvOp) == 1 );
            CPPAD_ASSERT_UNKNOWN( local::NumArg(local::MulpvOp) == 2 );

            // put operand addresses in tape
            addr_t p = taddr_;
            if( ! dyn_left )
                p = tape->Rec_.put_con_par(left);
            tape->Rec_.PutArg(p, right.taddr_);

            // put operator in the tape
            taddr_ = tape->Rec_.PutOp(local::MulpvOp);

            // make this a variable
            tape_id_ = tape_id;
            ad_type_ = local::var_ad_type;
        }
    }
    else if( dyn_left | dyn_right )
    {   addr_t arg0 = taddr_;
        addr_t arg1 = right.taddr_;
        if( ! dyn_left )
            arg0 = tape->Rec_.put_con_par(left);
        if( ! dyn_right )
            arg1 = tape->Rec_.put_con_par(right.value_);
        //
        // parameters with a dynamic parameter results
        taddr_ = tape->Rec_.put_dyn_par(
                value_, local::mul_dyn, arg0, arg1
        );
        tape_id_ = tape_id;
        ad_type_ = local::dyn_ad_type;
    }
    return *this;
}

CPPAD_FOLD_ASSIGNMENT_OPERATOR(*=)

} // END CppAD namespace

# endif
