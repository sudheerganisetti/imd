
/******************************************************************************
*
*  common routines and global variables, prototyps, makros, typedefs etc.
*  for various IMD utility programs
*
******************************************************************************/

/******************************************************************************
* $Revision$
* $Date$
******************************************************************************/

/******************************************************************************
*
*  some usful macros 
*
******************************************************************************/

/* number of slots in the histograms */
#ifndef SLOTS
#define SLOTS 1000 
#endif

/* maximal number of neighbors for CONN */
#ifndef MAXNEIGH
#define MAXNEIGH 10
#endif

/* number of slots in the histograms */
#ifdef TWOD
#define DIM 2
#else
#define DIM 3
#endif

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define SQR(a) (a)*(a)
#define TRUNC (int)

/* scalar product for vectors */
#ifdef TWOD
#define SPROD(a,b) (((a).x * (b).x) + ((a).y * (b).y))
#else
#define SPROD(a,b) (((a).x * (b).x) + ((a).y * (b).y) + ((a).z * (b).z))
#endif

/* Dynamically allocated 5D arrray -- half vector version */
#define PTR_5D_V(var,n,a,b,c,d,dim) (((var) + \
				      ((n)*(dim.i)*(dim.j)*(dim.k)*(dim.l)) + \
				      ((a)*(dim.j)*(dim.k)*(dim.l)) + \
				      ((b)*(dim.k)*(dim.l)) + \
				      ((c)*(dim.l)) + \
				      (d)))

/* Dynamically allocated 4D arrray -- half vector version */
#define PTR_4D_V(var,i,j,k,l,dim) (((var) + \
                                 ((i)*(dim.x)*(dim.y)*(dim.z)) + \
                                 ((j)*(dim.y)*(dim.z)) + \
                                 ((k)*(dim.z)) + \
                                 (l)))

/* Dynamically allocated 3D arrray -- half vector version */
#define PTR_3D_V(var,i,j,k,dim) (((var) + \
                                 ((i)*(dim.y)*(dim.z)) + \
                                 ((j)*(dim.z)) + \
                                 (k)))

/* Dynamically allocated 3D arrray -- full vector version */
#define PTR_3D_VV(var,coord,dim) (((var) + \
                                 ((coord.x)*(dim.y)*(dim.z)) + \
                                 ((coord.y)*(dim.z)) + \
                                 (coord.z)))

/* Dynamically allocated 2D arrray -- half vector version */
#define PTR_2D_V(var,i,j,dim) (((var) + \
                               ((i)*(dim.y)) + \
                                (j)))
                                

/* Dynamically allocated 2D arrray -- full vector version */
#define PTR_2D_VV(var,coord,dim) (((var) + \
                                 ((coord.x)*(dim.y)) + \
                                  (coord.y)))

#ifdef TWOD
#define PTR_VV   PTR_2D_VV
#else
#define PTR_VV   PTR_3D_VV
#endif

/* allocation increment */
#ifdef STRAIN
#define CSTEP   2
#else
#define CSTEP   10
#endif

/* Tolerance values for imd_stress */
#ifdef STRESS
#define NUM        100
#define TOL        1.0e-6
#define TOL2       1.0e-10
#define TOL_VERT2  1.0e-6
#define TOL_DIST2  2.0e-11
#endif

/*****************************************************************************
*
*  include files
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


/*****************************************************************************
*
*  typedefs
*
*****************************************************************************/

#ifdef SINGLE
typedef float real;
#else
typedef double real;
#endif

#ifdef TWOD
typedef struct {real x; real y; } vektor;
typedef struct {int  x; int  y; } ivektor;
#else
typedef struct {real x; real y; real z; } vektor;
typedef struct {int  x; int  y;  int z; } ivektor;
#endif

typedef struct {int  x; int  y; } ivektor2d;
typedef struct {int  x; int  y; int z; } ivektor3d;
typedef struct {int  i; int  x; int  y; int z; } ivektor4d;
typedef struct {int n; int  i; int  j; int  k; int l; } ivektor5d;

/* Basic Data Type - The Cell */
typedef struct { 
  vektor *ort;
#ifdef STRAIN
  vektor *dsp;
  vektor *strain;
  vektor *strain_offdia;
  short  *empty;
#elif defined(STRESS)
  vektor *stress;
  vektor *stress_offdia;
  real   *vol;
#else
  int    *sorte;
#endif
#ifdef CONN
  int    *nummer;
#endif
  int     n;
  int     n_max;
} cell;

