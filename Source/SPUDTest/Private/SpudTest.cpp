﻿#include "Misc/AutomationTest.h"
#include "Engine.h"
#include "SpudState.h"
#include "TestSaveObject.h"


template<typename T>
void PopulateAllTypes(T& Obj)
{
	Obj.IntVal = -123456789;
	Obj.UInt8Val = 245;
	Obj.UInt16Val = 61234;
	Obj.UInt32Val = 4123456789;
	Obj.UInt64Val = 18123456789123456789ull;
	Obj.Int8Val = -123;
	Obj.Int16Val = -32123;
	Obj.Int32Val = -2112345678;
	Obj.Int64Val = -9123456789123456789;
	Obj.FloatVal = 12.3456f;
	Obj.DoubleVal = 234.56789012;
	Obj.EnumVal = ETestEnum::Second;
	Obj.VectorVal = FVector(234.56f, -456.78f, 78.9f);
	Obj.RotatorVal = FRotator(10.5f, -20.1f, 30.8f);
	Obj.TransformVal = FTransform::Identity;
	Obj.TransformVal.SetComponents(Obj.RotatorVal.Quaternion(), Obj.VectorVal, FVector(1, 2, 1));
	Obj.NameVal = FName("SpudNameTest");
	Obj.StringVal = "A string test for SPUD";
	Obj.TextVal = FText::FromString("SpudTextTest");
	Obj.UObjectVal = NewObject<UTestNestedUObject>();
	Obj.UObjectVal->NestedIntVal = 96;
	Obj.UObjectVal->NestedStringVal = "A string inside a nested UObject";

	Obj.ActorSubclass = AStaticMeshActor::StaticClass();
	Obj.ActorSubclassArray.Add(AStaticMeshActor::StaticClass());
	Obj.ActorSubclassArray.Add(APointLight::StaticClass());

	Obj.IntArray.Add(136);
	Obj.IntArray.Add(-31913);
	Obj.UInt8Array.Add(2);
	Obj.UInt8Array.Add(4);
	Obj.UInt8Array.Add(6);
	Obj.UInt16Array.Add(34);
	Obj.UInt16Array.Add(23765);
	Obj.UInt16Array.Add(2355);
	Obj.UInt32Array.Add(93465862);
	Obj.UInt64Array.Add(38629649029043254);
	Obj.UInt64Array.Add(2939536);
	Obj.Int8Array.Add(-35);
	Obj.Int8Array.Add(-123);
	Obj.Int8Array.Add(90);
	Obj.Int8Array.Add(12);
	Obj.Int8Array.Add(-48);
	Obj.Int16Array.Add(9825);
	Obj.Int16Array.Add(-125);
	Obj.Int32Array.Add(93465529);
	Obj.Int32Array.Add(-289556);
	Obj.Int32Array.Add(1);
	Obj.Int64Array.Add(345629348923);
	Obj.Int64Array.Add(-2345256);
	Obj.Int64Array.Add(62486);
	Obj.Int64Array.Add(49562);
	Obj.Int64Array.Add(-7723765);
	Obj.Int64Array.Add(30035560992345);
	Obj.FloatArray.Add(-3456.236);
	Obj.FloatArray.Add(-1.99);
	Obj.FloatArray.Add(87.666);
	Obj.FloatArray.Add(39496.1);
	Obj.DoubleArray.Add(349587.233456);
	Obj.DoubleArray.Add(8833.2233);
	Obj.DoubleArray.Add(24983.8923956238);
	Obj.DoubleArray.Add(-1122768946.1223);
	Obj.DoubleArray.Add(0.00356);
	Obj.EnumArray.Add(ETestEnum::Third);
	Obj.EnumArray.Add(ETestEnum::First);
	Obj.EnumArray.Add(ETestEnum::Second);
	Obj.VectorArray.Add(FVector(123, 876, -234));
	Obj.VectorArray.Add(FVector(0, 0, 0));
	Obj.VectorArray.Add(FVector(3, 4, 5));
	Obj.RotatorArray.Add(FRotator(25, -45, 90));
	Obj.RotatorArray.Add(FRotator(-125, 180, 0));
	Obj.TransformArray.Add(FTransform(Obj.RotatorArray[0], Obj.VectorArray[0], Obj.VectorArray[2]));
	Obj.TransformArray.Add(FTransform(Obj.RotatorArray[1], Obj.VectorArray[2], Obj.VectorArray[0]));
	Obj.NameArray.Add(FName("NameOne"));
	Obj.NameArray.Add(FName("NameTwo"));
	Obj.NameArray.Add(FName("NameTwoAndAHalf"));
	Obj.NameArray.Add(FName("NameTwoAndThreeQuarters"));
	Obj.NameArray.Add(FName("NameThree"));
	Obj.StringArray.Add("Test data is super boring to write!");
	Obj.StringArray.Add("I know, right?");
	Obj.StringArray.Add("Still, we're almost there now chuck");
	Obj.StringArray.Add("Yeah, best crack on eh?");	
	Obj.TextArray.Add(FText::FromString("TextOne"));
	Obj.TextArray.Add(FText::FromString("TextTwo"));

	Obj.IntMap.Add(136, 137);
	Obj.IntMap.Add(-31913, -31914);
	Obj.FloatMap.Add(-3456.236, -3457.236);
	Obj.FloatMap.Add(-1.99, -2.99);
	Obj.IntFloatMap.Add(136, 3.99);
	Obj.IntFloatMap.Add(138, -4.99);
	Obj.StringVectorMap.Add("Test data is super boring to write!", FVector(1, 2, 3));
	Obj.StringVectorMap.Add("I know, right?", FVector(3, 4, 5));
}

