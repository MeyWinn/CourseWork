#include <windows.h>

#pragma pack(push, 1)

 struct BIOS_PARAMETER_BLOCK
{
	/*0x0b*/USHORT bytes_per_sector;		/* ������ �������, � ������ */
	/*0x0d*/UCHAR  sectors_per_cluster;		/* �������� � �������� */
	/*0x0e*/USHORT reserved_sectors;		/* ������ ���� ���� */
	/*0x10*/UCHAR  fats;					/* ������ ���� ���� */
	/*0x11*/USHORT root_entries;			/* ������ ���� ���� */
	/*0x13*/USHORT sectors;					/* ������ ���� ���� */
	/*0x15*/UCHAR  media_type;				/* ��� ��������, 0xf8 = hard disk */
	/*0x16*/USHORT sectors_per_fat;			/* ������ ���� ���� */
	/*0x18*/USHORT sectors_per_track;		/* �� ������������ */
	/*0x1a*/USHORT heads;					/* �� ������������ */
	/*0x1c*/ULONG hidden_sectors;			/* �� ������������ */
	/*0x20*/ULONG large_sectors;			/* ������ ���� ���� */
	/* sizeof() = 25 (0x19) bytes */
};

struct NTFS_BOOT_SECTOR
{
	/*0x00*/UCHAR	jump[3];					/* ������� �� ����������� ��� */
	/*0x03*/ULARGE_INTEGER oem_id;				/* ��������� "NTFS    ". */
	/*0x0b*/BIOS_PARAMETER_BLOCK bpb;
	/*0x24*/UCHAR physical_drive;				/* �� ������������ */
	/*0x25*/UCHAR current_head;					/* �� ������������ */
	/*0x26*/UCHAR extended_boot_signature;		/* �� ������������ */
	/*0x27*/UCHAR reserved2;					/* �� ������������ */
	/*0x28*/ULARGE_INTEGER number_of_sectors;	/* ���������� �������� �� ����. */
	/*0x30*/LARGE_INTEGER mft_lcn;				/* ��������� ������� MFT. */
	/*0x38*/ULARGE_INTEGER mftmirr_lcn;			/* ��������� ������� ����� MFT */
	/*0x40*/CHAR  clusters_per_mft_record;		/* ������ MFT ������ � ���������. */
	/*0x41*/UCHAR  reserved0[3];				/* ��������������� */
	/*0x44*/CHAR  clusters_per_index_record;	/* ������ ��������� ������ � ���������. */
	/*0x45*/UCHAR  reserved1[3];				/* ��������������� */
	/*0x48*/ULARGE_INTEGER volume_serial_number;	/* ���������� �������� ����� ���� */
	/*0x50*/ULONG checksum;						/* �� ������������ */
	/*0x54*/UCHAR  bootstrap[426];				/* �����������-��� */
	/*0x1fe*/USHORT end_of_sector_marker;		/* ����� ������������ �������, ��������� 0xaa55 */
												/* sizeof() = 512 (0x200) bytes */
} ;

typedef enum {
	MFT_RECORD_NOT_USED = 0,		//������ �� ������������
	MFT_RECORD_IN_USE = 1,			//������ ������������
	MFT_RECORD_IS_DIRECTORY = 2		//������ ��������� �������
} MFT_RECORD_FLAGS;

 struct MFT_RECORD_DATA_NODE
{
	 DWORD startByte = 0;			// bytes, used only in resident data
	LARGE_INTEGER offset;		// bytes
	LARGE_INTEGER len;			// bytes
	ULARGE_INTEGER lowest_vcn;	// bytes
	ULARGE_INTEGER highest_vcn; // bytes
};

 struct MFT_RECORD_HEADER
{
	/*0x00*/	ULONG signature;				//��������� 'FILE'
	/*0x04*/	USHORT usa_offs;
	/*0x06*/	USHORT usa_count;
	/*0x08*/	ULARGE_INTEGER lsn;
	/*0x10*/	USHORT sequence_number;
	/*0x12*/	USHORT link_count;
	/*0x14*/	USHORT attrs_offset;
	/*0x16*/	USHORT flags;					//�����, ��. MFT_RECORD_FLAGS 
	/*0x18*/	ULONG bytes_in_use;
	/*0x1C*/	ULONG bytes_allocated;
	/*0x20*/	ULARGE_INTEGER base_mft_record; //����� ������� MFT-������
	/*0x28*/	USHORT next_attr_instance;
	/*0x2A*/	USHORT reserved;
	/*0x2C*/	ULONG mft_record_number;
	//size - 48 b 
} ;