/* String used for Filenames etc. */
typedef char str255[255];

#ifdef STRESS
#ifndef TWOD
typedef vektor vektorstr[NUM]; 
#endif
#endif

/* for parameter reading */
typedef enum ParamType {
  PARAM_STR, PARAM_STRPTR,
  PARAM_INT, PARAM_INT_COPY,
  PARAM_INTEGER, PARAM_INTEGER_COPY,
  PARAM_REAL, PARAM_REAL_COPY
} PARAMTYPE;


/*****************************************************************************
*
*  prototypes
*
*****************************************************************************/

int  main (int argc, char **argv);
void read_parameters(int argc, char **argv);
void read_atoms(str255 infilename);
void usage(void);
void getparamfile(char *infile);
int  getparam(char *param_name, void *param, PARAMTYPE ptype, 
              int pnum_min, int pnum_max);
ivektor cell_coord(vektor pos);
void init_cells(void);
void error(char *mesg);
void alloc_cell(cell *cl, int count);
void do_work(void (*do_cell_pair)(cell *p, cell *q, vektor pbc));
void do_cell_pair(cell *p, cell *q, vektor pbc);
void write_data(void);

#ifdef ANGLE
void do_angle(cell *p, cell *q, cell *qq, vektor pbc, vektor ppbc);
void calc_angles(void);
#endif

#ifdef STRAIN
void read_displacement(str255 infilename);
void calc_strain(void);
#endif

#ifdef STRESS
void read_stress(str255 infilename);
void voronoi(void);
#ifdef TWOD
void do_voronoi_2d(void);
#else
void do_voronoi_3d(void);
#endif
void sort(void);
void write_stress(int restart);
#endif

#ifdef TORSION
void do_angle(cell *p, cell *q, cell *r, cell *s, 
              vektor pbc_q, vektor pbc_r, vektor pbc_s);
void calc_angles(void);
#endif


/*****************************************************************************
*
*  global variables
*
*****************************************************************************/

cell *cell_array;    /* Array of Cells */
ivektor cell_dim;    /* Dimension of above */
int natoms;          /* Total number of atoms */
int ntypes;          /* Total number of different atom types */
str255 progname;     /* Name of current executable argv[0] */
int curline;         /* Number of current line for parameter reading */
str255 error_msg;    /* string for error message */

/* The simulation box and its inverse */
vektor box_x, tbox_x;
vektor box_y, tbox_y;
#ifndef TWOD
vektor box_z, tbox_z;
#endif
vektor height;
#ifdef TWOD
ivektor pbc_dirs = {1,1};    /* directions with pbc - default is PBC */
#else
ivektor pbc_dirs = {1,1,1};  /* directions with pbc - default is PBC */
#endif

/* Filenames */
str255 infilename;    /* Input File */
str255 outfilename;   /* Output File */
char *paramfilename;  /* parameter file */

/* Bookkeeping */
int  restart;
real r2_cut;
real r_min = 1.0;     /* default value */
#ifdef PAIR
real r_max = 5.0;     /* default value */
#else
real r_max = 1.0;     /* default value */
#endif
int  nangles = 0;
#ifdef CONN
real r_max2d[2][2] = { {1.4, 1.4}, {1.4, 0.9} };  /* should be read */
#endif
#ifdef STRAIN
real r_cell = 1.0;    /* default value */
#endif
#ifdef STRESS
int  atomcount = 0;   /* statistics */ 
int  maxneigh  = 0, sumneigh  = 0;
int  maxvert   = 0, sumvert   = 0;
int  maxedges  = 0, sumedges  = 0;
#ifndef TWOD
int  maxfaces  = 0, sumfaces  = 0;
#endif
#endif

/* The histograms */
real      *histogram;
#ifdef PAIR
ivektor3d hist_dim;
#endif
#ifdef TORSION
ivektor5d hist_dim;
#endif
#ifdef ANGLE
ivektor4d hist_dim;
#endif

