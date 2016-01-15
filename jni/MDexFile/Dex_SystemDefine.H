/*
 * Dex_SystemDefine.H
 *
 *  Created on: 2015年9月23日
 *      Author: Ls Mouse
 */
#ifndef DEX_SYSTEMDEFINE_H_
#define DEX_SYSTEMDEFINE_H_
#ifdef HAVE_STDINT_H
# include <stdint.h>    /* C99 */
typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;
typedef unsigned char       U1;
typedef unsigned short      U2;
typedef unsigned int        U4;
typedef unsigned long long  U8;
typedef signed char         S1;
typedef signed short        S2;
typedef signed int          S4;
typedef signed long long    S8;
typedef unsigned char       Char;
typedef unsigned short      Short;
typedef unsigned int        Int;
typedef unsigned long long  Long;
typedef unsigned char       UChar;
typedef unsigned short      UShort;
typedef unsigned int        UInt;
typedef unsigned long long  ULong;
typedef signed char         SChar;
typedef signed short        SUShort;
typedef signed int          SInt;
typedef signed long long    SLong;
#else
typedef unsigned char       u1;
typedef unsigned short      u2;
typedef unsigned int        u4;
typedef unsigned long long  u8;
typedef signed char         s1;
typedef signed short        s2;
typedef signed int          s4;
typedef signed long long    s8;
typedef unsigned char       U1;
typedef unsigned short      U2;
typedef unsigned int        U4;
typedef unsigned long long  U8;
typedef signed char         S1;
typedef signed short        S2;
typedef signed int          S4;
typedef signed long long    S8;
typedef unsigned char       Char;
typedef unsigned short      Short;
typedef unsigned int        Int;
typedef unsigned long long  Long;
typedef unsigned char       UChar;
typedef unsigned short      UShort;
typedef unsigned int        UInt;
typedef unsigned long long  ULong;
typedef signed char         SChar;
typedef signed short        SUShort;
typedef signed int          SInt;
typedef signed long long    SLong;
#endif
#ifndef _DEX_GEN_INLINES             /* only defined by DexInlines.c */
	#define DEX_INLINE 		extern __inline__
#else
	#define DEX_INLINE
#endif

/* DEX file magic number */
#define DEX_MAGIC       	"dex\n"
/* version, encoded in 4 bytes of ASCII */
#define DEX_MAGIC_VERS 		"035\0"
/* same, but for optimized DEX header */
#define DEX_OPT_MAGIC   	"dey\n"
#define DEX_OPT_MAGIC_VERS	"036\0"
#define DEX_DEP_MAGIC   	"deps"
/*
 * 160-bit SHA-1 digest.
 */
enum {
	kSHA1DigestLen = 20,
	kSHA1DigestOutputLen = kSHA1DigestLen*2 +1 };
/* general constants */
enum {
    kDexEndianConstant = 0x12345678,    /* the endianness indicator */
    kDexNoIndex = 0xffffffff,           /* not a valid index value */
};
/*
 * access flags and masks; the "standard" ones are all <= 0x4000
 *
 * Note: There are related declarations in vm/oo/Object.h in the ClassFlags
 * enum.
 */
