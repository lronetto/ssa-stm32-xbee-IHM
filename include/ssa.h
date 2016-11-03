/*
 * ssa.h
 *
 *  Created on: 1 de jun de 2016
 *      Author: Leandro
 */

#ifndef INCLUDE_SSA_H_
#define INCLUDE_SSA_H_

//aciona saida
#define SSA_F_OUT			0x00
//informa estado saida
#define	SSA_F_OUTP			0x01
//informa entrada
#define	SSA_F_IN			0x02
//pergunta qual o endereço do coordenator
#define	SSA_F_CORD_ADDR		0x03
//recebe o endereco do coordenator
#define	SSA_F_CORD_ADDRP	0x04
//recebe funcao
//type in out dest[8]
#define SSA_F_FUNC			0x05
//envia dado analogico
#define SSA_F_ANALOG		0x06
//recebe tempo de atualização
#define SSA_F_ANALOG_TEMPO	0x07





#endif /* INCLUDE_SSA_H_ */
