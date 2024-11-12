
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     mat_tools.cpp                     //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, either version 2.1 of the        //
// License, or (at your option) any later version.       //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, see <http://www.gnu.org/licenses/>.              //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <time.h>
#include <cfloat>

#include "mat_tools.h"

#include "table.h"
#include "grid.h"
#include "grids.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double			SG_Get_Square(double Value)
{
	return( Value * Value );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double			SG_Get_Rounded(double Value, int Decimals)
{
	if( Decimals < 0 )
	{
		return( Value );
	}

	if( Decimals == 0 )
	{
		return( floor(0.5 + Value) );
	}

	double	d	= pow(10., Decimals);
	double	v	= Value * d;

	if( fabs(v - floor(v)) > 0. )
	{
		return( floor(0.5 + v) / d );
	}

	return( Value );
}

//---------------------------------------------------------
double			SG_Get_Rounded_To_SignificantFigures(double Value, int Decimals)
{
	if( Decimals <= 0 || Value == 0. )
	{
		return( (int)(0.5 + Value) );
	}

	Decimals	= (int)(-(ceil(log10(fabs(Value))) - Decimals));

	if( Decimals > 0 )
	{
		double	d	= pow(10., Decimals);

		return( Value < 0.
			? -((int)(0.5 - Value * d)) / d
			:  ((int)(0.5 + Value * d)) / d
		); 
	}
	else
	{
		double	d	= pow(10., -Decimals);

		return( Value < 0.
			? -((int)(0.5 - Value / d)) * d
			:  ((int)(0.5 + Value / d)) * d
		);
	}
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int				SG_Get_Digit_Count(int Number)
{
	Number	= abs(Number);

	return( Number < 10 ? 1 : 1 + (int)log10((double)Number) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String		SG_Get_Double_asString(double Number, int Width, int Precision, bool bScientific)
{
	if( bScientific )
	{
		if( Width > 0 && Precision >= 0 )	return( CSG_String::Format("%*.*e", Width, Precision, Number) );
		if( Width > 0                   )	return( CSG_String::Format("%*e"  , Width           , Number) );
		if(              Precision >= 0 )	return( CSG_String::Format("%.*e" ,        Precision, Number) );

		return( CSG_String::Format("%e", Number) );
	}
	else
	{
		if( Width > 0 && Precision >= 0 )	return( CSG_String::Format("%*.*f", Width, Precision, Number) );
		if( Width > 0                   )	return( CSG_String::Format("%*f"  , Width           , Number) );
		if(              Precision >= 0 )	return( CSG_String::Format("%.*f" ,        Precision, Number) );

		return( CSG_String::Format("%f", Number) );
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int SG_Compare_Int(const void *a, const void *b)
{
	if( *((int *)a) < *((int *)b) )
		return( -1  );

	if( *((int *)a) > *((int *)b) )
		return(  1  );

	return(  0  );
}

//---------------------------------------------------------
int SG_Compare_Double(const void *a, const void *b)
{
	if( *((double *)a) < *((double *)b) )
		return( -1  );

	if( *((double *)a) > *((double *)b) )
		return(  1  );

	return(  0  );
}

//---------------------------------------------------------
int SG_Compare_Char_Ptr(const void *a, const void *b)
{
	return( strcmp((const char *)a, (const char *)b) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double	SG_Degree_To_Decimal(double Deg, double Min, double Sec)
{
	return( Deg > 0.
		? (Deg + Min / 60. + Sec / 3600.)
		: (Deg - Min / 60. - Sec / 3600.)
	);
}

//---------------------------------------------------------
void	SG_Decimal_To_Degree(double Value, double &Deg, double &Min, double &Sec)
{
	Sec	= fmod(Value < 0. ? -Value : Value, 360.);

	Deg	= (int)Sec; Sec = 60. * (Sec - Deg);
	Min	= (int)Sec; Sec = 60. * (Sec - Min);

	if( Value < 0. )
	{
		Deg	= -Deg;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Random::CSG_Random(void)
{
	Initialize();
}

//---------------------------------------------------------
void CSG_Random::Initialize(void)
{
	Initialize((unsigned)time(NULL));
}

//---------------------------------------------------------
void CSG_Random::Initialize(unsigned int Value)
{
	srand(Value);
}

//---------------------------------------------------------
// Uniform distributed pseudo-random numbers in the range from 0 to 1.
//
double CSG_Random::Get_Uniform(void)
{
	return( 1. * rand() / (double)RAND_MAX );
}

//---------------------------------------------------------
// Uniform distributed pseudo-random numbers in the range from min to max.
//
double CSG_Random::Get_Uniform(double min, double max)
{
	return( min + (max - min) * rand() / (double)RAND_MAX );
}

//---------------------------------------------------------
// Generating Gaussian pseudo-random numbers using
// the polar form of the Box-Muller transformation.
//
// Box, G.E.P, Muller, M.E. (1958):
//   'A note on the generation of random normal deviates',
//    Annals Math. Stat, V. 29, pp. 610-611
//
// Link: http://www.taygeta.com/random/gaussian.html
//
//---------------------------------------------------------
double CSG_Random::Get_Gaussian(double mean, double stddev)
{
	double	x1, x2, w;

	do
	{
		x1	= 2. * Get_Uniform() - 1.;
		x2	= 2. * Get_Uniform() - 1.;

		w	= x1 * x1 + x2 * x2;
	}
	while( w >= 1. );

	w	= sqrt((-2. * log(w)) / w);

	return( mean + stddev * x1 * w );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Simple_Statistics::CSG_Simple_Statistics(void)
{
	Create(false);
}

CSG_Simple_Statistics::CSG_Simple_Statistics(bool bHoldValues)
{
	Create(bHoldValues);
}

CSG_Simple_Statistics::CSG_Simple_Statistics(const CSG_Simple_Statistics &Statistics)
{
	Create(Statistics);
}

CSG_Simple_Statistics::CSG_Simple_Statistics(double Mean, double StdDev, sLong Count)
{
	Create(Mean, StdDev, Count);
}

CSG_Simple_Statistics::CSG_Simple_Statistics(const CSG_Vector &Values, bool bHoldValues)
{
	Create(Values, bHoldValues);
}

//---------------------------------------------------------
bool CSG_Simple_Statistics::Create(bool bHoldValues)
{
	Invalidate();

	m_Values.Create(bHoldValues ? sizeof(double) : 0, 0, TSG_Array_Growth::SG_ARRAY_GROWTH_1);

	return( true );
}

bool CSG_Simple_Statistics::Create(const CSG_Simple_Statistics &Statistics)
{
	m_bEvaluated	= Statistics.m_bEvaluated;

	m_nValues		= Statistics.m_nValues;
	m_Weights		= Statistics.m_Weights;
	m_Sum			= Statistics.m_Sum;
	m_Sum2			= Statistics.m_Sum2;

	m_Minimum		= Statistics.m_Minimum;
	m_Maximum		= Statistics.m_Maximum;
	m_Range			= Statistics.m_Range;
	m_Mean			= Statistics.m_Mean;
	m_Variance		= Statistics.m_Variance;
	m_StdDev		= Statistics.m_StdDev;

	m_Kurtosis		= Statistics.m_Kurtosis;
	m_Skewness		= Statistics.m_Skewness;

	m_Gini			= Statistics.m_Gini;

	m_bSorted		= Statistics.m_bSorted;
	m_Values		.Create(Statistics.m_Values);

	return( true );
}

bool CSG_Simple_Statistics::Create(double Mean, double StdDev, sLong Count)
{
	Invalidate();

	m_bEvaluated	= 1;

	m_Mean			= Mean;
	m_StdDev		= StdDev;
	m_Variance		= StdDev*StdDev;
	m_nValues		= Count;
	m_Weights		= (double)Count;

	m_Sum			= m_Weights *  m_Mean;
	m_Sum2			= m_Weights * (m_Mean*m_Mean + m_Variance);

	m_Minimum		= m_Mean - 1.5 * m_StdDev;
	m_Maximum		= m_Mean + 1.5 * m_StdDev;
	m_Range			= m_Maximum - m_Minimum;

	return( true );
}

bool CSG_Simple_Statistics::Create(const CSG_Vector &Values, bool bHoldValues)
{
	if( Create(bHoldValues) )
	{
		for(sLong i=0; i<Values.Get_Size(); i++)
		{
			Add_Value(Values[i]);
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Simple_Statistics::Set_Count(sLong nValues)
{
	if( m_nValues < 1 || nValues < 1 || m_nValues == nValues )
	{
		return( false );
	}

	double	Scale	= nValues / (double)m_nValues;

	m_Weights	*= Scale;
	m_Sum		*= Scale;
	m_Sum2		*= Scale;

	m_nValues	 = nValues;

	m_bEvaluated	= 0;

	m_Values.Destroy();

	return( true );
}

//---------------------------------------------------------
void CSG_Simple_Statistics::Invalidate(void)
{
	m_bEvaluated	= 0;

	m_nValues		= 0;
	m_Weights		= 0.;
	m_Sum			= 0.;
	m_Sum2			= 0.;

	m_Minimum		= 0.;
	m_Maximum		= 0.;
	m_Range			= 0.;
	m_Mean			= 0.;
	m_Variance		= 0.;
	m_StdDev		= 0.;

	m_Kurtosis		= 0.;
	m_Skewness		= 0.;

	m_Gini			= -1.;

	m_bSorted		= false;
	m_Values		.Destroy();
}

//---------------------------------------------------------
bool CSG_Simple_Statistics::Evaluate(void)
{
	_Evaluate();

	return( is_Evaluated() > 0 );
}

//---------------------------------------------------------
void CSG_Simple_Statistics::Add(const CSG_Simple_Statistics &Statistics)
{
	if( Statistics.m_nValues <= 0 )
	{
		return;
	}

	if( m_nValues == 0 )
	{
		Create(Statistics);

		return;
	}

	//--------------------------------------------------------
	if( (sLong)m_Values.Get_Size() == m_nValues && (sLong)Statistics.m_Values.Get_Size() == Statistics.m_nValues && m_Values.Set_Array((size_t)(m_nValues + Statistics.m_nValues)) )
	{
		for(sLong i=0, j=m_nValues; i<Statistics.m_nValues; i++, j++)
		{
			((double *)m_Values.Get_Array())[j]	= Statistics.Get_Value(i);
		}
	}
	else
	{
		m_Values.Destroy();
	}

	m_nValues		+= Statistics.m_nValues;
	m_Weights		+= Statistics.m_Weights;
	m_Sum			+= Statistics.m_Sum;
	m_Sum2			+= Statistics.m_Sum2;

	if( m_Minimum > Statistics.m_Minimum )
		m_Minimum	= Statistics.m_Minimum;

	if( m_Maximum < Statistics.m_Maximum )
		m_Maximum	= Statistics.m_Maximum;

	m_Kurtosis		= 0.;
	m_Skewness		= 0.;

	m_bEvaluated	= 0;
	m_bSorted		= false;
}

//---------------------------------------------------------
void CSG_Simple_Statistics::Add_Value(double Value, double Weight)
{
	if( m_nValues == 0 )
	{
		m_Minimum = m_Maximum = Value;
	}
	else if( m_Minimum > Value )
	{
		m_Minimum = Value;
	}
	else if( m_Maximum < Value )
	{
		m_Maximum = Value;
	}

	if( Weight )
	{
		m_Weights += fabs(Weight);
		m_Sum     += Weight * Value;
		m_Sum2    += Weight * Value*Value;

		m_bEvaluated = 0;

		if( m_Values.Get_Value_Size() > 0 && m_Values.Inc_Array() )
		{
			m_bSorted = false;

			((double *)m_Values.Get_Array())[m_nValues]	= Value;
		}

		m_nValues++;
	}
}

//---------------------------------------------------------
void CSG_Simple_Statistics::_Evaluate(int Level)
{
	if( m_bEvaluated == 0 && m_Weights > 0. )
	{
		m_bEvaluated	= 1;

		m_Range			= m_Maximum - m_Minimum;
		m_Mean			= m_Sum  / m_Weights;
		m_Variance		= m_Sum2 / m_Weights - m_Mean*m_Mean;
		m_StdDev		= m_Variance > 0. ? sqrt(m_Variance) : 0.;
	}

	//-----------------------------------------------------
	if( m_bEvaluated == 1 && Level > 1 )
	{
		m_bEvaluated	= 2;

		m_Kurtosis		= 0.;
		m_Skewness		= 0.;

		if( Get_StdDev() > 0. && m_Values.Get_Size() > 0 )
		{
			for(sLong i=0; i<Get_Count(); i++)
			{
				double	d	= (Get_Value(i) - Get_Mean()) / Get_StdDev();

				m_Kurtosis	+= d*d*d*d;
				m_Skewness	+= d*d*d;
			}

			m_Kurtosis	/= Get_Count();
			m_Skewness	/= Get_Count();
		//	m_Skewness	*= Get_Count() / ((Get_Count() - 1) * (Get_Count() - 2));
		}
	}
}

//---------------------------------------------------------
/**
* Skewness after Pearson, i.e. the difference of mean and
* median divided by standard deviation.
* Remark: Skewness calculation is only possible, if statistics
* has been created with the bHoldValues flag set to true.
*/
double CSG_Simple_Statistics::Get_SkewnessPearson(void)
{
	return( Get_StdDev() != 0. ? (Get_Mean() - Get_Median()) / Get_StdDev() : 0. );
}

//---------------------------------------------------------
/**
* Returns the requested quantile (value between 0 and 1).
* The 0.5 quantile returns the median. Remark:
* Remark: any quantile calculation is only possible, if statistics
* has been created with the bHoldValues option set to true.
*/
double CSG_Simple_Statistics::Get_Quantile(double Quantile)
{
	if( m_Values.Get_Size()  < 1 )
	{
		return( m_Mean );
	}

	//-----------------------------------------------------
	if( !m_bSorted )
	{
		qsort(m_Values.Get_Array(), m_Values.Get_Size(), sizeof(double), SG_Compare_Double);

		m_bSorted	= true;
	}

	if( Quantile <= 0. || m_Values.Get_Size() == 1 )
	{
		return( Get_Values()[0] );
	}

	if( Quantile >= 1. )
	{
		return( Get_Values()[m_Values.Get_Size() - 1] );
	}

	//-----------------------------------------------------
	double	r	= Quantile * (m_Values.Get_Size() - 1);

	sLong	i	= (sLong)r; r -= i;

	return( r == 0. ? Get_Values()[i] : ((1. - r) * Get_Values()[i] + r * Get_Values()[i + 1]) );
}

//---------------------------------------------------------
/**
* Returns the requested percentile (i.e. the quantile requested as
* percentage, the 50 percentile is the medium value).
* Remark: any quantile calculation is only possible, if statistics
* has been created with the bHoldValues option set to true.
*/
double CSG_Simple_Statistics::Get_Percentile(double Percentile)
{
	return( Get_Quantile(Percentile / 100.) );
}

//---------------------------------------------------------
/**
* Returns the medium (i.e. thee 50 percentile or 0.5 quantile).
* Remark: any quantile calculation is only possible, if statistics
* has been created with the bHoldValues option set to true.
*/
double CSG_Simple_Statistics::Get_Median(void)
{
	return( Get_Quantile(0.5) );
}

//---------------------------------------------------------
/**
* The Gini coefficient is a measure of statistical dispersion
* intended to represent the income or wealth distribution of 
* a nation's residents, and is the most commonly used measure
* of inequality.
*/
double CSG_Simple_Statistics::Get_Gini(void)
{
	if( m_Gini < 0. && m_Values.Get_Size() > 1 )
	{
		if( !m_bSorted )
		{
			qsort(m_Values.Get_Array(), m_Values.Get_Size(), sizeof(double), SG_Compare_Double);

			m_bSorted	= true;
		}

		m_Gini	= 0.;

		for(sLong i=0; i<Get_Count(); i++)
		{
			m_Gini	+= (i + 1.) * Get_Value(i);
		}

		m_Gini	= 2. * m_Gini / (Get_Count() * Get_Sum()) - (Get_Count() + 1.) / Get_Count();
	}

	return( m_Gini );
}

//---------------------------------------------------------
/**
* Returns the index of the minimum value in the order values have been added to the statistics.
* This is only supported for statistics that have been created with the bHoldValues option set to true.
*/
sLong CSG_Simple_Statistics::Get_IndexOfMinimum(void)
{
	if( m_Values.Get_Size() == 0 )	{	return( -1 );	}

	size_t	Index	= 0;
	double	Value	= Get_Values()[Index];

	for(size_t i=1; i<(size_t)m_Values.Get_Size(); i++)
	{
		if( Value > Get_Values()[i] )
		{
			Index	= i;
			Value	= Get_Values()[i];
		}
	}

	return( (sLong)Index );
}

//---------------------------------------------------------
/**
* Returns the index of the maximum value in the order values have been added to the statistics.
* This is only supported for statistics that have been created with the bHoldValues option set to true.
*/
sLong CSG_Simple_Statistics::Get_IndexOfMaximum(void)
{
	if( m_Values.Get_Size() == 0 )	{	return( -1 );	}

	size_t	Index	= 0;
	double	Value	= Get_Values()[Index];

	for(size_t i=1; i<(size_t)m_Values.Get_Size(); i++)
	{
		if( Value < Get_Values()[i] )
		{
			Index	= i;
			Value	= Get_Values()[i];
		}
	}

	return( (sLong)Index );
}

//---------------------------------------------------------
/**
* Returns the number of values greater than the threshold value.
* This is only supported for statistics that have been created with the bHoldValues option set to true.
*/
sLong CSG_Simple_Statistics::Get_nValues_Above(double Threshold, bool bEquals)
{
	if( m_Values.Get_Size() == 0 )	{	return( -1 );	}

	sLong	n	= 0;

	for(sLong i=0; i<Get_Count(); i++)
	{
		if( (bEquals && Get_Value(i) >= Threshold) || Get_Value(i) > Threshold )
		{
			n++;
		}
	}

	return( n );
}

//---------------------------------------------------------
/**
* Returns the number of values lower than the threshold value.
* This is only supported for statistics that have been created with the bHoldValues option set to true.
*/
sLong CSG_Simple_Statistics::Get_nValues_Below(double Threshold, bool bEquals)
{
	if( m_Values.Get_Size() == 0 )	{	return( -1 );	}

	sLong	n	= 0;

	for(sLong i=0; i<Get_Count(); i++)
	{
		if( (bEquals && Get_Value(i) <= Threshold) || Get_Value(i) < Threshold )
		{
			n++;
		}
	}

	return( n );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_Unique_Value_Statistics::Get_Majority(bool bWeighted) const
{
	int		Index	= 0;

	bWeighted	= bWeighted && m_bWeights;

	for(int i=1; i<Get_Count(); i++)
	{
		if( bWeighted )
		{
			if( m_Weight[i] > m_Weight[Index] )
			{
				Index = i;
			}
		}
		else
		{
			if( m_Count[i] > m_Count[Index] )
			{
				Index = i;
			}
		}
	}

	return( Index );
}

//---------------------------------------------------------
int CSG_Unique_Value_Statistics::Get_Minority(bool bWeighted) const
{
	int		Index	= 0;

	bWeighted	= bWeighted && m_bWeights;

	for(int i=1; i<Get_Count(); i++)
	{
		if( bWeighted )
		{
			if( m_Weight[i] < m_Weight[Index] )
			{
				Index = i;
			}
		}
		else
		{
			if( m_Count[i] < m_Count[Index] )
			{
				Index = i;
			}
		}
	}

	return( Index );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Unique_Number_Statistics::Create(bool bWeights)
{
	m_bWeights	= bWeights;

	m_Count.Destroy();
	m_Value.Destroy();
}

//---------------------------------------------------------
void CSG_Unique_Number_Statistics::Add_Value(double Value, double Weight)
{
	for(int i=0; i<Get_Count(); i++)
	{
		if( Value == m_Value[i] )
		{
			m_Count[i]++;

			if( m_bWeights && Weight > 0. )
			{
				m_Weight[i]	+= Weight;
			}

			return;
		}
	}

	m_Count.Add(1);
	m_Value.Add_Row(Value);

	if( m_bWeights && Weight > 0. )
	{
		m_Weight.Add_Row(Weight);
	}
}

//---------------------------------------------------------
int CSG_Unique_Number_Statistics::Get_Class_Index(double Value) const
{
	for(int i=0; i<Get_Count(); i++)
	{
		if( Value == m_Value[i] )
		{
			return( i );
		}
	}

	return( -1 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Unique_String_Statistics::Create(bool bWeights)
{
	m_bWeights	= bWeights;

	m_Count.Destroy();
	m_Value.Clear();
}

//---------------------------------------------------------
void CSG_Unique_String_Statistics::Add_Value(const CSG_String &Value, double Weight)
{
	for(int i=0; i<Get_Count(); i++)
	{
		if( Value.Cmp(m_Value[i]) == 0 )
		{
			m_Count[i]++;

			if( m_bWeights && Weight > 0. )
			{
				m_Weight[i]	+= Weight;
			}

			return;
		}
	}

	m_Count.Add(1);
	m_Value.Add(Value);

	if( m_bWeights && Weight > 0. )
	{
		m_Weight.Add_Row(Weight);
	}
}

//---------------------------------------------------------
int CSG_Unique_String_Statistics::Get_Class_Index(const CSG_String &Value) const
{
	for(int i=0; i<Get_Count(); i++)
	{
		if( Value.Cmp(m_Value[i]) == 0 )
		{
			return( i );
		}
	}

	return( -1 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Category_Statistics::CSG_Category_Statistics(TSG_Data_Type Type)
{
	m_pTable	= new CSG_Table;

	Create(Type);
}

//---------------------------------------------------------
CSG_Category_Statistics::~CSG_Category_Statistics(void)
{
	delete(m_pTable);
}

//---------------------------------------------------------
void CSG_Category_Statistics::Create(TSG_Data_Type Type)
{
	m_pTable->Destroy();

	m_pTable->Add_Field("VALUE", Type);
	m_pTable->Add_Field("COUNT", SG_DATATYPE_ULong);
}

//---------------------------------------------------------
void CSG_Category_Statistics::Destroy(void)
{
	m_pTable->Del_Records();
}

//---------------------------------------------------------
TSG_Data_Type CSG_Category_Statistics::Get_Category_Type(void)	const
{
	return( m_pTable->Get_Field_Type(0) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_Category_Statistics::Get_Category(int Value)	const
{
	CSG_Table_Record *pRecord = m_pTable->Find_Record(0, Value);

	return( pRecord ? (int)pRecord->Get_Index() : -1);
}

//---------------------------------------------------------
int CSG_Category_Statistics::Get_Category(double Value)	const
{
	CSG_Table_Record *pRecord = m_pTable->Find_Record(0, Value);

	return( pRecord ? (int)pRecord->Get_Index() : -1);
}

//---------------------------------------------------------
int CSG_Category_Statistics::Get_Category(const CSG_String &Value)	const
{
	CSG_Table_Record *pRecord = m_pTable->Find_Record(0, Value);

	return( pRecord ? (int)pRecord->Get_Index() : -1);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_Category_Statistics::Add_Value(int Value)
{
	CSG_Table_Record *pRecord = m_pTable->Get_Record_byIndex(Get_Category(Value));

	if( !pRecord )
	{
		(pRecord = m_pTable->Add_Record())->Set_Value(0, Value);

		if( m_pTable->Get_Count() > 16 || m_pTable->is_Indexed() )
		{
			Sort();
		}
	}

	pRecord->Add_Value(1, 1);

	return( (int)(m_pTable->Get_Count() - 1) );
}

//---------------------------------------------------------
int CSG_Category_Statistics::Add_Value(double Value)
{
	CSG_Table_Record *pRecord = m_pTable->Get_Record_byIndex(Get_Category(Value));

	if( !pRecord )
	{
		(pRecord = m_pTable->Add_Record())->Set_Value(0, Value);

		if( m_pTable->Get_Count() > 16 || m_pTable->is_Indexed() )
		{
			Sort();
		}
	}

	pRecord->Add_Value(1, 1);

	return( (int)(m_pTable->Get_Count() - 1) );
}

//---------------------------------------------------------
int CSG_Category_Statistics::Add_Value(const CSG_String &Value)
{
	CSG_Table_Record *pRecord = m_pTable->Get_Record_byIndex(Get_Category(Value));

	if( !pRecord )
	{
		(pRecord = m_pTable->Add_Record())->Set_Value(0, Value);

		if( m_pTable->Get_Count() > 16 || m_pTable->is_Indexed() )
		{
			Sort();
		}
	}

	pRecord->Add_Value(1, 1);

	return( (int)(m_pTable->Get_Count() - 1) );
}

//---------------------------------------------------------
// sort categories ascending
bool CSG_Category_Statistics::Sort(void)
{
	return( m_pTable->Set_Index(0, TABLE_INDEX_Ascending) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// returns the number of categories.
int CSG_Category_Statistics::Get_Count(void)	const
{
	return( (int)m_pTable->Get_Count() );
}

//---------------------------------------------------------
// returns the number of observations for the i'th category.
int CSG_Category_Statistics::Get_Count(int i)	const
{
	CSG_Table_Record	*pRecord	= m_pTable->Get_Record_byIndex(i);

	return( pRecord ? pRecord->asInt(1) : 0 );
}

//---------------------------------------------------------
int CSG_Category_Statistics::asInt(int i)	const
{
	CSG_Table_Record	*pRecord	= m_pTable->Get_Record_byIndex(i);

	return( pRecord ? pRecord->asInt(0) : 0 );
}

//---------------------------------------------------------
double CSG_Category_Statistics::asDouble(int i)	const
{
	CSG_Table_Record	*pRecord	= m_pTable->Get_Record_byIndex(i);

	return( pRecord ? pRecord->asDouble(0) : 0 );
}

//---------------------------------------------------------
CSG_String CSG_Category_Statistics::asString(int i)	const
{
	CSG_Table_Record	*pRecord	= m_pTable->Get_Record_byIndex(i);

	return( pRecord ? pRecord->asString(0) : SG_T("") );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_Category_Statistics::Get_Majority(void)
{
	if( m_pTable->Get_Count() > 0 )
	{
		int	Index = 0, Count = m_pTable->Get_Record_byIndex(0)->asInt(1);

		for(int i=1; i<m_pTable->Get_Count(); i++)
		{
			if( Count < m_pTable->Get_Record_byIndex(i)->asInt(1) )
			{
				Index	= i;
				Count	= m_pTable->Get_Record_byIndex(i)->asInt(1);
			}
		}

		return( Index );
	}

	return( -1 );
}

//---------------------------------------------------------
int CSG_Category_Statistics::Get_Minority(void)
{
	if( m_pTable->Get_Count() > 0 )
	{
		int	Index = 0, Count = m_pTable->Get_Record_byIndex(0)->asInt(1);

		for(int i=1; i<m_pTable->Get_Count(); i++)
		{
			if( Count > m_pTable->Get_Record_byIndex(i)->asInt(1) )
			{
				Index	= i;
				Count	= m_pTable->Get_Record_byIndex(i)->asInt(1);
			}
		}

		return( Index );
	}

	return( -1 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(void)
{
	_On_Construction();
}

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(const CSG_Histogram &Histogram)
{
	_On_Construction();

	Create(Histogram);
}

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(size_t nClasses, double Minimum, double Maximum)
{
	_On_Construction();

	Create(nClasses, Minimum, Maximum);
}

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(size_t nClasses, double Minimum, double Maximum, const CSG_Vector &Values, size_t maxSamples)
{
	_On_Construction();

	Create(nClasses, Minimum, Maximum, Values, maxSamples);
}

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(size_t nClasses, double Minimum, double Maximum, CSG_Table *pTable, int Field, size_t maxSamples)
{
	_On_Construction();

	Create(nClasses, Minimum, Maximum, pTable, Field, maxSamples);
}

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(size_t nClasses, double Minimum, double Maximum, class CSG_Grid *pGrid, size_t maxSamples)
{
	_On_Construction();

	Create(nClasses, Minimum, Maximum, pGrid, maxSamples);
}

//---------------------------------------------------------
CSG_Histogram::CSG_Histogram(size_t nClasses, double Minimum, double Maximum, CSG_Grids *pGrids, size_t maxSamples)
{
	_On_Construction();

	Create(nClasses, Minimum, Maximum, pGrids, maxSamples);
}

//---------------------------------------------------------
CSG_Histogram::~CSG_Histogram(void)
{
	Destroy();
}

//---------------------------------------------------------
bool CSG_Histogram::Destroy(void)
{
	m_Statistics.Create();

	SG_FREE_SAFE(m_Elements  );
	SG_FREE_SAFE(m_Cumulative);

	_On_Construction();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Histogram::_On_Construction(void)
{
	m_nClasses		= 0;
	m_Elements		= NULL;
	m_Cumulative	= NULL;
	m_Minimum		= 0.;
	m_Maximum		= 0.;
	m_ClassWidth	= 1.;
}

//---------------------------------------------------------
bool CSG_Histogram::_Create(size_t nClasses, double Minimum, double Maximum)
{
	if( nClasses > 0 && Minimum < Maximum )
	{
		Destroy();

		m_Elements		= (size_t *)SG_Calloc(nClasses, sizeof(size_t));
		m_Cumulative	= (size_t *)SG_Calloc(nClasses, sizeof(size_t));

		if( m_Elements && m_Cumulative )
		{
			m_nClasses		= nClasses;
			m_Minimum		= Minimum;
			m_Maximum		= Maximum;
			m_ClassWidth	= (Maximum - Minimum) / (double)m_nClasses;

			return( true );
		}
	}

	Destroy();

	return( false );
}

//---------------------------------------------------------
void CSG_Histogram::Add_Value(double Value)
{
	m_Statistics	+= Value;

	if( m_nClasses > 0 && m_Minimum <= Value && Value <= m_Maximum )
	{
		size_t	Class	= (size_t)((Value - m_Minimum) / m_ClassWidth);

		if( Class >= m_nClasses )
		{
			Class	= m_nClasses - 1;
		}

		m_Elements[Class]++;
	}
}

//---------------------------------------------------------
bool CSG_Histogram::Scale_Element_Count(double Scale)
{
	if( m_nClasses > 0 && Scale > 0. )
	{
		m_Statistics.Set_Count((sLong)(Scale * Get_Element_Count()));

		for(size_t i=0; i<m_nClasses; i++)
		{
			m_Elements[i]	= (size_t)(Scale * m_Elements[i]);
		}

		return( Update() );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Histogram::Update(void)
{
	if( m_nClasses > 0 )
	{
		m_Statistics.Get_Mean();	// _Evaluate()

		m_nMaximum	= m_Cumulative[0]	= m_Elements[0];

		for(size_t i=1; i<m_nClasses; i++)
		{
			m_Cumulative[i]	= m_Cumulative[i - 1] + m_Elements[i];

			if( m_nMaximum < m_Elements[i] )
			{
				m_nMaximum	= m_Elements[i];
			}
		}

		return( Get_Element_Count() > 0 );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Histogram::_Update(sLong nElements)
{
	if( nElements > 0 && m_Statistics.Get_Count() > 0 )
	{
		double	Scale	= (double)nElements / (double)m_Statistics.Get_Count();

		m_Statistics.Create(m_Statistics.Get_Mean(), m_Statistics.Get_StdDev(), nElements);

		for(size_t i=1; i<m_nClasses; i++)
		{
			m_Elements[i]	= (size_t)(0.5 + Scale * m_Elements[i]);
		}
	}

	return( Update() );
}

//---------------------------------------------------------
/**
* Returns the correspondent value for the requested quantile.
*/
double CSG_Histogram::Get_Quantile(double Quantile)	const
{
	if( m_nClasses < 2 ) { return( 0. ); }

	if( Quantile <= 0. ) { return( m_Minimum ); }
	if( Quantile >= 1. ) { return( m_Maximum ); }

	size_t	n = (size_t)(Quantile * Get_Element_Count());	// number of elements

	for(size_t i=0, n0=0; i<m_nClasses; n0=m_Cumulative[i++])
	{
		if( n < m_Cumulative[i] )
		{
			if( m_Cumulative[i] >= n0 )
			{
				return( Get_Center(i) );
			}

			double	d	= (n - n0) / (double)(m_Cumulative[i] - n0);

			return( Get_Break(i) + d * m_ClassWidth );
		}
		else if( n == m_Cumulative[i] )
		{
			return( Get_Break(i + 1) );
		}
	}

	return( m_Maximum );
}

//---------------------------------------------------------
/**
* Returns the correspondent value for the requested percentile.
*/
double CSG_Histogram::Get_Percentile(double Percentile) const
{
	return( Get_Quantile(Percentile / 100.) );
}

//---------------------------------------------------------
/**
* Returns the correspondent quantile for the requested value.
*/
double CSG_Histogram::Get_Quantile_Value(double Value) const
{
	if( m_nClasses < 2 ) { return( 0. ); }

	if( Value <= m_Minimum ) { return( 0. ); }
	if( Value >= m_Maximum ) { return( 1. ); }

	size_t	Class	= (size_t)(m_nClasses * (Value - m_Minimum) / (m_Maximum - m_Minimum));

	if( Class >= m_nClasses )
	{
		return( 1. );
	}

	if( Class < 1 )
	{
		double	dq	= m_Cumulative[Class] / (double)Get_Element_Count();

		return( dq * (Value - m_Minimum) / m_ClassWidth );
	}

	double	q0	=  m_Cumulative[Class - 1] / (double)Get_Element_Count();
	double	dq	= (m_Cumulative[Class    ] / (double)Get_Element_Count()) - q0;

	return( q0 + dq * (Value - Get_Break(Class)) / m_ClassWidth );
}

//---------------------------------------------------------
/**
* Returns the correspondent percentile for the requested value.
*/
double CSG_Histogram::Get_Percentile_Value(double Value) const
{
	return( Get_Quantile_Value(Value) * 100. );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Histogram::Create(const CSG_Histogram &Histogram)
{
	if( !_Create(Histogram.m_nClasses, Histogram.m_Minimum, Histogram.m_Maximum) )
	{
		return( false );
	}

	m_Statistics = Histogram.m_Statistics;
	m_ClassWidth = Histogram.m_ClassWidth;
	m_nMaximum   = Histogram.m_nMaximum  ;

	for(size_t i=0; i<m_nClasses; i++)
	{
		m_Cumulative[i]	= Histogram.m_Cumulative[i];
		m_Elements  [i]	= Histogram.m_Elements  [i];
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_Histogram::Create(size_t nClasses, double Minimum, double Maximum)
{
	return( _Create(nClasses, Minimum, Maximum) );
}

//---------------------------------------------------------
bool CSG_Histogram::Create(size_t nClasses, double Minimum, double Maximum, const CSG_Vector &Values, size_t maxSamples)
{
	if( Minimum >= Maximum )
	{
		CSG_Simple_Statistics	s(Values);

		Minimum	= s.Get_Minimum();
		Maximum	= s.Get_Maximum();
	}

	if( !_Create(nClasses, Minimum, Maximum) )
	{
		return( false );
	}

	//-----------------------------------------------------
	if( maxSamples > 0 && maxSamples < (size_t)Values.Get_N() )
	{
		double	d	= (double)Values.Get_N() / (double)maxSamples;

		for(double i=0; i<(double)Values.Get_N(); i+=d)
		{
			Add_Value(Values[(sLong)i]);
		}

		d	= (double)m_Statistics.Get_Count() / (double)maxSamples;

		return( _Update(d < 1. ? (int)(d * (double)Values.Get_N()) : Values.Get_N()) );
	}

	//-----------------------------------------------------
	for(int i=0; i<Values.Get_N(); i++)
	{
		Add_Value(Values[i]);
	}

	return( Update() );
}

//---------------------------------------------------------
bool CSG_Histogram::Create(size_t nClasses, double Minimum, double Maximum, CSG_Table *pTable, int Field, size_t maxSamples)
{
	if( !pTable || Field < 0 || Field >= pTable->Get_Field_Count() || !_Create(nClasses,
		Minimum < Maximum ? Minimum : pTable->Get_Minimum(Field),
		Minimum < Maximum ? Maximum : pTable->Get_Maximum(Field)) )
	{
		return( false );
	}

	//-----------------------------------------------------
	if( maxSamples > 0 && maxSamples < (size_t)pTable->Get_Count() )
	{
		double	d	= (double)pTable->Get_Count() / (double)maxSamples;

		for(double i=0; i<(double)pTable->Get_Count(); i+=d)
		{
			double	Value	= pTable->Get_Record((int)i)->asDouble(Field);

			if( !pTable->is_NoData_Value(Value) )
			{
				Add_Value(Value);
			}
		}

		d	= (double)m_Statistics.Get_Count() / (double)maxSamples;

		return( _Update(d < 1. ? (int)(d * pTable->Get_Count()) : pTable->Get_Count()) );
	}

	//-----------------------------------------------------
	for(sLong i=0; i<pTable->Get_Count(); i++)
	{
		double	Value	= pTable->Get_Record(i)->asDouble(Field);

		if( !pTable->is_NoData_Value(Value) )
		{
			Add_Value(Value);
		}
	}

	return( Update() );
}

//---------------------------------------------------------
bool CSG_Histogram::Create(size_t nClasses, double Minimum, double Maximum, CSG_Grid *pGrid, size_t maxSamples)
{
	if( !pGrid || !_Create(nClasses,
		Minimum < Maximum ? Minimum : pGrid->Get_Min(),
		Minimum < Maximum ? Maximum : pGrid->Get_Max()) )
	{
		return( false );
	}

	//-----------------------------------------------------
	if( maxSamples > 0 && (sLong)maxSamples < pGrid->Get_NCells() )
	{
		double	d	= (double)pGrid->Get_NCells() / (double)maxSamples;

		for(double i=0; i<(double)pGrid->Get_NCells(); i+=d)
		{
			double	Value	= pGrid->asDouble((sLong)i);

			if( !pGrid->is_NoData_Value(Value) )
			{
				Add_Value(Value);
			}
		}

		d	= (double)m_Statistics.Get_Count() / (double)maxSamples;

		return( _Update(d < 1. ? (sLong)(d * (double)pGrid->Get_NCells()) : pGrid->Get_NCells()) );
	}

	//-----------------------------------------------------
	for(sLong i=0; i<pGrid->Get_NCells(); i++)
	{
		if( !pGrid->is_NoData(i) )
		{
			Add_Value(pGrid->asDouble(i));
		}
	}

	return( Update() );
}

//---------------------------------------------------------
bool CSG_Histogram::Create(size_t nClasses, double Minimum, double Maximum, CSG_Grids *pGrids, size_t maxSamples)
{
	if( !pGrids || !_Create(nClasses,
		Minimum < Maximum ? Minimum : pGrids->Get_Min(),
		Minimum < Maximum ? Maximum : pGrids->Get_Max()) )
	{
		return( false );
	}

	//-----------------------------------------------------
	if( maxSamples > 0 && (sLong)maxSamples < pGrids->Get_NCells() )
	{
		double	d	= (double)pGrids->Get_NCells() / (double)maxSamples;

		for(double i=0; i<(double)pGrids->Get_NCells(); i+=d)
		{
			double	Value	= pGrids->asDouble((sLong)i);

			if( !pGrids->is_NoData_Value(Value) )
			{
				Add_Value(Value);
			}
		}

		d	= (double)m_Statistics.Get_Count() / (double)maxSamples;

		return( _Update(d < 1. ? (sLong)(d * (double)pGrids->Get_NCells()) : pGrids->Get_NCells()) );
	}

	//-----------------------------------------------------
	for(sLong i=0; i<pGrids->Get_NCells(); i++)
	{
		if( !pGrids->is_NoData(i) )
		{
			Add_Value(pGrids->asDouble(i));
		}
	}

	return( Update() );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Histogram & CSG_Histogram::operator = (const CSG_Histogram &Histogram)
{
	Create(Histogram);

	return( *this );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Natural_Breaks::CSG_Natural_Breaks(void)
{}

//---------------------------------------------------------
CSG_Natural_Breaks::~CSG_Natural_Breaks(void)
{}

//---------------------------------------------------------
CSG_Natural_Breaks::CSG_Natural_Breaks(CSG_Table *pTable, int Field, int nClasses, int Histogram)
{
	Create(pTable, Field, nClasses, Histogram);
}

//---------------------------------------------------------
CSG_Natural_Breaks::CSG_Natural_Breaks(CSG_Grid *pGrid, int nClasses, int Histogram)
{
	Create(pGrid, nClasses, Histogram);
}

//---------------------------------------------------------
CSG_Natural_Breaks::CSG_Natural_Breaks(CSG_Grids *pGrids, int nClasses, int Histogram)
{
	Create(pGrids, nClasses, Histogram);
}

//---------------------------------------------------------
CSG_Natural_Breaks::CSG_Natural_Breaks(const CSG_Vector &Values, int nClasses, int Histogram)
{
	Create(Values, nClasses, Histogram);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Natural_Breaks::Create(CSG_Table *pTable, int Field, int nClasses, int Histogram)
{
	bool	bResult	= false;

	if( Histogram > 0 )
	{
		bResult	= m_Histogram.Create(Histogram, 0, 0, pTable, Field) && _Histogram(nClasses);
	}
	else if( Field >= 0 && Field < pTable->Get_Field_Count() )
	{
		for(sLong i=0; i<pTable->Get_Count(); i++)
		{
			CSG_Table_Record	*pRecord	= pTable->Get_Record(i);

			if( !pRecord->is_NoData(Field) )
			{
				m_Values.Add_Row(pRecord->asDouble(Field));
			}
		}

		bResult	= m_Values.Sort() && _Calculate(nClasses);

		m_Values.Destroy();
	}

	return( bResult );
}

//---------------------------------------------------------
bool CSG_Natural_Breaks::Create(CSG_Grid *pGrid, int nClasses, int Histogram)
{
	bool	bResult	= false;

	if( Histogram > 0 )
	{
		bResult	= m_Histogram.Create(Histogram, 0, 0, pGrid) && _Histogram(nClasses);
	}
	else
	{
		for(sLong i=0; i<pGrid->Get_NCells(); i++)
		{
			if( !pGrid->is_NoData(i) )
			{
				m_Values.Add_Row(pGrid->asDouble(i));
			}
		}

		bResult	= m_Values.Sort() && _Calculate(nClasses);

		m_Values.Destroy();
	}

	return( bResult );
}

//---------------------------------------------------------
bool CSG_Natural_Breaks::Create(CSG_Grids *pGrids, int nClasses, int Histogram)
{
	bool	bResult	= false;

	if( Histogram > 0 )
	{
		bResult	= m_Histogram.Create(Histogram, 0, 0, pGrids) && _Histogram(nClasses);
	}
	else
	{
		for(sLong i=0; i<pGrids->Get_NCells(); i++)
		{
			if( !pGrids->is_NoData(i) )
			{
				m_Values.Add_Row(pGrids->asDouble(i));
			}
		}

		bResult	= m_Values.Sort() && _Calculate(nClasses);

		m_Values.Destroy();
	}

	return( bResult );
}

//---------------------------------------------------------
bool CSG_Natural_Breaks::Create(const CSG_Vector &Values, int nClasses, int Histogram)
{
	bool	bResult	= false;

	if( Histogram > 0 )
	{
		bResult	= m_Histogram.Create(Histogram, 0, 0, Values) && _Histogram(nClasses);
	}
	else
	{
		bResult	= m_Values.Create(Values) && m_Values.Sort() && _Calculate(nClasses);

		m_Values.Destroy();
	}

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Natural_Breaks::_Histogram(int nClasses)
{
	if( _Calculate(nClasses) )
	{
		double	d	= (double)m_Histogram.Get_Class_Count() / m_Histogram.Get_Cumulative((int)(m_Histogram.Get_Class_Count() - 1));

		m_Breaks[0]	= m_Histogram.Get_Break(0);

		for(int i=1; i<Get_Count(); i++)
		{
			m_Breaks[i]	= m_Histogram.Get_Value(m_Breaks[i] * d);
		}

		m_Breaks[nClasses]	= m_Histogram.Get_Break((int)m_Histogram.Get_Class_Count());

		m_Histogram.Destroy();

		return( true );
	}

	m_Histogram.Destroy();

	return( false );
}

//---------------------------------------------------------
inline double CSG_Natural_Breaks::_Get_Value(int i)
{
	if( m_Histogram.Get_Class_Count() > 0 )
	{
		return( (double)m_Histogram.Get_Cumulative(i) );
	}

	return( m_Values[i] );
}

//---------------------------------------------------------
bool CSG_Natural_Breaks::_Calculate(int nClasses)
{
	if( m_Histogram.Get_Class_Count() == 0 && m_Values.Get_Size() == 0 )
	{
		return( false );
	}

	int nValues = m_Histogram.Get_Class_Count() > 0 ? (int)m_Histogram.Get_Class_Count() : m_Values.Get_N();

	CSG_Matrix mv(nClasses, nValues); mv.Assign(FLT_MAX);

	int	**mc = (int **)SG_Malloc(nValues * sizeof(int *));

	mc[0] = (int *)SG_Calloc((size_t)nClasses * nValues, sizeof(int));

	for(int i=0; i<nValues; i++)
	{
		mc[i] = mc[0] + i * (size_t)nClasses;
	}

	//-----------------------------------------------------
	for(int i=1; i<nValues; i++)
	{
		double v = 0., s1 = 0., s2 = 0., w = 0.;

		for(int m=0, n=i+1; m<=i; m++, n--)
		{
			v   = _Get_Value(n);
			s2 += v*v;
			s1 += v;
			w  ++;
			v   = s2 - (s1 * s1) / w;

			if( n > 0 )
			{
				for(int j=1; j<nClasses; j++)
				{
					if( mv[i][j] >= (v + mv[n - 1][j - 1]) )
					{
						mc[i][j] = n;
						mv[i][j] = v + mv[n - 1][j - 1];
					}
				}
			}
		}

		mc[i][0] = 0;
		mv[i][0] = v;
	}

	//-----------------------------------------------------
	CSG_Array_Int Class(nClasses);

	for(int i=0; i<nClasses; i++)
	{
		Class[i] = i;
	}

	int j = Class[(size_t)nClasses - 1] = nValues - 1;

	for(int i=nClasses-1; i>0; i--)
	{
		Class[(size_t)i - 1] = j = mc[j - 1][i];
	}

	//-----------------------------------------------------
	m_Breaks.Create((size_t)nClasses + 1);

	m_Breaks[0] = _Get_Value(0);

	for(int i=1; i<nClasses; i++)
	{
		m_Breaks[i] = _Get_Value(Class[i - 1]);
	}

	m_Breaks[nClasses] = _Get_Value(nValues - 1);

	SG_Free(mc[0]); SG_Free(mc);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Cluster_Analysis::CSG_Cluster_Analysis(void)
{
	m_nFeatures = 0;
	m_Iteration = 0;
}

//---------------------------------------------------------
CSG_Cluster_Analysis::~CSG_Cluster_Analysis(void)
{
	Destroy();
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Destroy(void)
{
	m_Centroid.Destroy();
	m_Variance.Destroy();
	m_nMembers.Destroy();
	m_Clusters.Destroy();
	m_Features.Destroy();
	m_nFeatures = 0;
	m_Iteration = 0;

	return( true );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Create(int nFeatures)
{
	Destroy();

	if( nFeatures > 0 )
	{
		m_nFeatures = nFeatures;

		m_Features.Create(m_nFeatures * sizeof(double), 0, TSG_Array_Growth::SG_ARRAY_GROWTH_3);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Add_Element(void)
{
	return( m_nFeatures > 0 && m_Features.Inc_Array() );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Set_Feature(sLong iElement, int iFeature, double Value)
{
	if( iElement >= 0 && iElement < Get_nElements() && iFeature >= 0 && iFeature < m_nFeatures )
	{
		((double *)m_Features.Get_Entry(iElement))[iFeature] = Value;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
/**
* Performs the cluster analysis using the features added prior
* to this step. Method is minimum distance (= default), hill
* climbing (= 1), or both methods in combination. If nMaxIterations
* is set to zero, the analysis is iterated until it converges.
* Initialization is done randomly (= default), periodically (= 1),
* or skipped (= 2). The latter case allows starting the clustering
* with user supplied start partitions.
*/
//---------------------------------------------------------
bool CSG_Cluster_Analysis::Execute(int Method, int nClusters, int nMaxIterations, int Initialization)
{
	if( Get_nElements() < 2 || nClusters < 2 )
	{
		return( false );
	}

	//-----------------------------------------------------
	m_nMembers.Create(nClusters);
	m_Variance.Create(nClusters);
	m_Centroid.Create(m_nFeatures, nClusters);

	//-----------------------------------------------------
	m_Clusters.Create(Get_nElements());

	for(int iElement=0; iElement<Get_nElements(); iElement++)
	{
		switch( Initialization )
		{
		default:	// random
			if( (m_Clusters[iElement] = (int)CSG_Random::Get_Uniform(0, nClusters)) >= nClusters )
			{
				m_Clusters[iElement] = nClusters - 1;
			}
			break;

		case  1:	// periodic
			{
				m_Clusters[iElement] = iElement % nClusters;
			}
			break;

		case  2:	// keep as is, but check for valid cluster ids
			if( 0 > m_Clusters[iElement] || m_Clusters[iElement] >= nClusters )
			{
				m_Clusters[iElement] = iElement % nClusters;
			}
			break;
		}
	}

	//-----------------------------------------------------
	bool bResult;

	m_Iteration = 0;

	switch( Method )
	{
	default: bResult = _Minimum_Distance(true , nMaxIterations); break;
	case  1: bResult = _Hill_Climbing   (true , nMaxIterations); break;
	case  2: bResult = _Minimum_Distance(true , nMaxIterations)
	              &&   _Hill_Climbing   (false, nMaxIterations); break;
	}

	//-----------------------------------------------------
	if( bResult )
	{
		for(int iCluster=0; iCluster<nClusters; iCluster++)
		{
			m_Variance[iCluster] = m_nMembers[iCluster] <= 0 ? 0. : m_Variance[iCluster] / m_nMembers[iCluster];
		}
	}

	return( bResult );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::_Minimum_Distance(bool bInitialize, int nMaxIterations)
{
	int iElement, iCluster, nClusters = m_Variance.Get_N();

	double SP_Last = -1.;

	//-----------------------------------------------------
	for(m_Iteration=1; SG_UI_Process_Get_Okay(); m_Iteration++)
	{
		m_Variance = 0.;
		m_Centroid = 0.;
		m_nMembers = 0;

		//-------------------------------------------------
		for(iElement=0; iElement<Get_nElements(); iElement++)
		{
			m_nMembers[iCluster = m_Clusters[iElement]]++;

			double *Feature = (double *)m_Features.Get_Entry(iElement);

			for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
			{
				m_Centroid[iCluster][iFeature] += Feature[iFeature];
			}
		}

		//-------------------------------------------------
		for(iCluster=0; iCluster<nClusters; iCluster++)
		{
			double d = m_nMembers[iCluster] > 0 ? 1. / m_nMembers[iCluster] : 0.;

			for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
			{
				m_Centroid[iCluster][iFeature] *= d;
			}
		}

		//-------------------------------------------------
		int nShifts = 0; m_SP = 0.;

		for(iElement=0; iElement<Get_nElements(); iElement++)
		{
			double *Feature = (double *)m_Features.Get_Entry(iElement);

			double minVariance = -1.;
			int    minCluster  = -1;

			for(iCluster=0; iCluster<nClusters; iCluster++)
			{
				double Variance = 0.;

				for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
				{
					Variance += SG_Get_Square(m_Centroid[iCluster][iFeature] - Feature[iFeature]);
				}

				if( minVariance < 0. || Variance < minVariance )
				{
					minVariance = Variance;
					minCluster  = iCluster;
				}
			}

			if( m_Clusters[iElement] != minCluster )
			{
				m_Clusters[iElement] = minCluster;

				nShifts++;
			}

			m_SP                   += minVariance;
			m_Variance[minCluster] += minVariance;
		}

		//-------------------------------------------------
		m_SP /= Get_nElements();

		SG_UI_Process_Set_Text(CSG_String::Format("%s: %d >> %s %f",
			_TL("pass"  ), m_Iteration,
			_TL("change"), m_Iteration < 2 ? m_SP : SP_Last - m_SP
		));

		SP_Last = m_SP;

		if( nShifts == 0 || (nMaxIterations > 0 && nMaxIterations <= m_Iteration) )
		{
			return( true );
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::_Hill_Climbing(bool bInitialize, int nMaxIterations)
{
	int iElement, iCluster, nClusters = m_Variance.Get_N();

	//-----------------------------------------------------
	m_Variance = 0.;
	m_Centroid = 0.;
	m_nMembers = 0;

	for(iElement=0; iElement<Get_nElements(); iElement++)
	{
		m_nMembers[iCluster = m_Clusters[iElement]]++;

		double *Feature = (double *)m_Features.Get_Entry(iElement);

		for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
		{
			double d = Feature[iFeature];

			m_Centroid[iCluster][iFeature] += d;
			m_Variance[iCluster]           += d*d;
		}
	}

	//-----------------------------------------------------
	for(iCluster=0; iCluster<nClusters; iCluster++)
	{
		double v = 0., d = m_nMembers[iCluster] <= 0 ? 0. : 1. / (double)m_nMembers[iCluster];

		for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
		{
			m_Centroid[iCluster][iFeature] *= d;
			v += SG_Get_Square(m_Centroid[iCluster][iFeature]);
		}

		m_Variance[iCluster] -= v * m_nMembers[iCluster];
	}

	//-----------------------------------------------------
	double SP_Last = -1.; int noShift = 0;

	for(m_Iteration=1; SG_UI_Process_Get_Okay(false); m_Iteration++)
	{
		for(iElement=0; iElement<Get_nElements(); iElement++)
		{
			iCluster = m_Clusters[iElement];

			if( noShift++ < Get_nElements() && m_nMembers[iCluster] > 1 )
			{
				double *Feature = (double *)m_Features.Get_Entry(iElement);

				double Variance = 0.;

				for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
				{
					Variance += SG_Get_Square(m_Centroid[iCluster][iFeature] - Feature[iFeature]);
				}

				double V1 = Variance * m_nMembers[iCluster] / (m_nMembers[iCluster] - 1.);

				//-----------------------------------------
				int kCluster = 0; double VMin = -1.;

				for(int jCluster=0; jCluster<nClusters; jCluster++)
				{
					if( jCluster != iCluster )
					{
						Variance = 0.; 

						for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
						{
							Variance += SG_Get_Square(m_Centroid[jCluster][iFeature] - Feature[iFeature]);
						}

						double V2 = Variance * m_nMembers[jCluster] / (m_nMembers[jCluster] + 1.);

						if( VMin < 0. || V2 < VMin )
						{
							VMin     = V2;
							kCluster = jCluster;
						}
					}
				}

				//-----------------------------------------
				if( VMin >= 0 && VMin < V1 )
				{
					noShift               = 0;
					m_Variance[iCluster] -= V1;
					m_Variance[kCluster] += VMin;
					       V1             = 1. / (m_nMembers[iCluster] - 1.);
					double V2             = 1. / (m_nMembers[kCluster] + 1.);

					for(int iFeature=0; iFeature<m_nFeatures; iFeature++)
					{
						double d = Feature[iFeature];

						m_Centroid[iCluster][iFeature] = (m_nMembers[iCluster] * m_Centroid[iCluster][iFeature] - d) * V1;
						m_Centroid[kCluster][iFeature] = (m_nMembers[kCluster] * m_Centroid[kCluster][iFeature] + d) * V2;
					}

					m_Clusters[iElement] = kCluster;

					m_nMembers[iCluster]--;
					m_nMembers[kCluster]++;
				}
			}
		}

		//-------------------------------------------------
		for(iCluster=0, m_SP=0.; iCluster<nClusters; iCluster++)
		{
			m_SP += m_Variance[iCluster];
		}

		m_SP /= Get_nElements();

		SG_UI_Process_Set_Text(CSG_String::Format("%s: %d >> %s %f",
			_TL("pass"  ), m_Iteration,
			_TL("change"), m_Iteration <= 1 ? m_SP : SP_Last - m_SP
		));

		SP_Last = m_SP;

		if( noShift >= Get_nElements() || (nMaxIterations > 0 && nMaxIterations <= m_Iteration) )
		{
			return( true );
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Classifier_Supervised::CSG_Classifier_Supervised(void)
{
	m_nFeatures = 0; m_pClasses = NULL; m_nClasses = 0;

	m_Threshold_Distance    = 0.;
	m_Threshold_Angle       = 0.;
	m_Threshold_Probability = 0.;
	m_Probability_Relative  = false;

	for(int i=0; i<SG_CLASSIFY_SUPERVISED_WTA; i++)
	{
		m_bWTA[i] =  i == SG_CLASSIFY_SUPERVISED_MinimumDistance
		//        || i == SG_CLASSIFY_SUPERVISED_Mahalonobis
		          || i == SG_CLASSIFY_SUPERVISED_MaximumLikelihood
		          || i == SG_CLASSIFY_SUPERVISED_SAM;
	}
}

//---------------------------------------------------------
CSG_Classifier_Supervised::~CSG_Classifier_Supervised(void)
{
	Destroy();
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::Create(int nFeatures)
{
	Destroy();

	if( nFeatures > 0 )
	{
		m_nFeatures = nFeatures;
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::Destroy(void)
{
	if( m_nClasses > 0 )
	{
		for(int i=0; i<m_nClasses; i++)
		{
			delete(m_pClasses[i]);
		}

		SG_FREE_SAFE(m_pClasses);
	}

	m_nFeatures = 0;

	m_Info.Clear();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void   CSG_Classifier_Supervised::Set_Threshold_Distance   (double Value)	{	m_Threshold_Distance    = Value;	}
double CSG_Classifier_Supervised::Get_Threshold_Distance   (void)			{	return( m_Threshold_Distance );		}

//---------------------------------------------------------
void   CSG_Classifier_Supervised::Set_Threshold_Angle      (double Value)	{	m_Threshold_Angle       = Value;	}
double CSG_Classifier_Supervised::Get_Threshold_Angle      (void)			{	return( m_Threshold_Angle );		}

//---------------------------------------------------------
void   CSG_Classifier_Supervised::Set_Threshold_Probability(double Value)	{	m_Threshold_Probability	= Value;	}
double CSG_Classifier_Supervised::Get_Threshold_Probability(void)			{	return( m_Threshold_Probability );	}

//---------------------------------------------------------
void   CSG_Classifier_Supervised::Set_Probability_Relative (bool   Value)	{	m_Probability_Relative  = Value;	}
bool   CSG_Classifier_Supervised::Get_Probability_Relative (void)			{	return( m_Probability_Relative );	}

//---------------------------------------------------------
void CSG_Classifier_Supervised::Set_WTA(int Method, bool bOn)
{
	if( Method >= 0 && Method < SG_CLASSIFY_SUPERVISED_WTA )
	{
		m_bWTA[Method] = bOn;
	}
}

bool CSG_Classifier_Supervised::Get_WTA(int Method)
{
	return( Method >= 0 && Method < SG_CLASSIFY_SUPERVISED_WTA ? m_bWTA[Method] : false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "saga_api.h"

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Load(const CSG_String &File)
{
	int nFeatures = m_nFeatures; Destroy(); m_nFeatures = nFeatures;

	//-----------------------------------------------------
	CSG_MetaData Data;

	if( !Data.Load(File) || !Data.Cmp_Name("supervised_classifier") || SG_Compare_Version(Data.Get_Property("saga-version"), "2.1.4") < 0 )
	{
		return( false );
	}

	if( !Data("classes") || !Data("features") || !Data["features"]("count") || Data["features"]["count"].Get_Content().asInt() != m_nFeatures || m_nFeatures == 0 )
	{
		return( false );
	}

	if( Data["features"]("info") )
	{
		m_Info = Data["features"]["info"].Get_Content();
	}

	//-----------------------------------------------------
	CSG_MetaData &Classes = *Data.Get_Child("CLASSES");

	for(int i=0; i<Classes.Get_Children_Count(); i++)
	{
		if( Classes[i].Cmp_Name("class") && Classes[i].Get_Child("id") )
		{
			bool bAdd = true;

			CClass *pClass = new CClass(Classes[i]["id"].Get_Content());

			if( !pClass->m_Cov .from_String(Classes[i]["cov" ].Get_Content()) || pClass->m_Cov .Get_NX() != m_nFeatures || !pClass->m_Cov.is_Square() ) { bAdd = false; }
			if( !pClass->m_Mean.from_String(Classes[i]["mean"].Get_Content()) || pClass->m_Mean.Get_N () != m_nFeatures ) { bAdd = false; }
			if( !pClass->m_Min .from_String(Classes[i]["min" ].Get_Content()) || pClass->m_Min .Get_N () != m_nFeatures ) { bAdd = false; }
			if( !pClass->m_Max .from_String(Classes[i]["max" ].Get_Content()) || pClass->m_Max .Get_N () != m_nFeatures ) { bAdd = false; }

			//---------------------------------------------
			if( !bAdd )
			{
				delete(pClass);
			}
			else
			{
				m_pClasses = (CClass **)SG_Realloc(m_pClasses, ((size_t)m_nClasses + 1) * sizeof(CClass *));
				m_pClasses[m_nClasses++] = pClass;

				pClass->m_Cov_Det = pClass->m_Cov.Get_Determinant();
				pClass->m_Cov_Inv = pClass->m_Cov.Get_Inverse();

				pClass->m_Mean_Spectral = CSG_Simple_Statistics(pClass->m_Mean).Get_Mean();
			}
		}
	}

	return( m_nClasses > 0 );
}

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Save(const CSG_String &File, const SG_Char *Feature_Info)
{
	if( m_nFeatures < 1 || m_nClasses < 1 || File.is_Empty() )
	{
		return( false );
	}

	CSG_MetaData Data;

	Data.Set_Name    ("supervised_classifier");
	Data.Add_Property("saga-version", SAGA_VERSION);

	CSG_MetaData &Features = *Data.Add_Child("features");
	
	Features.Add_Child("count", m_nFeatures);

	if( Feature_Info && *Feature_Info )
	{
		Features.Add_Child("info", Feature_Info);
	}

	CSG_MetaData &Classes = *Data.Add_Child("classes");
	
	Classes.Add_Property("count", m_nClasses);

	for(int i=0; i<m_nClasses; i++)
	{
		CSG_MetaData &Class = *Classes.Add_Child("class");

		CClass *pClass = m_pClasses[i];

		Class.Add_Child("id"  , pClass->m_ID              );
		Class.Add_Child("mean", pClass->m_Mean.to_String());
		Class.Add_Child("min" , pClass->m_Min .to_String());
		Class.Add_Child("max" , pClass->m_Max .to_String());
		Class.Add_Child("cov" , pClass->m_Cov .to_String());
	}

	return( Data.Save(File) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_Classifier_Supervised::Print(void)
{
	CSG_String s;

	if( m_nFeatures > 0 && m_nClasses > 0 )
	{
		s += "\n";

		for(int iClass=0; iClass<m_nClasses; iClass++)
		{
			CClass *pClass = m_pClasses[iClass];

			s += "\n____\n" + pClass->m_ID + "\nFeature\tMean\tMin\tMax\tStdDev";

			for(int i=0; i<m_nFeatures; i++)
			{
				s += CSG_String::Format("\n%3d.", i + 1);
				s += "\t" + SG_Get_String(pClass->m_Mean[i]);
				s += "\t" + SG_Get_String(pClass->m_Min [i]);
				s += "\t" + SG_Get_String(pClass->m_Max [i]);
				s += "\t" + SG_Get_String(sqrt(pClass->m_Cov[i][i]));
			}

			s += "\n";
		}
	}

	return( s );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Add_Class(const CSG_String &Class_ID, const CSG_Vector &Mean, const CSG_Vector &Min, const CSG_Vector &Max, const CSG_Matrix &Cov)
{
	if( m_nFeatures < 1 || Mean.Get_N() != m_nFeatures || Min.Get_N() != m_nFeatures || Max.Get_N() != m_nFeatures || Cov.Get_NCols() != m_nFeatures || Cov.Get_NRows() != m_nFeatures )
	{
		return( false );
	}

	CClass *pClass, **pClasses = (CClass **)SG_Realloc(m_pClasses, ((size_t)m_nClasses + 1) * sizeof(CClass *));

	if( pClasses )
	{
		m_pClasses = pClasses;

		m_pClasses[m_nClasses++] = pClass = new CClass(Class_ID);

		pClass->m_ID      = Class_ID;
		pClass->m_Mean    = Mean;
		pClass->m_Min     = Min;
		pClass->m_Max     = Max;
		pClass->m_Cov     = Cov;
		pClass->m_Cov_Inv = Cov.Get_Inverse();
		pClass->m_Cov_Det = Cov.Get_Determinant();

		pClass->m_Mean_Spectral = CSG_Simple_Statistics(Mean).Get_Mean();

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Train_Clr_Samples(void)
{
	for(int i=0; i<m_nClasses; i++)
	{
		m_pClasses[i]->m_Samples.Destroy();
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Train_Add_Sample(const CSG_String &Class_ID, const CSG_Vector &Features)
{
	if( m_nFeatures > 0 && m_nFeatures == Features.Get_N() )
	{
		int iClass = Get_Class(Class_ID);

		if( iClass < 0 )
		{
			CClass **pClasses = (CClass **)SG_Realloc(m_pClasses, ((size_t)m_nClasses + 1) * sizeof(CClass *));

			if( pClasses )
			{
				m_pClasses = pClasses;

				m_pClasses[iClass = m_nClasses++] = new CClass(Class_ID);
			}
		}

		if( iClass >= 0 )
		{
			return( m_pClasses[iClass]->m_Samples.Add_Row(Features) );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Train(bool bClear_Samples)
{
	if( m_nFeatures < 1 || m_nClasses < 1 )
	{
		return( false );
	}

	for(int iClass=0; iClass<m_nClasses; iClass++)
	{
		if( !m_pClasses[iClass]->Train() )
		{
			return( false );
		}
	}

	if( bClear_Samples )
	{
		Train_Clr_Samples();
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Classifier_Supervised::CClass::Train(void)
{
	if( m_Samples.Get_NCols() < 1 || m_Samples.Get_NRows() < 1 )
	{
		return( false );
	}

	//-----------------------------------------------------
	m_Mean.Create(m_Samples.Get_NCols());
	m_Min .Create(m_Samples.Get_NCols());
	m_Max .Create(m_Samples.Get_NCols());

	for(int iFeature=0; iFeature<m_Samples.Get_NCols(); iFeature++)
	{
		CSG_Simple_Statistics s;

		for(int iSample=0; iSample<m_Samples.Get_NRows(); iSample++)
		{
			s += m_Samples[iSample][iFeature];
		}

		m_Mean[iFeature] = s.Get_Mean   ();
		m_Min [iFeature] = s.Get_Minimum();
		m_Max [iFeature] = s.Get_Maximum();
	}

	//-----------------------------------------------------
	m_Cov.Create(m_Samples.Get_NCols(), m_Samples.Get_NCols());

	for(int iFeature=0; iFeature<m_Samples.Get_NCols(); iFeature++)
	{
		for(int jFeature=iFeature; jFeature<m_Samples.Get_NCols(); jFeature++)
		{
			double cov = 0.;

			for(int iSample=0; iSample<m_Samples.Get_NRows(); iSample++)
			{
				cov += (m_Samples[iSample][iFeature] - m_Mean[iFeature]) * (m_Samples[iSample][jFeature] - m_Mean[jFeature]); 
			}

			if( m_Samples.Get_NRows() > 1 )
			{
				cov /= m_Samples.Get_NRows() - 1;
			}

			m_Cov[iFeature][jFeature] = m_Cov[jFeature][iFeature]	= cov;
		}
	}

	m_Cov_Inv = m_Cov.Get_Inverse    ();
	m_Cov_Det = m_Cov.Get_Determinant();

	m_Mean_Spectral = CSG_Simple_Statistics(m_Mean).Get_Mean();

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_Classifier_Supervised::Get_Class(const CSG_String &Class_ID)
{
	if( m_nFeatures > 0 )
	{
		for(int iClass=0; iClass<Get_Class_Count(); iClass++)
		{
			if( !Get_Class_ID(iClass).Cmp(Class_ID) )
			{
				return( iClass );
			}
		}
	}

	return( -1 );
}

//---------------------------------------------------------
bool CSG_Classifier_Supervised::Get_Class(const CSG_Vector &Features, int &Class, double &Quality, int Method)
{
	Class = -1; Quality = 0.;

	if( Get_Feature_Count() == Features.Get_N() )
	{
		switch( Method )
		{
		case SG_CLASSIFY_SUPERVISED_BinaryEncoding   :	_Get_Binary_Encoding       (Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_ParallelEpiped   :	_Get_Parallel_Epiped       (Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_MinimumDistance  :	_Get_Minimum_Distance      (Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_Mahalonobis      :	_Get_Mahalanobis_Distance  (Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_MaximumLikelihood:	_Get_Maximum_Likelihood    (Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_SAM              :	_Get_Spectral_Angle_Mapping(Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_SID              :	_Get_Spectral_Divergence   (Features, Class, Quality);	break;
		case SG_CLASSIFY_SUPERVISED_WTA              :	_Get_Winner_Takes_All      (Features, Class, Quality);	break;
		}

		return( Class >= 0 );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_Classifier_Supervised::Get_Name_of_Method(int Method)
{
	switch( Method )
	{
	case SG_CLASSIFY_SUPERVISED_BinaryEncoding   :	return( _TL("Binary Encoding") );
	case SG_CLASSIFY_SUPERVISED_ParallelEpiped   :	return( _TL("Parallelepiped") );
	case SG_CLASSIFY_SUPERVISED_MinimumDistance  :	return( _TL("Minimum Distance") );
	case SG_CLASSIFY_SUPERVISED_Mahalonobis      :	return( _TL("Mahalanobis Distance") );
	case SG_CLASSIFY_SUPERVISED_MaximumLikelihood:	return( _TL("Maximum Likelihood") );
	case SG_CLASSIFY_SUPERVISED_SAM              :	return( _TL("Spectral Angle Mapping") );
	case SG_CLASSIFY_SUPERVISED_SID              :	return( _TL("Spectral Information Divergence") );
	case SG_CLASSIFY_SUPERVISED_SVM              :	return( _TL("Support Vector Machine") );
	case SG_CLASSIFY_SUPERVISED_WTA              :	return( _TL("Winner Takes All") );
	}

	return( SG_T("") );
}

//---------------------------------------------------------
CSG_String CSG_Classifier_Supervised::Get_Name_of_Quality(int Method)
{
	switch( Method )
	{
	case SG_CLASSIFY_SUPERVISED_BinaryEncoding   :	return( _TL("Difference") );
	case SG_CLASSIFY_SUPERVISED_ParallelEpiped   :	return( _TL("Memberships") );
	case SG_CLASSIFY_SUPERVISED_MinimumDistance  :	return( _TL("Distance") );
	case SG_CLASSIFY_SUPERVISED_Mahalonobis      :	return( _TL("Distance") );
	case SG_CLASSIFY_SUPERVISED_MaximumLikelihood:	return( _TL("Proximity") );
	case SG_CLASSIFY_SUPERVISED_SAM              :	return( _TL("Angle") );
	case SG_CLASSIFY_SUPERVISED_SID              :	return( _TL("Divergence") );
	case SG_CLASSIFY_SUPERVISED_SVM              :	return( _TL("") );
	case SG_CLASSIFY_SUPERVISED_WTA              :	return( _TL("Votes") );
	}

	return( SG_T("") );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// Mazer, A. S., Martin, M., Lee, M., and Solomon, J. E. (1988):
// Image Processing Software for Imaging Spectrometry Analysis.
// Remote Sensing of Environment, v. 24, no. 1, p. 201-210.
//
void CSG_Classifier_Supervised::_Get_Binary_Encoding(const CSG_Vector &Features, int &Class, double &Quality)
{
	for(int iClass=0; iClass<Get_Class_Count(); iClass++)
	{
		CClass *pClass = m_pClasses[iClass];

		double Mean_Spectral = CSG_Simple_Statistics(Features).Get_Mean();

		int d = 0;

		for(int iFeature=0; iFeature<Get_Feature_Count(); iFeature++)
		{
			d += (Features(iFeature) < Mean_Spectral) == (pClass->m_Mean[iFeature] < pClass->m_Mean_Spectral) ? 0 : 1;

			if( iFeature == 0 )	// spectral slopes
			{
				d += (Features[iFeature    ] < Features[iFeature + 1]) == (pClass->m_Mean[iFeature    ] < pClass->m_Mean[iFeature + 1]) ? 0 : 1;
			}
			else if( iFeature == Get_Feature_Count() - 1 )
			{
				d += (Features[iFeature - 1] < Features[iFeature    ]) == (pClass->m_Mean[iFeature - 1] < pClass->m_Mean[iFeature    ]) ? 0 : 1;
			}
			else
			{
				d += (Features[iFeature - 1] < Features[iFeature + 1]) == (pClass->m_Mean[iFeature - 1] < pClass->m_Mean[iFeature + 1]) ? 0 : 1;
			}
		}

		if( Class < 0 || Quality > d )	// find the minimum 'Hamming' distance
		{
			Class = iClass; Quality = d;
		}
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Parallel_Epiped(const CSG_Vector &Features, int &Class, double &Quality)
{
	for(int iClass=0; iClass<Get_Class_Count(); iClass++)
	{
		CClass *pClass = m_pClasses[iClass];

		bool bMember = true;

		for(int iFeature=0; bMember && iFeature<Get_Feature_Count(); iFeature++)
		{
			bMember = pClass->m_Min[iFeature] <= Features[iFeature] && Features[iFeature] <= pClass->m_Max[iFeature];
		}

		if( bMember )
		{
			Class = iClass; Quality++;
		}
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Minimum_Distance(const CSG_Vector &Features, int &Class, double &Quality)
{
	for(int iClass=0; iClass<Get_Class_Count(); iClass++)
	{
		CClass *pClass = m_pClasses[iClass];

		double Distance = (Features - pClass->m_Mean).Get_Length();

		if( Class < 0 || Quality > Distance )
		{
			Class = iClass; Quality = Distance;
		}
	}

	if( m_Threshold_Distance > 0. && Quality > m_Threshold_Distance )
	{
		Class = -1;
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Mahalanobis_Distance(const CSG_Vector &Features, int &Class, double &Quality)
{
	for(int iClass=0; iClass<Get_Class_Count(); iClass++)
	{
		CClass *pClass = m_pClasses[iClass];

		CSG_Vector D = Features - pClass->m_Mean;

		double Distance = D * (pClass->m_Cov_Inv * D);

		if( Class < 0 || Quality > Distance )
		{
			Class = iClass; Quality = Distance;
		}
	}

	if( m_Threshold_Distance > 0. && Quality > m_Threshold_Distance )
	{
		Class = -1;
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Maximum_Likelihood(const CSG_Vector &Features, int &Class, double &Quality)
{
	double dSum = 0.;

	for(int iClass=0; iClass<Get_Class_Count(); iClass++)
	{
		CClass *pClass = m_pClasses[iClass];

		CSG_Vector D = Features - pClass->m_Mean;

		double Distance = D * (pClass->m_Cov_Inv * D);

		double Probability = pow(2. * M_PI, -0.5 * m_nFeatures) * pow(pClass->m_Cov_Det, -0.5) * exp(-0.5 * Distance);
	//	double Probability = -log(pClass->m_Cov_Det) - Distance;

		dSum += Probability;

		if( Class < 0 || Quality < Probability )
		{
			Class = iClass; Quality = Probability;
		}
	}

	if( Class >= 0 )
	{
		if( m_Probability_Relative )
		{
			Quality = 100. * Quality / dSum;
		}

		if( m_Threshold_Probability > 0. && Quality < m_Threshold_Probability )
		{
			Class = -1;
		}
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Spectral_Angle_Mapping(const CSG_Vector &Features, int &Class, double &Quality)
{
	for(int iClass=0; iClass<Get_Class_Count(); iClass++)
	{
		CClass *pClass = m_pClasses[iClass];

		double Angle = Features.Get_Angle(pClass->m_Mean);

		if( Class < 0 || Quality > Angle )
		{
			Class = iClass; Quality = Angle;
		}
	}

	Quality *= M_RAD_TO_DEG;

	if( m_Threshold_Angle > 0. && Quality > m_Threshold_Angle )
	{
		Class = -1;
	}
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Spectral_Divergence(const CSG_Vector &Features, int &Class, double &Quality)
{
}

//---------------------------------------------------------
void CSG_Classifier_Supervised::_Get_Winner_Takes_All(const CSG_Vector &Features, int &Class, double &Quality)
{
	int *Votes = (int *)SG_Calloc(Get_Class_Count(), sizeof(int));

	for(int iMethod=0; iMethod<SG_CLASSIFY_SUPERVISED_WTA; iMethod++)
	{
		int iClass; double iQuality;

		if( m_bWTA[iMethod] && Get_Class(Features, iClass, iQuality, iMethod) && ++Votes[iClass] > Quality )
		{
			Class = iClass; Quality = Votes[iClass];
		}
	}

	SG_Free(Votes);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// source: http://psydok.sulb.uni-saarland.de/volltexte/2004/268/html/

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Tail(double T, int df, TSG_Test_Distribution_Type Type)
{	// Hill's approx. to cumulative t-dist, Commun.A.C.M. 13,617-619.
	// See: J.H.Maindonald, Computational Statistics, p.295.
	// Calculates p given t and tail type.

	if( !T || !df || df < 1. )
	{
		return( -1. );
	}

	return( _Change_Tail_Type(Get_T_P(T, df), TESTDIST_TYPE_TwoTail, Type, T < 0.) );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Inverse(double p, int df, TSG_Test_Distribution_Type Type)
{	// Keith Dear & Robert Brennan.
	// Returns an accurate t to tol sig. fig.'s given p & df.

	if( p <= 0. || p >= 1. || df < 1 )
	{
		return( -1. );
	}

	bool bNegative = (Type == TESTDIST_TYPE_Left && p < 0.5) || (Type == TESTDIST_TYPE_Right && p > 0.5);
	double t, p0, p1, diff = 1.;

	p0 = p1 = _Change_Tail_Type(p, Type, TESTDIST_TYPE_TwoTail, bNegative);

	while( fabs(diff) > 0.0001 )
	{
		t    = Get_T_Inv(p1, df);   // initial rough value
		diff = Get_T_P(t, df) - p0; // compare result with forward fn
		p1   = p1 - diff;           // small adjustment to p1
	}

	return( bNegative ? -t : t );
}

//---------------------------------------------------------
double CSG_Test_Distribution::_Change_Tail_Type(double p, TSG_Test_Distribution_Type from, TSG_Test_Distribution_Type to, bool bNegative)
{
	if( from != to )
	{
		switch( from )	// convert any tail type to 'left'
		{
		case TESTDIST_TYPE_Left   :                                               break;
		case TESTDIST_TYPE_Right  :                                   p = 1. - p; break;
		case TESTDIST_TYPE_Middle : p = p / 2. + 0.5; if( bNegative ) p = 1. - p; break;
		case TESTDIST_TYPE_TwoTail: p = 1. - p / 2. ; if( bNegative ) p = 1. - p; break;
	//	case TESTDIST_TYPE_Half   : p = p + 0.5     ; if( bNegative ) p = 1. - p; break;
		}

		switch( to )	// convert p from tail type 'left' to any other
		{
		case TESTDIST_TYPE_Left   :                                                break;
		case TESTDIST_TYPE_Right  :                 p = 1. - p;                    break;
		case TESTDIST_TYPE_Middle : if( bNegative ) p = 1. - p; p = 2. * (1. - p); break;
		case TESTDIST_TYPE_TwoTail: if( bNegative ) p = 1. - p; p = 2. * p - 1.  ; break;
	//	case TESTDIST_TYPE_Half   : if( bNegative ) p = 1. - p; p = p - 0.5      ; break;
		}
	}

	return( p );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Norm_P(double z)
{	// Returns the two-tailed standard normal probability of z
	const double a1 = 0.0000053830, a2 = 0.0000488906, a3 = 0.0000380036;
	const double a4 = 0.0032776263, a5 = 0.0211410061, a6 = 0.0498673470;

	z = fabs(z);

	double p = (((((a1 * z + a2) * z + a3) * z + a4) * z + a5) * z + a6) * z + 1.;

	return( pow(p, -16) );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Norm_Z(double p)
{	// Returns z given a half-middle tail type p.
	const double a0 =  2.5066282, a1 = -18.6150006, a2 =  41.3911977, a3 = -25.4410605;
	const double b1 = -8.4735109, b2 =  23.0833674, b3 = -21.0622410, b4 =   3.1308291;
	const double c0 = -2.7871893, c1 =  -2.2979648, c2 =   4.8501413, c3 =   2.3212128;
	const double d1 =  3.5438892, d2 =   1.6370678;

	if( p > 0.42 )
	{
		double r = sqrt(-log(0.5 - p));

		return( (((c3 * r + c2) * r + c1) * r + c0) / ((d2 * r + d1) * r + 1.) );
	}
	else
	{
		double r = p * p;

		return( p * (((a3 * r + a2) * r + a1) * r + a0) / ((((b4 * r + b3) * r + b2) * r + b1) * r + 1.) );
	}
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_P(double T, int df)
{	// Returns two-tail probability level given t and df.
	return(	df == 1 ? 1. - 2. * atan(fabs(T)) / M_PI
		:	df == 2 ? 1. - fabs(T) / sqrt(T*T + 2.)
		:	df == 3 ? 1. - 2. * (atan(fabs(T) / sqrt(3.)) + fabs(T) * sqrt(3.) / (T*T + 3.)) / M_PI
		:	df == 4 ? 1. - fabs(T) * (1. + 2. / (T*T + 4.)) / sqrt(T*T + 4.)
		:	Get_Norm_P(Get_T_Z(fabs(T), df))
	);
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Z(double T, int df)
{	// Converts a t value to an approximate z value w.r.t the given df
	// s.t. std.norm.(z) = t(z, df) at the two-tail probability level.

	double	A9, B9, T9, Z8, P7, B7, z;

	A9	= df - 0.5;
	B9	= 48. * A9*A9,
    T9	= T*T / df;
	Z8	= T9 >= 0.04
		? A9 * log(1. + T9)
		: A9 * (((1. - T9 * 0.75) * T9 / 3. - 0.5) * T9 + 1.) * T9;
	P7	= ((0.4 * Z8 + 3.3) * Z8 + 24.) * Z8 + 85.5;
	B7	= 0.8 * pow(Z8, 2.) + 100. + B9;
	z	= (1. + (-P7 / B7 + Z8 + 3.) / B9) * sqrt(Z8);

	return( z );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Inv(double p, int df)
{	// Hill's approx. inverse t-dist.: Comm. of A.C.M Vol.13 No.10 1970 pg 620.
	// Calculates t given df and two-tail probability.

	if( df == 1 )
	{
		return( cos(p * M_PI / 2.) / sin(p * M_PI / 2.) );
	}

	if( df == 2 )
	{
		return( sqrt(2. / (p * (2. - p)) - 2.) );
	}

	double a = 1. / (df - 0.5);
	double b = 48. / (a*a);
	double c = ((20700. * a / b - 98.) * a - 16.) * a + 96.36;
	double d = ((94.5 / (b + c) - 3.) / b + 1.) * sqrt(a * M_PI / 2.) * df;
	double x = d * p;
	double y = pow(x, 2. / df);

	if( y > 0.05 + a )
	{
		x = Get_Norm_Z(0.5 * (1. - p));
		y = x*x;

		if( df < 5 )
		{
			c = c + 0.3 * (df - 4.5) * (x + 0.6);
		}

		c = (((0.05 * d * x - 5) * x - 7.) * x - 2.) * x + b + c;
		y = (((((0.4 * y + 6.3) * y + 36.) * y + 94.5) / c - y - 3.) / b + 1.) * x;
		y = a * y*y;

		if( y > 0.002 )
		{
			y = exp(y) - 1.;
		}
		else
		{
			y = 0.5 * y*y + y;
		}
	}
	else
	{
		y = ((1. / (((df + 6.) / (df * y) - 0.089 * d - 0.822) * (df + 2.) * 3.)
		    + 0.5 / (df + 4.)) * y - 1.) * (df + 1.) / (df + 2.) + 1. / y;
	}

	return( sqrt(df * y) );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CSG_Test_Distribution::Get_F_Tail_from_R2(double R2, int nPredictors, int nSamples, TSG_Test_Distribution_Type Type)
{
	double F = ((sLong)nSamples - (sLong)nPredictors - 1) * (R2 / nPredictors) / (1. - R2);

	return( CSG_Test_Distribution::Get_F_Tail(F, nPredictors, nSamples - nPredictors - 1, Type) );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_F_Tail(double F, int dfn, int dfd, TSG_Test_Distribution_Type Type)
{
	// calculates for F, dfn(ominator) and dfd(enominator) the "tail" of the F-distribution

	double	p	= 1.;

	if( F >= 0.00001 && dfn > 0 && dfd > 0 )
	{
		if( F * dfn >= dfd || F > 1. + 20. / dfn + 10. / sqrt((double)dfn) )
		{
			p	= Get_Gamma(F, dfn, dfd);
		}
		else
		{
			p	= 1. - Get_Gamma(1. / F, dfd, dfn);
		}
	}

	if( p <= 0. || p >= 1. )
	{
		p	= F > 1. ? 0. : F < 1. ? 1. : 0.5;
	}

	return( Type == TESTDIST_TYPE_Right ? p : 1. - p );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_F_Inverse(double alpha, int dfn, int dfd, TSG_Test_Distribution_Type Type)
{
	if( alpha < 0. || alpha > 1. || dfd < 0 || dfn < 0 )
	{
		return( -1 );
	}

	if( Type != TESTDIST_TYPE_Right )
	{
		alpha	= 1. - alpha;
	}

	const int		ITERMAX		= 100;
	const double	EPSILON		= 0.0001;

	int		i;
	double	lo, hi, mid, p;

	if( alpha <= 0.5 )
	{
		lo	= 0.5;
		hi	= lo;

		for(i=0; i<ITERMAX; i++)
		{
			hi	*= 2.;
			p	= Get_F_Tail(hi, dfn, dfd);

			if( p > alpha )
			{
				lo	= hi;
			}
			else
			{
				break;
			}
		}

		if( p > alpha )
		{
			return( hi );
		}
	}
	else
	{
		hi	= 2;
		lo	= hi;

		for(i=0; i<ITERMAX; i++)
		{
			lo	/= 2.;
			p	= Get_F_Tail(lo, dfn, dfd);

			if( p < alpha )
			{
				hi	= lo;
			}
			else
			{
				break;
			}
		}

		if( p < alpha )
		{
			return( lo );
		}
	}

	mid	= (hi + lo) / 2.;

	for(i=0; i<ITERMAX && (hi-lo)>EPSILON*mid; i++)
	{
		mid	= (hi + lo) / 2.;
		p	= Get_F_Tail(mid, dfn, dfd);

		if( p < alpha )
			hi	= mid;
		else if( p > alpha )
			lo	= mid;
		else
			break;
	}

	return( mid );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Gamma(double F, double dfn, double dfd)
{
	// calculates for F, dfn(ominator) and dfd(enominator) the incomplete Gamma-function

	const double	EXPMIN	= -30.;
	const double	SMALL	= 0.00000000001;

	double	x, c, er, s, n, t1, t;

	dfn		/= 2.;
	dfd		/= 2.;

	x	= dfd / (dfd + dfn * F);
	c	= Get_Log_Gamma(dfn + dfd) - Get_Log_Gamma(dfn) - Get_Log_Gamma(dfd + 1.) + dfd * log(x) + dfn * log(1. - x);

	if( c < EXPMIN )
	{
		return( -1. );
	}

	dfn	+= dfd;
	dfd	+= 1.;
	c	= exp(c);
	er	= SMALL / c;
	t	= dfn * x / dfd;
	t1	= 0.;
	s	= t + 1.;
	n	= 0;

	while( t > er || t > t1 )
	{
		n	+= 1;
		t1	 = t;
		t	*= ((dfn + n) * x / (dfd + n));
		s	+= t;
	}

	return( s * c );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Log_Gamma(double a)
{
	// calculates the logarithm of the Gamma-function

	const int		ARGMIN	= 6;

    const double	HL2PI	= 0.91893853320467275;	//  = log(2. * M_PI) / 2.

	int	n	= (int)floor(ARGMIN - a + 0.0001);

	if( n > 0 )
	{
		a	+= n;
	}

	double	g;

	g	= 1. / (a*a);
	g	= (1. - g * (1. / 30. - g * (1. / 105. - g * (1. / 140. - g / 99.)))) / (12. * a);
	g	= g + ((a - 0.5) * log(a) - a + HL2PI);

	for(int i=0; i<n; i++)
	{
		a	= a - 1.;
		g	= g - log(a);
	}

	return( g );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Matrix	SG_Get_Correlation_Matrix	(const CSG_Matrix &Values, bool bCovariances)
{
	int		nVariables	= Values.Get_NX();
	int		nSamples	= Values.Get_NY();

	//-----------------------------------------------------
	int						i, j, k;
	CSG_Simple_Statistics	*S;
	CSG_Matrix				C;

	C.Create(nVariables, nVariables);

	//-----------------------------------------------------
	S	= new CSG_Simple_Statistics[nVariables];

	for(j=0; j<nVariables; j++)
	{
		for(i=0; i<nSamples; i++)
		{
			S[j]	+= Values[i][j];
		}
	}

	//-----------------------------------------------------
	for(k=0; k<nVariables; k++)
	{
		for(j=k; j<nVariables; j++)
		{
			double	cov	= 0.;

			for(i=0; i<nSamples; i++)
			{
				cov	+= (Values[i][j] - S[j].Get_Mean()) * (Values[i][k] - S[k].Get_Mean());
			}

			cov	/= nSamples;

			if( !bCovariances )
			{
				cov	/= (S[j].Get_StdDev() * S[k].Get_StdDev());
			}

			C[j][k]	= C[k][j]	= cov;
		}
	}

	//-----------------------------------------------------
	delete[](S);

	return( C );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
