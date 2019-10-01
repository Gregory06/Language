# Language

It's a custom programming language project.

Supporting

  - only integer type
  - conditions like 
  
    `if ( cond ) one_line_code`
  - loops like
  
    `for ( number_of_repeatings ) one_line_code`
  - recursion
  
  ---
  
  The example of program 
  ------
 ```ruby
fun main (var c, var d,)
{
	var a
	var b
	a = 1;
	b = 1;
	call fibonacci (a, b,);
}

fun fibonacci (var x, var y,)
{
	var sum
	var h
	h = sum;
	sum = x + y;
	y = h;

	if (sum < 1000)
		call fibonacci (sum, y,);
}
```

Counts Fibonacci numbers under 1000 recursively.
