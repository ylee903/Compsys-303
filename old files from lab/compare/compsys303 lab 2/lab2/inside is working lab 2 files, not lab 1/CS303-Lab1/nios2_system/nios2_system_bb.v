
module nios2_system (
	clk_clk,
	reset_reset_n,
	leds_green_external_connection_export,
	lcd_external_RS,
	lcd_external_RW,
	lcd_external_data,
	lcd_external_E,
	leds_red_external_connection_export,
	switches_external_connection_export,
	keys_external_connection_export,
	uart_external_connection_rxd,
	uart_external_connection_txd,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	sdram_pll_sdram_clk_clk);	

	input		clk_clk;
	input		reset_reset_n;
	output	[8:0]	leds_green_external_connection_export;
	output		lcd_external_RS;
	output		lcd_external_RW;
	inout	[7:0]	lcd_external_data;
	output		lcd_external_E;
	output	[17:0]	leds_red_external_connection_export;
	input	[17:0]	switches_external_connection_export;
	input	[2:0]	keys_external_connection_export;
	input		uart_external_connection_rxd;
	output		uart_external_connection_txd;
	output	[11:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[15:0]	sdram_wire_dq;
	output	[1:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	output		sdram_pll_sdram_clk_clk;
endmodule