template<typename T>
void CheckArray(FAutomationTestBase* Test, const FString& Prefix, const TArray<T>& Actual, const TArray<T>& Expected)
{
	Test->TestEqual(Prefix + "Array Length should match", Actual.Num(), Expected.Num());
	for (int i = 0; i < Actual.Num() && i < Expected.Num(); ++i)
	{
		Test->TestEqual(FString::Printf(TEXT("%sItem %d should match"), *Prefix, i), Actual[i], Expected[i]);
	}

}

template<typename K, typename V>
void CheckMap(FAutomationTestBase* Test, const FString& Prefix, const TMap<K, V>& Actual, const TMap<K, V>& Expected)
{
	Test->TestEqual(Prefix + "Map Length should match", Actual.Num(), Expected.Num());
	int i = 0;
	for (auto& Elem : Actual)
	{
		Test->TestEqual(FString::Printf(TEXT("%sItem %d should match"), *Prefix, i), Elem.Value, Expected.FindRef(Elem.Key));
		i++;
	}
}

template<typename T>
void CheckArrayExplicitEquals(FAutomationTestBase* Test, const FString& Prefix, const TArray<T>& Actual, const TArray<T>& Expected)
{
	Test->TestEqual(Prefix + "Array Length should match", Actual.Num(), Expected.Num());
	for (int i = 0; i < Actual.Num() && i < Expected.Num(); ++i)
	{
		Test->TestTrue(FString::Printf(TEXT("%sItem %d should match"), *Prefix, i), Actual[i].Equals(Expected[i]));
	}
}