enum {
    ACC_PUBLIC       = 0x00000001,       // class, field, method, ic
    ACC_PRIVATE      = 0x00000002,       // field, method, ic
    ACC_PROTECTED    = 0x00000004,       // field, method, ic
    ACC_STATIC       = 0x00000008,       // field, method, ic
    ACC_FINAL        = 0x00000010,       // class, field, method, ic
    ACC_SYNCHRONIZED = 0x00000020,       // method (only allowed on natives)
    ACC_SUPER        = 0x00000020,       // class (not used in Dalvik)
    ACC_VOLATILE     = 0x00000040,       // field
    ACC_BRIDGE       = 0x00000040,       // method (1.5)
    ACC_TRANSIENT    = 0x00000080,       // field
    ACC_VARARGS      = 0x00000080,       // method (1.5)
    ACC_NATIVE       = 0x00000100,       // method
    ACC_INTERFACE    = 0x00000200,       // class, ic
    ACC_ABSTRACT     = 0x00000400,       // class, method, ic
    ACC_STRICT       = 0x00000800,       // method
    ACC_SYNTHETIC    = 0x00001000,       // field, method, ic
    ACC_ANNOTATION   = 0x00002000,       // class, ic (1.5)
    ACC_ENUM         = 0x00004000,       // class, field, ic (1.5)
    ACC_CONSTRUCTOR  = 0x00010000,       // method (Dalvik only)
    ACC_DECLARED_SYNCHRONIZED =
                       0x00020000,       // method (Dalvik only)
    ACC_CLASS_MASK =
        (ACC_PUBLIC | ACC_FINAL | ACC_INTERFACE | ACC_ABSTRACT
                | ACC_SYNTHETIC | ACC_ANNOTATION | ACC_ENUM),
    ACC_INNER_CLASS_MASK =
        (ACC_CLASS_MASK | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC),
    ACC_FIELD_MASK =
        (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC | ACC_FINAL
                | ACC_VOLATILE | ACC_TRANSIENT | ACC_SYNTHETIC | ACC_ENUM),
    ACC_METHOD_MASK =
        (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED | ACC_STATIC | ACC_FINAL
                | ACC_SYNCHRONIZED | ACC_BRIDGE | ACC_VARARGS | ACC_NATIVE
                | ACC_ABSTRACT | ACC_STRICT | ACC_SYNTHETIC | ACC_CONSTRUCTOR
                | ACC_DECLARED_SYNCHRONIZED),
};
/* annotation constants */
enum {
    kDexVisibilityBuild         = 0x00,     /* annotation visibility */
    kDexVisibilityRuntime       = 0x01,
    kDexVisibilitySystem        = 0x02,
    kDexAnnotationByte          = 0x00,
    kDexAnnotationShort         = 0x02,
    kDexAnnotationChar          = 0x03,
    kDexAnnotationInt           = 0x04,
    kDexAnnotationLong          = 0x06,
    kDexAnnotationFloat         = 0x10,
    kDexAnnotationDouble        = 0x11,
    kDexAnnotationString        = 0x17,
    kDexAnnotationType          = 0x18,
    kDexAnnotationField         = 0x19,
    kDexAnnotationMethod        = 0x1a,
    kDexAnnotationEnum          = 0x1b,
    kDexAnnotationArray         = 0x1c,
    kDexAnnotationAnnotation    = 0x1d,
    kDexAnnotationNull          = 0x1e,
    kDexAnnotationBoolean       = 0x1f,
    kDexAnnotationValueTypeMask = 0x1f,     /* low 5 bits */
    kDexAnnotationValueArgShift = 5,
};
/* map item type codes */
enum {
    kDexTypeHeaderItem               = 0x0000,
    kDexTypeStringIdItem             = 0x0001,
    kDexTypeTypeIdItem               = 0x0002,
    kDexTypeProtoIdItem              = 0x0003,
    kDexTypeFieldIdItem              = 0x0004,
    kDexTypeMethodIdItem             = 0x0005,
    kDexTypeClassDefItem             = 0x0006,
    kDexTypeMapList                  = 0x1000,
    kDexTypeTypeList                 = 0x1001,
    kDexTypeAnnotationSetRefList     = 0x1002,
    kDexTypeAnnotationSetItem        = 0x1003,
    kDexTypeClassDataItem            = 0x2000,
    kDexTypeCodeItem                 = 0x2001,
    kDexTypeStringDataItem           = 0x2002,
    kDexTypeDebugInfoItem            = 0x2003,
    kDexTypeAnnotationItem           = 0x2004,
    kDexTypeEncodedArrayItem         = 0x2005,
    kDexTypeAnnotationsDirectoryItem = 0x2006,
};
/* auxillary data section chunk codes */
enum {
    kDexChunkClassLookup            = 0x434c4b50,   /* CLKP */
    kDexChunkRegisterMaps           = 0x524d4150,   /* RMAP */
    kDexChunkReducingIndexMap       = 0x5249584d,   /* RIXM */
    kDexChunkExpandingIndexMap      = 0x4549584d,   /* EIXM */
    kDexChunkEnd                    = 0x41454e44,   /* AEND */
};
/* debug info opcodes and constants */
enum {
    DBG_END_SEQUENCE         = 0x00,
    DBG_ADVANCE_PC           = 0x01,
    DBG_ADVANCE_LINE         = 0x02,
    DBG_START_LOCAL          = 0x03,
    DBG_START_LOCAL_EXTENDED = 0x04,
    DBG_END_LOCAL            = 0x05,
    DBG_RESTART_LOCAL        = 0x06,
    DBG_SET_PROLOGUE_END     = 0x07,
    DBG_SET_EPILOGUE_BEGIN   = 0x08,
    DBG_SET_FILE             = 0x09,
    DBG_FIRST_SPECIAL        = 0x0a,
    DBG_LINE_BASE            = -4,
    DBG_LINE_RANGE           = 15,
};

