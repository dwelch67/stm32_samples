
with System; use System;
with System.Unsigned_Types; use System.Unsigned_Types;

package GPIOA with Preelaborate is

   GPIOA_Base_Address    : constant := 16#4002_0000#;
   MODER_Offset_Address  : constant := 16#00#;
   BSRR_Offset_Address   : constant := 16#18#;

   MODER : Unsigned
     with Volatile, Address =>
       System'To_Address (GPIOA_Base_Address + MODER_Offset_Address);

   BSRR : Unsigned
     with Volatile, Address =>
       System'To_Address (GPIOA_Base_Address + BSRR_Offset_Address);

end GPIOA;