template<typename T>
void CheckAllTypes(FAutomationTestBase* Test, const FString& Prefix, const T& Actual, const T& Expected)
{
	Test->TestEqual(Prefix + "IntVal should match", Actual.IntVal, Expected.IntVal);
	Test->TestEqual(Prefix + "UInt8Val should match", Actual.UInt8Val, Expected.UInt8Val);
	Test->TestEqual(Prefix + "UInt16Val should match", Actual.UInt16Val, Expected.UInt16Val);
	Test->TestEqual(Prefix + "UInt32Val should match", Actual.UInt32Val, Expected.UInt32Val);
	Test->TestEqual(Prefix + "UInt64Val should match", Actual.UInt64Val, Expected.UInt64Val);
	Test->TestEqual(Prefix + "Int8Val should match", Actual.Int8Val, Expected.Int8Val);
	Test->TestEqual(Prefix + "Int16Val should match", Actual.Int16Val, Expected.Int16Val);
	Test->TestEqual(Prefix + "Int32Val should match", Actual.Int32Val, Expected.Int32Val);
	Test->TestEqual(Prefix + "Int64Val should match", Actual.Int64Val, Expected.Int64Val);
	Test->TestEqual(Prefix + "FloatVal should match", Actual.FloatVal, Expected.FloatVal);
	Test->TestEqual(Prefix + "DoubleVal should match", Actual.DoubleVal, Expected.DoubleVal);
	Test->TestEqual(Prefix + "EnumVal should match", Actual.EnumVal, Expected.EnumVal);
	Test->TestEqual(Prefix + "VectorVal should match", Actual.VectorVal, Expected.VectorVal);
	Test->TestEqual(Prefix + "RotatorVal should match", Actual.RotatorVal, Expected.RotatorVal);
	// No equals testequal for FTransform
	Test->TestTrue(Prefix + "TransformVal should match", Actual.TransformVal.Equals(Expected.TransformVal));
	Test->TestEqual(Prefix + "NameVal should match", Actual.NameVal, Expected.NameVal);
	Test->TestEqual(Prefix + "StringVal should match", Actual.StringVal, Expected.StringVal);
	Test->TestEqual(Prefix + "TextVal should match", Actual.TextVal.ToString(), Expected.TextVal.ToString());

	Test->TestNotNull(Prefix + "UObject shouldn't be null", Actual.UObjectVal);
	if (Actual.UObjectVal)
	{
		Test->TestEqual(Prefix + "UObject String should match", Actual.UObjectVal->NestedStringVal, Expected.UObjectVal->NestedStringVal);
		Test->TestEqual(Prefix + "UObject Int should match", Actual.UObjectVal->NestedIntVal, Expected.UObjectVal->NestedIntVal);
		
	}

	Test->TestEqual(Prefix + "SubclassOf field should match", Actual.ActorSubclass.Get(), AStaticMeshActor::StaticClass());
	if (Test->TestEqual(Prefix + "SubclassOf array should be correct size", Actual.ActorSubclassArray.Num(), 2))
	{
		Test->TestEqual(Prefix + "SubclassOf array 0 should match", Actual.ActorSubclassArray[0].Get(), AStaticMeshActor::StaticClass());
		Test->TestEqual(Prefix + "SubclassOf array 1 should match", Actual.ActorSubclassArray[1].Get(), APointLight::StaticClass());
	}

	CheckArray(Test, Prefix + "IntArray|", Actual.IntArray, Expected.IntArray);
	CheckArray(Test, Prefix + "UInt8Array|", Actual.UInt8Array, Expected.UInt8Array);
	CheckArray(Test, Prefix + "UInt16Array|", Actual.UInt16Array, Expected.UInt16Array);
	CheckArray(Test, Prefix + "UInt32Array|", Actual.UInt32Array, Expected.UInt32Array);
	CheckArray(Test, Prefix + "UInt64Array|", Actual.UInt64Array, Expected.UInt64Array);
	CheckArray(Test, Prefix + "Int8Array|", Actual.Int8Array, Expected.Int8Array);
	CheckArray(Test, Prefix + "Int16Array|", Actual.Int16Array, Expected.Int16Array);
	CheckArray(Test, Prefix + "Int32Array|", Actual.Int32Array, Expected.Int32Array);
	CheckArray(Test, Prefix + "Int64Array|", Actual.Int64Array, Expected.Int64Array);
	CheckArray(Test, Prefix + "FloatArray|", Actual.FloatArray, Expected.FloatArray);
	CheckArray(Test, Prefix + "DoubleArray|", Actual.DoubleArray, Expected.DoubleArray);
	CheckArray(Test, Prefix + "EnumArray|", Actual.EnumArray, Expected.EnumArray);
	CheckArray(Test, Prefix + "VectorArray|", Actual.VectorArray, Expected.VectorArray);
	CheckArray(Test, Prefix + "RotatorArray|", Actual.RotatorArray, Expected.RotatorArray);
	CheckArrayExplicitEquals(Test, Prefix + "TransformArray|", Actual.TransformArray, Expected.TransformArray);
	CheckArray(Test, Prefix + "NameArray|", Actual.NameArray, Expected.NameArray);
	CheckArray(Test, Prefix + "StringArray|", Actual.StringArray, Expected.StringArray);
	Test->TestEqual(Prefix + "TextArray 0 should match", Actual.TextArray[0].ToString(), Expected.TextArray[0].ToString());
	Test->TestEqual(Prefix + "TextArray 1 should match", Actual.TextArray[1].ToString(), Expected.TextArray[1].ToString());

	CheckMap(Test, Prefix + "IntMap|", Actual.IntMap, Expected.IntMap);
	CheckMap(Test, Prefix + "FloatMap|", Actual.FloatMap, Expected.FloatMap);
	CheckMap(Test, Prefix + "IntFloatMap|", Actual.IntFloatMap, Expected.IntFloatMap);
	CheckMap(Test, Prefix + "StringVectorMap|", Actual.StringVectorMap, Expected.StringVectorMap);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestBasicAllTypes, "SPUDTest.BasicAllTypes",
                                 EAutomationTestFlags::EditorContext |
                                 EAutomationTestFlags::ClientContext |
                                 EAutomationTestFlags::ProductFilter)

