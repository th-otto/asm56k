// -----------------------------------------------------------------------------------------------
/*

Project:    asm56k
Author:     M.Buras (sqward)


*/
// -----------------------------------------------------------------------------------------------

#ifndef _GENARITH_H_
#define _GENARITH_H_

#include <asm_types.h>
#include <CodeUtils.h>

int	add_patterns[3];
int	sub_patterns[3];
int	cmp_patterns[3];
int and_patterns[3];
int eor_patterns[3];
int or_patterns[3];
int asl_patterns[3];
int asr_patterns[3];
int lsl_patterns[3];
int lsr_patterns[3];

int incdec_patterns[2];

uint mpy_pattern[4];
uint macr_pattern[3];
uint mpyr_pattern[3];
uint max_pattern[2];
uint mac_pattern[4];

void GenDiv(uint src_reg,uint dst_reg);
void GenDMac(uint modifier,uint plusminus,int val, uint dest_reg);
void GenAndEorOr(uint insn_patt,uint src_reg,uint dest_reg,bcode *par_move );
void GenAdcSbc(uint insn_patt,uint src_reg,uint dest_reg,bcode *par_move );
void GenAddSub(uint insn_patt,uint src_reg,uint dest_reg,bcode *par_move );
void GenCmp(uint insn_patt,uint src_reg,uint dest_reg,bcode *par_move );
void GenAddSubEorOrLong(uint insn_patt,int val,uint dest_reg);
void GenAddSubEorOrShort(uint insn_patt,int val,uint dest_reg);
void GenAddxSubx(uint insn_patt,uint src_reg,uint dest_reg,bcode *par_move );
void GenAndiOri(uint insn_patt,int val,uint dest_reg);
void GenAsxImmediate(uint insn_patt,int val,uint src_reg,uint dest_reg);
void GenAsxReg(uint insn_patt,int val_reg,uint src_reg,uint dest_reg);
void GenCmpm(uint src_reg, uint dest_reg,bcode *par_move);
void GenCmpu(uint src_reg, uint dest_reg);
void GenIncDec(uint inst_patt,uint reg);
void GenLsxReg( int *insn_patt,int src_reg, uint dest_reg);
void GenLsxImmediate( int *insn_patt,int val, uint dest_reg);
void GenMul1(uint *insn_patt,uint plusminus,uint reg_pair,uint dest_reg,bcode *par_move);
void GenMul2(uint *insn_patt,uint plusminus,uint src_reg,int val,uint dest_reg);
void GenMuli(uint *insn_patt,uint plusminus,int val,uint src_reg,uint dest_reg);
void GenMulxx(uint *insn_patt,uint code, uint plusminus,uint reg_pair,uint dest_reg);
void GenMax(uint insn_patt,uint src_reg,uint dst_reg,bcode *par_move);
void GenNorm(uint src_reg,uint dst_reg);
void GenNormf(uint src_reg,uint dst_reg);

#endif //_GENARITH_H_