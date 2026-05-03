library IEEE;
use IEEE.std_logic_1164.all;

entity switcher is
  port(
    clk            : in  std_logic;
    reset          : in  std_logic;
    button         : in  std_logic;
    manual_en      : in  std_logic;
    ctrl_reg_write : in  std_logic;
    ctrl_mem_write : in  std_logic_vector(3 downto 0);
    pc_en          : out std_logic;
    reg_write      : out std_logic;
    dmem_write     : out std_logic;
    cycle_out      : out std_logic_vector(1 downto 0);
    dmem_byteen    : out std_logic_vector(3 downto 0)
    );
end switcher;

architecture env_switcher of switcher is
  signal step_pulse  : std_logic;
  signal btn_sync_0  : std_logic := '0';
  signal btn_sync_1  : std_logic := '0';
  signal btn_sync_2  : std_logic := '0';
  signal cycle_state : std_logic_vector(1 downto 0);

begin
  process(clk)
  begin
    if rising_edge(clk) then
      btn_sync_0 <= not button;
      btn_sync_1 <= btn_sync_0;
      btn_sync_2 <= btn_sync_1;
    end if;
  end process;

  step_pulse <= (btn_sync_1 and not btn_sync_2) when manual_en = '1' else '1';

  process(clk)
  begin
    if rising_edge(clk) then
      if reset = '1' then
        cycle_state <= "00";
      else
        if step_pulse = '1' then
          case cycle_state is
            when "00"   => cycle_state <= "01";
            when "01"   => cycle_state <= "10";
            when others => cycle_state <= "00";
          end case;
        end if; end if;
      end if;
    end process;

      pc_en     <= '1'            when (cycle_state = "10" and step_pulse = '1') else '0';
      reg_write <= ctrl_reg_write when (cycle_state = "10" and step_pulse = '1') else '0';

      dmem_write  <= '1'            when (ctrl_mem_write = "1111" and cycle_state = "01" and step_pulse = '1') else '0';
      dmem_byteen <= ctrl_mem_write when cycle_state = "01"                                                    else "0000";
      cycle_out <= cycle_state;

    end env_switcher;