bool FTestBasicAllTypes::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectBasic>();


	PopulateAllTypes(*SavedObj);

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestSaveObjectBasic>();
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	CheckAllTypes(this, "BasicObject|", *LoadedObj, *SavedObj);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestRequireFastPath, "SPUDTest.FastPath",
								EAutomationTestFlags::EditorContext |
								EAutomationTestFlags::ClientContext |
								EAutomationTestFlags::ProductFilter)

bool FTestRequireFastPath::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectBasic>();


	PopulateAllTypes(*SavedObj);

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestSaveObjectBasic>();
	State->bTestRequireFastPath = true;
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	CheckAllTypes(this, "BasicObject|", *LoadedObj, *SavedObj);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestRequireSlowPath, "SPUDTest.SlowPath",
								EAutomationTestFlags::EditorContext |
								EAutomationTestFlags::ClientContext |
								EAutomationTestFlags::ProductFilter)

bool FTestRequireSlowPath::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectBasic>();


	PopulateAllTypes(*SavedObj);

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestSaveObjectBasic>();
	State->bTestRequireSlowPath = true;
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	CheckAllTypes(this, "BasicObject|", *LoadedObj, *SavedObj);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestStructs, "SPUDTest.Structs",
								EAutomationTestFlags::EditorContext |
								EAutomationTestFlags::ClientContext |
								EAutomationTestFlags::ProductFilter)

bool FTestStructs::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectStructs>();

	PopulateAllTypes(SavedObj->SimpleStruct);
	PopulateAllTypes(SavedObj->NestedStruct.Nested);

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "StructTest");
	
	auto LoadedObj = NewObject<UTestSaveObjectStructs>();
	State->RestoreGlobalObject(LoadedObj, "StructTest");

	CheckAllTypes(this, "SimpleStruct|", LoadedObj->SimpleStruct, SavedObj->SimpleStruct);
	CheckAllTypes(this, "NestedStruct|", LoadedObj->NestedStruct.Nested, SavedObj->NestedStruct.Nested);

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestCustomData, "SPUDTest.CustomData",
								 EAutomationTestFlags::EditorContext |
								 EAutomationTestFlags::ClientContext |
								 EAutomationTestFlags::ProductFilter)

bool FTestCustomData::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectCustomData>();


	SavedObj->bSomeBoolean = true;
	SavedObj->SomeInteger = 203001;
	SavedObj->SomeString = "Hello from custom data";
	SavedObj->SomeFloat = 1.3245978893;

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestSaveObjectCustomData>();
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	TestEqual("CustomData|Bool should match", LoadedObj->bSomeBoolean, SavedObj->bSomeBoolean);
	TestEqual("CustomData|Int should match", LoadedObj->SomeInteger, SavedObj->SomeInteger);
	TestEqual("CustomData|String should match", LoadedObj->SomeString, SavedObj->SomeString);
	TestEqual("CustomData|Float should match", LoadedObj->SomeFloat, SavedObj->SomeFloat);

	TestTrue("CustomData|Peek 1 should have worked", LoadedObj->Peek1Succeeded);
	TestTrue("CustomData|Peek 1 chunk ID should match", LoadedObj->Peek1IDOK);
	TestTrue("CustomData|Peek 2 should have worked", LoadedObj->Peek2Succeeded);
	TestTrue("CustomData|Peek 2 chunk ID should match", LoadedObj->Peek2IDOK);
	TestTrue("CustomData|Skip 1 should have worked", LoadedObj->Skip1Succeeded);
	TestTrue("CustomData|Skip 1 data position should match", LoadedObj->Skip1PosOK);
	TestTrue("CustomData|Skip 2 should have worked", LoadedObj->Skip2Succeeded);
	TestTrue("CustomData|Skip 2 data position should match", LoadedObj->Skip2PosOK);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestNestedObject, "SPUDTest.NestedObject",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter)

