/*

Project:    asm56k
Author:     M.Buras (sqward)


*/

#ifndef _GENPARMOVES_H_
#define _GENPARMOVES_H_

#include <asm_types.h>
#include <CodeUtils.h>

extern uint const xmem_reg_pattern1[2];
extern uint const xmem_reg_pattern2[2];
extern uint const ymem_reg_pattern1[2];
extern uint const ymem_reg_pattern2[2];
extern uint const lmem_reg_pattern1[2];
extern uint const lmem_reg_pattern2[2];
extern uint const XRegYReg_pattern[4];

extern bcode GenParIFcc(uint condition, uint opcode);
extern bcode GenParExpReg(int val, uint dst_reg);
extern bcode GenParExpRegShort(int val, uint dst_reg);
extern bcode GenParRegReg(uint src_reg, uint dst_reg);
extern bcode GenParUpdate(uint update_op);
extern bcode GenMemReg(const uint *opcodes, bcode *ea, uint dst_reg);
extern bcode GenParExpRegRegReg(int val,uint dst_reg1, uint src_reg2, uint dst_reg2);
extern bcode GenParEaRegRegReg(bcode *ea,uint dst_reg1, uint src_reg2, uint dst_reg2);
extern bcode GenParRegEaRegReg(uint src_reg1,bcode *ea, uint src_reg2, uint dst_reg2);
extern bcode GenParRegRegExpReg(uint src_reg1, uint dst_reg1, int val, uint dst_reg2);
extern bcode GenParRegRegEaReg(uint src_reg1, uint dst_reg1, bcode *ea, uint dst_reg2);
extern bcode GenParRegRegRegEa(uint src_reg1, uint dst_reg1, uint dst_reg2, bcode *ea);
extern bcode GenLMemReg(const uint *opcodes, bcode *ea, uint dst_reg);
extern bcode GenParXRegYReg(uint opcode,bcode *src_ea1, uint dst_reg1, bcode *src_ea2, uint dst_reg2);
extern bcode GenParEmpty(void);

#endif /* _GENPARMOVES_H_ */
