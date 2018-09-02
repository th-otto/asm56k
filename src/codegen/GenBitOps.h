/*

Project:    asm56k
Author:     M.Buras (sqward)


*/

#ifndef _GENBITOPS_H_
#define _GENBITOPS_H_

#include <asm_types.h>
#include <CodeUtils.h>

int bchg_patterns[6];
int bclr_patterns[6];
int bset_patterns[6];
int btst_patterns[6];
int brclr_patterns[6];
int brset_patterns[6];
int bsclr_patterns[6];
int bsset_patterns[6];
int bra_patterns[3];
int extract_patterns[6];

void GenBitOp( int *insn_patt,int val,int xory,bcode *par_move  );
void GenBitOpReg(uint insn_patt,int val,int dest_reg);
void GenInsExt1(uint insn_patt,uint reg1,uint src_reg,uint dest_reg);
void GenInsExt2(uint insn_patt,uint val,uint src_reg,uint dest_reg);


#endif /* _GENBITOPS_H_ */