bool FTestNestedObject::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectParent>();
	SavedObj->UObjectVal1 = NewObject<UTestNestedChild1>();
	SavedObj->UObjectVal2 = NewObject<UTestNestedChild2>();
	SavedObj->UObjectVal3 = NewObject<UTestNestedChild3>();
	SavedObj->UObjectVal4 = NewObject<UTestNestedChild4>();
	SavedObj->UObjectVal5 = NewObject<UTestNestedChild5>();

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestSaveObjectParent>();
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	TestNotNull("UObject1 shouldn't be null", LoadedObj->UObjectVal1);
	TestNotNull("UObject2 shouldn't be null", LoadedObj->UObjectVal2);
	TestNotNull("UObject3 shouldn't be null", LoadedObj->UObjectVal3);
	TestNotNull("UObject4 shouldn't be null", LoadedObj->UObjectVal4);
	TestNotNull("UObject5 shouldn't be null", LoadedObj->UObjectVal5);

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestMultipleObjectPtr, "SPUDTest.MultipleObjectPtr",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter)

bool FTestMultipleObjectPtr::RunTest(const FString& Parameters)
{
	auto SavedParent1 = NewObject<UTestSaveMultipleParentsOwner>();
	auto SavedParent2 = NewObject<UTestSaveMultipleParentsOther>();
	auto SavedParent3 = NewObject<UTestSaveMultipleParentsWeak>();
	SavedParent1->UObjectVal1 = NewObject<UTestSaveMultipleParentsChild>();
	SavedParent2->UObjectVal1 = SavedParent1->UObjectVal1;
	SavedParent3->UObjectVal1 = SavedParent1->UObjectVal1;
	SavedParent1->UObjectVal1->StringVal = TEXT("TestString");

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedParent1, "TestObject1");
	State->StoreGlobalObject(SavedParent2, "TestObject2");
	State->StoreGlobalObject(SavedParent3, "TestObject3");

	auto LoadedParent1 = NewObject<UTestSaveMultipleParentsOwner>();
	auto LoadedParent2 = NewObject<UTestSaveMultipleParentsOther>();
	auto LoadedParent3 = NewObject<UTestSaveMultipleParentsWeak>();
	State->RestoreGlobalObject(LoadedParent1, "TestObject1");
	State->RestoreGlobalObject(LoadedParent2, "TestObject2");
	State->RestoreGlobalObject(LoadedParent3, "TestObject3");
	LoadedParent1->UObjectVal1->StringVal = TEXT("NewString");

	TestEqual("UObject1 and UObject2 should be same", LoadedParent1->UObjectVal1, LoadedParent2->UObjectVal1);
	TestEqual("UObject1 and UObject2 should have same string", LoadedParent1->UObjectVal1->StringVal, LoadedParent2->UObjectVal1->StringVal);
	TestEqual("UObject1 and UObject3 should be same", LoadedParent1->UObjectVal1, LoadedParent3->UObjectVal1.Get());
	TestEqual("UObject1 and UObject3 should have same string", LoadedParent1->UObjectVal1->StringVal, LoadedParent3->UObjectVal1.Get() ? LoadedParent3->UObjectVal1->StringVal : TEXT(""));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestObjectCycle, "SPUDTest.TestObjectCycle",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter)