/*
 * Direct-mapped "header_item" struct.
 */
typedef struct DexHeader {
    u1  magic[8];           /* includes version number */
    u4  checksum;           /* adler32 checksum */
    u1  signature[kSHA1DigestLen]; /* SHA-1 hash */
    u4  fileSize;           /* length of entire file */
    u4  headerSize;         /* offset to start of next section */
    u4  endianTag;
    u4  linkSize;
    u4  linkOff;
    u4  mapOff;
    u4  stringIdsSize;
    u4  stringIdsOff;
    u4  typeIdsSize;
    u4  typeIdsOff;
    u4  protoIdsSize;
    u4  protoIdsOff;
    u4  fieldIdsSize;
    u4  fieldIdsOff;
    u4  methodIdsSize;
    u4  methodIdsOff;
    u4  classDefsSize;
    u4  classDefsOff;
    u4  dataSize;
    u4  dataOff;
} DexHeader;
/*
 * Direct-mapped "map_item".
 */
typedef struct DexMapItem {
    u2  type;              /* type code (see kDexType* above) */
    u2  unused;
    u4  size;              /* count of items of the indicated type */
    u4  offset;            /* file offset to the start of data */
} DexMapItem;

/*
 * Direct-mapped "map_list".
 */
typedef struct DexMapList {
    u4  size;               /* #of entries in list */
    DexMapItem list[1];     /* entries */
} DexMapList;

/*
 * Direct-mapped "string_id_item".
 */
typedef struct DexStringId {
    u4  stringDataOff;      /* file offset to string_data_item */
} DexStringId;
/*
 * Direct-mapped "type_id_item".
 */
typedef struct DexTypeId {
    u4  descriptorIdx;      /* index into stringIds list for type descriptor */
} DexTypeId;
/*
 * Direct-mapped "field_id_item".
 */
typedef struct DexFieldId {
    u2  classIdx;           /* index into typeIds list for defining class */
    u2  typeIdx;            /* index into typeIds for field type */
    u4  nameIdx;            /* index into stringIds for field name */
} DexFieldId;

/*
 * Direct-mapped "method_id_item".
 */
typedef struct DexMethodId {
    u2  classIdx;           /* index into typeIds list for defining class */
    u2  protoIdx;           /* index into protoIds for method prototype */
    u4  nameIdx;            /* index into stringIds for method name */
} DexMethodId;
/*
 * Direct-mapped "proto_id_item".
 */
typedef struct DexProtoId {
    u4  shortyIdx;          /* index into stringIds for shorty descriptor */
    u4  returnTypeIdx;      /* index into typeIds list for return type */
    u4  parametersOff;      /* file offset to type_list for parameter types */
} DexProtoId;
/**/
typedef struct DexMapId{
 	u2	type;       		 /*Section type*/

	u2	unused;			/*unused*/
	u4	size;        		/* section size*/
	u4	offset;     		/* section offset */
}DexMapId;
/*
 * Direct-mapped "class_def_item".
 */
