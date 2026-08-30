#ifndef	SKILL_BASE_H
#define SKILL_BASE_H

struct skillPacket
{
	int size;
	int code;
	int classId;
	int skillId;

	skillPacket()
	{
		size = 0;
		code = 0x41502865;
		classId = 0;
		skillId = 0;
	}

};

#endif