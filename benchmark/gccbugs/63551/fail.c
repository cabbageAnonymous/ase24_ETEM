union U
{
  unsigned int f0;
  int f1;
};

int a;
int d;

void
fn1 (union U p)
{
  if (p.f1 <= 0)
  {
    if (a)
    {
      d = 0; 
    }
  }
}

void
fn2 ()
{
  d = 0; 
  union U b = { 4294967286 };
  fn1 (b);
}

int
main ()
{
  fn2 ();
  return 0;
}