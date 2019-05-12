#pragma once

struct address_t
{
protected:
	uintptr_t m_ptr;
public:

	// default c/dtor
	__forceinline address_t() : m_ptr{} {};
	__forceinline ~address_t() {};

	__forceinline address_t( uintptr_t a ) :
		m_ptr{ a }
	{}

	__forceinline address_t( const void* a ) :
		m_ptr{ ( uintptr_t )a }
	{}

	__forceinline operator uintptr_t()
	{
		return m_ptr;
	}

	__forceinline operator void* ( )
	{
		return ( void* )m_ptr;
	}

	__forceinline operator const void* ( )
	{
		return ( const void* )m_ptr;
	}

	// to is like as but dereferences.
	template< typename t = address_t >
	__forceinline t to() const
	{
		return *( t* )m_ptr;
	}

	template< typename t = address_t >
	__forceinline t as() const
	{
		return ( t )m_ptr;
	}

	template< typename t = address_t >
	__forceinline t at( ptrdiff_t offset ) const
	{
		return *( t* )( m_ptr + offset );
	}

	template< typename t = address_t >
	__forceinline t add( ptrdiff_t offset ) const
	{
		return ( t )( m_ptr + offset );
	}

	template< typename t = address_t >
	__forceinline t sub( ptrdiff_t offset ) const
	{
		return ( t )( m_ptr - offset );
	}

	template< typename t = address_t >
	__forceinline t get( size_t dereferences = 1 )
	{
		return ( t )( get_( dereferences ) );
	}

	template< typename t = address_t >
	__forceinline void set( t val )
	{
		*( t* )m_ptr = val;
	}

	template< typename t = address_t >
	__forceinline t rel( size_t offset = 0 )
	{
		uintptr_t out;
		uint32_t rel;

		out = m_ptr + offset;

		rel = *( uint32_t* )out;
		if ( !rel )
			return t{};

		out = ( out + 0x4 ) + rel;

		return ( t )out;
	}

	__forceinline static bool safe( address_t to_check )
	{
		static MEMORY_BASIC_INFORMATION32 mbi{};

		if ( !to_check
			 || to_check < 0x10000
			 || to_check > 0xFFE00000
			 || !VirtualQuery( to_check, ( PMEMORY_BASIC_INFORMATION )& mbi, sizeof( mbi ) ) )
			return false;

		if ( !mbi.AllocationBase
			 || mbi.State != MEM_COMMIT
			 || mbi.Protect == PAGE_NOACCESS
			 || mbi.Protect & PAGE_GUARD )
			return false;

		return true;
	}
private:
	__forceinline uintptr_t get_( size_t dereferences )
	{
		uintptr_t temp = m_ptr;

		while ( dereferences-- && safe( temp ) )
			temp = *reinterpret_cast< uintptr_t* >( temp );

		return temp;
	}
};
