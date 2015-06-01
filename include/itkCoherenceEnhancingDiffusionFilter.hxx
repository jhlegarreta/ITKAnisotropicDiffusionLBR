/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef itkCoherenceEnhancingDiffusionFilter_hxx
#define itkCoherenceEnhancingDiffusionFilter_hxx

namespace itk
{

template< typename TImage, typename TScalar >
CoherenceEnhancingDiffusionFilter< TImage, TScalar >
::CoherenceEnhancingDiffusionFilter():
  m_Lambda( 0.05 ),
  m_Exponent( 2 ),
  m_Alpha( 0.01 ),
  m_Enhancement( CED )
{
}


template< typename TImage, typename TScalar >
typename CoherenceEnhancingDiffusionFilter< TImage, TScalar >::EigenValuesArrayType
CoherenceEnhancingDiffusionFilter< TImage, TScalar >
::EigenValuesTransform(const EigenValuesArrayType & ev0) const
{
  const ScalarType evMin = ev0[0];
  const ScalarType evMax = ev0[Dimension-1];

  EigenValuesArrayType ev;
  switch(m_Enhancement)
    {
        // Weickert's filter.
    case CED:
        for( unsigned int i = 0; i < Dimension; ++i )
          {
          ev[i] = g_CED(evMax-ev0[i]);
          }
        break;

        // A variance, requiring stronger coherence.
    case cCED:
        for( unsigned int i = 0; i < Dimension; ++i )
          {
          ev[i] = g_CED( (evMax-ev0[i])/(1.+ev0[i]/m_Lambda) );
          }
        break;

        // Weickert's filter.
    case EED:
        for( unsigned int i = 0; i < Dimension; ++i )
          {
          ev[i] = g_EED(ev0[i]-evMin);
          }
        break;

        // A variant, promoting diffusion in at least one direction at each point.
    case cEED:
        for( unsigned int i = 0; i < Dimension; ++i )
          {
          ev[i] = g_EED(ev0[i]);
          }
        break;

        // Isotropic tensors, closely related to Perona-Malik's approach.
    case Isotropic:
        for( unsigned int i = 0; i < Dimension; ++i )
          {
          ev[i] = g_EED(evMax);
          }
        break;

    default:
        itkExceptionMacro("Unsupported diffusion type");
  }
  return ev;
}

} // end namespace itk

#endif
