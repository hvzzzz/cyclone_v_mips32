library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity fifo is
  port(
    clk                 : in  std_logic;
    reset               : in  std_logic;
    dmem_write          : in  std_logic;
    r2_data_o           : in  std_logic_vector(31 downto 0);
    fifo_0              : out std_logic_vector(31 downto 0);
    fifo_1              : out std_logic_vector(31 downto 0);
    fifo_2              : out std_logic_vector(31 downto 0);
    fifo_valid          : out std_logic_vector(2 downto 0);
    fifo_update_counter : out std_logic_vector(31 downto 0)
    );
end fifo;

architecture env_fifo of fifo is
  signal data_0               : std_logic_vector(31 downto 0) := (others => '0');
  signal data_1               : std_logic_vector(31 downto 0) := (others => '0');
  signal data_2               : std_logic_vector(31 downto 0) := (others => '0');
  signal valid_slots          : std_logic_vector(2 downto 0)  := (others => '0');
  signal update_counter_value : unsigned(31 downto 0)         := (others => '0');

begin
  process(clk)
  begin
    if rising_edge(clk) then
      if reset = '1' then
        data_0 <= (others => '0');
        data_1 <= (others => '0');
        data_2 <= (others => '0');
        valid_slots <= (others => '0');
        update_counter_value <= (others => '0');
      else
        if dmem_write = '1' then
          data_0 <= r2_data_o;
          data_1 <= data_0;
          data_2 <= data_1;
          valid_slots(0) <= '1';
          valid_slots(1) <= valid_slots(0);
          valid_slots(2) <= valid_slots(1);
          update_counter_value <= update_counter_value + 1;
        end if;
      end if;
    end if;
  end process;
  fifo_0 <= data_0;
  fifo_1 <= data_1;
  fifo_2 <= data_2;
  fifo_valid <= valid_slots;
  fifo_update_counter <= std_logic_vector(update_counter_value);
end env_fifo;