typedef enum
{
	AT_STANDARD_INFORMATION = 0x10,
	AT_ATTRIBUTE_LIST = 0x20,
	AT_FILE_NAME = 0x30,
	AT_OBJECT_ID = 0x40,
	AT_SECURITY_DESCRIPTOR = 0x50,
	AT_VOLUME_NAME = 0x60,
	AT_VOLUME_INFORMATION = 0x70,
	AT_DATA = 0x80,
	AT_INDEX_ROOT = 0x90,
	AT_INDEX_ALLOCATION = 0xa0,
	AT_BITMAP = 0xb0,
	AT_REPARSE_POINT = 0xc0,
	AT_END = 0xffffffff
} ATTR_TYPES;

typedef enum {
	ATTR_IS_COMPRESSED = 0x1,						//������� ���� (compressed)
	ATTR_IS_ENCRYPTED = 0x4000,						//������� ���������� (encrypted)
	ATTR_IS_SPARSE = 0x8000							//������� �������� (sparse)
} ATTR_FLAGS;

 struct ATTR_RECORD_HEADER
{
	/*0x00*/	ATTR_TYPES type;					//��� ��������
	/*0x04*/	USHORT length;						//����� �������� ������������ ��� �������� � ����������   ��������
	/*0x06*/	USHORT Reserved;
	/*0x08*/	UCHAR non_resident;					//1 ���� ������� �������������, 0 - �����������
	/*0x09*/	UCHAR name_length;					//����� ����� ��������, � ��������
	/*0x0A*/	USHORT name_offset;					//�������� ����� ��������, ������������ ��������� ��������
	/*0x0C*/	USHORT flags;						//�����, ����������� � ATTR_FLAGS
	/*0x0E*/	USHORT instance;
	union
	{
		//����������� �������
		struct
		{
			/*0x10*/	ULONG value_length;			//������, � ������, ���� ��������
			/*0x14*/	USHORT value_offset;		//�������� �������� ����, ������������ ��������� ��������
			/*0x16*/	UCHAR resident_flags;		//�����, ����������� � RESIDENT_ATTR_FLAGS
			/*0x17*/	UCHAR reserved;
		} r;
		//������������� �������
		struct
		{
			/*0x10*/	ULARGE_INTEGER lowest_vcn;
			/*0x18*/	ULARGE_INTEGER highest_vcn;
			/*0x20*/	USHORT mapping_pairs_offset;		//�������� ������ �������� 
			/*0x22*/	UCHAR compression_unit;
			/*0x23*/	UCHAR reserved1[5];
			/*0x28*/	ULARGE_INTEGER allocated_size;		//������ ��������� ������������, ������� ���� �������� ��� ���� ��������
			/*0x30*/	ULARGE_INTEGER data_size;			//�������� ������ ��������
			/*0x38*/	ULARGE_INTEGER initialized_size;
		} nr;
	} u;
};


 struct ATTR_LIST_REC
{
	DWORD attr_type;
	USHORT len;
	byte name_len;
	byte name_offset;
	LARGE_INTEGER star_VCN;
	LARGE_INTEGER base_record;
	byte id;

};

struct RootPath
{
	USHORT	seq_num;
	LARGE_INTEGER rootPath_id;
	bool operator==(const RootPath& rp) { return seq_num == rp.seq_num && rootPath_id.QuadPart == rp.rootPath_id.QuadPart; }
	bool operator!=(const RootPath& rp) { return !(*this == rp); }
};
#pragma pack(pop)