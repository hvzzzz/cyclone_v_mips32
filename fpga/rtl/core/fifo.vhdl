library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity fifo is
  port(
    clk                 : in  std_logic;
    reset               : in  std_logic;
    dmem_write          : in  std_logic;
    r2_data_o           : in  std_logic_vector(31 downto 0);
    fifo_hex0           : out std_logic_vector(6 downto 0);
    fifo_hex1           : out std_logic_vector(6 downto 0);
    fifo_hex2           : out std_logic_vector(6 downto 0);
    fifo_hex3           : out std_logic_vector(6 downto 0);
    fifo_hex4           : out std_logic_vector(6 downto 0);
    fifo_hex5           : out std_logic_vector(6 downto 0);
    fifo_0              : out std_logic_vector(31 downto 0);
    fifo_1              : out std_logic_vector(31 downto 0);
    fifo_2              : out std_logic_vector(31 downto 0);
    fifo_valid          : out std_logic_vector(2 downto 0);
    fifo_update_counter : out std_logic_vector(31 downto 0)
    );
end fifo;

architecture env_fifo of fifo is

  function bits_2_display7(bits : in std_logic_vector(3 downto 0))
    return std_logic_vector is variable display7 : std_logic_vector(6 downto 0);
  begin
    case bits is
      when "0000" => display7 := "1000000";  -- 0
      when "0001" => display7 := "1111001";  -- 1
      when "0010" => display7 := "0100100";  -- 2
      when "0011" => display7 := "0110000";  -- 3
      when "0100" => display7 := "0011001";  -- 4
      when "0101" => display7 := "0010010";  -- 5
      when "0110" => display7 := "0000010";  -- 6
      when "0111" => display7 := "1111000";  -- 7
      when "1000" => display7 := "0000000";  -- 8
      when "1001" => display7 := "0010000";  -- 9
      when "1010" => display7 := "0001000";  -- A
      when "1011" => display7 := "0000011";  -- b
      when "1100" => display7 := "1000110";  -- C
      when "1101" => display7 := "0100001";  -- d
      when "1110" => display7 := "0000110";  -- E
      when "1111" => display7 := "0001110";  -- F
      when others => display7 := "1111111";  -- blank
    end case;
    return display7;
  end;


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
        data_0               <= (others => '0');
        data_1               <= (others => '0');
        data_2               <= (others => '0');
        valid_slots          <= (others => '0');
        update_counter_value <= (others => '0');
      else
        if dmem_write = '1' then
          data_0               <= r2_data_o;
          data_1               <= data_0;
          data_2               <= data_1;
          valid_slots(0)       <= '1';
          valid_slots(1)       <= valid_slots(0);
          valid_slots(2)       <= valid_slots(1);
          update_counter_value <= update_counter_value + 1;
        end if;
      end if;
    end if;
  end process;
  fifo_0              <= data_0;
  fifo_1              <= data_1;
  fifo_2              <= data_2;
  fifo_valid          <= valid_slots;
  fifo_update_counter <= std_logic_vector(update_counter_value);

  fifo_hex0 <= bits_2_display7(data_0(3 downto 0));
  fifo_hex1 <= bits_2_display7(data_0(7 downto 4));
  fifo_hex2 <= bits_2_display7(data_1(3 downto 0));
  fifo_hex3 <= bits_2_display7(data_1(7 downto 4));
  fifo_hex4 <= bits_2_display7(data_2(3 downto 0));
  fifo_hex5 <= bits_2_display7(data_2(7 downto 4));

end env_fifo;
