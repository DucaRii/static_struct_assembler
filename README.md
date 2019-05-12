# static_struct_assembler
automatically assemble struct fields with given static addresses into a full struct

edit STRUCT_NAME on line 20 to your wished name

example:

### main.cpp
```c++
auto struct_fields = std::vector<info_entry_t>
{
	ADD_FIELD( "m_iExample", int32_t, 0x02 ),
	ADD_FIELD( "m_iExample2", int32_t, 0x22C ),
	ADD_FIELD( "m_fExample1", float_t, 0x420 ),
};
```

### generated out.hpp
```c++
struct example_t
{
	int m_iExample; // 0x2
	char pad_0[ 0x226 ];// 0x6
	int m_iExample2; // 0x22c
	char pad_1[ 0x1f0 ];// 0x230
	float m_fExample2; // 0x420
};
```