typedef struct DexClassDef {
    u4  classIdx;           /* index into typeIds for this class */
    u4  accessFlags;
    u4  superclassIdx;      /* index into typeIds for superclass */
    u4  interfacesOff;      /* file offset to DexTypeList */
    u4  sourceFileIdx;      /* index into stringIds for source file name */
    u4  annotationsOff;     /* file offset to annotations_directory_item */
    u4  classDataOff;       /* file offset to class_data_item */
    u4  staticValuesOff;    /* file offset to DexEncodedArray */
} DexClassDef;
/*
 * Direct-mapped "type_item".
 */
typedef struct DexTypeItem {
    u2  typeIdx;            /* index into typeIds */
} DexTypeItem;

/*
 * Direct-mapped "type_list".
 */
typedef struct DexTypeList {
    u4  size;               /* #of entries in list */
    DexTypeItem list[1];    /* entries */
} DexTypeList;
/*
 * Link table.  Currently undefined.
 */
typedef struct DexLink {
    u1  bleargh;
} DexLink;
/*
 * Direct-mapped "annotations_directory_item".
 */
typedef struct DexAnnotationsDirectoryItem {
    u4  classAnnotationsOff;  /* offset to DexAnnotationSetItem */
    u4  fieldsSize;           /* count of DexFieldAnnotationsItem */
    u4  methodsSize;          /* count of DexMethodAnnotationsItem */
    u4  parametersSize;       /* count of DexParameterAnnotationsItem */
    /* followed by DexFieldAnnotationsItem[fieldsSize] */
    /* followed by DexMethodAnnotationsItem[methodsSize] */
    /* followed by DexParameterAnnotationsItem[parametersSize] */
} DexAnnotationsDirectoryItem;

/*
 * Direct-mapped "field_annotations_item".
 */
typedef struct DexFieldAnnotationsItem {
    u4  fieldIdx;
    u4  annotationsOff;             /* offset to DexAnnotationSetItem */
} DexFieldAnnotationsItem;

/*
 * Direct-mapped "method_annotations_item".
 */
typedef struct DexMethodAnnotationsItem {
    u4  methodIdx;
    u4  annotationsOff;             /* offset to DexAnnotationSetItem */
} DexMethodAnnotationsItem;

/*
 * Direct-mapped "parameter_annotations_item".
 */
typedef struct DexParameterAnnotationsItem {
    u4  methodIdx;
    u4  annotationsOff;             /* offset to DexAnotationSetRefList */
} DexParameterAnnotationsItem;

/*
 * Direct-mapped "annotation_set_ref_item".
 */
typedef struct DexAnnotationSetRefItem {
    u4  annotationsOff;             /* offset to DexAnnotationSetItem */
} DexAnnotationSetRefItem;

/*
 * Direct-mapped "annotation_set_ref_list".
 */
typedef struct DexAnnotationSetRefList {
    u4  size;
    DexAnnotationSetRefItem list[1];
} DexAnnotationSetRefList;

/*
 * Direct-mapped "anotation_set_item".
 */
typedef struct DexAnnotationSetItem {
    u4  size;
    u4  entries[1];                 /* offset to DexAnnotationItem */
} DexAnnotationSetItem;

/*
 * Direct-mapped "annotation_item".
 *
 * NOTE: this structure is byte-aligned.
 */
typedef struct DexAnnotationItem {
    u1  visibility;
    u1  annotation[1];              /* data in encoded_annotation format */
} DexAnnotationItem;

/*
 * Direct-mapped "encoded_array".
 *
 * NOTE: this structure is byte-aligned.
 */
typedef struct DexEncodedArray {
    u1  array[1];                   /* data in encoded_array format */
} DexEncodedArray;

/*
 * Lookup table for classes.  It provides a mapping from class name to
 * class definition.  Used by dexFindClass().
 *
 * We calculate this at DEX optimization time and embed it in the file so we
 * don't need the same hash table in every VM.  This is slightly slower than
 * a hash table with direct pointers to the items, but because it's shared
 * there's less of a penalty for using a fairly sparse table.
 */