#ifdef STRESS
int     neighnum;        /* Number of neighbour atoms */
vektor  *candcoord;      /* Coordinates of neighbour atoms */
real    *canddist2;      /* Distance squared of neighbour atoms */ 
#ifndef TWOD
real    volume;          /* Volume of Voronoi cell, 3d */
#else
real    area;            /* Area of Voronoi cell, 2d */
#endif
#endif

#ifdef COORD
real      *numbers;
ivektor2d num_dim;
#endif

#ifdef CONN
/* Connection matrix */
ivektor2d cm_dim = {0,MAXNEIGH};
int *cm, *nn, *tp;
vektor *pos;
#endif


/******************************************************************************
*
*  Common functions
*
******************************************************************************/

/******************************************************************************
*
*  To determine the cell into which a given particle belongs, we
*  have to transform the cartesian coordinates of the particle into
*  the coordinate system spanned by the vectors of the box edges. 
*  This yields coordinates in the interval [0..1] that are
*  multiplied by cell_dim to get the cell's index.
*
******************************************************************************/

#ifdef TWOD

/* compute transformation matrix */
void make_box(void)
{
  real volume;

  /* volume */
  volume = box_x.x * box_y.y - box_x.y * box_y.x;
  if (0==volume) error("Box Edges are parallel.");

  /* compute tbox_j such that SPROD(box_i,tbox_j) == delta_ij */
  tbox_x.x =   box_y.y / volume;
  tbox_x.y = - box_y.x / volume;
  tbox_y.x = - box_x.y / volume;
  tbox_y.y =   box_x.x / volume;

  /* squares of the box heights perpendicular to the faces */
  height.x = 1.0 / SPROD(tbox_x,tbox_x);
  height.y = 1.0 / SPROD(tbox_y,tbox_y);
}

#else

/* vector product */ 
vektor vec_prod(vektor u, vektor v)
{
  vektor w;
  w.x = u.y * v.z - u.z * v.y;
  w.y = u.z * v.x - u.x * v.z;
  w.z = u.x * v.y - u.y * v.x;
  return w;
}

/* compute transformation matrix */
void make_box(void)
{
  real volume;

  /* compute tbox_j such that SPROD(box_i,tbox_j) == delta_ij */
  /* first unnormalized */
  tbox_x = vec_prod( box_y, box_z );
  tbox_y = vec_prod( box_z, box_x );
  tbox_z = vec_prod( box_x, box_y );

  /* volume */
  volume = SPROD( box_x, tbox_x );
  if (0==volume) error("Box Edges are parallel.");

  /* normalization */
  tbox_x.x /= volume;  tbox_x.y /= volume;  tbox_x.z /= volume;
  tbox_y.x /= volume;  tbox_y.y /= volume;  tbox_y.z /= volume;
  tbox_z.x /= volume;  tbox_z.y /= volume;  tbox_z.z /= volume;

  /* squares of the box heights perpendicular to the faces */
  height.x = 1.0 / SPROD(tbox_x,tbox_x);
  height.y = 1.0 / SPROD(tbox_y,tbox_y);
  height.z = 1.0 / SPROD(tbox_z,tbox_z);
}

#endif


/******************************************************************************
*
*  Compute the size of the cells, and initialize the cell array.
*  There must be at least two cells in each direction.
*
******************************************************************************/

