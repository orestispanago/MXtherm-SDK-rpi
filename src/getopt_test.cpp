#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

char *ip, *user, *pass, *outdir, *pos_x, *pos_y;

int arg_parser(int argc, char **argv)
{
  int c;
  while (1)
  {
    static struct option long_options[] =
        {
            {"ip", required_argument, 0, 'i'},
            {"user", required_argument, 0, 'u'},
            {"pass", required_argument, 0, 'p'},
            {"outdir", required_argument, 0, 'o'},
            {"pos_x", required_argument, 0, 'x'},
            {"pos_y", required_argument, 0, 'y'},
            {0, 0, 0, 0}};
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long(argc, argv, "i:u:p:o:x:y:",
                    long_options, &option_index);
    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
    {
    case 'i':
      ip = optarg;
      break;

    case 'u':
      user = optarg;
      break;

    case 'p':
      pass = optarg;
      break;

    case 'o':
      outdir = optarg;
      break;

    case 'x':
      pos_x = optarg;
      break;

    case 'y':
      pos_y = optarg;
      break;

    case '?':
      /* getopt_long already printed an error message. */
      break;

    default:
      abort();
    }
  }

  printf("ip: %s\n", ip);
  printf("user: %s\n", user);
  printf("pass: %s\n", pass);
  printf("outdir: %s\n", outdir);
  printf("pos_x: %s\n", pos_x);
  printf("pos_y: %s\n", pos_y);

  if (argv[optind] == NULL || argv[optind + 1] == NULL)
  {
    printf("Mandatory argument(s) missing, exiting...\n");
    exit(1);
  }
  
  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
  {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    putchar('\n');
  }

  return 0;
}

int main(int argc, char **argv)
{
  arg_parser(argc, argv);
  exit(0);
}
