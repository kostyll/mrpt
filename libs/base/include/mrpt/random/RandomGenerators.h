/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2010  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */
#ifndef RandomGenerator_H
#define RandomGenerator_H

#include <mrpt/utils/utils_defs.h>
#include <mrpt/math/CMatrixTemplateNumeric.h>
#include <mrpt/math/ops_matrices.h>

namespace mrpt
{
	/** A namespace of pseudo-random numbers genrators of diferent distributions. The central class in this namespace is mrpt::random::CRandomGenerator
	 */
	namespace random
	{
		using namespace mrpt::utils;
		using namespace mrpt::math;

		/** A thred-safe pseudo random number generator, based on an internal MT19937 randomness generator.
		  * The base algorithm for randomness is platform-independent. See http://en.wikipedia.org/wiki/Mersenne_twister
		  *
		  * For real thread-safety, each thread must create and use its own instance of this class.
		  *
		  * Single-thread programs can use the static object mrpt::random::randomGenerator
		  */
		class BASE_IMPEXP CRandomGenerator
		{
		protected:
			/** Data used internally by the MT19937 PRNG algorithm. */
			struct  TMT19937_data
			{
				TMT19937_data() : index(0), seed_initialized(false)
				{}
				uint32_t	MT[624];
				uint32_t	index;
				bool		seed_initialized;
			} m_MT19937_data;

			void MT19937_generateNumbers();
			void MT19937_initializeGenerator(const uint32_t &seed);

		public:

			/** @name Initialization
			 @{ */

				/** Default constructor: initialize random seed based on current time */
				CRandomGenerator() : m_MT19937_data() { randomize(); }

				/** Constructor for providing a custom random seed to initialize the PRNG */
				CRandomGenerator(const uint32_t seed) : m_MT19937_data() { randomize(seed); }

				void randomize(const uint32_t seed);  //!< Initialize the PRNG from the given random seed
				void randomize();	//!< Randomize the generators, based on current time

			/** @} */

			/** @name Uniform pdf
			 @{ */

				/** Generate a uniformly distributed pseudo-random number using the MT19937 algorithm, in the whole range of 32-bit integers.
				  *  See: http://en.wikipedia.org/wiki/Mersenne_twister */
				uint32_t drawUniform32bit();

				/** Generate a uniformly distributed pseudo-random number using the MT19937 algorithm, scaled to the selected range. */
				double drawUniform( const double Min, const double Max) {
					return Min + (Max-Min)* drawUniform32bit() * 2.3283064370807973754314699618685e-10; // 0xFFFFFFFF ^ -1
				}

				/** Fills the given matrix with independent, uniformly distributed samples.
				  * Matrix classes can be CMatrixTemplateNumeric or CMatrixFixedNumeric
				  * \sa drawUniform
  				  */
				template <class MAT>
				void drawUniformMatrix(
					MAT &matrix,
					const  double unif_min = 0,
					const  double unif_max = 1 )
				{
					for (size_t r=0;r<matrix.getRowCount();r++)
						for (size_t c=0;c<matrix.getColCount();c++)
							matrix.get_unsafe(r,c) = static_cast<typename MAT::value_type>( drawUniform(unif_min,unif_max) );
				}

				/** Fills the given vector with independent, uniformly distributed samples.
				  * \sa drawUniform
  				  */
				template <class T>
				void drawUniformVector(
					std::vector<T> & v,
					const  double unif_min = 0,
					const  double unif_max = 1 )
				{
					const size_t N = v.size();
					for (size_t c=0;c<N;c++)
						v[c] = static_cast<T>( drawUniform(unif_min,unif_max) );
				}

			/** @} */

			/** @name Normal/Gaussian pdf
			 @{ */

				/** Generate a normalized (mean=0, std=1) normally distributed sample.
				 *  \param likelihood If desired, pass a pointer to a double which will receive the likelihood of the given sample to have been obtained, that is, the value of the normal pdf at the sample value.
				 */
				double drawGaussian1D_normalized( double *likelihood = NULL);

				/** Generate a normally distributed pseudo-random number.
				 * \param mean The mean value of desired normal distribution
				 * \param std  The standard deviation value of desired normal distribution
				 */
				double drawGaussian1D( const double mean, const double std ) {
					return mean+std*drawGaussian1D_normalized();
				}

