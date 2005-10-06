// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
$begin OdeGearControl.cpp$$
$spell
	Runge
$$

$section OdeGearControl: Example and Test$$

$index OdeGearControl, example$$
$index example, OdeGearControl$$
$index test, OdeGearControl$$

Define 
$latex X : \R \rightarrow \R^2$$ by
$latex \[
\begin{array}{rcl}
X_0 (t) & = &  - \exp ( - w_0 t )  \\
X_1 (t) & = & \frac{w_0}{w_1 - w_0} [ \exp ( - w_0 t ) - \exp( - w_1 t )]
\end{array}
\] $$
It follows that $latex X_0 (0) = 1$$, $latex X_1 (0) = 0$$ and
$latex \[
\begin{array}{rcl}
	X_0^{(1)} (t) & = & - w_0 X_0 (t)  \\
	X_1^{(1)} (t) & = & + w_0 X_0 (t) - w_1 X_1 (t) 
\end{array}
\] $$
The example tests OdeGearControl using the relations above:

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/OdeGearControl.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <CppAD/OdeGearControl.h>   // CppAD::OdeGearControl

namespace {
	// --------------------------------------------------------------
	class Fun {
	private:
		 CppADvector<double> w;
	public:
		// constructor
		Fun(const CppADvector<double> &w_) : w(w_)
		{ } 

		// set f = x'(t)
		template <typename Scalar>
		void Ode(
			const Scalar              &t, 
			const CppADvector<Scalar> &x, 
			CppADvector<Scalar>       &f)
		{	f[0] = - w[0] * x[0];
			f[1] = + w[0] * x[0] - w[1] * x[1];	
		}

		void Ode_dep(
			const double              &t, 
			const CppADvector<double> &x, 
			CppADvector<double>       &f_x)
		{	using namespace CppAD;

			size_t n  = x.size();	
			CppADvector< AD<double> > T(1);
			CppADvector< AD<double> > X(n);
			CppADvector< AD<double> > F(n);

			// set argument values
			T[0] = t;
			size_t i, j;
			for(i = 0; i < n; i++)
				X[i] = x[i];

			// declare independent variables
			Independent(X);

			// compute f(t, x)
			this->Ode(T[0], X, F);

			// define AD function object
			ADFun<double> Fun(X, F);

			// compute partial of f w.r.t x
			CppADvector<double> dx(n);
			CppADvector<double> df(n);
			for(j = 0; j < n; j++)
				dx[j] = 0.;
			for(j = 0; j < n; j++)
			{	dx[j] = 1.;
				df = Fun.Forward(1, dx);
				for(i = 0; i < n; i++)
					f_x [i * n + j] = df[i];
				dx[j] = 0.;
			}
		}
	};
}

bool OdeGearControl(void)
{	bool ok = true;     // initial return value

	CppADvector<double> w(2);
	w[0] = 10.;
	w[1] = 1.;
	Fun F(w);

	CppADvector<double> xi(2);
	xi[0] = 1.;
	xi[1] = 0.;

	CppADvector<double> eabs(2);
	eabs[0] = 1e-4;
	eabs[1] = 1e-4;

	// return values
	CppADvector<double> ef(2);
	CppADvector<double> maxabs(2);
	CppADvector<double> xf(2);
	size_t                nstep;

	// input values
	size_t  M   = 5;
	double ti   = 0.;
	double tf   = 1.;
	double smin = 1e-8;
	double smax = 1.;
	double sini = 1e-10;
	double erel = 0.;
	
	xf = CppAD::OdeGearControl(F, M,
		ti, tf, xi, smin, smax, sini, eabs, erel, ef, maxabs, nstep);

	double x0 = exp(-w[0]*tf);
	ok &= CppAD::NearEqual(x0, xf[0], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[0], 1e-4, 1e-4);

	double x1 = w[0] * (exp(-w[0]*tf) - exp(-w[1]*tf))/(w[1] - w[0]);
	ok &= CppAD::NearEqual(x1, xf[1], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[1], 1e-4, 1e-4);

	return ok;
}

// END PROGRAM
