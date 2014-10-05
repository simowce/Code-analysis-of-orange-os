PUBLIC void init_8259()
{
	out_byte(INT_M_CTL, 0x11);
	out_byte(INT_S_CTL, 0x11);

	out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);
	out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);

	out_byte(INT_M_CTLMASK, 0x4);
	out_byte(INT_S_CTLMASK, 0x2);

	out_byte(INT_M_CTLMASK, 0x1);
	out_byte(INT_S_CTLMASK, 0x1);

	out_byte(INT_M_CTLMASK, 0xFF);
	out_byte(INT_S_CTLMASK, 0xFF);
}
