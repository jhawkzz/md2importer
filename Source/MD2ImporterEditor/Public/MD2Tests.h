
#pragma once

//#define MD2_ENABLE_TESTS

#ifdef MD2_ENABLE_TESTS
class UMD2Tests
{
public:
	static void TestMessageBox( );
	static void TestCreateRawMesh( FRawMesh& OutRawMesh );
	static void TestAttachToNewActor( const UStaticMesh& StaticMesh );
};
#endif