void init_cells(void)
{
  vektor cell_scale;
  cell   *p;
  int    nmax, i;

  make_box();

  /* scaling factors box/cell */
  cell_scale.x = sqrt( (double)(r2_cut / height.x) );
  cell_scale.y = sqrt( (double)(r2_cut / height.y) );
#ifndef TWOD
  cell_scale.z = sqrt( (double)(r2_cut / height.z) );
#endif

#ifdef TWOD
  printf("Minimal cell size: \n\t ( %f %f ) \n\t ( %f %f ) \n",
	 box_x.x * cell_scale.x, box_x.y * cell_scale.x, 
	 box_y.x * cell_scale.y, box_y.y * cell_scale.y); 
#else
  printf("Minimal cell size: \n");
  printf("\t ( %f %f %f ) \n\t ( %f %f %f ) \n\t ( %f %f %f )\n",
     box_x.x * cell_scale.x, box_x.y * cell_scale.x, box_x.z * cell_scale.x,
     box_y.x * cell_scale.y, box_y.y * cell_scale.y, box_y.z * cell_scale.y,
     box_z.x * cell_scale.z, box_z.y * cell_scale.z, box_z.z * cell_scale.z);
#endif

  /* set up cell array */
  cell_dim.x = (int) ( 1.0 / cell_scale.x );
  cell_dim.y = (int) ( 1.0 / cell_scale.y );
#ifndef TWOD
  cell_dim.z = (int) ( 1.0 / cell_scale.z );
#endif
  
  /* If an integer number of cells does not fit exactly into the box, the
     cells are enlarged accordingly */
  cell_scale.x = 1.0 / cell_dim.x;
  cell_scale.y = 1.0 / cell_dim.y;
#ifndef TWOD
  cell_scale.z = 1.0 / cell_dim.z;
#endif

#ifdef TWOD
  printf("Actual cell size: \n\t ( %f %f ) \n\t ( %f %f ) \n",
	 box_x.x * cell_scale.x, box_x.y * cell_scale.x, 
	 box_y.x * cell_scale.y, box_y.y * cell_scale.y); 
  printf("Cell array dimensions: %d %d\n", cell_dim.x,cell_dim.y);
  cell_array = (cell *) malloc(cell_dim.x * cell_dim.y * sizeof(cell));
#else
  printf("Actual cell size: \n");
  printf("\t ( %f %f %f ) \n\t ( %f %f %f ) \n\t ( %f %f %f )\n",
     box_x.x * cell_scale.x, box_x.y * cell_scale.x, box_x.z * cell_scale.x,
     box_y.x * cell_scale.y, box_y.y * cell_scale.y, box_y.z * cell_scale.y,
     box_z.x * cell_scale.z, box_z.y * cell_scale.z, box_z.z * cell_scale.z);
  printf("Cell array dimensions: %d %d %d\n",cell_dim.x,cell_dim.y,cell_dim.z);
  cell_array = (cell *) malloc(cell_dim.x*cell_dim.y*cell_dim.z*sizeof(cell));
#endif
  if (NULL == cell_array) error("Cannot allocate memory for cells");

  nmax = cell_dim.x * cell_dim.y;
#ifndef TWOD
  nmax = nmax * cell_dim.z;
#endif

  /* initialize cells */
  for (i=0; i<nmax; ++i) {
    p = cell_array + i;
    p->n_max = 0;
    alloc_cell(p, CSTEP);
  }

}


/*****************************************************************************
*
*  cell_coord gives the (integral) cell_coorinates of a position
*
*****************************************************************************/

ivektor cell_coord(vektor ort)
{
  ivektor coord;

  /* Map positions to boxes */
  coord.x = (int) TRUNC( cell_dim.x * SPROD(ort,tbox_x) );
  coord.y = (int) TRUNC( cell_dim.y * SPROD(ort,tbox_y) );
#ifndef TWOD
  coord.z = (int) TRUNC( cell_dim.z * SPROD(ort,tbox_z) );
#endif

  /* Roundoff errors put atoms slightly out of the simulation cell */
  coord.x = coord.x <   0         ?             0 : coord.x;
  coord.x = coord.x >= cell_dim.x ? cell_dim.x -1 : coord.x;
  coord.y = coord.y <   0         ?             0 : coord.y;
  coord.y = coord.y >= cell_dim.y ? cell_dim.y -1 : coord.y;
#ifndef TWOD
  coord.z = coord.z <   0         ?             0 : coord.z;
  coord.z = coord.z >= cell_dim.z ? cell_dim.z -1 : coord.z;
#endif
  return coord;
}


/******************************************************************************
*
*  allocate memory for a cell
*
******************************************************************************/