				/** Fills the given matrix with independent, 1D-normally distributed samples.
				  * Matrix classes can be CMatrixTemplateNumeric or CMatrixFixedNumeric
				  * \sa drawGaussian1D
  				  */
				template <class MAT>
				void drawGaussian1DMatrix(
					MAT &matrix,
					const double mean = 0,
					const double std = 1 )
				{
					for (size_t r=0;r<matrix.getRowCount();r++)
						for (size_t c=0;c<matrix.getColCount();c++)
							matrix.get_unsafe(r,c) = static_cast<typename MAT::value_type>( drawGaussian1D(mean,std) );
				}

				/** Fills the given vector with independent, 1D-normally distributed samples.
				  * \sa drawGaussian1D
  				  */
				template <class T>
				void drawGaussian1DVector(
					std::vector<T> & v,
					const double mean = 0,
					const double std = 1 )
				{
					const size_t N = v.size();
					for (size_t c=0;c<N;c++)
						v[c] = static_cast<T>( drawGaussian1D(mean,std) );
				}

				/** Generate multidimensional random samples according to a given covariance matrix.
				 *  Mean is assumed to be zero if mean==NULL.
				 * \exception std::exception On invalid covariance matrix
				 * \sa drawGaussianMultivariateMany
				 */
				 template <typename T>
				 void  drawGaussianMultivariate(
					std::vector<T>		&out_result,
					const CMatrixTemplateNumeric<T>	&cov,
					const std::vector<T>*  mean = NULL
					);

				 /** Generate a given number of multidimensional random samples according to a given covariance matrix.
				 * \param cov The covariance matrix where to draw the samples from.
				 * \param desiredSamples The number of samples to generate.
				 * \param samplesLikelihoods If desired, set to a valid pointer to a vector, where it will be stored the likelihoods of having obtained each sample: the product of the gaussian-pdf for each independent variable.
				 * \param ret The output list of samples
				 * \param mean The mean, or zeros if mean==NULL.
				 *
				 * \exception std::exception On invalid covariance matrix
				 *
				 * \sa drawGaussianMultivariate
				 */
				 template <typename VECTORLIKE, typename MATRIXLIKE>
				 void  drawGaussianMultivariateMany(
					std::vector< VECTORLIKE >	&ret,
					size_t						desiredSamples,
					const MATRIXLIKE	&cov,
					const VECTORLIKE*	mean = NULL,
					VECTORLIKE			*samplesLikelihoods = NULL)
				{
					typename VECTORLIKE::iterator  liks_it = samplesLikelihoods ? samplesLikelihoods->begin() : typename VECTORLIKE::iterator();
					const size_t dim = cov.getColCount();
					MATRIXLIKE Z,D;

					MRPT_START;

					if (samplesLikelihoods)
						samplesLikelihoods->assign(desiredSamples, 0);

					ret.resize(desiredSamples);
					ASSERT_(cov.getRowCount() == cov.getColCount() )

					if (mean) ASSERT_(mean->size() == dim )


					/** Computes the eigenvalues/eigenvector decomposition of this matrix,
					*    so that: M = Z � D � Z<sup>T</sup>, where columns in Z are the
					*	  eigenvectors and the diagonal matrix D contains the eigenvalues
					*    as diagonal elements, sorted in <i>ascending</i> order.
					*/
					cov.eigenVectors( Z, D );

					// Scale eigenvectors with eigenvalues:
					// Efficient implementation of: D.Sqrt(); Z = Z * D;
					for (size_t i=0;i<dim;i++)
					{
						const typename VECTORLIKE::value_type eig_sqrt = std::sqrt(D.get_unsafe(i,i));
						for (size_t j=0;j<dim;j++)
							Z.get_unsafe(j,i)*=eig_sqrt;
					}

					for (typename std::vector<VECTORLIKE>::iterator ret_it=ret.begin();ret_it!=ret.end();ret_it++)
					{
						size_t		i;
						double	likelihood_1, likelihood_all;

						// Reset vector to 0:
						ret_it->assign(dim,0);
						likelihood_all = 1.0;

						if (samplesLikelihoods)
						{
							// Save likelihoods also:
							// ------------------------------
							for (i=0;i<dim;i++)
							{
								const typename VECTORLIKE::value_type	rnd = this->drawGaussian1D_normalized(&likelihood_1);
								for (size_t d=0;d<dim;d++)	(*ret_it)[d]+=Z.get_unsafe(d,i)*rnd;

								// Product of each independent gaussian sample:
								likelihood_all *= likelihood_1;
							}
							if (mean)
								for (size_t d=0;d<dim;d++)	(*ret_it)[d]+=(*mean)[d];

							// Save the total likelihood of the generated vector:
							*liks_it = static_cast<typename VECTORLIKE::value_type>(likelihood_all);
							liks_it++;
						}
						else
						{
							// DO NOT save likelihoods also, just the samples
							// ----------------------------------------------------
							for (i=0;i<dim;i++)
							{
								const typename VECTORLIKE::value_type rnd = this->drawGaussian1D_normalized();
								for (size_t d=0;d<dim;d++)	(*ret_it)[d]+=Z.get_unsafe(d,i)*rnd;
							}
							if (mean)
								for (size_t d=0;d<dim;d++)	(*ret_it)[d]+=(*mean)[d];
						}

					} // For each vector sample to generate...

					MRPT_END_WITH_CLEAN_UP( \
						printf("\nEXCEPTION: Dumping variables for debuging:\n"); \
						std::cout << "Z:\n" << Z << "D:\n" << D << "Cov:\n" << cov; \
						try \
						{
							cov.eigenVectors(Z,D); \
							std::cout << "Original Z:" << Z << "Original D:" << D;  \
						} \
						catch(...) {}; \
						);

				}


