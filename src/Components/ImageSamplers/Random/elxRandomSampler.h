/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxRandomSampler_h
#define __elxRandomSampler_h

#include "itkImageRandomSampler.h"
#include "elxIncludes.h"

namespace elastix
{


  /**
   * \class RandomSampler
   * \brief An interpolator based on the itk::ImageRandomSampler.
   *
   * This image sampler randomly samples 'NumberOfSamples' voxels in
   * the InputImageRegion. Voxels may be selected multiple times.
   * If a mask is given, the sampler tries to find samples within the
   * mask. If the mask is very sparse, this may take some time. In this case,
   * consider using the RandomSparseMask image sampler.
   *
   * This sampler is suitable to used in combination with the
   * NewSamplesEveryIteration parameter (defined in the elx::OptimizerBase).
   *
   * The parameters used in this class are:
   * \parameter ImageSampler: Select this image sampler as follows:\n
   *    <tt>(ImageSampler "Random")</tt>
   * \parameter NumberOfSpatialSamples: The number of image voxels used for computing the
   *    metric value and its derivative in each iteration. Must be given for each resolution.\n
   *    example: <tt>(NumberOfSpatialSamples 2048 2048 4000)</tt> \n
   *    The default is 5000.
   *
   * \ingroup ImageSamplers
   */

  template < class TElastix >
    class RandomSampler :
    public
      itk::ImageRandomSampler<
      typename elx::ImageSamplerBase<TElastix>::InputImageType >,
    public
      elx::ImageSamplerBase<TElastix>
  {
  public:

    /** Standard ITK-stuff. */
    typedef RandomSampler                           Self;
    typedef itk::ImageRandomSampler<
      typename elx::ImageSamplerBase<TElastix>::InputImageType >
                                                    Superclass1;
    typedef elx::ImageSamplerBase<TElastix>         Superclass2;
    typedef itk::SmartPointer<Self>                 Pointer;
    typedef itk::SmartPointer<const Self>           ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro( RandomSampler, itk::ImageRandomSampler );

    /** Name of this class.
     * Use this name in the parameter file to select this specific interpolator. \n
     * example: <tt>(ImageSampler "Random")</tt>\n
     */
    elxClassNameMacro( "Random" );

    /** Typedefs inherited from the superclass. */
    typedef typename Superclass1::DataObjectPointer            DataObjectPointer;
    typedef typename Superclass1::OutputVectorContainerType    OutputVectorContainerType;
    typedef typename Superclass1::OutputVectorContainerPointer OutputVectorContainerPointer;
    typedef typename Superclass1::InputImageType               InputImageType;
    typedef typename Superclass1::InputImagePointer            InputImagePointer;
    typedef typename Superclass1::InputImageConstPointer       InputImageConstPointer;
    typedef typename Superclass1::InputImageRegionType         InputImageRegionType;
    typedef typename Superclass1::InputImagePixelType          InputImagePixelType;
    typedef typename Superclass1::ImageSampleType              ImageSampleType;
    typedef typename Superclass1::ImageSampleContainerType     ImageSampleContainerType;
    typedef typename Superclass1::MaskType                     MaskType;
    typedef typename Superclass1::InputImageIndexType          InputImageIndexType;
    typedef typename Superclass1::InputImagePointType          InputImagePointType;

    /** The input image dimension. */
    itkStaticConstMacro( InputImageDimension, unsigned int, Superclass1::InputImageDimension );

    /** Typedefs inherited from Elastix. */
    typedef typename Superclass2::ElastixType               ElastixType;
    typedef typename Superclass2::ElastixPointer            ElastixPointer;
    typedef typename Superclass2::ConfigurationType         ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
    typedef typename Superclass2::RegistrationType          RegistrationType;
    typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
    typedef typename Superclass2::ITKBaseType               ITKBaseType;

    /** Execute stuff before each resolution:
     * \li Set the number of samples.
     */
    virtual void BeforeEachResolution(void);

  protected:

    /** The constructor. */
    RandomSampler() {}
    /** The destructor. */
    virtual ~RandomSampler() {}

  private:

    /** The private constructor. */
    RandomSampler( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );      // purposely not implemented

  }; // end class RandomSampler


} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxRandomSampler.hxx"
#endif

#endif // end #ifndef __elxRandomSampler_h