void alloc_cell(cell *cl, int count)
{
  /* initialize if it is the first call */
  if (cl->n_max == 0) {
    cl->ort           = NULL;
#ifdef STRAIN
    cl->dsp           = NULL;
    cl->strain        = NULL;
    cl->strain_offdia = NULL;
    cl->empty         = NULL;
#elif defined(STRESS)
    cl->stress        = NULL;
    cl->stress_offdia = NULL;
    cl->vol           = NULL;
#else
    cl->sorte         = NULL;
#endif
#ifdef CONN
    cl->nummer        = NULL;
#endif
    cl->n             = 0;
  }

  /* (re)allocate */
  cl->ort    = (vektor *) realloc(cl->ort,    count * sizeof(vektor));
#ifdef STRAIN
  cl->dsp    = (vektor *) realloc(cl->dsp,    count * sizeof(vektor));
  cl->strain = (vektor *) realloc(cl->strain, count * sizeof(vektor));
  cl->strain_offdia = (vektor *) realloc(cl->strain_offdia,
                                              count * sizeof(vektor));
  cl->empty  = (short  *) realloc(cl->empty,  count * sizeof(short));
#elif defined(STRESS)
  cl->stress = (vektor *) realloc(cl->stress, count * sizeof(vektor));
  cl->stress_offdia = (vektor *) realloc(cl->stress_offdia,
                                              count * sizeof(vektor));
  cl->vol    = (real   *) realloc(cl->vol,  count * sizeof(real));
#else
  cl->sorte  = (int    *) realloc(cl->sorte,  count * sizeof(int));
#endif
#ifdef CONN
  cl->nummer = (int    *) realloc(cl->nummer, count * sizeof(int));
#endif

  /* check if it worked */
  if ( (NULL==cl->ort)
#ifdef STRAIN
    || (NULL==cl->dsp)
    || (NULL==cl->strain)
    || (NULL==cl->strain_offdia)
    || (NULL==cl->empty)
#elif defined(STRESS)
    || (NULL==cl->stress)
    || (NULL==cl->stress_offdia)
    || (NULL==cl->vol)
#else   
    || (NULL==cl->sorte)
#endif
#ifdef CONN
    || (NULL==cl->nummer)
#endif
  ) error("Cannot allocate memory for cell.");
  cl->n_max  = count;
}


/******************************************************************************
*
*   read_parameters reads the command line parameters, 
*   and the parameter file given on the command line.
*
******************************************************************************/

void read_parameters(int argc,char **argv)
{
  str255 tmpname;
  str255 fname;
  str255 parfilename;
  FILE *infile;
  FILE *tmpfile;

/* Check for Restart, process options */

  strcpy(progname,argv[0]);
  while ((argc > 1) && (argv[1][0] =='-')) {
    switch (argv[1][1]) {
      /* r - restart */
    case 'r':
      restart = atoi(&argv[1][2]);
      break;
#ifdef PAIR
      /* a - minimum radius */
    case 'a':
      r_min = atof(&argv[1][2]);
      break;
#endif
#ifndef CONN
      /* e - maximum radius */
    case 'e':
      r_max = atof(&argv[1][2]);
#ifdef STRAIN
      r_cell = r_max;
#endif
      break;
#endif
#ifdef STRAIN
      /* c - cell width */
    case 'c':
      r_cell = atof(&argv[1][2]);
      break;
#endif
    case 'p':
      if (argv[1][2]=='\0') {
        if (NULL != argv[2]) {
          paramfilename = strdup(argv[2]);
          --argc;
          ++argv;
        }
      }
      else paramfilename = strdup(&argv[1][2]);
      break;
    default:
      printf("Illegal option %s \n",argv[1]);
      usage();
      exit(-1);
    }
    ++argv;
    --argc;
  }

  getparamfile(paramfilename);

  /* Get restart parameters if restart */
  if (0 != restart) {
    sprintf(fname,"%s.%d.itr",outfilename,restart);
    getparamfile(fname);
  }

#ifdef STRAIN
  sprintf(infilename, "%s.dsp", outfilename);

  if (0!=restart) sprintf(infilename,"%s.%u.dsp",outfilename,restart);
#elif defined(STRESS)
  sprintf(infilename, "%s.press",outfilename);

  if (0!=restart) sprintf(infilename,"%s.%u.press",outfilename,restart);
#else
  if (0!=restart) sprintf(infilename,"%s.%u",outfilename,restart);
#endif

#ifdef STRAIN
  /* r_cell >= r_max is required */
  if (r_cell < r_max) error("Cell smaller than cutoff radius!");
  r2_cut = SQR(r_cell);
#endif
#ifdef STRESS
  r2_cut = SQR(r_max);  
#endif

} 


/*****************************************************************************
*
*  read one parameter (routine from imd_param.c)
*
*****************************************************************************/

