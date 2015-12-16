/************************************************************************************
*                                                                                   *
*   Copyright (c) 2013 Axel Menzel <info@axelmenzel.de>                             *
*                                                                                   *
*   This file is part of the Runtime Type Reflection System (RTTR).                 *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#include "TestClasses.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <rttr>

#define BOOST_TEST_MODULE TypeInfo-UnitTests
#include "BoostTest.h"

using namespace RTTR;

typedef std::vector<int> VectorList;
typedef std::map<int, std::string> IntToStringMap;
typedef std::map<int, int> IntToIntMap;

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(std::string)
RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS(std::string)

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(VectorList)
RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS(VectorList)

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(IntToStringMap)
RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS(IntToStringMap)

RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS(IntToIntMap)
RTTR_DEFINE_STANDARD_META_TYPE_VARIANTS(IntToIntMap)

RTTR_DECLARE_META_TYPE(int***)
RTTR_DEFINE_META_TYPE(int***)

BOOST_AUTO_TEST_SUITE(TypeIdTests)

BOOST_AUTO_TEST_CASE(TypeIdTests_BasicType)
{
    using namespace RTTR;
    // check normal atomic types
    {
        int intVar = 23;
        const TypeInfo intTypeInfo = TypeInfo::get(intVar);
        BOOST_CHECK_EQUAL(intTypeInfo.getName(), "int");
        BOOST_CHECK(intTypeInfo == TypeInfo::get<int>());

        bool boolVar = true;
        const TypeInfo boolTypeInfo = TypeInfo::get(boolVar);
        BOOST_CHECK_EQUAL(boolTypeInfo.getName(), "bool");
        BOOST_CHECK(boolTypeInfo == TypeInfo::get<bool>());

        BOOST_CHECK(boolTypeInfo != intTypeInfo);

        int ***intPtr = NULL;
        BOOST_CHECK(TypeInfo::get<int***>() == TypeInfo::get(intPtr));

    }

    // check pointer types
    {
        int intVar = 23;
        int* intPtrVar = &intVar;
        const TypeInfo intPtrTypeInfo = TypeInfo::get(intPtrVar);
        BOOST_CHECK_EQUAL(intPtrTypeInfo.getName(), "int*");
        BOOST_CHECK(intPtrTypeInfo == TypeInfo::get<int*>());

        bool boolVar = true;
        bool* boolPtrVar = &boolVar;
        const TypeInfo boolPtrTypeInfo = TypeInfo::get(boolPtrVar);
        BOOST_CHECK_EQUAL(boolPtrTypeInfo.getName(), "bool*");
        BOOST_CHECK(boolPtrTypeInfo == TypeInfo::get<bool*>());

        BOOST_CHECK(boolPtrTypeInfo != intPtrTypeInfo);
    }

    // check that cv was removed
    {
        int intVar = 42;
        const int constIntVar = 42;
        BOOST_CHECK(TypeInfo::get(intVar) == TypeInfo::get(constIntVar));
        BOOST_CHECK(TypeInfo::get<int>()  == TypeInfo::get(constIntVar));
        BOOST_CHECK(TypeInfo::get<int>()  == TypeInfo::get<const int>());
        BOOST_CHECK(TypeInfo::get<int>()  == TypeInfo::get<const int &>());

        BOOST_CHECK(TypeInfo::get<int*>() == TypeInfo::get(&intVar));
        BOOST_CHECK(TypeInfo::get<int*>() == TypeInfo::get<int *const>());
        BOOST_CHECK(TypeInfo::get<const int*>() == TypeInfo::get(&constIntVar));
        BOOST_CHECK(TypeInfo::get<const int*>() == TypeInfo::get<const int *const>());

        const int& intConstRef = intVar;
        BOOST_CHECK(TypeInfo::get<int>() == TypeInfo::get(intConstRef));
        int*** ptr = NULL;
        BOOST_CHECK(TypeInfo::get<int***>() == TypeInfo::get(ptr));
    }
}

BOOST_AUTO_TEST_CASE(TypeIdTests_ComplexerTypes)
{

    VectorList myList;
    std::vector<int> myList2;
    BOOST_CHECK(TypeInfo::get<std::vector<int> >() == TypeInfo::get<VectorList>());
    BOOST_CHECK(TypeInfo::get<std::vector<int> >() == TypeInfo::get(myList));
    BOOST_CHECK(TypeInfo::get<VectorList>()        == TypeInfo::get(myList));
    BOOST_CHECK(TypeInfo::get(myList)              == TypeInfo::get(myList2));

    IntToStringMap myMap;
    std::map<int, std::string> myMap2;

    BOOST_CHECK((TypeInfo::get<std::map<int, std::string> >() == TypeInfo::get<IntToStringMap>()));
    BOOST_CHECK((TypeInfo::get<std::map<int, std::string> >() == TypeInfo::get(myMap)));
    BOOST_CHECK((TypeInfo::get<IntToStringMap>()              == TypeInfo::get(myMap)));
    BOOST_CHECK((TypeInfo::get(myMap)                         == TypeInfo::get(myMap2)));

    BOOST_CHECK((TypeInfo::get(myMap) != TypeInfo::get<std::map<int, int> >()));
}

BOOST_AUTO_TEST_CASE(TypeIdTests_SingleClassInheritance)
{
    {
        ClassSingle6A instance6A;
        ClassSingleBase& baseSingle = instance6A;

        // down cast cast
        BOOST_CHECK(rttr_cast<ClassSingle6A*>(&baseSingle) != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle3A*>(&baseSingle) != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle6B*>(&baseSingle) == NULL);

        // up cast cast
        BOOST_CHECK(rttr_cast<ClassSingleBase*>(&instance6A) != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle3A*>(&instance6A) != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle1A*>(&instance6A) != NULL);
    }

    {
        ClassSingle6E instance6E;
        ClassSingleBase& baseSingle = instance6E;

        // down cast cast
        BOOST_CHECK(rttr_cast<ClassSingle6A*>(&baseSingle) == NULL);
        BOOST_CHECK(rttr_cast<ClassSingle3E*>(&baseSingle) != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle6E*>(&baseSingle) != NULL);

        // up cast cast
        BOOST_CHECK(rttr_cast<ClassSingleBase*>(&instance6E)!= NULL);
        BOOST_CHECK(rttr_cast<ClassSingle3E*>(&instance6E)  != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle1E*>(&instance6E)  != NULL);
        BOOST_CHECK(rttr_cast<ClassSingle6E*>(&instance6E)  != NULL);
    }
}

BOOST_AUTO_TEST_CASE(TypeIdTests_MultipleClassInheritance)
{
    {
        FinalClass final;
        ClassMultipleBaseA& baseMultiA = final;
        ClassMultipleBaseB& baseMultiB = final;
        ClassMultipleBaseC& baseMultiC = final;
        ClassMultipleBaseD& baseMultiD = final;
        ClassMultipleBaseE& baseMultiE = final;

        // down cast cast
        BOOST_CHECK(rttr_cast<FinalClass*>(&baseMultiA) != NULL);
        BOOST_CHECK(rttr_cast<FinalClass*>(&baseMultiB) != NULL);
        BOOST_CHECK(rttr_cast<FinalClass*>(&baseMultiC) != NULL);
        BOOST_CHECK(rttr_cast<FinalClass*>(&baseMultiD) != NULL);
        BOOST_CHECK(rttr_cast<FinalClass*>(&baseMultiE) != NULL);

        // up cast cast
        BOOST_CHECK(rttr_cast<ClassMultipleBaseA*>(&final) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultipleBaseB*>(&final) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultipleBaseC*>(&final) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultipleBaseD*>(&final) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultipleBaseE*>(&final) != NULL);

        // down cast cast to the middle
        BOOST_CHECK(rttr_cast<ClassMultiple3A*>(&baseMultiA) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple3B*>(&baseMultiB) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple3C*>(&baseMultiC) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple3D*>(&baseMultiD) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple3E*>(&baseMultiE) != NULL);
    }

    {
        FinalClass final;
        ClassMultiple6A classMulti6A;
        ClassMultiple3B classMulti3B;
        ClassMultiple1E classMulti1E;

        // check for invalid casts
        BOOST_CHECK(rttr_cast<FinalClass*>(&classMulti6A) == NULL);

        BOOST_CHECK(rttr_cast<ClassMultiple4B*>(&classMulti3B) == NULL);

        BOOST_CHECK(rttr_cast<ClassMultiple2E*>(&classMulti1E) == NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple6E*>(&classMulti1E) == NULL);

        // check for valid casts
        BOOST_CHECK(rttr_cast<ClassMultiple5A*>(&classMulti6A) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultipleBaseA*>(&classMulti6A) != NULL);

        BOOST_CHECK(rttr_cast<ClassMultiple2B*>(&classMulti3B) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple1B*>(&classMulti3B) != NULL);

        BOOST_CHECK(rttr_cast<ClassMultipleBaseE*>(&classMulti1E) != NULL);
        BOOST_CHECK(rttr_cast<ClassMultiple1E*>(&classMulti1E) != NULL);
    }
}

BOOST_AUTO_TEST_CASE(TypeIdTests_TypeIdAndClassInheritance)
{
    ClassSingle6A instance6A;
    ClassSingleBase& baseSingle = instance6A;

    BOOST_CHECK(TypeInfo::get<ClassSingle6A>() == TypeInfo::get(baseSingle));
    BOOST_CHECK(TypeInfo::get<ClassSingleBase*>() == TypeInfo::get(&baseSingle));

    ClassSingle3A instance3A;
    BOOST_CHECK(TypeInfo::get<ClassSingle3A>() == TypeInfo::get(instance3A));
    BOOST_CHECK(TypeInfo::get<ClassSingle6A>() != TypeInfo::get(instance3A));
}

BOOST_AUTO_TEST_SUITE_END()