bool FTestObjectCycle::RunTest(const FString& Parameters)
{
	{
		// First test with cycle of 2 objects.
		auto SavedObj = NewObject<UTestSaveObjectCycle>();
		SavedObj->StringVal = TEXT("First");
		SavedObj->UObjectVal = NewObject<UTestSaveObjectCycle>();
		SavedObj->UObjectVal->StringVal = TEXT("Second");
		SavedObj->UObjectVal->UObjectVal = SavedObj;

		auto State = NewObject<USpudState>();
		State->StoreGlobalObject(SavedObj, "TestObject");

		auto LoadedObj = NewObject<UTestSaveObjectCycle>();
		State->RestoreGlobalObject(LoadedObj, "TestObject");
		LoadedObj->UObjectVal->UObjectVal->StringVal = TEXT("FirstNew");

		TestEqual("First object string should be same as first-second-first string", LoadedObj->StringVal, LoadedObj->UObjectVal->UObjectVal->StringVal);
	}

	{
		// Second test with cycle of 3 objects.
		auto SavedObj = NewObject<UTestSaveObjectCycle>();
		SavedObj->StringVal = TEXT("First");
		SavedObj->UObjectVal = NewObject<UTestSaveObjectCycle>();
		SavedObj->UObjectVal->StringVal = TEXT("Second");
		SavedObj->UObjectVal->UObjectVal = NewObject<UTestSaveObjectCycle>();
		SavedObj->UObjectVal->UObjectVal->StringVal = TEXT("Third");
		SavedObj->UObjectVal->UObjectVal->UObjectVal = SavedObj;

		auto State = NewObject<USpudState>();
		State->StoreGlobalObject(SavedObj, "TestObject");

		auto LoadedObj = NewObject<UTestSaveObjectCycle>();
		State->RestoreGlobalObject(LoadedObj, "TestObject");
		LoadedObj->UObjectVal->UObjectVal->StringVal = TEXT("FirstNew");

		TestEqual("First object string should be same as first-second-third-first string", LoadedObj->StringVal, LoadedObj->UObjectVal->UObjectVal->UObjectVal->StringVal);
	}

	{
		// Second test with cycle of 3 objects (second and third in cycle).
		auto SavedObj = NewObject<UTestSaveObjectCycle>();
		SavedObj->StringVal = TEXT("First");
		SavedObj->UObjectVal = NewObject<UTestSaveObjectCycle>();
		SavedObj->UObjectVal->StringVal = TEXT("Second");
		SavedObj->UObjectVal->UObjectVal = NewObject<UTestSaveObjectCycle>();
		SavedObj->UObjectVal->UObjectVal->StringVal = TEXT("Third");
		SavedObj->UObjectVal->UObjectVal->UObjectVal = SavedObj->UObjectVal;

		auto State = NewObject<USpudState>();
		State->StoreGlobalObject(SavedObj, "TestObject");

		auto LoadedObj = NewObject<UTestSaveObjectCycle>();
		State->RestoreGlobalObject(LoadedObj, "TestObject");
		LoadedObj->UObjectVal->UObjectVal->StringVal = TEXT("FirstNew");

		TestEqual("First object string should be same as first-second-third-first string", LoadedObj->UObjectVal->StringVal, LoadedObj->UObjectVal->UObjectVal->UObjectVal->StringVal);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestObjectArray, "SPUDTest.TestObjectArray",
EAutomationTestFlags::EditorContext |
EAutomationTestFlags::ClientContext |
EAutomationTestFlags::ProductFilter)

bool FTestObjectArray::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestSaveObjectArrayParent>();
	SavedObj->StringVal = TEXT("Parent");
	for (int i = 0; i<3; i++)
	{
		UTestSaveObjectCycle* ChildObj = SavedObj->UObjects.Add_GetRef(NewObject<UTestSaveObjectCycle>());
		ChildObj->StringVal = FString::Printf(TEXT("Child_%d"), i);
		//SavedObj->UObjectVal->UObjectVal = SavedObj;
	}

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestSaveObjectArrayParent>();
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	TestEqual("Parent string should be same loaded Parent", SavedObj->StringVal, LoadedObj->StringVal);
	LoadedObj->StringVal = TEXT("ParentNew");
	TestEqual("Parent string should be same loaded Parent", LoadedObj->StringVal, TEXT("ParentNew"));
	for (int i = 0; i<3; i++)
	{
		TestEqual("Child string should be same as loaded child string", SavedObj->UObjects[i]->StringVal, LoadedObj->UObjects[i]->StringVal);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestStructArray, "SPUDTest.TestStructArray",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter)

bool FTestStructArray::RunTest(const FString& Parameters)
{
	auto SavedObj = NewObject<UTestStructInArray>();
	SavedObj->SimpleStruct.TestString = TEXT("Parent");
	SavedObj->SimpleStruct.TestFloat = 1.0f;
	for (int i = 0; i<3; i++)
	{
		FTestSimpleStruct& ChildStruct = SavedObj->SimpleStructArray.Add_GetRef(FTestSimpleStruct());
		ChildStruct.TestString = FString::Printf(TEXT("Struct_%d"), i);
		ChildStruct.TestFloat = i;
	}

	auto State = NewObject<USpudState>();
	State->StoreGlobalObject(SavedObj, "TestObject");

	auto LoadedObj = NewObject<UTestStructInArray>();
	State->RestoreGlobalObject(LoadedObj, "TestObject");

	TestEqual("Parent string should be same loaded Parent", SavedObj->SimpleStruct.TestString, LoadedObj->SimpleStruct.TestString);
	TestEqual("Parent float should be same loaded Parent", SavedObj->SimpleStruct.TestFloat, LoadedObj->SimpleStruct.TestFloat);
	for (int i = 0; i<3; i++)
	{
		TestEqual("Child string should be same as loaded child string", SavedObj->SimpleStructArray[i].TestString, LoadedObj->SimpleStructArray[i].TestString);
		TestEqual("Child float should be same as loaded child string", SavedObj->SimpleStructArray[i].TestFloat, LoadedObj->SimpleStructArray[i].TestFloat);
	}
	return true;
}