int getparam(char *param_name, void *param, PARAMTYPE ptype, 
             int pnum_min, int pnum_max)
{
  static char errmsg[256];
  char *str;
  int i;
  int numread;

  numread = 0;
  if (ptype == PARAM_STR) {
    str = strtok(NULL," \t\n");
    if (str == NULL) {
      fprintf(stderr,"parameter for %s missing in line %u\n",
              param_name,curline);
      error("string expected");
    }
    else strncpy((char *)param,str,pnum_max);
    numread++;
  }
  else if (ptype == PARAM_STRPTR) {
    str = strtok(NULL," \t\n");
    if (str == NULL) {
      fprintf(stderr,"parameter for %s missing in line %u\n",
              param_name,curline);
      error("string expected");
    }
    else *((char**)param) = strdup(str);
    numread++;
  }
  else if (ptype == PARAM_INT) {
    for (i=0; i<pnum_min; i++) {
      str = strtok(NULL," \t\n");
      if (str == NULL) {
        fprintf(stderr,"parameter for %s missing in line %u\n",
                param_name,curline);
        sprintf(errmsg,"integer vector of dim %u expected",
                (unsigned)pnum_min);
        error(errmsg);
      }
      else ((int*)param)[i] = atoi(str);
      numread++;
    }
    for (i=pnum_min; i<pnum_max; i++) {
      if ((str = strtok(NULL," \t\n")) != NULL) {
        ((int*)param)[i] = atoi(str);
        numread++;
      }
      else break;
    }
  }
  else if (ptype == PARAM_INT_COPY) {
    int ival = 0;
    for (i=0; i<pnum_max; i++) {
      str = strtok(NULL," \t\n");
      if (str != NULL) {
        ival = atoi(str);
        numread++; /* return number of parameters actually read */
      }
      else if (i<pnum_min) {
        fprintf(stderr,"parameter for %s missing in line %u\n",
                param_name,curline);
        sprintf(errmsg,"integer vector of dim %u expected",
                (unsigned)pnum_min);
        error(errmsg);
      }
      ((int*)param)[i] = ival;
    }
  }
  else if (ptype == PARAM_INTEGER) {
    for (i=0; i<pnum_min; i++) {
      str = strtok(NULL," \t\n");
      if (str == NULL) {
        fprintf(stderr,"parameter for %s missing in line %u\n",
                param_name,curline);
        sprintf(errmsg,"integer vector of dim %u expected",
                (unsigned)pnum_min);
        error(errmsg);
      }
      else ((int *)param)[i] = atoi(str);
      numread++;
    }
    for (i=pnum_min; i<pnum_max; i++) {
      if ((str = strtok(NULL," \t\n")) != NULL) {
        ((int *)param)[i] = atoi(str);
        numread++;
      }
      else break;
    }
  }
  else if (ptype == PARAM_INTEGER_COPY) {
    int ival = 0;
    for (i=0; i<pnum_max; i++) {
      str = strtok(NULL," \t\n");
      if (str != NULL) {
        ival = atoi(str);
        numread++; /* return number of parameters actually read */
      }
      else if (i<pnum_min) {
        fprintf(stderr,"parameter for %s missing in line %u\n",
                param_name,curline);
        sprintf(errmsg,"integer vector of dim %u expected",
                (unsigned)pnum_min);
        error(errmsg);
      }
      ((int *)param)[i] = (int)ival;
    }
  }
  else if (ptype == PARAM_REAL) {
    for (i=0; i<pnum_min; i++) {
      str = strtok(NULL," \t\n");
      if (str == NULL) {
        fprintf(stderr,"parameter for %s missing in line %u\n",
                param_name,curline);
        sprintf(errmsg,"real vector of dim %u expected",
                (unsigned)pnum_min);
        error(errmsg);
      }
      else ((real*)param)[i] = atof(str);
      numread++;
    }
    for (i=pnum_min; i<pnum_max; i++) {
      if ((str = strtok(NULL," \t\n")) != NULL) {
        ((real*)param)[i] = atof(str);
        numread++;
      }
      else break;
    }
  }
  else if (ptype == PARAM_REAL_COPY) {
    real rval = 0;
    for (i=0; i<pnum_max; i++) {
      str = strtok(NULL," \t\n");
      if (str != NULL) {
        rval = atof(str);
        numread++; /* return number of parameters actually read */
      }
      else if (i<pnum_min) {
        fprintf(stderr,"parameter for %s missing in line %u\n",
                param_name,curline);
        sprintf(errmsg,"real vector of dim %u expected",
                (unsigned)pnum_min);
        error(errmsg);
      }
      ((real*)param)[i] = rval;
    }
  }
  return numread;
} /* getparam */


