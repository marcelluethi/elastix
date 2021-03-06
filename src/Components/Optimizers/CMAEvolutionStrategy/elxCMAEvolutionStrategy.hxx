/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxCMAEvolutionStrategy_hxx
#define __elxCMAEvolutionStrategy_hxx

#include "elxCMAEvolutionStrategy.h"
#include <iomanip>
#include <string>
#include "vnl/vnl_math.h"

namespace elastix
{



  /**
   * ***************** StartOptimization ************************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>::
    StartOptimization(void)
  {
    /** Check if the entered scales are correct and != [ 1 1 1 ...] */
    this->SetUseScales(false);
    const ScalesType & scales = this->GetScales();
    if ( scales.GetSize() == this->GetInitialPosition().GetSize() )
    {
      ScalesType unit_scales( scales.GetSize() );
      unit_scales.Fill(1.0);
      if (scales != unit_scales)
      {
        /** only then: */
        this->SetUseScales(true);
      }
    }

    /** Call the superclass */
    this->Superclass1::StartOptimization();

  } //end StartOptimization


  /**
   * ***************** InitializeProgressVariables ************************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>::
    InitializeProgressVariables(void)
  {
    this->Superclass1::InitializeProgressVariables();

    /** Print some settings that can be automatically determined by the optimizer. */
    elxout
      << "The CMAEvolutionStrategy optimizer uses the following settings:\n"
      << "PopulationSize = " << this->GetPopulationSize() << "\n"
      << "NumberOfParents = " << this->GetNumberOfParents() << "\n"
      << "UseCovarianceMatrixAdaptation = " << this->GetUseCovarianceMatrixAdaptation() << "\n"
      << "UpdateBDPeriod = " << this->GetUpdateBDPeriod() << "\n" << std::endl;

  } // end InitializeProgressVariables


  /**
   * ***************** BeforeRegistration ***********************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>::
    BeforeRegistration(void)
  {
    using namespace xl;

    /** Add target cells to xout["iteration"].*/
    xout["iteration"].AddTargetCell("2:Metric");
    xout["iteration"].AddTargetCell("3:StepLength");
    xout["iteration"].AddTargetCell("4:||Step||");
    xout["iteration"].AddTargetCell("5a:Sigma");
    xout["iteration"].AddTargetCell("5b:MaximumD");
    xout["iteration"].AddTargetCell("5c:MinimumD");

    /** Format the metric and stepsize as floats */
    xout["iteration"]["2:Metric"]   << std::showpoint << std::fixed;
    xout["iteration"]["3:StepLength"] << std::showpoint << std::fixed;
    xout["iteration"]["4:||Step||"] << std::showpoint << std::fixed;
    xout["iteration"]["5a:Sigma"] << std::showpoint << std::fixed;
    xout["iteration"]["5b:MaximumD"] << std::showpoint << std::fixed;
    xout["iteration"]["5c:MinimumD"] << std::showpoint << std::fixed;

  } // end BeforeRegistration


  /**
   * ***************** BeforeEachResolution ***********************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>
    ::BeforeEachResolution(void)
  {
    /** Get the current resolution level.*/
    unsigned int level = static_cast<unsigned int>(
      this->m_Registration->GetAsITKBaseType()->GetCurrentLevel() );

    /** Set MaximumNumberOfIterations.*/
    unsigned int maximumNumberOfIterations = 500;
    this->m_Configuration->ReadParameter( maximumNumberOfIterations,
      "MaximumNumberOfIterations", this->GetComponentLabel(), level, 0 );
    this->SetMaximumNumberOfIterations( maximumNumberOfIterations );

    /** Set the length of the initial step (InitialSigma). */
    double stepLength = 1.0;
    this->m_Configuration->ReadParameter( stepLength,
      "StepLength", this->GetComponentLabel(), level, 0 );
    this->SetInitialSigma( stepLength );

    /** Set ValueTolerance */
    double valueTolerance = 0.00001;
    this->m_Configuration->ReadParameter( valueTolerance,
      "ValueTolerance", this->GetComponentLabel(), level, 0 );
    this->SetValueTolerance(valueTolerance);

    /** Set PopulationSize */
    unsigned int populationSize = 0;
    this->m_Configuration->ReadParameter( populationSize,
      "PopulationSize", this->GetComponentLabel(), level, 0 );
    this->SetPopulationSize( populationSize );

    /** Set NumberOfParents */
    unsigned int numberOfParents = 0;
    this->m_Configuration->ReadParameter( numberOfParents,
      "NumberOfParents", this->GetComponentLabel(), level, 0 );
    this->SetNumberOfParents( numberOfParents );

    /** Set UseDecayingSigma */
    bool useDecayingSigma = false;
    this->m_Configuration->ReadParameter( useDecayingSigma,
      "UseDecayingSigma", this->GetComponentLabel(), level, 0 );
    this->SetUseDecayingSigma( useDecayingSigma );

    /** Set SigmaDecayA */
    double sigmaDecayA = 50.0;
    this->m_Configuration->ReadParameter( sigmaDecayA,
      "SP_A", this->GetComponentLabel(), level, 0 );
    this->SetSigmaDecayA( sigmaDecayA );

    /** Set SigmaDecayAlpha */
    double sigmaDecayAlpha = 0.602;
    this->m_Configuration->ReadParameter( sigmaDecayAlpha,
      "SP_alpha", this->GetComponentLabel(), level, 0 );
    this->SetSigmaDecayAlpha( sigmaDecayAlpha );

    /** Set UseCovarianceMatrixAdaptation */
    bool useCovarianceMatrixAdaptation = true;
    this->m_Configuration->ReadParameter( useCovarianceMatrixAdaptation,
      "UseCovarianceMatrixAdaptation", this->GetComponentLabel(), level, 0 );
    this->SetUseCovarianceMatrixAdaptation( useCovarianceMatrixAdaptation );

    /** Set RecombinationWeightsPreset */
    std::string recombinationWeightsPreset = "superlinear";
    this->m_Configuration->ReadParameter( recombinationWeightsPreset,
      "RecombinationWeightsPreset", this->GetComponentLabel(), level, 0 );
    this->SetRecombinationWeightsPreset( recombinationWeightsPreset );

    /** Set UpdateBDPeriod */
    unsigned int updateBDPeriod = 0;
    this->m_Configuration->ReadParameter( updateBDPeriod,
      "UpdateBDPeriod", this->GetComponentLabel(), level, 0 );
    this->SetUpdateBDPeriod( updateBDPeriod );

    /** Set PositionToleranceMin */
    double positionToleranceMin = 1e-8;
    this->m_Configuration->ReadParameter( positionToleranceMin,
      "PositionToleranceMin", this->GetComponentLabel(), level, 0 );
    this->SetPositionToleranceMin( positionToleranceMin );

    /** Set PositionToleranceMax */
    double positionToleranceMax = 1e8;
    this->m_Configuration->ReadParameter( positionToleranceMax,
      "PositionToleranceMax", this->GetComponentLabel(), level, 0 );
    this->SetPositionToleranceMax( positionToleranceMax );

    /** Set MaximumDeviation */
    double maximumDeviation = 10.0 * positionToleranceMax * stepLength;
    this->m_Configuration->ReadParameter( maximumDeviation,
      "MaximumDeviation", this->GetComponentLabel(), level, 0 );
    this->SetMaximumDeviation( maximumDeviation );

    /** Set MinimumDeviation */
    double minimumDeviation = 0.0;
    this->m_Configuration->ReadParameter( minimumDeviation,
      "MinimumDeviation", this->GetComponentLabel(), level, 0 );
    this->SetMinimumDeviation( minimumDeviation );

  } // end BeforeEachResolution


  /**
   * ***************** AfterEachIteration *************************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>
    ::AfterEachIteration(void)
  {
    using namespace xl;

    /** Print some information. */
    xout["iteration"]["2:Metric"] << this->GetCurrentValue();
    xout["iteration"]["3:StepLength"] << this->GetCurrentStepLength();
    xout["iteration"]["4:||Step||"] << this->GetCurrentScaledStep().magnitude();
    xout["iteration"]["5a:Sigma"] << this->GetCurrentSigma();
    xout["iteration"]["5b:MaximumD"] << this->GetCurrentMaximumD();
    xout["iteration"]["5c:MinimumD"] << this->GetCurrentMinimumD();

    /** Select new samples if desired. These
     * will be used in the next iteration */
    if ( this->GetNewSamplesEveryIteration() )
    {
      this->SelectNewSamples();
    }

  } // end AfterEachIteration


  /**
   * ***************** AfterEachResolution *************************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>
    ::AfterEachResolution(void)
  {
    /**
      typedef enum {
      MetricError,
      MaximumNumberOfIterations,
      PositionToleranceMin,
      PositionToleranceMax,
      ValueTolerance,
      ZeroStepLength,
      Unknown }    StopConditionType;  */

    std::string stopcondition;

    switch( this->GetStopCondition() )
    {
    case MetricError :
      stopcondition = "Error in metric";
      break;

    case MaximumNumberOfIterations :
      stopcondition = "Maximum number of iterations has been reached";
      break;

    case PositionToleranceMin :
      stopcondition = "The minimum step length condition has been reached";
      break;

    case PositionToleranceMax :
      stopcondition = "The maximum step length condition has been reached";
      break;

    case ValueTolerance :
      stopcondition = "Almost no decrease in function value anymore";
      break;

    case ZeroStepLength :
      stopcondition = "The step length is 0";
      break;

    default:
      stopcondition = "Unknown";
      break;
    }

    /** Print the stopping condition */
    elxout << "Stopping condition: " << stopcondition << "." << std::endl;


  } // end AfterEachResolution


  /**
   * ******************* AfterRegistration ************************
   */

  template <class TElastix>
    void CMAEvolutionStrategy<TElastix>
    ::AfterRegistration(void)
  {
    /** Print the best metric value */

    double bestValue = this->GetCurrentValue();
    elxout
      << std::endl
      << "Final metric value  = "
      << bestValue
      << std::endl;

  } // end AfterRegistration


} // end namespace elastix

#endif // end #ifndef __elxCMAEvolutionStrategy_hxx


