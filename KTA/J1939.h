/*
 * J1939.h
 *
 * Created: 08-08-2016 13:28:48
 *  Author: ROHANP
 */ 


#ifndef J1939_H_
#define J1939_H_

#define PGN_MASK	( 65535 )

#define PGN_BAM		( 60416 )
#define PGN_DT		( 60160 )
#define PGN_EEC2	( 61443 )
#define PGN_EEC1	( 61444 )
#define PGN_DM1		( 65226 )
#define PGN_AUXIO1  ( 65241 )
#define PGN_SOFT	( 65242 )
#define PGN_HOURS	( 65253 )
#define PGN_CI		( 65259 )
#define PGN_ET1		( 65262 )
#define PGN_EFL_P1	( 65263 )
#define PGN_LFE1	( 65266 )
#define PGN_IC1		( 65270 )
#define PGN_VEP1	( 65271 )
#define PGN_WFI		( 65279 )
#define PGN_PTO		( 65264 )
#define PGN_AMB		( 65269 )
#define PGN_LFC		( 65257 )
#define PGN_TCO1	( 65132 )
#define PGN_ACK		( 59392 )
#define PGN_DM2		( 65227	)


//void j1939_clear_DTC(uint8_t DMtype);

//void j1939_set_filter(uint8_t mob_num,uint32_t pgn,uint32_t pgn_mask);

//void j1939_send_ack(uint32_t PGN, uint8_t control_byte);

//void j1939_request(uint32_t pgn);
#endif /* J1939_H_ */