/*****************************************************************************
*
*  read tag-based parameter file
*  lines beginning with comment characters '#' or blank lines are skipped
*
*****************************************************************************/

void getparamfile(char *paramfname)
{
  FILE *pf;
  char buffer[1024];
  char *token;
  char *res;

  curline = 0;
  pf = fopen(paramfname,"r");
  if (NULL == pf) {
    sprintf(error_msg,"Cannot open parameter file %s",paramfname);
    error(error_msg);
  }

  do {
    res=fgets(buffer,1024,pf);
    if (NULL == res) break; /* probably EOF reached */
    curline++;
    token = strtok(buffer," \t\n");
    if (NULL == token) continue; /* skip blank lines */
    if (token[0]=='#') continue; /* skip comments */

    if (strcasecmp(token,"coordname")==0) {
      /* file name for atom coordinate input data */
      getparam("coordname",infilename,PARAM_STR,1,255);
    }
    else if (strcasecmp(token,"outfiles")==0) {
      /* output file basename */
      getparam("outfiles",outfilename,PARAM_STR,1,255);
    }
    else if (strcasecmp(token,"box_x")==0) {
      /* 'x' or first vector for box */
      getparam("box_x",&box_x,PARAM_REAL,DIM,DIM);
    }
    else if (strcasecmp(token,"box_y")==0) {
      /* 'y' or second vector for box */
      getparam("box_y",&box_y,PARAM_REAL,DIM,DIM);
    }
#ifndef TWOD
    else if (strcasecmp(token,"box_z")==0) {
      /* 'z' or third vector for box */
      getparam("box_z",&box_z,PARAM_REAL,DIM,DIM);
    }
#endif
    else if (strcasecmp(token,"pbc_dirs")==0) {
      /* directions with periodic boundary conditions */
      getparam("pbc_dirs",&pbc_dirs,PARAM_INT,DIM,DIM);
    }
    else if (strcasecmp(token,"ntypes")==0) {
      /* number of atom types */
      getparam("ntypes",&ntypes,PARAM_INT,1,1);
    }
  } while (!feof(pf));
  fclose(pf);

#ifdef STRAIN
  if ( box_x.x < r_cell || box_y.y < r_cell 
#ifndef TWOD
       || box_z.z < r_cell 
#endif
       )
    error("Cell size greater than box!");
#endif

} /* getparamfile */


/******************************************************************************
*
*  error -- Complain and abort
*
******************************************************************************/

void error(char *mesg)
{
  printf("Error: %s\n",mesg);
  exit(2);
}


/******************************************************************************
*
*  read_atoms - reads atoms and velocities into the cell-array
*
*  The file format is flat ascii, one atom per line, lines beginning
*  with '#' denote comments. Each line consists of
*
*  number type mass x y [z] [rest]
*
*  where
*
*  number   is an arbitrary integer number assigned to each atom
*  type     is the atom's index to the potenital table
*  mass     is the mass of the atom
*  x,y,z    are the atom's coordinates
*  rest     is ignored until end of line
*
******************************************************************************/

void read_atoms(str255 infilename)
{
  FILE *infile;
  char buf[512];
  int p, s, n;
  vektor pos;
  real m;
  cell *to;
  ivektor cellc;

  /* we first try the old checkpoint name, then the new */
  infile = fopen(infilename,"r");
  if ((NULL==infile) && (restart!=0)) {
    infilename = strcat(infilename,".chkpt");
    infile = fopen(infilename,"r");
  }
  if (NULL==infile) {
    sprintf(error_msg,"Cannot open atoms file %s",infilename);
    error(error_msg);
  }

  natoms=0;
  
  /* Read the input file line by line */
  while (!feof(infile)) {

    buf[0] = (char) NULL;
    fgets(buf,sizeof(buf),infile);
    while ('#'==buf[1]) fgets(buf,sizeof(buf),infile); /* eat comments */

#ifdef SINGLE
#ifdef TWOD
    p = sscanf(buf,"%d %d %f %f %f",&n,&s,&m,&pos.x,&pos.y);
#else
    p = sscanf(buf,"%d %d %f %f %f %f",&n,&s,&m,&pos.x,&pos.y,&pos.z);
#endif
#else
#ifdef TWOD
    p = sscanf(buf,"%d %d %lf %lf %lf",&n,&s,&m,&pos.x,&pos.y);
#else
    p = sscanf(buf,"%d %d %lf %lf %lf %lf",&n,&s,&m,&pos.x,&pos.y,&pos.z);
#endif
#endif

    if (p>0) {
      /* compute target cell */
      cellc = cell_coord(pos);
      to = PTR_VV(cell_array,cellc,cell_dim);
      /* enlarge it if necessary */
      if (to->n >= to->n_max) alloc_cell(to,to->n_max+CSTEP);
      /* put the data */
      to->ort   [to->n] = pos;
#if (!defined(STRAIN) && !defined(STRESS))
      to->sorte [to->n] = s;
#endif
#ifdef CONN
      to->nummer[to->n] = natoms;
#endif
      to->n++;
      natoms++;
    }
  }
  fclose(infile);  
}


