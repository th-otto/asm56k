/*

Project:    asm56k
Author:     M.Buras (sqward)


*/

#ifndef _GENPARMOVES_H_
#define _GENPARMOVES_H_

#include <asm_types.h>
#include <CodeUtils.h>

uint xmem_reg_pattern1[2];
uint xmem_reg_pattern2[2];
uint ymem_reg_pattern1[2];
uint ymem_reg_pattern2[2];
uint lmem_reg_pattern1[2];
uint lmem_reg_pattern2[2];
uint XRegYReg_pattern[4];

bcode GenParIFcc(uint condition, uint opcode);
bcode GenParExpReg( int val, uint dst_reg);
bcode GenParExpRegShort( int val, uint dst_reg );
bcode GenParRegReg( uint src_reg, uint dst_reg );
bcode GenParUpdate(uint update_op);
bcode GenMemReg(uint *opcodes, bcode *ea,uint dst_reg);
bcode GenParExpRegRegReg(int val,uint dst_reg1,uint src_reg2,uint dst_reg2 );
bcode GenParEaRegRegReg( bcode *ea,uint dst_reg1,uint src_reg2,uint dst_reg2 );
bcode GenParRegEaRegReg( uint src_reg1,bcode *ea,uint src_reg2,uint dst_reg2 );
bcode GenParRegRegExpReg( uint src_reg1,uint dst_reg1,int val,uint dst_reg2 );
bcode GenParRegRegEaReg( uint src_reg1,uint dst_reg1,bcode *ea,uint dst_reg2 );
bcode GenParRegRegRegEa( uint src_reg1,uint dst_reg1,uint dst_reg2,bcode *ea );
bcode GenLMemReg(uint *opcodes,bcode *ea,uint dst_reg);
bcode GenParXRegYReg(uint opcode,bcode *src_ea1,uint dst_reg1,bcode *src_ea2,uint dst_reg2);
bcode GenParEmpty();

#endif /* _GENPARMOVES_H_ */