				/** Generate multidimensional random samples according to a given covariance matrix.
				 *  Mean is assumed to be zero if mean==NULL.
				 * \exception std::exception On invalid covariance matrix
				 * \sa drawGaussianMultivariateMany
				 */
				 template <class VECTORLIKE,typename T,size_t N>
				 void  drawGaussianMultivariate(
					VECTORLIKE	&out_result,
					const CMatrixFixedNumeric<T,N,N> &cov,
					const VECTORLIKE* mean = NULL
					)
				{
					if (mean) ASSERT_(mean->size()==N)

					CMatrixFixedNumeric<T,N,N>	Z,D;

					// Set size of output vector:
					out_result.assign(N,0);

					/** Computes the eigenvalues/eigenvector decomposition of this matrix,
					*    so that: M = Z � D � Z<sup>T</sup>, where columns in Z are the
					*	  eigenvectors and the diagonal matrix D contains the eigenvalues
					*    as diagonal elements, sorted in <i>ascending</i> order.
					*/
					cov.eigenVectors( Z, D );

					// Scale eigenvectors with eigenvalues:
					// Efficient implementation of: D.Sqrt(); Z = Z * D;
					for (size_t i=0;i<N;i++)
					{
						const T eig_sqrt = std::sqrt(D.get_unsafe(i,i));
						for (size_t j=0;j<N;j++)
							Z.get_unsafe(j,i)*=eig_sqrt;
					}


					for (size_t i=0;i<N;i++)
					{
						T rnd = drawGaussian1D_normalized();
						for (size_t d=0;d<N;d++)
							out_result[d]+= ( Z(d,i)* rnd );
					}
					if (mean)
						for (size_t d=0;d<N;d++)
							out_result[d]+= (*mean)[d];
				}

				/** Generate a given number of multidimensional random samples according to a given covariance matrix.
				 * \param cov The covariance matrix where to draw the samples from.
				 * \param desiredSamples The number of samples to generate.
				 * \param ret The output list of samples
				 * \param mean The mean, or zeros if mean==NULL.
				 */
				 template <typename T,size_t N>
				 void  drawGaussianMultivariateMany(
					std::vector< std::vector<T> >	&ret,
					size_t							desiredSamples,
					const CMatrixFixedNumeric<T,N,N> &cov,
					const std::vector<T>*			mean = NULL )
				{
					if (mean) ASSERT_(mean->size()==N)

					CMatrixFixedNumeric<T,N,N>	Z,D;

					/** Computes the eigenvalues/eigenvector decomposition of this matrix,
					*    so that: M = Z � D � Z<sup>T</sup>, where columns in Z are the
					*	  eigenvectors and the diagonal matrix D contains the eigenvalues
					*    as diagonal elements, sorted in <i>ascending</i> order.
					*/
					cov.eigenVectors( Z, D );

					// Scale eigenvectors with eigenvalues:
					// Efficient implementation of: D.Sqrt(); Z = Z * D;
					for (size_t i=0;i<N;i++)
					{
						const T eig_sqrt = std::sqrt(D.get_unsafe(i,i));
						for (size_t j=0;j<N;j++)
							Z.get_unsafe(j,i)*=eig_sqrt;
					}

					// Set size of output vector:
					ret.resize(desiredSamples);

					for (size_t k=0;k<desiredSamples;k++)
					{
						ret[k].assign(N,0);
						for (size_t i=0;i<N;i++)
						{
							T rnd = drawGaussian1D_normalized();
							for (size_t d=0;d<N;d++)
								ret[k][d]+= ( Z.get_unsafe(d,i)* rnd );
						}
						if (mean)
							for (size_t d=0;d<N;d++)
								ret[k][d]+= (*mean)[d];
					}
				}


