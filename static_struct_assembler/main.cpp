#include "includes.hpp"

struct info_entry_t
{
	info_entry_t() = default;
	info_entry_t( const std::string& name, const std::string& type, uintptr_t size, address_t address ) :
		m_name( name ), m_type( type ), m_size( size ), m_offset( address )
	{}

	~info_entry_t() = default;

	std::string m_name;
	std::string m_type;

	address_t m_offset;

	uintptr_t m_size;
};

#define STRUCT_NAME "example_t"

#define ADD_FIELD( field_name, type, offset ) info_entry_t( field_name, typeid( type ).name(), sizeof( type ), offset )

BOOL main()
{
	try
	{
		auto struct_fields = std::vector<info_entry_t>
		{
			ADD_FIELD( "m_iExample", int32_t, 0x02 ),
			ADD_FIELD( "m_iExample2", int32_t, 0x22C ),
			ADD_FIELD( "m_fExample1", float_t, 0x420 ),
		};

		/// Sort vector by offset so everything is at the right position
		std::sort( struct_fields.begin(), struct_fields.end(),
				   []( const auto & first, const auto & second ) -> bool
				   {
					   return first.m_offset.as<uintptr_t>() < second.m_offset.as<uintptr_t>();
				   } );


		std::ofstream output( "out.hpp" );
		if ( !output.is_open() )
			throw std::exception( "Failed to open output file" );

		output << "struct " << STRUCT_NAME <<std::endl <<
			"{" << std::endl;

		/// Go through all struct fields and assemble the struct.
		/// ( Add paddings where needed )
		for ( auto i = 0u; i < struct_fields.size(); i++ )
		{
			auto& field = struct_fields.at( i );

			output << "\t";
			output << field.m_type << " ";
			output << field.m_name << "; ";
			output << "// 0x" << std::hex << field.m_offset.as<uintptr_t>();
			output << std::endl;

			/// Check if it's not the last item
			if ( i < struct_fields.size() - 1 )
			{
				auto& next = struct_fields.at( i + 1 );
				auto delta = next.m_offset - ( field.m_offset + field.m_size );

				/// Check if we need a padding
				if ( delta > 0 )
				{
					/// Add padding
					output << "\t";
					output << "char ";
					output << "pad_" << std::dec << i << "[ 0x" << std::hex << delta << " ];";
					output << "// 0x" << std::hex << field.m_offset + field.m_size;
					output << std::endl;
				}
			}
		}

		/// Everything good, let's complete the struct and close the file
		output << "};";

		output.close();
	}
	catch ( const std::exception & ex )
	{
		MessageBoxA( NULL, ex.what(), "Error!", MB_OK | MB_ICONERROR );
	}

	return TRUE;
}