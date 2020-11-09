
--with System; use System;
with System.Unsigned_Types; use System.Unsigned_Types;

package RCC with Preelaborate is

   RCC_Base_Address         : constant := 16#4002_3800#;
   AHB1ENR_Offset_Address   : constant := 16#30#;

   AHB1ENR : Unsigned
     with Volatile, Address =>
       System'To_Address (RCC_Base_Address + AHB1ENR_Offset_Address);

end RCC;