			/** @} */


			/** @name Miscellaneous
			 @{ */

				/** Returns a random permutation of a vector: all the elements of the input vector are in the output but at random positions.
				  */
				template <class T>
				void  permuteVector(
					const std::vector<T> &in_vector,
					std::vector<T>       &out_result)
				{
					out_result = in_vector;
					std::random_shuffle( out_result.begin(),out_result.end()  );
				}

			/** @} */

		}; // end of CRandomGenerator --------------------------------------------------------------


		/** A static instance of a CRandomGenerator class, for use in single-thread applications */
		extern BASE_IMPEXP CRandomGenerator randomGenerator;


		/** A random number generator for usage in STL algorithms expecting a function like this (eg, random_shuffle):
		  */
		inline ptrdiff_t random_generator_for_STL(ptrdiff_t i)
		{
			return randomGenerator.drawUniform32bit() % i;
		}

		/** Generate a normalized normally distributed pseudo-random number.
		 *  \param likelihood If desired, pass a pointer to a double which will receive the likelihood of the given sample to have been obtained, that is, the value of the normal pdf at the sample value.
		 */
		MRPT_DECLARE_DEPRECATED_FUNCTION("** deprecated **: Use mrpt::random::randomGenerator instead",
		double normalizedGaussian( double *likelihood = NULL) );

		/** Generate a normally distributed pseudo-random number.
		 * \param mean The mean value of desired normal distribution
		 * \param std  The standard deviation value of desired normal distribution
		 */
		MRPT_DECLARE_DEPRECATED_FUNCTION("** deprecated **: Use mrpt::random::randomGenerator instead",
		double RandomNormal( double mean = 0, double std = 1) );

		/** Generate a uniformly distributed pseudo-random number using the MT19937 algorithm, in the whole range of 32-bit integers.
		  *  See: http://en.wikipedia.org/wiki/Mersenne_twister
		  * \sa RandomUni, Randomize
		  */
		MRPT_DECLARE_DEPRECATED_FUNCTION("** deprecated **: Use mrpt::random::randomGenerator instead",
		uint32_t RandomUniInt() );

		/** Generate a uniformly distributed pseudo-random number using the MT19937 algorithm, scaled to the selected range.
		  *  This function uses internally RandomUniInt to generate the number, then scales it to the desired range.
		  *  Since MRPT 0.6.0 the MT19937 algorithm is used instead of C runtime library "rand" version.
		  *  See: http://en.wikipedia.org/wiki/Mersenne_twister
		  * \sa RandomUniInt, Randomize
		  */
		MRPT_DECLARE_DEPRECATED_FUNCTION("** deprecated **: Use mrpt::random::randomGenerator instead",
		double RandomUni( const double min, const double max) );

		/** Fills the given matrix with independent, uniformly distributed samples.
		  * Matrix classes can be CMatrixTemplateNumeric or CMatrixFixedNumeric
		  * \sa matrixRandomNormal
		  */
		template <class MAT>
		void matrixRandomUni(
			MAT &matrix,
			const  double unif_min = 0,
			const  double unif_max = 1 )
		{
			for (size_t r=0;r<matrix.getRowCount();r++)
				for (size_t c=0;c<matrix.getColCount();c++)
					matrix.get_unsafe(r,c) = static_cast<typename MAT::value_type>( randomGenerator.drawUniform(unif_min,unif_max) );
		}

