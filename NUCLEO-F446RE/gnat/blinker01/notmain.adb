
with System.Unsigned_Types; use System.Unsigned_Types;

with RCC; use RCC;
with GPIOA; use GPIOA;
with Dummy;

procedure Notmain is
I : Integer;
RA : Unsigned;
begin

    RA := RCC.AHB1ENR;
	RA := RA or 16#00000001#;
    RCC.AHB1ENR := RA;

    ra := GPIOA.MODER;
	-- 7766 5544 3322 1100
	ra := ra and 16#FFFFF3FF#;
	ra := ra or 16#00000400#;
    GPIOA.MODER := ra;
	
	loop
   
	GPIOA.BSRR := 16#00000020#;
    for I in 1 .. 500000 loop
	    Dummy (I);
		end loop;
   
	GPIOA.BSRR := 16#00200000#;
    for I in 1 .. 500000 loop
	    Dummy (I);
		end loop;
   
    end loop;
   
end Notmain;