/******************************************************************************
*
*  loop over all cell pairs
*
******************************************************************************/

void do_work(void (*do_cell_pair)(cell *p, cell *q, vektor pbc))
{
  cell *p,*q;
  int i,j,k;
  int l,m,n;
  int r,s,t;
  vektor pbc;

  /* for each cell */
  for (i=0; i < cell_dim.x; ++i)
    for (j=0; j < cell_dim.y; ++j)
#ifndef TWOD
      for (k=0; k < cell_dim.z; ++k)
#endif
      {
#ifdef TWOD
        p = PTR_2D_V(cell_array,i,j  ,cell_dim);
#else
        p = PTR_3D_V(cell_array,i,j,k,cell_dim);
#endif
        /* For half of the neighbours of this cell */
        for (l=0; l <= 1; ++l)
          for (m=-l; m <= 1; ++m)
#ifndef TWOD
            for (n=(l==0 ? -m  : -l ); n <= 1; ++n)
#endif
            {
              /* Calculate Indicies of Neighbour */
              r = i+l;  pbc.x = 0;
              s = j+m;  pbc.y = 0;
#ifndef TWOD
              t = k+n;  pbc.z = 0;
#endif

              /* deal with periodic boundary conditions if necessary */
              if (r<0) {
                if (pbc_dirs.x==1) {
                  r = cell_dim.x-1; 
                  pbc.x -= box_x.x;      
                  pbc.y -= box_x.y;
#ifndef TWOD
                  pbc.z -= box_x.z;
#endif
                } else continue;
              }
              if (s<0) {
                if (pbc_dirs.y==1) {
                  s = cell_dim.y-1;
                  pbc.x -= box_y.x;      
                  pbc.y -= box_y.y;
#ifndef TWOD
                  pbc.z -= box_y.z;
#endif
                } else continue;
              }
#ifndef TWOD
              if (t<0) {
                if (pbc_dirs.z==1) {
                  t = cell_dim.z-1;
                  pbc.x -= box_z.x;      
                  pbc.y -= box_z.y;
                  pbc.z -= box_z.z;
                } else continue;
              }
#endif
              if (r>cell_dim.x-1) {
                if (pbc_dirs.x==1) {
                  r = 0; 
                  pbc.x += box_x.x;      
                  pbc.y += box_x.y;
#ifndef TWOD
                  pbc.z += box_x.z;
#endif
                } else continue;
              }
              if (s>cell_dim.y-1) {
                if (pbc_dirs.y==1) {
                  s = 0; 
                  pbc.x += box_y.x;      
                  pbc.y += box_y.y;
#ifndef TWOD
                  pbc.z += box_y.z;
#endif
                } else continue;
              }
#ifndef TWOD
              if (t>cell_dim.z-1) {
                if (pbc_dirs.z==1) {
                  t = 0; 
                  pbc.x += box_z.x;      
                  pbc.y += box_z.y;
                  pbc.z += box_z.z;
                } else continue;
              }
#endif

	      /* Neighbour cell (note that p==q ist possible) */
#ifdef TWOD
              q = PTR_2D_V(cell_array,r,s,cell_dim);
#else
              q = PTR_3D_V(cell_array,r,s,t,cell_dim);
#endif
              /* Do the work */
              do_cell_pair(p,q,pbc);
            }
      }
}