typedef struct DexClassLookup {
    int     size;                       // total size, including "size"
    int     numEntries;                 // size of table[]; always power of 2
    struct {
        u4      classDescriptorHash;    // class descriptor hash code
        int     classDescriptorOffset;  // in bytes, from start of DEX
        int     classDefOffset;         // in bytes, from start of DEX
    } table[1];
} DexClassLookup;

/*
 * Map constant pool indices from one form to another.  Some or all of these
 * may be NULL.
 *
 * The map values are 16-bit unsigned values.  If the values we map to
 * require a larger range, we omit the mapping for that category (which
 * requires that the lookup code recognize that the data will not be
 * there for all DEX files in all categories.)
 */
typedef struct DexIndexMap {
    const u2* classMap;         /* map, either expanding or reducing */
    u4  classFullCount;         /* same as typeIdsSize */
    u4  classReducedCount;      /* post-reduction count */
    const u2* methodMap;
    u4  methodFullCount;
    u4  methodReducedCount;
    const u2* fieldMap;
    u4  fieldFullCount;
    u4  fieldReducedCount;
    const u2* stringMap;
    u4  stringFullCount;
    u4  stringReducedCount;
} DexIndexMap;

/*
 * Header added by DEX optimization pass.  Values are always written in
 * local byte and structure padding.  The first field (magic + version)
 * is guaranteed to be present and directly readable for all expected
 * compiler configurations; the rest is version-dependent.
 *
 * Try to keep this simple and fixed-size.
 */
typedef struct DexOptHeader {
    u1  magic[8];           /* includes version number */
    u4  dexOffset;          /* file offset of DEX header */
    u4  dexLength;
    u4  depsOffset;         /* offset of optimized DEX dependency table */
    u4  depsLength;
    u4  auxOffset;          /* file offset of pre-calc auxillary data */
    u4  auxLength;
    u4  flags;              /* some info flags */
    u4  checksum;           /* adler32 checksum covering deps/aux */
    /* pad for 64-bit alignment if necessary */
} DexOptHeader;

#define DEX_FLAG_VERIFIED           (1)     /* tried to verify all classes */
#define DEX_OPT_FLAG_BIG            (1<<1)  /* swapped to big-endian */
#define DEX_OPT_FLAG_FIELDS         (1<<2)  /* field access optimized */
#define DEX_OPT_FLAG_INVOCATIONS    (1<<3)  /* method calls optimized */
#define DEX_INTERFACE_CACHE_SIZE    128     /* must be power of 2 */

/*
 * Structure representing a DEX file.
 *
 * Code should regard DexFile as opaque, using the API calls provided here
 * to access specific structures.
 */
typedef struct DexFile {
    /* directly-mapped "opt" header */
    const DexOptHeader* pOptHeader;
    /* pointers to directly-mapped structs and arrays in base DEX */
    const DexHeader*    pHeader;
    const DexStringId*  pStringIds;
    const DexTypeId*    pTypeIds;
    const DexFieldId*   pFieldIds;
    const DexMethodId*  pMethodIds;
    const DexProtoId*   pProtoIds;
    const DexClassDef*  pClassDefs;
    const DexLink*      pLinkData;
    /*
     * These are mapped out of the "auxillary" section, and may not be
     * included in the file.
     */
    const DexClassLookup* pClassLookup;
    DexIndexMap         indexMap;
    const void*         pRegisterMapPool;       // RegisterMapClassPool

    /* points to start of DEX file data */
    const u1*           baseAddr;

    /* track memory overhead for auxillary structures */
    int                 overhead;

    /* additional app-specific data structures associated with the DEX */
    //void*               auxData;
} DexFile;
//
struct DexCode {
	u2  registersSize;
	u2  insSize;
	u2  outsSize;
	u2  triesSize;
	u4  debugInfoOff;       /* file offset to debug info stream */
	u4  insnsSize;          /* size of the insns array, in u2 units */
	u2  insns[1];
	/* followed by optional u2 padding */
	/* followed by try_item[triesSize] */
	/* followed by uleb128 handlersSize */
	/* followed by catch_handler_item[handlersSize] */
};
#endif
/* DEX_SYSTEMDEFINE_H_ */