		/** Fills the given matrix with independent, uniformly distributed samples.
		  * \sa vectorRandomNormal
		  */
		template <class T>
		void vectorRandomUni(
			std::vector<T> &v_out,
			const  T& unif_min = 0,
			const  T& unif_max = 1 )
		{
			size_t n = v_out.size();
			for (size_t r=0;r<n;r++)
				v_out[r] = randomGenerator.drawUniform(unif_min,unif_max);
		}

		/** Fills the given matrix with independent, normally distributed samples.
		  * Matrix classes can be CMatrixTemplateNumeric or CMatrixFixedNumeric
		  * \sa matrixRandomUni
		  */
		template <class MAT>
		void matrixRandomNormal(
			MAT &matrix,
			const double mean = 0,
			const double std = 1 )
		{
			for (size_t r=0;r<matrix.getRowCount();r++)
				for (size_t c=0;c<matrix.getColCount();c++)
					matrix.get_unsafe(r,c) = static_cast<typename MAT::value_type>( mean + std*randomGenerator.drawGaussian1D_normalized() );
		}

		/** Generates a random vector with independent, normally distributed samples.
		  * \sa matrixRandomUni
		  */
		template <class T>
		void vectorRandomNormal(
			std::vector<T> &v_out,
			const  T& mean = 0,
			const  T& std = 1 )
		{
			size_t n = v_out.size();
			for (size_t r=0;r<n;r++)
				v_out[r] = mean + std*randomGenerator.drawGaussian1D_normalized();
		}

		/** Randomize the generators.
		 *   A seed can be providen, or a current-time based seed can be used (default)
		 */
		inline void Randomize(const uint32_t seed)  {
			randomGenerator.randomize(seed);
		}
		inline void Randomize()  {
			randomGenerator.randomize();
		}

		/** Returns a random permutation of a vector: all the elements of the input vector are in the output but at random positions.
		  */
		template <class T>
		void  randomPermutation(
			const std::vector<T> &in_vector,
			std::vector<T>       &out_result)
		{
			randomGenerator.permuteVector(in_vector,out_result);
		}


		/** Generate multidimensional random samples according to a given covariance matrix.
		 * \exception std::exception On invalid covariance matrix
		 * \sa randomNormalMultiDimensionalMany
		 */
		template <typename T>
		void  randomNormalMultiDimensional(
			const CMatrixTemplateNumeric<T>	&cov,
			std::vector<T>		&out_result)
		 {
			randomGenerator.drawGaussianMultivariate(out_result,cov);
		 }

		 /** Generate a given number of multidimensional random samples according to a given covariance matrix.
		 * \param cov The covariance matrix where to draw the samples from.
		 * \param desiredSamples The number of samples to generate.
		 * \param samplesLikelihoods If desired, set to a valid pointer to a vector, where it will be stored the likelihoods of having obtained each sample: the product of the gaussian-pdf for each independent variable.
		 * \param ret The output list of samples
		 *
		 * \exception std::exception On invalid covariance matrix
		 *
		 * \sa randomNormalMultiDimensional
		 */
		 template <typename T>
		 void  randomNormalMultiDimensionalMany(
			const CMatrixTemplateNumeric<T>	&cov,
			size_t							desiredSamples,
			std::vector< std::vector<T> >	&ret,
			std::vector<T>					*samplesLikelihoods = NULL)
		{
			randomGenerator.drawGaussianMultivariateMany(ret,desiredSamples,cov,static_cast<const std::vector<T>*>(NULL),samplesLikelihoods);
		}

		/** Generate multidimensional random samples according to a given covariance matrix.
		 * \exception std::exception On invalid covariance matrix
		 * \sa randomNormalMultiDimensional
		 */
		 template <typename T,size_t N>
		 void  randomNormalMultiDimensionalMany(
			const CMatrixFixedNumeric<T,N,N> &cov,
			size_t							desiredSamples,
			std::vector< std::vector<T> >	&ret )
		 {
			 randomGenerator.drawGaussianMultivariateMany(ret,desiredSamples,cov);
		 }

		/** Generate multidimensional random samples according to a given covariance matrix.
		 * \exception std::exception On invalid covariance matrix
		 * \sa randomNormalMultiDimensionalMany
		 */
		 template <typename T,size_t N>
		 void  randomNormalMultiDimensional(
			const CMatrixFixedNumeric<T,N,N> &cov,
			std::vector<T>		&out_result)
		{
			randomGenerator.drawGaussianMultivariate(out_result,cov);
		}


	}// End of namespace

} // End of namespace

#endif
