PROGRAM Fibonacci;
VAR result: INTEGER;

FUNCTION F(n: INTEGER): INTEGER;
BEGIN
	IF n = 0 THEN
		F := 0
	ELSE
		IF n = 1 THEN
			F := 1
		ELSE
			F := F(n-1) + F(n-2)
END;

BEGIN
   result := F(20)
END.
