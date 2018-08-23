// /Users/alexfinck/Documents/CMF/WorkspaceUK/twenty-fifty-alf/twenty-fifty/model/model.xlsm approximately translated into C
// definitions
#define NUMBER_OF_REFS 3430
#define EXCEL_FILENAME  "/Users/alexfinck/Documents/CMF/WorkspaceUK/twenty-fifty-alf/twenty-fifty/model/model.xlsm"
// end of definitions

// First we have c versions of all the excel functions that we know
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

#ifndef NUMBER_OF_REFS
  #define NUMBER_OF_REFS 0
#endif

#ifndef EXCEL_FILENAME
  #define EXCEL_FILENAME "NoExcelFilename"
#endif

// Need to retain malloc'd values for a while, so can return to functions that use this library
// So to avoid a memory leak we keep an array of all the values we have malloc'd, which we then
// free when the reset() function is called.
#define MEMORY_TO_BE_FREED_LATER_HEAP_INCREMENT 1000

#define true 1
#define false 0

// These are the various types of excel cell, plus ExcelRange which allows the passing of arrays of cells
typedef enum {ExcelEmpty, ExcelNumber, ExcelString, ExcelBoolean, ExcelError, ExcelRange} ExcelType;

struct excel_value {
	ExcelType type;

	double number; // Used for numbers and for error types
	char *string; // Used for strings

	// The following three are used for ranges
	void *array;
	int rows;
	int columns;
};

typedef struct excel_value ExcelValue;


// These are used in the SUMIF and SUMIFS criteria (e.g., when passed a string like "<20")
typedef enum {LessThan, LessThanOrEqual, Equal, NotEqual, MoreThanOrEqual, MoreThan} ExcelComparisonType;

struct excel_comparison {
	ExcelComparisonType type;
	ExcelValue comparator;
};

typedef struct excel_comparison ExcelComparison;

// Headers
static ExcelValue more_than(ExcelValue a_v, ExcelValue b_v);
static ExcelValue more_than_or_equal(ExcelValue a_v, ExcelValue b_v);
static ExcelValue not_equal(ExcelValue a_v, ExcelValue b_v);
static ExcelValue less_than(ExcelValue a_v, ExcelValue b_v);
static ExcelValue less_than_or_equal(ExcelValue a_v, ExcelValue b_v);
static ExcelValue average(int array_size, ExcelValue *array);
static ExcelValue averageifs(ExcelValue average_range_v, int number_of_arguments, ExcelValue *arguments);
static ExcelValue excel_char(ExcelValue number_v);
static ExcelValue excel_ceiling_math_2(ExcelValue number_v, ExcelValue multiple_v);
static ExcelValue excel_ceiling_math(ExcelValue number_v, ExcelValue multiple_v, ExcelValue mode_v);
static ExcelValue ensure_is_number(ExcelValue maybe_number_v);
static ExcelValue find_2(ExcelValue string_to_look_for_v, ExcelValue string_to_look_in_v);
static ExcelValue find(ExcelValue string_to_look_for_v, ExcelValue string_to_look_in_v, ExcelValue position_to_start_at_v);
static ExcelValue hlookup_3(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue row_number_v);
static ExcelValue hlookup(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue row_number_v, ExcelValue match_type_v);
static ExcelValue iferror(ExcelValue value, ExcelValue value_if_error);
static ExcelValue iserr(ExcelValue value);
static ExcelValue iserror(ExcelValue value);
static ExcelValue excel_index(ExcelValue array_v, ExcelValue row_number_v, ExcelValue column_number_v);
static ExcelValue excel_index_2(ExcelValue array_v, ExcelValue row_number_v);
static ExcelValue excel_isnumber(ExcelValue number);
static ExcelValue excel_isblank(ExcelValue value);
static ExcelValue forecast(ExcelValue required_x, ExcelValue known_y, ExcelValue known_x);
static ExcelValue large(ExcelValue array_v, ExcelValue k_v);
static ExcelValue left(ExcelValue string_v, ExcelValue number_of_characters_v);
static ExcelValue left_1(ExcelValue string_v);
static ExcelValue len(ExcelValue string_v);
static ExcelValue excel_log(ExcelValue number);
static ExcelValue excel_log_2(ExcelValue number, ExcelValue base);
static ExcelValue ln(ExcelValue number);
static ExcelValue excel_exp(ExcelValue number);
static ExcelValue max(int number_of_arguments, ExcelValue *arguments);
static ExcelValue min(int number_of_arguments, ExcelValue *arguments);
static ExcelValue mmult(ExcelValue a_v, ExcelValue b_v);
static ExcelValue mod(ExcelValue a_v, ExcelValue b_v);
static ExcelValue na();
static ExcelValue negative(ExcelValue a_v);
static ExcelValue excel_not(ExcelValue a_v);
static ExcelValue number_or_zero(ExcelValue maybe_number_v);
static ExcelValue npv(ExcelValue rate, int number_of_arguments, ExcelValue *arguments);
static ExcelValue pmt(ExcelValue rate_v, ExcelValue number_of_periods_v, ExcelValue present_value_v);
static ExcelValue pmt_4(ExcelValue rate_v, ExcelValue number_of_periods_v, ExcelValue present_value_v, ExcelValue final_value_v);
static ExcelValue pmt_5(ExcelValue rate_v, ExcelValue number_of_periods_v, ExcelValue present_value_v, ExcelValue final_value_v, ExcelValue type_v);
static ExcelValue power(ExcelValue a_v, ExcelValue b_v);
static ExcelValue pv_3(ExcelValue a_v, ExcelValue b_v, ExcelValue c_v);
static ExcelValue pv_4(ExcelValue a_v, ExcelValue b_v, ExcelValue c_v, ExcelValue d_v);
static ExcelValue pv_5(ExcelValue a_v, ExcelValue b_v, ExcelValue c_v, ExcelValue d_v, ExcelValue e_v);
static ExcelValue excel_round(ExcelValue number_v, ExcelValue decimal_places_v);
static ExcelValue rank(ExcelValue number_v, ExcelValue range_v, ExcelValue order_v);
static ExcelValue rank_2(ExcelValue number_v, ExcelValue range_v);
static ExcelValue right(ExcelValue string_v, ExcelValue number_of_characters_v);
static ExcelValue right_1(ExcelValue string_v);
static ExcelValue rounddown(ExcelValue number_v, ExcelValue decimal_places_v);
static ExcelValue roundup(ExcelValue number_v, ExcelValue decimal_places_v);
static ExcelValue excel_int(ExcelValue number_v);
static ExcelValue string_join(int number_of_arguments, ExcelValue *arguments);
static ExcelValue subtotal(ExcelValue type, int number_of_arguments, ExcelValue *arguments);
static ExcelValue sumifs(ExcelValue sum_range_v, int number_of_arguments, ExcelValue *arguments);
static ExcelValue sumif(ExcelValue check_range_v, ExcelValue criteria_v, ExcelValue sum_range_v );
static ExcelValue sumif_2(ExcelValue check_range_v, ExcelValue criteria_v);
static ExcelValue sumproduct(int number_of_arguments, ExcelValue *arguments);
static ExcelValue text(ExcelValue number_v, ExcelValue format_v);
static ExcelValue value(ExcelValue string_v);
static ExcelValue vlookup_3(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue column_number_v);
static ExcelValue vlookup(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue column_number_v, ExcelValue match_type_v);
static ExcelValue scurve_4(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration);
static ExcelValue scurve(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration, ExcelValue startYear);
static ExcelValue halfscurve_4(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration);
static ExcelValue halfscurve(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration, ExcelValue startYear);
static ExcelValue lcurve_4(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration);
static ExcelValue lcurve(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration, ExcelValue startYear);
static ExcelValue curve_5(ExcelValue curveType, ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration);
static ExcelValue curve(ExcelValue curveType, ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration, ExcelValue startYear);

static ExcelValue product(int number_of_arguments, ExcelValue *arguments);
static ExcelValue excel_floor(ExcelValue number_v, ExcelValue multiple_v);
static ExcelValue rate(ExcelValue a1, ExcelValue a2, ExcelValue a3, ExcelValue a4);
static ExcelValue excel_sqrt(ExcelValue number_v);

// My little heap for keeping pointers to memory that I need to reclaim
void **memory_that_needs_to_be_freed;
int memory_that_needs_to_be_freed_counter = 0;
int memory_that_needs_to_be_freed_size = -1;

static void free_later(void *pointer) {
	if(memory_that_needs_to_be_freed_counter >= memory_that_needs_to_be_freed_size) {
    if(memory_that_needs_to_be_freed_size <= 0) {
      memory_that_needs_to_be_freed = malloc(MEMORY_TO_BE_FREED_LATER_HEAP_INCREMENT*sizeof(void*));
      memory_that_needs_to_be_freed_size = MEMORY_TO_BE_FREED_LATER_HEAP_INCREMENT;
    } else {
      memory_that_needs_to_be_freed_size += MEMORY_TO_BE_FREED_LATER_HEAP_INCREMENT;
      memory_that_needs_to_be_freed = realloc(memory_that_needs_to_be_freed, memory_that_needs_to_be_freed_size * sizeof(void*));
      if(!memory_that_needs_to_be_freed) {
        printf("Could not allocate new memory to memory that needs to be freed array. halting.");
        exit(-1);
      }
    }
  }
	memory_that_needs_to_be_freed[memory_that_needs_to_be_freed_counter] = pointer;
	memory_that_needs_to_be_freed_counter++;
}

static void free_all_allocated_memory() {
	int i;
	for(i = 0; i < memory_that_needs_to_be_freed_counter; i++) {
		free(memory_that_needs_to_be_freed[i]);
	}
	memory_that_needs_to_be_freed_counter = 0;
}

static int variable_set[NUMBER_OF_REFS];

// Resets all cached and malloc'd values
void reset() {
  free_all_allocated_memory();
  memset(variable_set, 0, sizeof(variable_set));
}

// Handy macros

#define EXCEL_NUMBER(numberdouble) ((ExcelValue) {.type = ExcelNumber, .number = numberdouble})
#define EXCEL_STRING(stringchar) ((ExcelValue) {.type = ExcelString, .string = stringchar})
#define EXCEL_RANGE(arrayofvalues, rangerows, rangecolumns) ((ExcelValue) {.type = ExcelRange, .array = arrayofvalues, .rows = rangerows, .columns = rangecolumns})

static void * new_excel_value_array(int size) {
	ExcelValue *pointer = malloc(sizeof(ExcelValue)*size); // Freed later
	if(pointer == 0) {
		printf("Out of memory in new_excel_value_array\n");
		exit(-1);
	}
	free_later(pointer);
	return pointer;
};

// Constants
static ExcelValue ORIGINAL_EXCEL_FILENAME = {.type = ExcelString, .string = EXCEL_FILENAME };

const ExcelValue BLANK = {.type = ExcelEmpty, .number = 0};

const ExcelValue ZERO = {.type = ExcelNumber, .number = 0};
const ExcelValue ONE = {.type = ExcelNumber, .number = 1};
const ExcelValue TWO = {.type = ExcelNumber, .number = 2};
const ExcelValue THREE = {.type = ExcelNumber, .number = 3};
const ExcelValue FOUR = {.type = ExcelNumber, .number = 4};
const ExcelValue FIVE = {.type = ExcelNumber, .number = 5};
const ExcelValue SIX = {.type = ExcelNumber, .number = 6};
const ExcelValue SEVEN = {.type = ExcelNumber, .number = 7};
const ExcelValue EIGHT = {.type = ExcelNumber, .number = 8};
const ExcelValue NINE = {.type = ExcelNumber, .number = 9};
const ExcelValue TEN = {.type = ExcelNumber, .number = 10};

// Booleans
const ExcelValue TRUE = {.type = ExcelBoolean, .number = true };
const ExcelValue FALSE = {.type = ExcelBoolean, .number = false };

// Errors
const ExcelValue VALUE = {.type = ExcelError, .number = 0};
const ExcelValue NAME = {.type = ExcelError, .number = 1};
const ExcelValue DIV0 = {.type = ExcelError, .number = 2};
const ExcelValue REF = {.type = ExcelError, .number = 3};
const ExcelValue NA = {.type = ExcelError, .number = 4};
const ExcelValue NUM = {.type = ExcelError, .number = 5};

// This is the error flag
static int conversion_error = 0;

// Helpful for debugging
static void inspect_excel_value(ExcelValue v) {
	ExcelValue *array;
	int i, j, k;
	switch (v.type) {
  	  case ExcelNumber:
		  printf("Number: %f\n",v.number);
		  break;
	  case ExcelBoolean:
		  if(v.number == true) {
			  printf("True\n");
		  } else if(v.number == false) {
			  printf("False\n");
		  } else {
			  printf("Boolean with undefined state %f\n",v.number);
		  }
		  break;
	  case ExcelEmpty:
	  	if(v.number == 0) {
	  		printf("Empty\n");
		} else {
			printf("Empty with unexpected state %f\n",v.number);
		}
		break;
	  case ExcelRange:
		 printf("Range rows: %d, columns: %d\n",v.rows,v.columns);
		 array = v.array;
		 for(i = 0; i < v.rows; i++) {
			 printf("Row %d:\n",i+1);
			 for(j = 0; j < v.columns; j++ ) {
				 printf("%d ",j+1);
				 k = (i * v.columns) + j;
				 inspect_excel_value(array[k]);
			 }
		 }
		 break;
	  case ExcelString:
		 printf("String: '%s'\n",v.string);
		 break;
	  case ExcelError:
		 printf("Error number %f ",v.number);
		 switch( (int)v.number) {
			 case 0: printf("VALUE\n"); break;
			 case 1: printf("NAME\n"); break;
			 case 2: printf("DIV0\n"); break;
			 case 3: printf("REF\n"); break;
			 case 4: printf("NA\n"); break;
			 case 5: printf("NUM\n"); break;
		 }
		 break;
    default:
      printf("Type %d not recognised",v.type);
	 };
}

// Extracts numbers from ExcelValues
// Excel treats empty cells as zero
static double number_from(ExcelValue v) {
	char *s;
	char *p;
	double n;
	ExcelValue *array;
	switch (v.type) {
  	  case ExcelNumber:
	  case ExcelBoolean:
	  	return v.number;
	  case ExcelEmpty:
	  	return 0;
	  case ExcelRange:
		 array = v.array;
	     return number_from(array[0]);
	  case ExcelString:
 	 	s = v.string;
		if (s == NULL || *s == '\0' || isspace(*s)) {
			return 0;
		}
		n = strtod (s, &p);
		if(*p == '\0') {
			return n;
		}
		conversion_error = 1;
		return 0;
	  case ExcelError:
	  	return 0;
  }
  return 0;
}

#define NUMBER(value_name, name) double name; if(value_name.type == ExcelError) { return value_name; }; name = number_from(value_name);
#define CHECK_FOR_CONVERSION_ERROR 	if(conversion_error) { conversion_error = 0; return VALUE; };
#define CHECK_FOR_PASSED_ERROR(name) 	if(name.type == ExcelError) return name;

static ExcelValue excel_abs(ExcelValue a_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	NUMBER(a_v, a)
	CHECK_FOR_CONVERSION_ERROR

	if(a >= 0.0 ) {
		return a_v;
	} else {
		return (ExcelValue) {.type = ExcelNumber, .number = -a};
	}
}

static ExcelValue excel_char(ExcelValue a_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	NUMBER(a_v, a)
	CHECK_FOR_CONVERSION_ERROR
  if(a <= 0) { return VALUE; }
  if(a >= 256) { return VALUE; }
  a = floor(a);
	char *string = malloc(1); // Freed later
	if(string == 0) {
	  printf("Out of memory in char");
	  exit(-1);
	}
  string[0] = a;
  free_later(string);
  return EXCEL_STRING(string);
}

static ExcelValue add(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)
	NUMBER(a_v, a)
	NUMBER(b_v, b)
	CHECK_FOR_CONVERSION_ERROR
	return EXCEL_NUMBER(a + b);
}

static ExcelValue ensure_is_number(ExcelValue maybe_number_v) {
  if(maybe_number_v.type == ExcelNumber) {
    return maybe_number_v;
  }
  if(maybe_number_v.type == ExcelError) {
    return maybe_number_v;
  }
  NUMBER(maybe_number_v, maybe_number)
	CHECK_FOR_CONVERSION_ERROR
	return EXCEL_NUMBER(maybe_number);
}

static ExcelValue number_or_zero(ExcelValue maybe_number_v) {
  if(maybe_number_v.type == ExcelNumber) {
    return maybe_number_v;
  }
  if(maybe_number_v.type == ExcelError) {
    return maybe_number_v;
  }
  return ZERO;
}

static ExcelValue excel_log(ExcelValue number) {
  return excel_log_2(number, TEN);
}

static ExcelValue excel_log_2(ExcelValue number_v, ExcelValue base_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	CHECK_FOR_PASSED_ERROR(base_v)
	NUMBER(number_v, n)
	NUMBER(base_v, b)
	CHECK_FOR_CONVERSION_ERROR

  if(n<=0) { return NUM; }
  if(b<=0) { return NUM; }

  return	EXCEL_NUMBER(log(n)/log(b));
}

static ExcelValue ln(ExcelValue number_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	NUMBER(number_v, n)
	CHECK_FOR_CONVERSION_ERROR

  if(n<=0) { return NUM; }

  return	EXCEL_NUMBER(log(n));
}

static ExcelValue excel_exp(ExcelValue number_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	NUMBER(number_v, n)
	CHECK_FOR_CONVERSION_ERROR

  return	EXCEL_NUMBER(exp(n));
}

static ExcelValue excel_sqrt(ExcelValue number_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	NUMBER(number_v, n)
	CHECK_FOR_CONVERSION_ERROR

  if(n<0) { return NUM; }

  return	EXCEL_NUMBER(sqrt(n));
}

static ExcelValue excel_floor(ExcelValue number_v, ExcelValue multiple_v) {
  CHECK_FOR_PASSED_ERROR(number_v)
  CHECK_FOR_PASSED_ERROR(multiple_v)
	NUMBER(number_v, n)
  NUMBER(multiple_v, m)
	CHECK_FOR_CONVERSION_ERROR
  if(m == 0) { return DIV0; }
  if(m < 0) { return NUM; }
  return EXCEL_NUMBER((n - fmod(n, m)));
}

static ExcelValue excel_ceiling_math_2(ExcelValue number_v, ExcelValue multiple_v) {
  return excel_ceiling_math(number_v, multiple_v, ZERO);
}

static ExcelValue excel_ceiling_math(ExcelValue number_v, ExcelValue multiple_v, ExcelValue mode_v) {
  CHECK_FOR_PASSED_ERROR(number_v)
  CHECK_FOR_PASSED_ERROR(multiple_v)
  CHECK_FOR_PASSED_ERROR(mode_v)
	NUMBER(number_v, n)
  NUMBER(multiple_v, m)
  NUMBER(mode_v, d)
	CHECK_FOR_CONVERSION_ERROR
  if(m == 0) { return ZERO; }
    
  if(d == 0 || n > 0 ) {
    double remainder = fmod(n, m);
    if(remainder == 0) {
      return number_v;
    } else {
      return EXCEL_NUMBER(((double) ceil(n/m))*m);
    }
    
    
  } else { // Need to round negative away from zero
    return negative(excel_ceiling_math(excel_abs(number_v), multiple_v, mode_v));
  }
  
  return EXCEL_NUMBER((n - fmod(n, m)));
}

static ExcelValue rate(ExcelValue periods_v, ExcelValue payment_v, ExcelValue presentValue_v, ExcelValue finalValue_v) {
  CHECK_FOR_PASSED_ERROR(periods_v)
  CHECK_FOR_PASSED_ERROR(payment_v)
  CHECK_FOR_PASSED_ERROR(presentValue_v)
  CHECK_FOR_PASSED_ERROR(finalValue_v)

  NUMBER(periods_v, periods)
  NUMBER(payment_v, payment)
  NUMBER(presentValue_v, presentValue)
  NUMBER(finalValue_v, finalValue)

  // FIXME: Only implemented the case where payment is zero
  if(payment != 0) {
    return NA;
  }

  return EXCEL_NUMBER(pow((finalValue/(-presentValue)),(1.0/periods))-1.0);
}

static ExcelValue excel_and(int array_size, ExcelValue *array) {
	int i;
	ExcelValue current_excel_value, array_result;

	for(i=0;i<array_size;i++) {
		current_excel_value = array[i];
		switch (current_excel_value.type) {
	  	  case ExcelNumber:
		  case ExcelBoolean:
			  if(current_excel_value.number == false) return FALSE;
			  break;
		  case ExcelRange:
		  	array_result = excel_and( current_excel_value.rows * current_excel_value.columns, current_excel_value.array );
			if(array_result.type == ExcelError) return array_result;
			if(array_result.type == ExcelBoolean && array_result.number == false) return FALSE;
			break;
		  case ExcelString:
		  case ExcelEmpty:
			 break;
		  case ExcelError:
			 return current_excel_value;
			 break;
		 }
	 }
	 return TRUE;
}

static ExcelValue excel_or(int array_size, ExcelValue *array) {
	int i;
	ExcelValue current_excel_value, array_result;

	for(i=0;i<array_size;i++) {
		current_excel_value = array[i];
		switch (current_excel_value.type) {
	  	case ExcelNumber:
		  case ExcelBoolean:
			  if(current_excel_value.number == true) return TRUE;
			  break;
		  case ExcelRange:
		  	array_result = excel_or( current_excel_value.rows * current_excel_value.columns, current_excel_value.array );
        if(array_result.type == ExcelError) return array_result;
        if(array_result.type == ExcelBoolean && array_result.number == true) return TRUE;
        break;
		  case ExcelString:
		  case ExcelEmpty:
        break;
		  case ExcelError:
			 return current_excel_value;
			 break;
		 }
	 }
	 return FALSE;
}

static ExcelValue excel_not(ExcelValue boolean_v) {
  switch (boolean_v.type) {
    case ExcelNumber:
      if(boolean_v.number == 0) return TRUE;
      return FALSE;

    case ExcelBoolean:
      if(boolean_v.number == false) return TRUE;
      return FALSE;

    case ExcelRange:
      return VALUE;

    case ExcelString:
      return VALUE;

    case ExcelEmpty:
      return TRUE;

    case ExcelError:
      return boolean_v;
  }
}

struct average_result {
	double sum;
	double count;
	int has_error;
	ExcelValue error;
};

static struct average_result calculate_average(int array_size, ExcelValue *array) {
	double sum = 0;
	double count = 0;
	int i;
	ExcelValue current_excel_value;
	struct average_result array_result, r;

	for(i=0;i<array_size;i++) {
		current_excel_value = array[i];
		switch (current_excel_value.type) {
	  	  case ExcelNumber:
			  sum += current_excel_value.number;
			  count++;
			  break;
		  case ExcelRange:
		  	array_result = calculate_average( current_excel_value.rows * current_excel_value.columns, current_excel_value.array );
			if(array_result.has_error == true) return array_result;
			sum += array_result.sum;
			count += array_result.count;
			break;
		  case ExcelBoolean:
		  case ExcelString:
		  case ExcelEmpty:
			 break;
		  case ExcelError:
			 r.has_error = true;
			 r.error = current_excel_value;
			 return r;
			 break;
		 }
	}
	r.count = count;
	r.sum = sum;
	r.has_error = false;
	return r;
}

static ExcelValue average(int array_size, ExcelValue *array) {
	struct average_result r = calculate_average(array_size, array);
	if(r.has_error == true) return r.error;
	if(r.count == 0) return DIV0;
	return EXCEL_NUMBER(r.sum/r.count);
}

static ExcelValue forecast(ExcelValue required_x_v, ExcelValue known_y, ExcelValue known_x) {
  CHECK_FOR_PASSED_ERROR(required_x_v)

	NUMBER(required_x_v, required_x)
	CHECK_FOR_CONVERSION_ERROR

  if(known_x.type != ExcelRange) { return NA; }
  if(known_y.type != ExcelRange) { return NA; }

  int known_x_size = known_x.rows * known_x.columns;
  int known_y_size = known_y.rows * known_y.columns;

  int i;
  ExcelValue *x_array, *y_array;
  ExcelValue vx, vy;

  x_array = known_x.array;
  y_array = known_y.array;

  for(i=0; i<known_x_size; i++) {
    vx = x_array[i];
    if(vx.type == ExcelError) {
      return vx;
    }
  }

  for(i=0; i<known_x_size; i++) {
    vy = y_array[i];
    if(vy.type == ExcelError) {
      return vy;
    }
  }

  if(known_x_size != known_y_size) { return NA; }
  if(known_x_size == 0) { return NA; }

  ExcelValue mean_y = average(1, &known_y);
  ExcelValue mean_x = average(1, &known_x);

  if(mean_y.type == ExcelError) { return mean_y; }
  if(mean_x.type == ExcelError) { return mean_x; }

  float mx = mean_x.number;
  float my = mean_y.number;

  float b_numerator, b_denominator, b, a;

  b_denominator = 0;
  b_numerator = 0;

  for(i=0; i<known_x_size; i++) {
    vx = x_array[i];
    vy = y_array[i];
    if(vx.type != ExcelNumber) { continue; }
    if(vy.type != ExcelNumber) { continue; }

    b_denominator = b_denominator + pow(vx.number - mx, 2);
    b_numerator = b_numerator + ((vx.number - mx)*(vy.number-my));
  }

  if(b_denominator == 0) { return DIV0; }

  b = b_numerator / b_denominator;
  a = mean_y.number - (b*mean_x.number);

  return EXCEL_NUMBER(a + (b*required_x));
}

static ExcelValue choose(ExcelValue index_v, int array_size, ExcelValue *array) {
	CHECK_FOR_PASSED_ERROR(index_v)

	int index = (int) number_from(index_v);
	CHECK_FOR_CONVERSION_ERROR
	int i;
	for(i=0;i<array_size;i++) {
		if(array[i].type == ExcelError) return array[i];
	}
	if(index < 1) return VALUE;
	if(index > array_size) return VALUE;
	return array[index-1];
}

static ExcelValue count(int array_size, ExcelValue *array) {
	int i;
	int n = 0;
	ExcelValue current_excel_value;

	for(i=0;i<array_size;i++) {
		current_excel_value = array[i];
		switch (current_excel_value.type) {
	  	  case ExcelNumber:
		  	n++;
			break;
		  case ExcelRange:
		  	n += count( current_excel_value.rows * current_excel_value.columns, current_excel_value.array ).number;
			break;
  		  case ExcelBoolean:
		  case ExcelString:
		  case ExcelEmpty:
		  case ExcelError:
			 break;
		 }
	 }
	 return EXCEL_NUMBER(n);
}

static ExcelValue counta(int array_size, ExcelValue *array) {
	int i;
	int n = 0;
	ExcelValue current_excel_value;

	for(i=0;i<array_size;i++) {
		current_excel_value = array[i];
    switch(current_excel_value.type) {
  	  case ExcelNumber:
      case ExcelBoolean:
      case ExcelString:
  	  case ExcelError:
        n++;
        break;
      case ExcelRange:
	  	  n += counta( current_excel_value.rows * current_excel_value.columns, current_excel_value.array ).number;
        break;
  	  case ExcelEmpty:
  		  break;
    }
	 }
	 return EXCEL_NUMBER(n);
}

static ExcelValue divide(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)
	NUMBER(a_v, a)
	NUMBER(b_v, b)
	CHECK_FOR_CONVERSION_ERROR
	if(b == 0) return DIV0;
	return EXCEL_NUMBER(a / b);
}

static ExcelValue excel_equal(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)

	if(a_v.type != b_v.type) return FALSE;

	switch (a_v.type) {
  	case ExcelNumber:
	  case ExcelBoolean:
	  case ExcelEmpty:
			if(a_v.number != b_v.number) return FALSE;
			return TRUE;
	  case ExcelString:
	  	if(strcasecmp(a_v.string,b_v.string) != 0 ) return FALSE;
		  return TRUE;
  	case ExcelError:
		  return a_v;
  	case ExcelRange:
  		return NA;
  }
  return FALSE;
}

static ExcelValue not_equal(ExcelValue a_v, ExcelValue b_v) {
	ExcelValue result = excel_equal(a_v, b_v);
	if(result.type == ExcelBoolean) {
		if(result.number == 0) return TRUE;
		return FALSE;
	}
	return result;
}

static ExcelValue excel_isnumber(ExcelValue potential_number) {
  if(potential_number.type == ExcelNumber) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static ExcelValue excel_isblank(ExcelValue value) {
  if(value.type == ExcelEmpty) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static ExcelValue excel_if(ExcelValue condition, ExcelValue true_case, ExcelValue false_case ) {
	CHECK_FOR_PASSED_ERROR(condition)

	switch (condition.type) {
  	  case ExcelBoolean:
  	  	if(condition.number == true) return true_case;
  	  	return false_case;
  	  case ExcelNumber:
		if(condition.number == false) return false_case;
		return true_case;
	  case ExcelEmpty:
		return false_case;
	  case ExcelString:
	  	return VALUE;
  	  case ExcelError:
		return condition;
  	  case ExcelRange:
  		return VALUE;
  }
  return condition;
}

static ExcelValue excel_if_2(ExcelValue condition, ExcelValue true_case ) {
	return excel_if( condition, true_case, FALSE );
}

static ExcelValue excel_index(ExcelValue array_v, ExcelValue row_number_v, ExcelValue column_number_v) {
	CHECK_FOR_PASSED_ERROR(array_v)
	CHECK_FOR_PASSED_ERROR(row_number_v)
	CHECK_FOR_PASSED_ERROR(column_number_v)

	ExcelValue *array;
	int rows;
	int columns;

	NUMBER(row_number_v, row_number)
	NUMBER(column_number_v, column_number)
	CHECK_FOR_CONVERSION_ERROR

	if(array_v.type == ExcelRange) {
		array = array_v.array;
		rows = array_v.rows;
		columns = array_v.columns;
	} else {
		ExcelValue tmp_array[] = {array_v};
		array = tmp_array;
		rows = 1;
		columns = 1;
	}

	if(row_number > rows) return REF;
	if(column_number > columns) return REF;

  if(row_number == 0 && rows == 1) row_number = 1;
  if(column_number == 0 && columns == 1) column_number = 1;

	if(row_number == 0) { // We need the whole column
		if(column_number < 1) return REF;
		ExcelValue *result = (ExcelValue *) new_excel_value_array(rows);
		int result_index = 0;
		ExcelValue r;
		int array_index;
		int i;
		for(i = 0; i < rows; i++) {
			array_index = (i*columns) + column_number - 1;
			r = array[array_index];
			if(r.type == ExcelEmpty) {
				result[result_index] = ZERO;
			} else {
				result[result_index] = r;
			}
			result_index++;
		}
		return EXCEL_RANGE(result,rows,1);
	} else if(column_number == 0 ) { // We need the whole row
		if(row_number < 1) return REF;
		ExcelValue *result = (ExcelValue*) new_excel_value_array(columns);
		ExcelValue r;
		int row_start = ((row_number-1)*columns);
		int row_finish = row_start + columns;
		int result_index = 0;
		int i;
		for(i = row_start; i < row_finish; i++) {
			r = array[i];
			if(r.type == ExcelEmpty) {
				result[result_index] = ZERO;
			} else {
				result[result_index] = r;
			}
			result_index++;
		}
		return EXCEL_RANGE(result,1,columns);
	} else { // We need a precise point
		if(row_number < 1) return REF;
		if(column_number < 1) return REF;
		int position = ((row_number - 1) * columns) + column_number - 1;
		ExcelValue result = array[position];
		if(result.type == ExcelEmpty) return ZERO;
		return result;
	}

	return FALSE;
};

static ExcelValue excel_index_2(ExcelValue array_v, ExcelValue offset) {
	if(array_v.type == ExcelRange) {
		if(array_v.rows == 1) {
			return excel_index(array_v,ONE,offset);
		} else if (array_v.columns == 1) {
			return excel_index(array_v,offset,ONE);
		} else {
			return REF;
		}
	} else if (offset.type == ExcelNumber && offset.number == 1) {
		return array_v;
	} else {
		return REF;
	}
	return REF;
};

int compare_doubles (const void *a, const void *b) {
  const double *da = (const double *) a;
  const double *db = (const double *) b;

  return (*da > *db) - (*da < *db);
}

static ExcelValue large(ExcelValue range_v, ExcelValue k_v) {
  CHECK_FOR_PASSED_ERROR(range_v)
  CHECK_FOR_PASSED_ERROR(k_v)

  int k = (int) number_from(k_v);
  CHECK_FOR_CONVERSION_ERROR;

  // Check for edge case where just a single number passed
  if(range_v.type == ExcelNumber) {
    if( k == 1 ) {
      return range_v;
    } else {
      return NUM;
    }
  }

  // Otherwise grumble if not a range
  if(range_v.type != ExcelRange) { return VALUE; }

  // Check that our k is within bounds
  if(k < 1) { return NUM; }
  int range_size = range_v.rows * range_v.columns;

  // OK this is a really naive implementation.
  // FIXME: implement the BFPRT algorithm
  double *sorted = malloc(sizeof(double)*range_size);
  int sorted_size = 0;
  ExcelValue *array_v = range_v.array;
  ExcelValue x_v;
  int i;
  for(i = 0; i < range_size; i++ ) {
    x_v = array_v[i];
    if(x_v.type == ExcelError) { free(sorted); return x_v; };
    if(x_v.type == ExcelNumber) {
      sorted[sorted_size] = x_v.number;
      sorted_size++;
    }
  }
  // Check other bound
  if(k > sorted_size) { free(sorted); return NUM; }

  qsort(sorted, sorted_size, sizeof (double), compare_doubles);

  ExcelValue result = EXCEL_NUMBER(sorted[sorted_size - k]);
  free(sorted);
  return result;
}


static ExcelValue excel_match(ExcelValue lookup_value, ExcelValue lookup_array, ExcelValue match_type ) {
	CHECK_FOR_PASSED_ERROR(lookup_value)
	CHECK_FOR_PASSED_ERROR(lookup_array)
	CHECK_FOR_PASSED_ERROR(match_type)

	// Blanks are treaked as zeros
	if(lookup_value.type == ExcelEmpty) lookup_value = ZERO;

	// Setup the array
	ExcelValue *array;
	int size;
	if(lookup_array.type == ExcelRange) {
		// Check that the range is a row or column rather than an area
		if((lookup_array.rows == 1) || (lookup_array.columns == 1)) {
			array = lookup_array.array;
			size = lookup_array.rows * lookup_array.columns;
		} else {
			// return NA error if covers an area.
			return NA;
		};
	} else {
		// Need to wrap the argument up as an array
		size = 1;
		ExcelValue tmp_array[1] = {lookup_array};
		array = tmp_array;
	}

	int type = (int) number_from(match_type);
	CHECK_FOR_CONVERSION_ERROR;

	int i;
	ExcelValue x;

	switch(type) {
		case 0:
			for(i = 0; i < size; i++ ) {
				x = array[i];
				if(x.type == ExcelEmpty) x = ZERO;
				if(excel_equal(lookup_value,x).number == true) return EXCEL_NUMBER(i+1);
			}
			return NA;
			break;
		case 1:
			for(i = 0; i < size; i++ ) {
				x = array[i];
				if(lookup_value.type != x.type ) { continue; }
				if(more_than(x,lookup_value).number == true) {
					if(i==0) return NA;
					return EXCEL_NUMBER(i);
				}
			}
			return EXCEL_NUMBER(size);
			break;
		case -1:
			for(i = 0; i < size; i++ ) {
				x = array[i];
        if(lookup_value.type != x.type ) { continue; }
				if(less_than(x,lookup_value).number == true) {
					if(i==0) return NA;
					return EXCEL_NUMBER(i);
				}
			}
			return EXCEL_NUMBER(size-1);
			break;
	}
	return NA;
}

static ExcelValue excel_match_2(ExcelValue lookup_value, ExcelValue lookup_array ) {
	return excel_match(lookup_value, lookup_array, ONE);
}

static ExcelValue find(ExcelValue find_text_v, ExcelValue within_text_v, ExcelValue start_number_v) {
	CHECK_FOR_PASSED_ERROR(find_text_v)
	CHECK_FOR_PASSED_ERROR(within_text_v)
	CHECK_FOR_PASSED_ERROR(start_number_v)

	char *find_text;
	char *within_text;
	char *within_text_offset;
	char *result;
	int start_number = number_from(start_number_v);
	CHECK_FOR_CONVERSION_ERROR

	// Deal with blanks
	if(within_text_v.type == ExcelString) {
		within_text = within_text_v.string;
	} else if( within_text_v.type == ExcelEmpty) {
		within_text = "";
	}

	if(find_text_v.type == ExcelString) {
		find_text = find_text_v.string;
	} else if( find_text_v.type == ExcelEmpty) {
		return start_number_v;
	}

	// Check length
	if(start_number < 1) return VALUE;
	if(start_number > strlen(within_text)) return VALUE;

	// Offset our within_text pointer
	// FIXME: No way this is utf-8 compatible
	within_text_offset = within_text + (start_number - 1);
	result = strstr(within_text_offset,find_text);
	if(result) {
		return EXCEL_NUMBER(result - within_text + 1);
	}
	return VALUE;
}

static ExcelValue find_2(ExcelValue string_to_look_for_v, ExcelValue string_to_look_in_v) {
	return find(string_to_look_for_v, string_to_look_in_v, ONE);
};

static ExcelValue left(ExcelValue string_v, ExcelValue number_of_characters_v) {
	CHECK_FOR_PASSED_ERROR(string_v)
	CHECK_FOR_PASSED_ERROR(number_of_characters_v)
	if(string_v.type == ExcelEmpty) return BLANK;
	if(number_of_characters_v.type == ExcelEmpty) return BLANK;

	int number_of_characters = (int) number_from(number_of_characters_v);
	CHECK_FOR_CONVERSION_ERROR

  if(number_of_characters < 0) {
    return VALUE;
  }

	char *string;
	int string_must_be_freed = 0;
	switch (string_v.type) {
  	case ExcelString:
  		string = string_v.string;
  		break;
  	case ExcelNumber:
		  string = malloc(20); // Freed
		  if(string == 0) {
			  printf("Out of memory in left");
			  exit(-1);
		  }
		  string_must_be_freed = 1;
		  snprintf(string,20,"%0.0f",string_v.number);
		  break;
	  case ExcelBoolean:
	  	if(string_v.number == true) {
	  		string = "TRUE";
		  } else {
			  string = "FALSE";
		  }
		  break;
	  case ExcelEmpty:
  	case ExcelError:
  	case ExcelRange:
		  return string_v;
	}

  if(number_of_characters > strlen(string)) {
    number_of_characters = strlen(string);
  }

	char *left_string = malloc(number_of_characters+1); // Freed
	if(left_string == 0) {
	  printf("Out of memoryn in left");
	  exit(-1);
	}
	memcpy(left_string,string,number_of_characters);
	left_string[number_of_characters] = '\0';
	if(string_must_be_freed == 1) {
		free(string);
	}
	free_later(left_string);
	return EXCEL_STRING(left_string);
}

static ExcelValue left_1(ExcelValue string_v) {
	return left(string_v, ONE);
}

static ExcelValue len(ExcelValue string_v) {
	CHECK_FOR_PASSED_ERROR(string_v)
	if(string_v.type == ExcelEmpty) return ZERO;

	char *string;
	int string_must_be_freed = 0;
	switch (string_v.type) {
  	case ExcelString:
  		string = string_v.string;
  		break;
  	case ExcelNumber:
		  string = malloc(20); // Freed
		  if(string == 0) {
			  printf("Out of memory in len");
			  exit(-1);
		  }
		  snprintf(string,20,"%0.0f",string_v.number);
		  string_must_be_freed = 1;
		  break;
	  case ExcelBoolean:
	  	if(string_v.number == true) {
	  		string = "TRUE";
		  } else {
			  string = "FALSE";
		  }
		  break;
	  case ExcelEmpty:
  	case ExcelError:
  	case ExcelRange:
		  return string_v;
	}

  int length = strlen(string);
	if(string_must_be_freed == 1) {
		free(string);
	}
	return EXCEL_NUMBER(length);
}

static ExcelValue right(ExcelValue string_v, ExcelValue number_of_characters_v) {
	CHECK_FOR_PASSED_ERROR(string_v)
	CHECK_FOR_PASSED_ERROR(number_of_characters_v)
	if(string_v.type == ExcelEmpty) return BLANK;
	if(number_of_characters_v.type == ExcelEmpty) return BLANK;

	int number_of_characters = (int) number_from(number_of_characters_v);
	CHECK_FOR_CONVERSION_ERROR

  if(number_of_characters < 0) {
    return VALUE;
  }

	char *string;
	int string_must_be_freed = 0;
	switch (string_v.type) {
  	case ExcelString:
  		string = string_v.string;
  		break;
  	case ExcelNumber:
		  string = malloc(20); // Freed
		  if(string == 0) {
			  printf("Out of memory in right");
			  exit(-1);
		  }
		  string_must_be_freed = 1;
		  snprintf(string,20,"%0.0f",string_v.number);
		  break;
	  case ExcelBoolean:
	  	if(string_v.number == true) {
	  		string = "TRUE";
		  } else {
			  string = "FALSE";
		  }
		  break;
	  case ExcelEmpty:
  	case ExcelError:
  	case ExcelRange:
		  return string_v;
	}

	char *right_string = malloc(number_of_characters+1); // Freed
	if(right_string == 0) {
	  printf("Out of memory in right");
	  exit(-1);
	}
  int length = strlen(string);
  if(number_of_characters > length) {
    number_of_characters = length;
  }
  memcpy(right_string,string+length-number_of_characters,number_of_characters);
  right_string[number_of_characters] = '\0';
  if(string_must_be_freed == 1) {
    free(string);
  }
  free_later(right_string);
  return EXCEL_STRING(right_string);
}

static ExcelValue right_1(ExcelValue string_v) {
	return right(string_v, ONE);
}

static ExcelValue iferror(ExcelValue value, ExcelValue value_if_error) {
	if(value.type == ExcelError) return value_if_error;
	return value;
}

static ExcelValue iserr(ExcelValue value) {
	if(value.type == ExcelError) {
    if(value.number == NA.number) {
      return FALSE;
    } else {
      return TRUE;
    }
  } else {
    return FALSE;
  }
}

static ExcelValue iserror(ExcelValue value) {
	if(value.type == ExcelError) {
    return TRUE;
  } else {
    return FALSE;
  }
}



// Order is TRUE, FALSE, String, Number; Blank is zero
static ExcelValue more_than(ExcelValue a_v, ExcelValue b_v) {
  CHECK_FOR_PASSED_ERROR(a_v)
  CHECK_FOR_PASSED_ERROR(b_v)

  if(a_v.type == ExcelEmpty) { a_v = ZERO; }
  if(b_v.type == ExcelEmpty) { b_v = ZERO; }

  switch (a_v.type) {
    case ExcelString:
      switch (b_v.type) {
        case ExcelString:
          if(strcasecmp(a_v.string,b_v.string) <= 0 ) {return FALSE;} else {return TRUE;}
        case ExcelNumber:
          return TRUE;
        case ExcelBoolean:
          return FALSE;
        // Following shouldn't happen
        case ExcelEmpty:
        case ExcelError:
        case ExcelRange:
          return NA;
      }
    case ExcelBoolean:
      switch (b_v.type) {
        case ExcelBoolean:
          if(a_v.number == true) {
            if (b_v.number == true) { return FALSE; } else { return TRUE; }
          } else { // a_v == FALSE
            return FALSE;
          }
        case ExcelString:
        case ExcelNumber:
          return TRUE;
        // Following shouldn't happen
        case ExcelEmpty:
        case ExcelError:
        case ExcelRange:
          return NA;
      }
    case ExcelNumber:
      switch (b_v.type) {
        case ExcelNumber:
          if(a_v.number > b_v.number) { return TRUE; } else { return FALSE; }
        case ExcelString:
        case ExcelBoolean:
          return FALSE;
        // Following shouldn't happen
        case ExcelEmpty:
        case ExcelError:
        case ExcelRange:
          return NA;
      }
    // Following shouldn't happen
    case ExcelEmpty:
    case ExcelError:
    case ExcelRange:
      return NA;
  }
  // Shouldn't reach here
  return NA;
}

static ExcelValue more_than_or_equal(ExcelValue a_v, ExcelValue b_v) {
  ExcelValue opposite = less_than(a_v, b_v);
  switch (opposite.type) {
    case ExcelBoolean:
      if(opposite.number == true) { return FALSE; } else { return TRUE; }
    case ExcelError:
      return opposite;
    // Shouldn't reach below
    case ExcelNumber:
    case ExcelString:
    case ExcelEmpty:
    case ExcelRange:
      return NA;
  }
}

// Order is TRUE, FALSE, String, Number; Blank is zero
static ExcelValue less_than(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)

  if(a_v.type == ExcelEmpty) { a_v = ZERO; }
  if(b_v.type == ExcelEmpty) { b_v = ZERO; }

	switch (a_v.type) {
    case ExcelString:
      switch (b_v.type) {
        case ExcelString:
          if(strcasecmp(a_v.string, b_v.string) >= 0 )  {
            return FALSE;
          } else {
            return TRUE;
          }
        case ExcelNumber:
          return FALSE;
        case ExcelBoolean:
          return TRUE;
        // The following shouldn't happen
        // FIXME: Should abort if it does
        case ExcelError:
        case ExcelRange:
        case ExcelEmpty:
          return NA;
      }
  	case ExcelNumber:
      switch(b_v.type) {
        case ExcelNumber:
          if(a_v.number < b_v.number) {
            return TRUE;
          } else {
            return FALSE;
          }
        case ExcelBoolean:
        case ExcelString:
          return TRUE;
        // The following shouldn't happen
        // FIXME: Should abort if it does
        case ExcelError:
        case ExcelRange:
        case ExcelEmpty:
          return NA;
      }
    case ExcelBoolean:
      switch(b_v.type) {
        case ExcelBoolean:
          if(a_v.number == true) {
            return FALSE;
          } else { // a_v.number == false
            if(b_v.number == true) {return TRUE;} else {return FALSE;}
          }
        case ExcelString:
        case ExcelNumber:
          return FALSE;
        // The following shouldn't happen
        // FIXME: Should abort if it does
        case ExcelError:
        case ExcelRange:
        case ExcelEmpty:
          return NA;
      }
    // The following shouldn't happen
    // FIXME: Should abort if it does
    case ExcelError:
    case ExcelRange:
    case ExcelEmpty:
      return VALUE;
  }
  // Shouldn't reach here
  return NA;
}

static ExcelValue less_than_or_equal(ExcelValue a_v, ExcelValue b_v) {
  ExcelValue opposite = more_than(a_v, b_v);
  switch (opposite.type) {
    case ExcelBoolean:
      if(opposite.number == true) { return FALSE; } else { return TRUE; }
    case ExcelError:
      return opposite;
    // Shouldn't reach below
    case ExcelNumber:
    case ExcelString:
    case ExcelEmpty:
    case ExcelRange:
      return VALUE;
  }
}

static ExcelValue subtract(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)
	NUMBER(a_v, a)
	NUMBER(b_v, b)
	CHECK_FOR_CONVERSION_ERROR
	return EXCEL_NUMBER(a - b);
}

static ExcelValue multiply(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)
	NUMBER(a_v, a)
	NUMBER(b_v, b)
	CHECK_FOR_CONVERSION_ERROR
	return EXCEL_NUMBER(a * b);
}

static ExcelValue sum(int array_size, ExcelValue *array) {
	double total = 0;
	int i;
  ExcelValue r;
	for(i=0;i<array_size;i++) {
    switch(array[i].type) {
      case ExcelNumber:
        total += array[i].number;
        break;
      case ExcelRange:
        r = sum( array[i].rows * array[i].columns, array[i].array );
        if(r.type == ExcelError) {
          return r;
        } else {
          total += number_from(r);
        }
        break;
      case ExcelError:
        return array[i];
        break;
      default:
        break;
    }
	}
	return EXCEL_NUMBER(total);
}

static ExcelValue npv(ExcelValue rate_v, int number_of_arguments, ExcelValue *arguments) {
	CHECK_FOR_PASSED_ERROR(rate_v)
	NUMBER(rate_v, rate)
	CHECK_FOR_CONVERSION_ERROR
  if(rate == -1) { return DIV0; }

  double npv = 0;
  int n = 1;
  int i;
  int j;
  double v;
  ExcelValue r;
  ExcelValue r2;
  ExcelValue *range;

  for(i=0;i<number_of_arguments;i++) {
    r = arguments[i];
    if(r.type == ExcelError) { return r; }
    if(r.type == ExcelRange) {
      range = r.array;
      for(j=0;j<(r.columns*r.rows);j++) {
        r2 = range[j];
        if(r2.type == ExcelError) { return r2; }
        v = number_from(r2);
        if(conversion_error) { conversion_error = 0; return VALUE; }
        npv = npv + (v/pow(1+rate, n));
        n++;
      }
    } else {
      v = number_from(r);
      if(conversion_error) { conversion_error = 0; return VALUE; }
      npv = npv + (v/pow(1+rate, n));
      n++;
    }
  }
  return EXCEL_NUMBER(npv);
}

static ExcelValue max(int number_of_arguments, ExcelValue *arguments) {
	double biggest_number_found;
	int any_number_found = 0;
	int i;
	ExcelValue current_excel_value;

	for(i=0;i<number_of_arguments;i++) {
		current_excel_value = arguments[i];
		if(current_excel_value.type == ExcelNumber) {
			if(!any_number_found) {
				any_number_found = 1;
				biggest_number_found = current_excel_value.number;
			}
			if(current_excel_value.number > biggest_number_found) biggest_number_found = current_excel_value.number;
		} else if(current_excel_value.type == ExcelRange) {
			current_excel_value = max( current_excel_value.rows * current_excel_value.columns, current_excel_value.array );
			if(current_excel_value.type == ExcelError) return current_excel_value;
			if(current_excel_value.type == ExcelNumber)
				if(!any_number_found) {
					any_number_found = 1;
					biggest_number_found = current_excel_value.number;
				}
				if(current_excel_value.number > biggest_number_found) biggest_number_found = current_excel_value.number;
		} else if(current_excel_value.type == ExcelError) {
			return current_excel_value;
		}
	}
	if(!any_number_found) {
		any_number_found = 1;
		biggest_number_found = 0;
	}
	return EXCEL_NUMBER(biggest_number_found);
}

static ExcelValue min(int number_of_arguments, ExcelValue *arguments) {
	double smallest_number_found = 0;
	int any_number_found = 0;
	int i;
	ExcelValue current_excel_value;

	for(i=0;i<number_of_arguments;i++) {
		current_excel_value = arguments[i];
		if(current_excel_value.type == ExcelNumber) {
			if(!any_number_found) {
				any_number_found = 1;
				smallest_number_found = current_excel_value.number;
			}
			if(current_excel_value.number < smallest_number_found) smallest_number_found = current_excel_value.number;
		} else if(current_excel_value.type == ExcelRange) {
			current_excel_value = min( current_excel_value.rows * current_excel_value.columns, current_excel_value.array );
			if(current_excel_value.type == ExcelError) return current_excel_value;
			if(current_excel_value.type == ExcelNumber)
				if(!any_number_found) {
					any_number_found = 1;
					smallest_number_found = current_excel_value.number;
				}
				if(current_excel_value.number < smallest_number_found) smallest_number_found = current_excel_value.number;
		} else if(current_excel_value.type == ExcelError) {
			return current_excel_value;
		}
	}
	if(!any_number_found) {
		any_number_found = 1;
		smallest_number_found = 0;
	}
	return EXCEL_NUMBER(smallest_number_found);
}

static ExcelValue mmult_error(ExcelValue a_v, ExcelValue b_v) {
  int rows = a_v.rows > b_v.rows ? a_v.rows : b_v.rows;
  int columns = a_v.columns > b_v.columns ? a_v.columns : b_v.columns;
  int i, j;

  ExcelValue *result = (ExcelValue*) new_excel_value_array(rows*columns);

  for(i=0; i<rows; i++) {
    for(j=0; j<columns; j++) {
      result[(i*columns) + j] = VALUE;
    }
  }
  return EXCEL_RANGE(result, rows, columns);
}

static ExcelValue mmult(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)
  if(a_v.type != ExcelRange) { return VALUE;}
  if(b_v.type != ExcelRange) { return VALUE;}
  if(a_v.columns != b_v.rows) { return mmult_error(a_v, b_v); }
  int n = a_v.columns;
  int a_rows = a_v.rows;
  int a_columns = a_v.columns;
  int b_columns = b_v.columns;
  ExcelValue *result = (ExcelValue*) new_excel_value_array(a_rows*b_columns);
  int i, j, k;
  double sum;
  ExcelValue *array_a = a_v.array;
  ExcelValue *array_b = b_v.array;

  ExcelValue a;
  ExcelValue b;

  for(i=0; i<a_rows; i++) {
    for(j=0; j<b_columns; j++) {
      sum = 0;
      for(k=0; k<n; k++) {
        a = array_a[(i*a_columns)+k];
        b = array_b[(k*b_columns)+j];
        if(a.type != ExcelNumber) { return mmult_error(a_v, b_v); }
        if(b.type != ExcelNumber) { return mmult_error(a_v, b_v); }
        sum = sum + (a.number * b.number);
      }
      result[(i*b_columns)+j] = EXCEL_NUMBER(sum);
    }
  }
  return EXCEL_RANGE(result, a_rows, b_columns);
}

static ExcelValue mod(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)

	NUMBER(a_v, a)
	NUMBER(b_v, b)
	CHECK_FOR_CONVERSION_ERROR
	if(b == 0) return DIV0;
	return EXCEL_NUMBER(fmod(a,b));
}

static ExcelValue na() {
  return NA;
}

static ExcelValue negative(ExcelValue a_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	NUMBER(a_v, a)
	CHECK_FOR_CONVERSION_ERROR
	return EXCEL_NUMBER(-a);
}

static ExcelValue pmt(ExcelValue rate_v, ExcelValue number_of_periods_v, ExcelValue present_value_v) {
	CHECK_FOR_PASSED_ERROR(rate_v)
	CHECK_FOR_PASSED_ERROR(number_of_periods_v)
	CHECK_FOR_PASSED_ERROR(present_value_v)

	NUMBER(rate_v,rate)
	NUMBER(number_of_periods_v,number_of_periods)
	NUMBER(present_value_v,present_value)
	CHECK_FOR_CONVERSION_ERROR

	if(rate == 0) return EXCEL_NUMBER(-(present_value / number_of_periods));
	return EXCEL_NUMBER(-present_value*(rate*(pow((1+rate),number_of_periods)))/((pow((1+rate),number_of_periods))-1));
}

static ExcelValue pmt_4(ExcelValue rate_v, ExcelValue number_of_periods_v, ExcelValue present_value_v, ExcelValue final_value_v) {
  CHECK_FOR_PASSED_ERROR(final_value_v)

    NUMBER(final_value_v, final_value)
    CHECK_FOR_CONVERSION_ERROR

    if(final_value == 0) return pmt(rate_v, number_of_periods_v, present_value_v);
    printf("PMT with non-zero final_value not implemented. halting.");
    exit(-1);
}

static ExcelValue pmt_5(ExcelValue rate_v, ExcelValue number_of_periods_v, ExcelValue present_value_v, ExcelValue final_value_v, ExcelValue type_v) {
  CHECK_FOR_PASSED_ERROR(type_v)

    NUMBER(type_v, type)
    CHECK_FOR_CONVERSION_ERROR

    if(type == 0) return pmt(rate_v, number_of_periods_v, present_value_v);
    printf("PMT with non-zero type not implemented. halting.");
    exit(-1);
}


static ExcelValue pv_3(ExcelValue rate_v, ExcelValue nper_v, ExcelValue pmt_v) {
  return pv_4(rate_v, nper_v, pmt_v, ZERO);
}

static ExcelValue pv_4(ExcelValue rate_v, ExcelValue nper_v, ExcelValue pmt_v, ExcelValue fv_v) {
  return pv_5(rate_v, nper_v, pmt_v, fv_v, ZERO);
}

static ExcelValue pv_5(ExcelValue rate_v, ExcelValue nper_v, ExcelValue pmt_v, ExcelValue fv_v, ExcelValue type_v ) {
  CHECK_FOR_PASSED_ERROR(rate_v)
  CHECK_FOR_PASSED_ERROR(nper_v)
  CHECK_FOR_PASSED_ERROR(pmt_v)
  CHECK_FOR_PASSED_ERROR(fv_v)
  CHECK_FOR_PASSED_ERROR(type_v)

  NUMBER(rate_v, rate)
  NUMBER(nper_v, nper)
  NUMBER(pmt_v, payment)
  NUMBER(fv_v, fv)
  NUMBER(type_v, start_of_period)
  CHECK_FOR_CONVERSION_ERROR

  if(rate< 0) {
    return VALUE;
  }

  double present_value = 0;

  // Sum up the payments
  if(rate == 0) {
    present_value = -payment * nper;
  } else {
    present_value = -payment * ((1-pow(1+rate,-nper))/rate);
  }

  // Adjust for beginning or end of period
  if(start_of_period == 0) {
   // Do Nothing
  } else if(start_of_period == 1) {
   present_value = present_value * (1+rate);
  } else {
   return VALUE;
  }

  // Add on the final value
  present_value = present_value - (fv/pow(1+rate,nper));

  return EXCEL_NUMBER(present_value);
}


static ExcelValue power(ExcelValue a_v, ExcelValue b_v) {
	CHECK_FOR_PASSED_ERROR(a_v)
	CHECK_FOR_PASSED_ERROR(b_v)

	NUMBER(a_v, a)
	NUMBER(b_v, b)
	CHECK_FOR_CONVERSION_ERROR
  double result = pow(a,b);
  if(isnan(result) == 1) {
    return NUM;
  } else {
    return EXCEL_NUMBER(result);
  }
}
static ExcelValue rank(ExcelValue number_v, ExcelValue range_v, ExcelValue order_v) {
  CHECK_FOR_PASSED_ERROR(number_v)
  CHECK_FOR_PASSED_ERROR(range_v)
  CHECK_FOR_PASSED_ERROR(order_v)

  NUMBER(number_v, number)
  NUMBER(order_v, order)

  ExcelValue *array;
  int size;

	CHECK_FOR_CONVERSION_ERROR

  if(range_v.type != ExcelRange) {
    array = new_excel_value_array(1);
    array[0] = range_v;
    size = 1;
  } else {
    array = range_v.array;
    size = range_v.rows * range_v.columns;
  }

  int ranked = 1;
  int found = false;

  int i;
  ExcelValue cell;

  for(i=0; i<size; i++) {
    cell = array[i];
    if(cell.type == ExcelError) { return cell; }
    if(cell.type == ExcelNumber) {
      if(cell.number == number) { found = true; }
      if(order == 0) { if(cell.number > number) { ranked++; } }
      if(order != 0) { if(cell.number < number) { ranked++; } }
    }
  }
  if(found == false) { return NA; }
  return EXCEL_NUMBER(ranked);
}

static ExcelValue rank_2(ExcelValue number_v, ExcelValue range_v) {
  return rank(number_v, range_v, ZERO);
}

static ExcelValue excel_round(ExcelValue number_v, ExcelValue decimal_places_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	CHECK_FOR_PASSED_ERROR(decimal_places_v)

	NUMBER(number_v, number)
	NUMBER(decimal_places_v, decimal_places)
	CHECK_FOR_CONVERSION_ERROR

	double multiple = pow(10,decimal_places);

	return EXCEL_NUMBER( round(number * multiple) / multiple );
}

static ExcelValue rounddown(ExcelValue number_v, ExcelValue decimal_places_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	CHECK_FOR_PASSED_ERROR(decimal_places_v)

	NUMBER(number_v, number)
	NUMBER(decimal_places_v, decimal_places)
	CHECK_FOR_CONVERSION_ERROR

	double multiple = pow(10,decimal_places);

	return EXCEL_NUMBER( trunc(number * multiple) / multiple );
}

static ExcelValue roundup(ExcelValue number_v, ExcelValue decimal_places_v) {
	CHECK_FOR_PASSED_ERROR(number_v)
	CHECK_FOR_PASSED_ERROR(decimal_places_v)

	NUMBER(number_v, number)
	NUMBER(decimal_places_v, decimal_places)
	CHECK_FOR_CONVERSION_ERROR

	double multiple = pow(10,decimal_places);
	if(number < 0) return EXCEL_NUMBER( floor(number * multiple) / multiple );
	return EXCEL_NUMBER( ceil(number * multiple) / multiple );
}

static ExcelValue excel_int(ExcelValue number_v) {
	CHECK_FOR_PASSED_ERROR(number_v)

	NUMBER(number_v, number)
	CHECK_FOR_CONVERSION_ERROR

	return EXCEL_NUMBER(floor(number));
}

static ExcelValue string_join(int number_of_arguments, ExcelValue *arguments) {
	int allocated_length = 100;
	int used_length = 0;
	char *string = malloc(allocated_length); // Freed later
	if(string == 0) {
	  printf("Out of memory in string_join");
	  exit(-1);
	}
	char *current_string;
	int current_string_length;
	int must_free_current_string;
	ExcelValue current_v;
	int i;
	for(i=0;i<number_of_arguments;i++) {
		must_free_current_string = 0;
		current_v = (ExcelValue) arguments[i];
		switch (current_v.type) {
  	  case ExcelString:
	  		current_string = current_v.string;
	  		break;
  	  case ExcelNumber:
			  current_string = malloc(20); // Freed
		  	if(current_string == 0) {
		  	  printf("Out of memory in string join");
		  	  exit(-1);
		  	}
			  must_free_current_string = 1;
			  snprintf(current_string,20,"%g",current_v.number);
			  break;
		  case ExcelBoolean:
		  	if(current_v.number == true) {
		  		current_string = "TRUE";
  			} else {
  				current_string = "FALSE";
  			}
        break;
		  case ExcelEmpty:
        current_string = "";
        break;
      case ExcelError:
        free(string);
        return current_v;
	  	case ExcelRange:
        free(string);
        return VALUE;
		}
		current_string_length = strlen(current_string);
		if( (used_length + current_string_length + 1) > allocated_length) {
			allocated_length = used_length + current_string_length + 1 + 100;
			string = realloc(string,allocated_length);
      if(!string) {
        printf("Out of memory in string join realloc trying to increase to %d", allocated_length);
        exit(-1);
      }
		}
		memcpy(string + used_length, current_string, current_string_length);
		if(must_free_current_string == 1) {
			free(current_string);
		}
		used_length = used_length + current_string_length;
	} // Finished looping through passed strings
	string = realloc(string,used_length+1);
  if(!string) {
    printf("Out of memory in string join realloc trying to increase to %d", used_length+1);
    exit(-1);
  }
  string[used_length] = '\0';
	free_later(string);
	return EXCEL_STRING(string);
}

static ExcelValue subtotal(ExcelValue subtotal_type_v, int number_of_arguments, ExcelValue *arguments) {
  CHECK_FOR_PASSED_ERROR(subtotal_type_v)
  NUMBER(subtotal_type_v,subtotal_type)
  CHECK_FOR_CONVERSION_ERROR

  switch((int) subtotal_type) {
    case 1:
    case 101:
      return average(number_of_arguments,arguments);
      break;
    case 2:
    case 102:
      return count(number_of_arguments,arguments);
      break;
    case 3:
    case 103:
      return counta(number_of_arguments,arguments);
      break;
    case 9:
    case 109:
      return sum(number_of_arguments,arguments);
      break;
    default:
      return VALUE;
      break;
  }
}


// FIXME: Check if this deals with errors correctly
static ExcelValue filter_range(ExcelValue original_range_v, int number_of_arguments, ExcelValue *arguments) {
  // First, set up the original_range
  //CHECK_FOR_PASSED_ERROR(original_range_v);

  // Set up the sum range
  ExcelValue *original_range;
  int original_range_rows, original_range_columns;

  if(original_range_v.type == ExcelRange) {
    original_range = original_range_v.array;
    original_range_rows = original_range_v.rows;
    original_range_columns = original_range_v.columns;
  } else {
    original_range = (ExcelValue*) new_excel_value_array(1);
	  original_range[0] = original_range_v;
    original_range_rows = 1;
    original_range_columns = 1;
  }

  // This is the filtered range
  ExcelValue *filtered_range = new_excel_value_array(original_range_rows*original_range_columns);
  int number_of_filtered_values = 0;

  // Then go through and set up the check ranges
  if(number_of_arguments % 2 != 0) return VALUE;
  int number_of_criteria = number_of_arguments / 2;
  ExcelValue *criteria_range =  (ExcelValue*) new_excel_value_array(number_of_criteria);
  ExcelValue current_value;
  int i;
  for(i = 0; i < number_of_criteria; i++) {
    current_value = arguments[i*2];
    if(current_value.type == ExcelRange) {
      criteria_range[i] = current_value;
      if(current_value.rows != original_range_rows) return VALUE;
      if(current_value.columns != original_range_columns) return VALUE;
    } else {
      if(original_range_rows != 1) return VALUE;
      if(original_range_columns != 1) return VALUE;
      ExcelValue *tmp_array2 =  (ExcelValue*) new_excel_value_array(1);
      tmp_array2[0] = current_value;
      criteria_range[i] =  EXCEL_RANGE(tmp_array2,1,1);
    }
  }

  // Now go through and set up the criteria
  ExcelComparison *criteria =  malloc(sizeof(ExcelComparison)*number_of_criteria); // freed at end of function
  if(criteria == 0) {
	  printf("Out of memory in filter_range\n");
	  exit(-1);
  }
  char *s;
  char *new_comparator;

  for(i = 0; i < number_of_criteria; i++) {
    current_value = arguments[(i*2)+1];

    if(current_value.type == ExcelString) {
      s = current_value.string;
      if(s[0] == '<') {
        if( s[1] == '>') {
          new_comparator = strndup(s+2,strlen(s)-2);
          free_later(new_comparator);
          criteria[i].type = NotEqual;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        } else if(s[1] == '=') {
          new_comparator = strndup(s+2,strlen(s)-2);
          free_later(new_comparator);
          criteria[i].type = LessThanOrEqual;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        } else {
          new_comparator = strndup(s+1,strlen(s)-1);
          free_later(new_comparator);
          criteria[i].type = LessThan;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        }
      } else if(s[0] == '>') {
        if(s[1] == '=') {
          new_comparator = strndup(s+2,strlen(s)-2);
          free_later(new_comparator);
          criteria[i].type = MoreThanOrEqual;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        } else {
          new_comparator = strndup(s+1,strlen(s)-1);
          free_later(new_comparator);
          criteria[i].type = MoreThan;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        }
      } else if(s[0] == '=') {
        new_comparator = strndup(s+1,strlen(s)-1);
        free_later(new_comparator);
        criteria[i].type = Equal;
        criteria[i].comparator = EXCEL_STRING(new_comparator);
      } else {
        criteria[i].type = Equal;
        criteria[i].comparator = current_value;
      }
    } else {
      criteria[i].type = Equal;
      criteria[i].comparator = current_value;
    }
  }

  int size = original_range_columns * original_range_rows;
  int j;
  int passed = 0;
  ExcelValue value_to_be_checked;
  ExcelComparison comparison;
  ExcelValue comparator;
  double number;
  // For each cell in the sum range
  for(j=0; j < size; j++ ) {
    passed = 1;
    for(i=0; i < number_of_criteria; i++) {
      value_to_be_checked = ((ExcelValue *) ((ExcelValue) criteria_range[i]).array)[j];
      comparison = criteria[i];
      comparator = comparison.comparator;

      // For the purposes of comparison, treates a blank criteria as matching zeros.
      if(comparator.type == ExcelEmpty) {
        comparator = ZERO;
      }

      switch(value_to_be_checked.type) {
        case ExcelError: // Errors match only errors
          if(comparison.type != Equal) passed = 0;
          if(comparator.type != ExcelError) passed = 0;
          if(value_to_be_checked.number != comparator.number) passed = 0;
          break;
        case ExcelBoolean: // Booleans match only booleans (FIXME: I think?)
          if(comparison.type != Equal) passed = 0;
          if(comparator.type != ExcelBoolean ) passed = 0;
          if(value_to_be_checked.number != comparator.number) passed = 0;
          break;
        case ExcelEmpty:
          // if(comparator.type == ExcelEmpty) break; // FIXME: Huh? In excel blank doesn't match blank?!
          if(comparator.type != ExcelString) {
            passed = 0;
            break;
          } else {
            if(strlen(comparator.string) != 0) passed = 0; // Empty strings match blanks.
            break;
          }
        case ExcelNumber:
          if(comparator.type == ExcelNumber) {
            number = comparator.number;
          } else if(comparator.type == ExcelString) {
            number = number_from(comparator);
            if(conversion_error == 1) {
              conversion_error = 0;
              passed = 0;
              break;
            }
          } else {
            passed = 0;
            break;
          }
          switch(comparison.type) {
            case Equal:
              if(value_to_be_checked.number != number) passed = 0;
              break;
            case LessThan:
              if(value_to_be_checked.number >= number) passed = 0;
              break;
            case LessThanOrEqual:
              if(value_to_be_checked.number > number) passed = 0;
              break;
            case NotEqual:
              if(value_to_be_checked.number == number) passed = 0;
              break;
            case MoreThanOrEqual:
              if(value_to_be_checked.number < number) passed = 0;
              break;
            case MoreThan:
              if(value_to_be_checked.number <= number) passed = 0;
              break;
          }
          break;
        case ExcelString:
          // First case, the comparator is a number, simplification is that it can only be equal
          if(comparator.type == ExcelNumber) {
            if(comparison.type != Equal) {
              printf("This shouldn't be possible?");
              passed = 0;
              break;
            }

            // Special case, empty strings don't match zeros here
            if(strlen(value_to_be_checked.string) == 0) {
              passed = 0;
              break;
            }

            number = number_from(value_to_be_checked);
            if(conversion_error == 1) {
              conversion_error = 0;
              passed = 0;
              break;
            }
            if(number != comparator.number) {
              passed = 0;
              break;
            } else {
              break;
            }
          // Second case, the comparator is also a string, so need to be able to do full range of tests
          } else if(comparator.type == ExcelString) {
            switch(comparison.type) {
              case Equal:
                if(excel_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case LessThan:
                if(less_than(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case LessThanOrEqual:
                if(less_than_or_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case NotEqual:
                if(not_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case MoreThanOrEqual:
                if(more_than_or_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case MoreThan:
                if(more_than(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              }
          } else {
            passed = 0;
            break;
          }
          break;
        case ExcelRange:
          free(criteria);
          return VALUE;
      }
      if(passed == 0) break;
    }
    if(passed == 1) {
      current_value = original_range[j];
      if(current_value.type == ExcelError) {
        free(criteria);
        return current_value;
      } else if(current_value.type == ExcelNumber) {
        filtered_range[number_of_filtered_values] = current_value;
        number_of_filtered_values += 1;
      }
    }
  }
  // Tidy up
  free(criteria);
  return EXCEL_RANGE(filtered_range, number_of_filtered_values, 1);
}

static ExcelValue sumifs(ExcelValue sum_range_v, int number_of_arguments, ExcelValue *arguments) {
  ExcelValue filtered_range = filter_range(sum_range_v, number_of_arguments, arguments);
  return sum(1,&filtered_range);
}

static ExcelValue countifs(int number_of_arguments, ExcelValue *arguments) {
  if(number_of_arguments < 2) { return NA;}
  // Set up the sum range
  ExcelValue range = arguments[0];
  int rows, columns;

  if(range.type == ExcelRange) {
    rows = range.rows;
    columns = range.columns;
  } else {
    rows = 1;
    columns = 1;
  }

  int count = 0;

  // Then go through and set up the check ranges
  if(number_of_arguments % 2 != 0) return VALUE;
  int number_of_criteria = number_of_arguments / 2;
  ExcelValue *criteria_range =  (ExcelValue*) new_excel_value_array(number_of_criteria);
  ExcelValue current_value;
  int i;
  for(i = 0; i < number_of_criteria; i++) {
    current_value = arguments[i*2];
    if(current_value.type == ExcelRange) {
      criteria_range[i] = current_value;
      if(current_value.rows != rows) return VALUE;
      if(current_value.columns != columns) return VALUE;
    } else {
      if(rows != 1) return VALUE;
      if(columns != 1) return VALUE;
      ExcelValue *tmp_array2 =  (ExcelValue*) new_excel_value_array(1);
      tmp_array2[0] = current_value;
      criteria_range[i] = EXCEL_RANGE(tmp_array2,1,1);
    }
  }

  // Now go through and set up the criteria
  ExcelComparison *criteria =  malloc(sizeof(ExcelComparison)*number_of_criteria); // freed at end of function
  if(criteria == 0) {
	  printf("Out of memory in filter_range\n");
	  exit(-1);
  }
  char *s;
  char *new_comparator;

  for(i = 0; i < number_of_criteria; i++) {
    current_value = arguments[(i*2)+1];

    if(current_value.type == ExcelString) {
      s = current_value.string;
      if(s[0] == '<') {
        if( s[1] == '>') {
          new_comparator = strndup(s+2,strlen(s)-2);
          free_later(new_comparator);
          criteria[i].type = NotEqual;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        } else if(s[1] == '=') {
          new_comparator = strndup(s+2,strlen(s)-2);
          free_later(new_comparator);
          criteria[i].type = LessThanOrEqual;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        } else {
          new_comparator = strndup(s+1,strlen(s)-1);
          free_later(new_comparator);
          criteria[i].type = LessThan;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        }
      } else if(s[0] == '>') {
        if(s[1] == '=') {
          new_comparator = strndup(s+2,strlen(s)-2);
          free_later(new_comparator);
          criteria[i].type = MoreThanOrEqual;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        } else {
          new_comparator = strndup(s+1,strlen(s)-1);
          free_later(new_comparator);
          criteria[i].type = MoreThan;
          criteria[i].comparator = EXCEL_STRING(new_comparator);
        }
      } else if(s[0] == '=') {
        new_comparator = strndup(s+1,strlen(s)-1);
        free_later(new_comparator);
        criteria[i].type = Equal;
        criteria[i].comparator = EXCEL_STRING(new_comparator);
      } else {
        criteria[i].type = Equal;
        criteria[i].comparator = current_value;
      }
    } else {
      criteria[i].type = Equal;
      criteria[i].comparator = current_value;
    }
  }

  int size = columns * rows;
  int j;
  int passed = 0;
  ExcelValue value_to_be_checked;
  ExcelComparison comparison;
  ExcelValue comparator;
  double number;
  // For each cell in the sum range
  for(j=0; j < size; j++ ) {
    passed = 1;
    for(i=0; i < number_of_criteria; i++) {
      value_to_be_checked = ((ExcelValue *) ((ExcelValue) criteria_range[i]).array)[j];
      comparison = criteria[i];
      comparator = comparison.comparator;

      // For the purposes of comparison, treates a blank criteria as matching zeros.
      if(comparator.type == ExcelEmpty) {
        comparator = ZERO;
      }

      switch(value_to_be_checked.type) {
        case ExcelError: // Errors match only errors
          if(comparison.type != Equal) passed = 0;
          if(comparator.type != ExcelError) passed = 0;
          if(value_to_be_checked.number != comparator.number) passed = 0;
          break;
        case ExcelBoolean: // Booleans match only booleans (FIXME: I think?)
          if(comparison.type != Equal) passed = 0;
          if(comparator.type != ExcelBoolean ) passed = 0;
          if(value_to_be_checked.number != comparator.number) passed = 0;
          break;
        case ExcelEmpty:
          // if(comparator.type == ExcelEmpty) break; // FIXME: Huh? In excel blank doesn't match blank?!
          if(comparator.type != ExcelString) {
            passed = 0;
            break;
          } else {
            if(strlen(comparator.string) != 0) passed = 0; // Empty strings match blanks.
            break;
          }
        case ExcelNumber:
          if(comparator.type == ExcelNumber) {
            number = comparator.number;
          } else if(comparator.type == ExcelString) {
            number = number_from(comparator);
            if(conversion_error == 1) {
              conversion_error = 0;
              passed = 0;
              break;
            }
          } else {
            passed = 0;
            break;
          }
          switch(comparison.type) {
            case Equal:
              if(value_to_be_checked.number != number) passed = 0;
              break;
            case LessThan:
              if(value_to_be_checked.number >= number) passed = 0;
              break;
            case LessThanOrEqual:
              if(value_to_be_checked.number > number) passed = 0;
              break;
            case NotEqual:
              if(value_to_be_checked.number == number) passed = 0;
              break;
            case MoreThanOrEqual:
              if(value_to_be_checked.number < number) passed = 0;
              break;
            case MoreThan:
              if(value_to_be_checked.number <= number) passed = 0;
              break;
          }
          break;
        case ExcelString:
          // First case, the comparator is a number, simplification is that it can only be equal
          if(comparator.type == ExcelNumber) {
            if(comparison.type != Equal) {
              printf("This shouldn't be possible?");
              passed = 0;
              break;
            }

            // Special case, empty strings don't match zeros here
            if(strlen(value_to_be_checked.string) == 0) {
              passed = 0;
              break;
            }

            number = number_from(value_to_be_checked);
            if(conversion_error == 1) {
              conversion_error = 0;
              passed = 0;
              break;
            }
            if(number != comparator.number) {
              passed = 0;
              break;
            } else {
              break;
            }
          // Second case, the comparator is also a string, so need to be able to do full range of tests
          } else if(comparator.type == ExcelString) {
            switch(comparison.type) {
              case Equal:
                if(excel_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case LessThan:
                if(less_than(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case LessThanOrEqual:
                if(less_than_or_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case NotEqual:
                if(not_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case MoreThanOrEqual:
                if(more_than_or_equal(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              case MoreThan:
                if(more_than(value_to_be_checked,comparator).number == 0) passed = 0;
                break;
              }
          } else {
            passed = 0;
            break;
          }
          break;
        case ExcelRange:
          free(criteria);
          return VALUE;
      }
      if(passed == 0) break;
    }
    if(passed == 1) {
        count += 1;
    }
  }
  // Tidy up
  free(criteria);
  return EXCEL_NUMBER(count);
}

static ExcelValue averageifs(ExcelValue average_range_v, int number_of_arguments, ExcelValue *arguments) {
  ExcelValue filtered_range = filter_range(average_range_v, number_of_arguments, arguments);
  return average(1,&filtered_range);
}

static ExcelValue sumif(ExcelValue check_range_v, ExcelValue criteria_v, ExcelValue sum_range_v ) {
	ExcelValue tmp_array_sumif[] = {check_range_v, criteria_v};
	return sumifs(sum_range_v,2,tmp_array_sumif);
}

static ExcelValue sumif_2(ExcelValue check_range_v, ExcelValue criteria_v) {
	ExcelValue tmp_array_sumif2[] = {check_range_v, criteria_v};
	return sumifs(check_range_v,2,tmp_array_sumif2);
}

static ExcelValue sumproduct(int number_of_arguments, ExcelValue *arguments) {
  if(number_of_arguments <1) return VALUE;

  int a;
  int i;
  int j;
  int rows;
  int columns;
  ExcelValue current_value;
  ExcelValue **ranges = malloc(sizeof(ExcelValue *)*number_of_arguments);
  if(ranges == 0) {
	  printf("Out of memory in sumproduct\n");
	  exit(-1);
  }
  double product = 1;
  double sum = 0;

  // Find out dimensions of first argument
  if(arguments[0].type == ExcelRange) {
    rows = arguments[0].rows;
    columns = arguments[0].columns;
  } else {
    rows = 1;
    columns = 1;
  }
  // Extract arrays from each of the given ranges, checking for errors and bounds as we go
  for(a=0;a<number_of_arguments;a++) {
    current_value = arguments[a];
    switch(current_value.type) {
      case ExcelRange:
        if(current_value.rows != rows || current_value.columns != columns) { free(ranges);  return VALUE; }
        ranges[a] = current_value.array;
        break;
      case ExcelError:
		    free(ranges);
        return current_value;
        break;
      case ExcelEmpty:
		    free(ranges);
        return VALUE;
        break;
      default:
        if(rows != 1 && columns !=1) { free(ranges); return VALUE; }
        ranges[a] = (ExcelValue*) new_excel_value_array(1);
        ranges[a][0] = arguments[a];
        break;
    }
  }

	for(i=0;i<rows;i++) {
		for(j=0;j<columns;j++) {
			product = 1;
			for(a=0;a<number_of_arguments;a++) {
				current_value = ranges[a][(i*columns)+j];
				if(current_value.type == ExcelNumber) {
					product *= current_value.number;
				} else {
					product *= 0;
				}
			}
			sum += product;
		}
	}
	free(ranges);
  return EXCEL_NUMBER(sum);
}

static ExcelValue product(int number_of_arguments, ExcelValue *arguments) {
  if(number_of_arguments <1) return VALUE;

  int a,b;
  ExcelValue sub_total;
  ExcelValue current_value;
  int sub_total_array_size;
  ExcelValue *sub_total_array;
  ExcelValue sub_total_value;
  double total = 0;

  // Extract arrays from each of the given ranges, checking for errors and bounds as we go
  for(a=0;a<number_of_arguments;a++) {
    current_value = arguments[a];
    switch(current_value.type) {
      case ExcelRange:
        sub_total_array_size = current_value.rows * current_value.columns;
        sub_total_array = current_value.array;
        // We don't use recursion, because we need to check if
        // the result is 0 becaues a zero, or zero because all blank.
        for(b=0;b<sub_total_array_size;b++) {
          sub_total_value = sub_total_array[b];
          switch(sub_total_value.type) {
            case ExcelError:
              return sub_total_value;
              break;

            case ExcelNumber:
              // We do this rather than starting with total = 1
              // so that the product of all blanks is zero
              if(total == 0) {
                total = sub_total_value.number;
              } else {
                total *= sub_total_value.number;
              }
              break;

            default:
              // Skip
              break;
          }
        }
        break;

      case ExcelError:
        return current_value;
        break;

      case ExcelNumber:
        if(total == 0) {
          total = current_value.number;
        } else {
          total *= current_value.number;
        }
        break;

      default:
        // Skip
        break;
    }
  }

  return EXCEL_NUMBER(total);
}

// FIXME: This could do with being done properly, rather than
// on a case by case basis.
static ExcelValue text(ExcelValue number_v, ExcelValue format_v) {
  CHECK_FOR_PASSED_ERROR(number_v)
  CHECK_FOR_PASSED_ERROR(format_v)

	char *s;
	char *p;
	double n;
  ExcelValue result;

  if(number_v.type == ExcelEmpty) {
    number_v = ZERO;
  }

  if(format_v.type == ExcelEmpty) {
    return EXCEL_STRING("");
  }

  if(format_v.type == ExcelNumber && format_v.number == 0) {
    format_v = EXCEL_STRING("0");
  }

  if(number_v.type == ExcelString) {
 	 	s = number_v.string;
		if (s == NULL || *s == '\0' || isspace(*s)) {
			number_v = ZERO;
		}
		n = strtod (s, &p);
		if(*p == '\0') {
		  number_v = EXCEL_NUMBER(n);
		}
  }

  if(number_v.type != ExcelNumber) {
    return number_v;
  }

  if(format_v.type != ExcelString) {
    return format_v;
  }

  // FIXME: Too little?
  s = malloc(100);
  setlocale(LC_ALL,"");

  if(strcmp(format_v.string,"0%") == 0) {
    snprintf(s, 99, "%0.0f%%", number_v.number*100);
  } else if(strcmp(format_v.string,"0.0%") == 0) {
    snprintf(s, 99, "%0.1f%%", number_v.number*100);
  } else if(strcmp(format_v.string,"0") == 0) {
    snprintf(s, 99, "%0.0f",number_v.number);
  } else if(strcmp(format_v.string,"0.0") == 0) {
    snprintf(s, 99, "%0.1f",number_v.number);
  } else if(strcmp(format_v.string,"0.00") == 0) {
    snprintf(s, 99, "%0.2f",number_v.number);
  } else if(strcmp(format_v.string,"0.000") == 0) {
    snprintf(s, 99, "%0.3f",number_v.number);
  } else if(strcmp(format_v.string,"#,##") == 0) {
    snprintf(s, 99, "%'0.0f",number_v.number);
  } else if(strcmp(format_v.string,"#,##0") == 0) {
    snprintf(s, 99, "%'0.0f",number_v.number);
  } else if(strcmp(format_v.string,"#,##0.0") == 0) {
    snprintf(s, 99, "%'0.1f",number_v.number);
  } else if(strcmp(format_v.string,"#,##0.00") == 0) {
    snprintf(s, 99, "%'0.2f",number_v.number);
  } else if(strcmp(format_v.string,"#,##0.000") == 0) {
    snprintf(s, 99, "%'0.3f",number_v.number);
  } else if(strcmp(format_v.string,"0000") == 0) {
    snprintf(s, 99, "%04.0f",number_v.number);
  } else {
    snprintf(s, 99, "Text format not recognised");
  }

  free_later(s);
  result = EXCEL_STRING(s);
  // inspect_excel_value(result);
  return result;
}

static ExcelValue vlookup_3(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue column_number_v) {
  return vlookup(lookup_value_v,lookup_table_v,column_number_v,TRUE);
}

static ExcelValue vlookup(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue column_number_v, ExcelValue match_type_v) {
  CHECK_FOR_PASSED_ERROR(lookup_value_v)
  CHECK_FOR_PASSED_ERROR(lookup_table_v)
  CHECK_FOR_PASSED_ERROR(column_number_v)
  CHECK_FOR_PASSED_ERROR(match_type_v)

  if(lookup_value_v.type == ExcelEmpty) return NA;
  if(lookup_table_v.type != ExcelRange) return NA;
  if(column_number_v.type != ExcelNumber) return NA;
  if(match_type_v.type == ExcelNumber && match_type_v.number >= 0 && match_type_v.number <= 1) {
    match_type_v.type = ExcelBoolean;
  }
  if(match_type_v.type != ExcelBoolean) return NA;

  int i;
  int last_good_match = 0;
  int rows = lookup_table_v.rows;
  int columns = lookup_table_v.columns;
  ExcelValue *array = lookup_table_v.array;
  ExcelValue possible_match_v;

  if(column_number_v.number > columns) return REF;
  if(column_number_v.number < 1) return VALUE;

  if(match_type_v.number == false) { // Exact match required
    for(i=0; i< rows; i++) {
      possible_match_v = array[i*columns];
      if(excel_equal(lookup_value_v,possible_match_v).number == true) {
        return array[(i*columns)+(((int) column_number_v.number) - 1)];
      }
    }
    return NA;
  } else { // Highest value that is less than or equal
    for(i=0; i< rows; i++) {
      possible_match_v = array[i*columns];
      if(lookup_value_v.type != possible_match_v.type) continue;
      if(more_than(possible_match_v,lookup_value_v).number == true) {
        if(i == 0) return NA;
        return array[((i-1)*columns)+(((int) column_number_v.number) - 1)];
      } else {
        last_good_match = i;
      }
    }
    return array[(last_good_match*columns)+(((int) column_number_v.number) - 1)];
  }
  return NA;
}

static ExcelValue hlookup_3(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue row_number_v) {
  return hlookup(lookup_value_v,lookup_table_v,row_number_v,TRUE);
}

static ExcelValue hlookup(ExcelValue lookup_value_v,ExcelValue lookup_table_v, ExcelValue row_number_v, ExcelValue match_type_v) {
  CHECK_FOR_PASSED_ERROR(lookup_value_v)
  CHECK_FOR_PASSED_ERROR(lookup_table_v)
  CHECK_FOR_PASSED_ERROR(row_number_v)
  CHECK_FOR_PASSED_ERROR(match_type_v)

  if(lookup_value_v.type == ExcelEmpty) return NA;
  if(lookup_table_v.type != ExcelRange) return NA;
  if(row_number_v.type != ExcelNumber) return NA;
  if(match_type_v.type == ExcelNumber && match_type_v.number >= 0 && match_type_v.number <= 1) {
    match_type_v.type = ExcelBoolean;
  }
  if(match_type_v.type != ExcelBoolean) return NA;

  int i;
  int last_good_match = 0;
  int rows = lookup_table_v.rows;
  int columns = lookup_table_v.columns;
  ExcelValue *array = lookup_table_v.array;
  ExcelValue possible_match_v;

  if(row_number_v.number > rows) return REF;
  if(row_number_v.number < 1) return VALUE;

  if(match_type_v.number == false) { // Exact match required
    for(i=0; i< columns; i++) {
      possible_match_v = array[i];
      if(excel_equal(lookup_value_v,possible_match_v).number == true) {
        return array[((((int) row_number_v.number)-1)*columns)+(i)];
      }
    }
    return NA;
  } else { // Highest value that is less than or equal
    for(i=0; i< columns; i++) {
      possible_match_v = array[i];
      if(lookup_value_v.type != possible_match_v.type) continue;
      if(more_than(possible_match_v,lookup_value_v).number == true) {
        if(i == 0) return NA;
        return array[((((int) row_number_v.number)-1)*columns)+(i-1)];
      } else {
        last_good_match = i;
      }
    }
    return array[((((int) row_number_v.number)-1)*columns)+(last_good_match)];
  }
  return NA;
}


static ExcelValue value(ExcelValue string_v) {
	CHECK_FOR_PASSED_ERROR(string_v)
	NUMBER(string_v, a)
	CHECK_FOR_CONVERSION_ERROR
	return EXCEL_NUMBER(a);
}

static ExcelValue scurve_4(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration) {
  ExcelValue startYear = EXCEL_NUMBER(2018);
  return scurve(currentYear, startValue, endValue, duration, startYear);
}

static ExcelValue halfscurve_4(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration) {
  ExcelValue startYear = EXCEL_NUMBER(2018);
  return halfscurve(currentYear, startValue, endValue, duration, startYear);
}

static ExcelValue lcurve_4(ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration) {
  ExcelValue startYear = EXCEL_NUMBER(2018);
  return lcurve(currentYear, startValue, endValue, duration, startYear);
}

static ExcelValue curve_5(ExcelValue curveType, ExcelValue currentYear, ExcelValue startValue, ExcelValue endValue, ExcelValue duration) {
  ExcelValue startYear = EXCEL_NUMBER(2018);
  return curve(curveType, currentYear, startValue, endValue, duration, startYear);
}

static ExcelValue scurve(ExcelValue currentYear_v, ExcelValue startValue_v, ExcelValue endValue_v, ExcelValue duration_v, ExcelValue startYear_v) {

	NUMBER(currentYear_v, currentYear)
	NUMBER(startValue_v, startValue)
	NUMBER(endValue_v, endValue)
	NUMBER(duration_v, duration)
	NUMBER(startYear_v, startYear)
	CHECK_FOR_CONVERSION_ERROR

  if(currentYear < startYear) {
    return startValue_v;
  }
  double x = (currentYear - startYear) / duration;
  double x0 = 0.0;
  double a = endValue - startValue;
  double sc = 0.999;
  double eps = 1.0 - sc;
  double mu = 0.5;
  double beta = (mu - 1.0) / log(1.0 / sc - 1);
  double scurve = a * (pow((exp(-(x - mu) / beta) + 1),-1) - pow((exp(-(x0 - mu) / beta) + 1),-1)) + startValue;
  return EXCEL_NUMBER(scurve);
}

static ExcelValue halfscurve(ExcelValue currentYear_v, ExcelValue startValue_v, ExcelValue endValue_v, ExcelValue duration_v, ExcelValue startYear_v) {

	NUMBER(currentYear_v, currentYear)
	NUMBER(startValue_v, startValue)
	NUMBER(endValue_v, endValue)
	NUMBER(duration_v, duration)
	NUMBER(startYear_v, startYear)
	CHECK_FOR_CONVERSION_ERROR

  if(currentYear < startYear) {
    return startValue_v;
  }

  ExcelValue newCurrentYear = EXCEL_NUMBER(currentYear + duration);
  ExcelValue newDuration = EXCEL_NUMBER(duration *2);
  ExcelValue result_v = scurve(newCurrentYear, startValue_v, endValue_v, newDuration, startYear_v);

	NUMBER(result_v, result)
	CHECK_FOR_CONVERSION_ERROR

  return EXCEL_NUMBER(result -((endValue - startValue)/2.0));
}

static ExcelValue lcurve(ExcelValue currentYear_v, ExcelValue startValue_v, ExcelValue endValue_v, ExcelValue duration_v, ExcelValue startYear_v) {

	NUMBER(currentYear_v, currentYear)
	NUMBER(startValue_v, startValue)
	NUMBER(endValue_v, endValue)
	NUMBER(duration_v, duration)
	NUMBER(startYear_v, startYear)
	CHECK_FOR_CONVERSION_ERROR

  if(currentYear > (startYear + duration)) {
    return endValue_v;
  }

  if(currentYear < startYear) {
    return startValue_v;
  }

  double result = startValue + (((endValue - startValue) / duration) * (currentYear - startYear));
  return EXCEL_NUMBER(result);
}

static ExcelValue curve(ExcelValue type_v, ExcelValue currentYear_v, ExcelValue startValue_v, ExcelValue endValue_v, ExcelValue duration_v, ExcelValue startYear_v) {

  if(type_v.type == ExcelString && strcasecmp(type_v.string, "s") == 0 ) {
    return scurve(currentYear_v, startValue_v, endValue_v, duration_v, startYear_v);
  }

  if(type_v.type == ExcelString && strcasecmp(type_v.string, "hs") == 0 ) {
    return halfscurve(currentYear_v, startValue_v, endValue_v, duration_v, startYear_v);
  }

  return lcurve(currentYear_v, startValue_v, endValue_v, duration_v, startYear_v);
}



// Allows numbers to be 0.1% different
static ExcelValue roughly_equal(ExcelValue a_v, ExcelValue b_v) {

  if(a_v.type == ExcelEmpty && b_v.type == ExcelNumber && b_v.number == 0) return TRUE;
  if(b_v.type == ExcelEmpty && a_v.type == ExcelNumber && a_v.number == 0) return TRUE;

	if(a_v.type != b_v.type) return FALSE;

  float epsilon, difference;

	switch (a_v.type) {
  	case ExcelNumber:
      // FIXME: Arbitrary choice of epsilons
      if(b_v.number > -1e-6 && b_v.number < 1e-6) {
        epsilon = 1e-6;
      } else {
        epsilon = b_v.number * 0.001;
      }
      if(epsilon < 0) epsilon = -epsilon;
      difference = a_v.number - b_v.number;
      if(difference < 0) difference = -difference;
      if(difference <= epsilon) return TRUE;
      // For debuging: printf("a: %e b:%e d: %e e: %e", a_v.number, b_v.number, difference, epsilon);
      return FALSE;
	  case ExcelBoolean:
	  case ExcelEmpty:
			if(a_v.number != b_v.number) return FALSE;
			return TRUE;
	  case ExcelString:
	  	if(strcasecmp(a_v.string,b_v.string) != 0 ) return FALSE;
		  return TRUE;
  	case ExcelError:
			if(a_v.number != b_v.number) return FALSE;
			return TRUE;
  	case ExcelRange:
  		return NA;
  }
  return FALSE;
}


static void assert_equal(ExcelValue expected, ExcelValue actual, char location[]) {
  ExcelValue comparison = roughly_equal(actual, expected);
  if(comparison.type == ExcelBoolean && comparison.number == 1) {
    putchar('.');
  } else {
    printf("\n\nFailed at %s\n", location);
    printf("Expected: ");
    inspect_excel_value(expected);
    printf("Got:      ");
    inspect_excel_value(actual);
    putchar('\n');
  }
}
// End of the generic c functions

// Start of the file specific functions

ExcelValue control_d17();
ExcelValue control_d18();
ExcelValue control_d19();
ExcelValue control_d20();
ExcelValue control_d21();
ExcelValue control_d22();
ExcelValue control_d23();
ExcelValue control_d24();
ExcelValue control_d25();
ExcelValue control_d26();
ExcelValue control_d27();
ExcelValue control_d28();
ExcelValue control_d29();
ExcelValue control_d30();
ExcelValue control_d31();
ExcelValue control_d32();
ExcelValue control_d33();
ExcelValue control_d34();
ExcelValue control_d35();
ExcelValue control_d36();
ExcelValue control_d37();
ExcelValue control_d38();
ExcelValue control_d39();
ExcelValue control_d40();
ExcelValue control_d41();
ExcelValue control_d42();
ExcelValue control_d43();
ExcelValue control_d44();
ExcelValue control_d45();
ExcelValue control_d46();
ExcelValue control_d47();
ExcelValue control_d48();
ExcelValue control_d49();
ExcelValue control_d50();
ExcelValue control_d51();
ExcelValue control_d52();
ExcelValue control_d53();
ExcelValue control_d54();
ExcelValue control_d55();
ExcelValue control_d56();
ExcelValue control_d57();
ExcelValue control_d58();
ExcelValue control_d59();
ExcelValue control_d60();
ExcelValue control_d61();
ExcelValue control_g17();
ExcelValue control_g18();
ExcelValue control_g19();
ExcelValue control_g20();
ExcelValue control_g21();
ExcelValue control_g22();
ExcelValue control_g23();
ExcelValue control_g24();
ExcelValue control_g25();
ExcelValue control_g26();
ExcelValue control_g27();
ExcelValue control_g28();
ExcelValue control_g29();
ExcelValue control_g30();
ExcelValue control_g31();
ExcelValue control_g32();
ExcelValue control_g33();
ExcelValue control_g34();
ExcelValue control_g35();
ExcelValue control_g36();
ExcelValue control_g37();
ExcelValue control_g38();
ExcelValue control_g39();
ExcelValue control_g40();
ExcelValue control_g41();
ExcelValue control_g42();
ExcelValue control_g43();
ExcelValue control_g44();
ExcelValue control_g45();
ExcelValue control_g46();
ExcelValue control_g47();
ExcelValue control_g48();
ExcelValue control_g49();
ExcelValue control_g50();
ExcelValue control_g51();
ExcelValue control_g52();
ExcelValue control_g53();
ExcelValue control_g54();
ExcelValue control_g55();
ExcelValue control_g56();
ExcelValue control_g57();
ExcelValue control_g58();
ExcelValue control_g59();
ExcelValue control_g60();
ExcelValue control_g61();
ExcelValue control_e17();
ExcelValue control_e18();
ExcelValue control_e19();
ExcelValue control_e20();
ExcelValue control_e21();
ExcelValue control_e22();
ExcelValue control_e23();
ExcelValue control_e24();
ExcelValue control_e25();
ExcelValue control_e26();
ExcelValue control_e27();
ExcelValue control_e28();
ExcelValue control_e29();
ExcelValue control_e30();
ExcelValue control_e31();
ExcelValue control_e32();
ExcelValue control_e33();
ExcelValue control_e34();
ExcelValue control_e35();
ExcelValue control_e36();
ExcelValue control_e37();
ExcelValue control_e38();
ExcelValue control_e39();
ExcelValue control_e40();
ExcelValue control_e41();
ExcelValue control_e42();
ExcelValue control_e43();
ExcelValue control_e44();
ExcelValue control_e45();
ExcelValue control_e46();
ExcelValue control_e47();
ExcelValue control_e48();
ExcelValue control_e49();
ExcelValue control_e50();
ExcelValue control_e51();
ExcelValue control_e52();
ExcelValue control_e53();
ExcelValue control_e54();
ExcelValue control_e55();
ExcelValue control_e56();
ExcelValue control_e57();
ExcelValue control_e58();
ExcelValue control_e59();
ExcelValue control_e60();
ExcelValue control_e61();
ExcelValue control_e62();
ExcelValue control_i16();
ExcelValue control_i17();
ExcelValue control_i18();
ExcelValue control_i19();
ExcelValue control_i20();
ExcelValue control_i21();
ExcelValue control_i22();
ExcelValue control_i23();
ExcelValue control_i24();
ExcelValue control_i25();
ExcelValue control_i26();
ExcelValue control_i27();
ExcelValue control_i28();
ExcelValue control_i29();
ExcelValue control_i30();
ExcelValue control_i31();
ExcelValue control_i32();
ExcelValue control_i33();
ExcelValue control_i34();
ExcelValue control_i35();
ExcelValue control_i36();
ExcelValue control_i37();
ExcelValue control_i38();
ExcelValue control_i39();
ExcelValue control_i40();
ExcelValue control_i41();
ExcelValue control_i42();
ExcelValue control_i43();
ExcelValue control_i44();
ExcelValue control_i45();
ExcelValue control_i46();
ExcelValue control_i47();
ExcelValue control_i48();
ExcelValue control_i49();
ExcelValue control_i50();
ExcelValue control_i51();
ExcelValue control_i52();
ExcelValue control_i53();
ExcelValue control_i54();
ExcelValue control_i55();
ExcelValue control_i56();
ExcelValue control_i57();
ExcelValue control_i58();
ExcelValue control_i59();
ExcelValue control_i60();
ExcelValue control_i61();
ExcelValue control_i62();
ExcelValue control_i63();
ExcelValue control_i64();
ExcelValue control_ef17();
ExcelValue control_eg17();
ExcelValue control_eh17();
ExcelValue control_ei17();
ExcelValue control_ef18();
ExcelValue control_eg18();
ExcelValue control_eh18();
ExcelValue control_ei18();
ExcelValue control_ef19();
ExcelValue control_eg19();
ExcelValue control_eh19();
ExcelValue control_ei19();
ExcelValue control_ef20();
ExcelValue control_eg20();
ExcelValue control_eh20();
ExcelValue control_ei20();
ExcelValue control_ef21();
ExcelValue control_eg21();
ExcelValue control_eh21();
ExcelValue control_ei21();
ExcelValue control_ef22();
ExcelValue control_eg22();
ExcelValue control_eh22();
ExcelValue control_ei22();
ExcelValue control_ef23();
ExcelValue control_eg23();
ExcelValue control_eh23();
ExcelValue control_ei23();
ExcelValue control_ef24();
ExcelValue control_eg24();
ExcelValue control_eh24();
ExcelValue control_ei24();
ExcelValue control_ef25();
ExcelValue control_eg25();
ExcelValue control_eh25();
ExcelValue control_ei25();
ExcelValue control_ef26();
ExcelValue control_eg26();
ExcelValue control_eh26();
ExcelValue control_ei26();
ExcelValue control_ef27();
ExcelValue control_eg27();
ExcelValue control_eh27();
ExcelValue control_ei27();
ExcelValue control_ef28();
ExcelValue control_eg28();
ExcelValue control_eh28();
ExcelValue control_ei28();
ExcelValue control_ef29();
ExcelValue control_eg29();
ExcelValue control_eh29();
ExcelValue control_ei29();
ExcelValue control_ef30();
ExcelValue control_eg30();
ExcelValue control_eh30();
ExcelValue control_ei30();
ExcelValue control_ef31();
ExcelValue control_eg31();
ExcelValue control_eh31();
ExcelValue control_ei31();
ExcelValue control_ef32();
ExcelValue control_eg32();
ExcelValue control_eh32();
ExcelValue control_ei32();
ExcelValue control_ef33();
ExcelValue control_eg33();
ExcelValue control_eh33();
ExcelValue control_ei33();
ExcelValue control_ef34();
ExcelValue control_eg34();
ExcelValue control_eh34();
ExcelValue control_ei34();
ExcelValue control_ef35();
ExcelValue control_eg35();
ExcelValue control_eh35();
ExcelValue control_ei35();
ExcelValue control_ef36();
ExcelValue control_eg36();
ExcelValue control_eh36();
ExcelValue control_ei36();
ExcelValue control_ef37();
ExcelValue control_eg37();
ExcelValue control_eh37();
ExcelValue control_ei37();
ExcelValue control_ef38();
ExcelValue control_eg38();
ExcelValue control_eh38();
ExcelValue control_ei38();
ExcelValue control_ef39();
ExcelValue control_eg39();
ExcelValue control_eh39();
ExcelValue control_ei39();
ExcelValue control_ef40();
ExcelValue control_eg40();
ExcelValue control_eh40();
ExcelValue control_ei40();
ExcelValue control_ef41();
ExcelValue control_eg41();
ExcelValue control_eh41();
ExcelValue control_ei41();
ExcelValue control_ef42();
ExcelValue control_eg42();
ExcelValue control_eh42();
ExcelValue control_ei42();
ExcelValue control_ef43();
ExcelValue control_eg43();
ExcelValue control_eh43();
ExcelValue control_ei43();
ExcelValue control_ef44();
ExcelValue control_eg44();
ExcelValue control_eh44();
ExcelValue control_ei44();
ExcelValue control_ef45();
ExcelValue control_eg45();
ExcelValue control_eh45();
ExcelValue control_ei45();
ExcelValue control_ef46();
ExcelValue control_eg46();
ExcelValue control_eh46();
ExcelValue control_ei46();
ExcelValue control_ef47();
ExcelValue control_eg47();
ExcelValue control_eh47();
ExcelValue control_ei47();
ExcelValue control_ef48();
ExcelValue control_eg48();
ExcelValue control_eh48();
ExcelValue control_ei48();
ExcelValue control_ef49();
ExcelValue control_eg49();
ExcelValue control_eh49();
ExcelValue control_ei49();
ExcelValue control_ef50();
ExcelValue control_eg50();
ExcelValue control_eh50();
ExcelValue control_ei50();
ExcelValue control_ef51();
ExcelValue control_eg51();
ExcelValue control_eh51();
ExcelValue control_ei51();
ExcelValue control_ef52();
ExcelValue control_eg52();
ExcelValue control_eh52();
ExcelValue control_ei52();
ExcelValue control_ef53();
ExcelValue control_eg53();
ExcelValue control_eh53();
ExcelValue control_ei53();
ExcelValue control_ef54();
ExcelValue control_eg54();
ExcelValue control_eh54();
ExcelValue control_ei54();
ExcelValue control_ef55();
ExcelValue control_eg55();
ExcelValue control_eh55();
ExcelValue control_ei55();
ExcelValue control_ef56();
ExcelValue control_eg56();
ExcelValue control_eh56();
ExcelValue control_ei56();
ExcelValue control_ef57();
ExcelValue control_eg57();
ExcelValue control_eh57();
ExcelValue control_ei57();
ExcelValue control_ef58();
ExcelValue control_eg58();
ExcelValue control_eh58();
ExcelValue control_ei58();
ExcelValue control_ef59();
ExcelValue control_eg59();
ExcelValue control_eh59();
ExcelValue control_ei59();
ExcelValue control_ef60();
ExcelValue control_eg60();
ExcelValue control_eh60();
ExcelValue control_ei60();
ExcelValue control_ef61();
ExcelValue control_eg61();
ExcelValue control_eh61();
ExcelValue control_ei61();
ExcelValue control_ef62();
ExcelValue control_eg62();
ExcelValue control_eh62();
ExcelValue control_ei62();
ExcelValue control_c17();
ExcelValue control_c18();
ExcelValue control_c19();
ExcelValue control_c20();
ExcelValue control_c21();
ExcelValue control_c22();
ExcelValue control_c23();
ExcelValue control_c24();
ExcelValue control_c25();
ExcelValue control_c26();
ExcelValue control_c27();
ExcelValue control_c28();
ExcelValue control_c29();
ExcelValue control_c30();
ExcelValue control_c31();
ExcelValue control_c32();
ExcelValue control_c33();
ExcelValue control_c34();
ExcelValue control_c35();
ExcelValue control_c36();
ExcelValue control_c37();
ExcelValue control_c38();
ExcelValue control_c39();
ExcelValue control_c40();
ExcelValue control_c41();
ExcelValue control_c42();
ExcelValue control_c43();
ExcelValue control_c44();
ExcelValue control_c45();
ExcelValue control_c46();
ExcelValue control_c47();
ExcelValue control_c48();
ExcelValue control_c49();
ExcelValue control_c50();
ExcelValue control_c51();
ExcelValue control_c52();
ExcelValue control_c53();
ExcelValue control_c54();
ExcelValue control_c55();
ExcelValue control_c56();
ExcelValue control_c57();
ExcelValue control_c58();
ExcelValue control_c59();
ExcelValue control_c60();
ExcelValue control_c61();
ExcelValue control_c62();
ExcelValue control_h17();
ExcelValue control_h18();
ExcelValue control_h19();
ExcelValue control_h20();
ExcelValue control_h21();
ExcelValue control_h22();
ExcelValue control_h23();
ExcelValue control_h24();
ExcelValue control_h25();
ExcelValue control_h26();
ExcelValue control_h27();
ExcelValue control_h28();
ExcelValue control_h29();
ExcelValue control_h30();
ExcelValue control_h31();
ExcelValue control_h32();
ExcelValue control_h33();
ExcelValue control_h34();
ExcelValue control_h35();
ExcelValue control_h36();
ExcelValue control_h37();
ExcelValue control_h38();
ExcelValue control_h39();
ExcelValue control_h40();
ExcelValue control_h41();
ExcelValue control_h42();
ExcelValue control_h43();
ExcelValue control_h44();
ExcelValue control_h45();
ExcelValue control_h46();
ExcelValue control_h47();
ExcelValue control_h48();
ExcelValue control_h49();
ExcelValue control_h50();
ExcelValue control_h51();
ExcelValue control_h52();
ExcelValue control_h53();
ExcelValue control_h54();
ExcelValue control_h55();
ExcelValue control_h56();
ExcelValue control_h57();
ExcelValue control_h58();
ExcelValue control_h59();
ExcelValue control_h60();
ExcelValue control_h61();
ExcelValue control_h62();
ExcelValue results_e583();
ExcelValue results_f583();
ExcelValue results_g583();
ExcelValue results_h583();
ExcelValue results_i583();
ExcelValue results_j583();
ExcelValue results_k583();
ExcelValue results_l583();
ExcelValue results_m583();
ExcelValue results_n583();
ExcelValue results_o583();
ExcelValue results_p583();
ExcelValue results_q583();
ExcelValue results_r583();
ExcelValue results_s583();
ExcelValue results_t583();
ExcelValue results_u583();
ExcelValue results_v583();
ExcelValue results_w583();
ExcelValue results_e584();
ExcelValue results_f584();
ExcelValue results_g584();
ExcelValue results_h584();
ExcelValue results_i584();
ExcelValue results_j584();
ExcelValue results_k584();
ExcelValue results_l584();
ExcelValue results_m584();
ExcelValue results_n584();
ExcelValue results_o584();
ExcelValue results_p584();
ExcelValue results_q584();
ExcelValue results_r584();
ExcelValue results_s584();
ExcelValue results_t584();
ExcelValue results_u584();
ExcelValue results_v584();
ExcelValue results_w584();
ExcelValue results_e585();
ExcelValue results_f585();
ExcelValue results_g585();
ExcelValue results_h585();
ExcelValue results_i585();
ExcelValue results_j585();
ExcelValue results_k585();
ExcelValue results_l585();
ExcelValue results_m585();
ExcelValue results_n585();
ExcelValue results_o585();
ExcelValue results_p585();
ExcelValue results_q585();
ExcelValue results_r585();
ExcelValue results_s585();
ExcelValue results_t585();
ExcelValue results_u585();
ExcelValue results_v585();
ExcelValue results_w585();
ExcelValue results_e586();
ExcelValue results_f586();
ExcelValue results_g586();
ExcelValue results_h586();
ExcelValue results_i586();
ExcelValue results_j586();
ExcelValue results_k586();
ExcelValue results_l586();
ExcelValue results_m586();
ExcelValue results_n586();
ExcelValue results_o586();
ExcelValue results_p586();
ExcelValue results_q586();
ExcelValue results_r586();
ExcelValue results_s586();
ExcelValue results_t586();
ExcelValue results_u586();
ExcelValue results_v586();
ExcelValue results_w586();
ExcelValue results_e587();
ExcelValue results_f587();
ExcelValue results_g587();
ExcelValue results_h587();
ExcelValue results_i587();
ExcelValue results_j587();
ExcelValue results_k587();
ExcelValue results_l587();
ExcelValue results_m587();
ExcelValue results_n587();
ExcelValue results_o587();
ExcelValue results_p587();
ExcelValue results_q587();
ExcelValue results_r587();
ExcelValue results_s587();
ExcelValue results_t587();
ExcelValue results_u587();
ExcelValue results_v587();
ExcelValue results_w587();
ExcelValue results_e588();
ExcelValue results_f588();
ExcelValue results_g588();
ExcelValue results_h588();
ExcelValue results_i588();
ExcelValue results_j588();
ExcelValue results_k588();
ExcelValue results_l588();
ExcelValue results_m588();
ExcelValue results_n588();
ExcelValue results_o588();
ExcelValue results_p588();
ExcelValue results_q588();
ExcelValue results_r588();
ExcelValue results_s588();
ExcelValue results_t588();
ExcelValue results_u588();
ExcelValue results_v588();
ExcelValue results_w588();
ExcelValue results_e589();
ExcelValue results_f589();
ExcelValue results_g589();
ExcelValue results_h589();
ExcelValue results_i589();
ExcelValue results_j589();
ExcelValue results_k589();
ExcelValue results_l589();
ExcelValue results_m589();
ExcelValue results_n589();
ExcelValue results_o589();
ExcelValue results_p589();
ExcelValue results_q589();
ExcelValue results_r589();
ExcelValue results_s589();
ExcelValue results_t589();
ExcelValue results_u589();
ExcelValue results_v589();
ExcelValue results_w589();
ExcelValue results_e590();
ExcelValue results_f590();
ExcelValue results_g590();
ExcelValue results_h590();
ExcelValue results_i590();
ExcelValue results_j590();
ExcelValue results_k590();
ExcelValue results_l590();
ExcelValue results_m590();
ExcelValue results_n590();
ExcelValue results_o590();
ExcelValue results_p590();
ExcelValue results_q590();
ExcelValue results_r590();
ExcelValue results_s590();
ExcelValue results_t590();
ExcelValue results_u590();
ExcelValue results_v590();
ExcelValue results_w590();
ExcelValue results_e591();
ExcelValue results_f591();
ExcelValue results_g591();
ExcelValue results_h591();
ExcelValue results_i591();
ExcelValue results_j591();
ExcelValue results_k591();
ExcelValue results_l591();
ExcelValue results_m591();
ExcelValue results_n591();
ExcelValue results_o591();
ExcelValue results_p591();
ExcelValue results_q591();
ExcelValue results_r591();
ExcelValue results_s591();
ExcelValue results_t591();
ExcelValue results_u591();
ExcelValue results_v591();
ExcelValue results_w591();
ExcelValue results_e526();
ExcelValue results_f526();
ExcelValue results_g526();
ExcelValue results_h526();
ExcelValue results_i526();
ExcelValue results_j526();
ExcelValue results_k526();
ExcelValue results_l526();
ExcelValue results_m526();
ExcelValue results_n526();
ExcelValue results_o526();
ExcelValue results_p526();
ExcelValue results_q526();
ExcelValue results_r526();
ExcelValue results_s526();
ExcelValue results_t526();
ExcelValue results_u526();
ExcelValue results_v526();
ExcelValue results_w526();
ExcelValue results_e527();
ExcelValue results_f527();
ExcelValue results_g527();
ExcelValue results_h527();
ExcelValue results_i527();
ExcelValue results_j527();
ExcelValue results_k527();
ExcelValue results_l527();
ExcelValue results_m527();
ExcelValue results_n527();
ExcelValue results_o527();
ExcelValue results_p527();
ExcelValue results_q527();
ExcelValue results_r527();
ExcelValue results_s527();
ExcelValue results_t527();
ExcelValue results_u527();
ExcelValue results_v527();
ExcelValue results_w527();
ExcelValue results_e528();
ExcelValue results_f528();
ExcelValue results_g528();
ExcelValue results_h528();
ExcelValue results_i528();
ExcelValue results_j528();
ExcelValue results_k528();
ExcelValue results_l528();
ExcelValue results_m528();
ExcelValue results_n528();
ExcelValue results_o528();
ExcelValue results_p528();
ExcelValue results_q528();
ExcelValue results_r528();
ExcelValue results_s528();
ExcelValue results_t528();
ExcelValue results_u528();
ExcelValue results_v528();
ExcelValue results_w528();
ExcelValue results_e529();
ExcelValue results_f529();
ExcelValue results_g529();
ExcelValue results_h529();
ExcelValue results_i529();
ExcelValue results_j529();
ExcelValue results_k529();
ExcelValue results_l529();
ExcelValue results_m529();
ExcelValue results_n529();
ExcelValue results_o529();
ExcelValue results_p529();
ExcelValue results_q529();
ExcelValue results_r529();
ExcelValue results_s529();
ExcelValue results_t529();
ExcelValue results_u529();
ExcelValue results_v529();
ExcelValue results_w529();
ExcelValue results_e530();
ExcelValue results_f530();
ExcelValue results_g530();
ExcelValue results_h530();
ExcelValue results_i530();
ExcelValue results_j530();
ExcelValue results_k530();
ExcelValue results_l530();
ExcelValue results_m530();
ExcelValue results_n530();
ExcelValue results_o530();
ExcelValue results_p530();
ExcelValue results_q530();
ExcelValue results_r530();
ExcelValue results_s530();
ExcelValue results_t530();
ExcelValue results_u530();
ExcelValue results_v530();
ExcelValue results_w530();
ExcelValue results_e531();
ExcelValue results_f531();
ExcelValue results_g531();
ExcelValue results_h531();
ExcelValue results_i531();
ExcelValue results_j531();
ExcelValue results_k531();
ExcelValue results_l531();
ExcelValue results_m531();
ExcelValue results_n531();
ExcelValue results_o531();
ExcelValue results_p531();
ExcelValue results_q531();
ExcelValue results_r531();
ExcelValue results_s531();
ExcelValue results_t531();
ExcelValue results_u531();
ExcelValue results_v531();
ExcelValue results_w531();
ExcelValue results_e532();
ExcelValue results_f532();
ExcelValue results_g532();
ExcelValue results_h532();
ExcelValue results_i532();
ExcelValue results_j532();
ExcelValue results_k532();
ExcelValue results_l532();
ExcelValue results_m532();
ExcelValue results_n532();
ExcelValue results_o532();
ExcelValue results_p532();
ExcelValue results_q532();
ExcelValue results_r532();
ExcelValue results_s532();
ExcelValue results_t532();
ExcelValue results_u532();
ExcelValue results_v532();
ExcelValue results_w532();
ExcelValue results_e533();
ExcelValue results_f533();
ExcelValue results_g533();
ExcelValue results_h533();
ExcelValue results_i533();
ExcelValue results_j533();
ExcelValue results_k533();
ExcelValue results_l533();
ExcelValue results_m533();
ExcelValue results_n533();
ExcelValue results_o533();
ExcelValue results_p533();
ExcelValue results_q533();
ExcelValue results_r533();
ExcelValue results_s533();
ExcelValue results_t533();
ExcelValue results_u533();
ExcelValue results_v533();
ExcelValue results_w533();
ExcelValue results_e534();
ExcelValue results_f534();
ExcelValue results_g534();
ExcelValue results_h534();
ExcelValue results_i534();
ExcelValue results_j534();
ExcelValue results_k534();
ExcelValue results_l534();
ExcelValue results_m534();
ExcelValue results_n534();
ExcelValue results_o534();
ExcelValue results_p534();
ExcelValue results_q534();
ExcelValue results_r534();
ExcelValue results_s534();
ExcelValue results_t534();
ExcelValue results_u534();
ExcelValue results_v534();
ExcelValue results_w534();
ExcelValue results_e535();
ExcelValue results_f535();
ExcelValue results_g535();
ExcelValue results_h535();
ExcelValue results_i535();
ExcelValue results_j535();
ExcelValue results_k535();
ExcelValue results_l535();
ExcelValue results_m535();
ExcelValue results_n535();
ExcelValue results_o535();
ExcelValue results_p535();
ExcelValue results_q535();
ExcelValue results_r535();
ExcelValue results_s535();
ExcelValue results_t535();
ExcelValue results_u535();
ExcelValue results_v535();
ExcelValue results_w535();
ExcelValue results_e34();
ExcelValue results_f34();
ExcelValue results_g34();
ExcelValue results_h34();
ExcelValue results_i34();
ExcelValue results_j34();
ExcelValue results_k34();
ExcelValue results_l34();
ExcelValue results_m34();
ExcelValue results_n34();
ExcelValue results_o34();
ExcelValue results_p34();
ExcelValue results_q34();
ExcelValue results_r34();
ExcelValue results_s34();
ExcelValue results_t34();
ExcelValue results_u34();
ExcelValue results_v34();
ExcelValue results_w34();
ExcelValue results_e35();
ExcelValue results_f35();
ExcelValue results_g35();
ExcelValue results_h35();
ExcelValue results_i35();
ExcelValue results_j35();
ExcelValue results_k35();
ExcelValue results_l35();
ExcelValue results_m35();
ExcelValue results_n35();
ExcelValue results_o35();
ExcelValue results_p35();
ExcelValue results_q35();
ExcelValue results_r35();
ExcelValue results_s35();
ExcelValue results_t35();
ExcelValue results_u35();
ExcelValue results_v35();
ExcelValue results_w35();
ExcelValue results_e36();
ExcelValue results_f36();
ExcelValue results_g36();
ExcelValue results_h36();
ExcelValue results_i36();
ExcelValue results_j36();
ExcelValue results_k36();
ExcelValue results_l36();
ExcelValue results_m36();
ExcelValue results_n36();
ExcelValue results_o36();
ExcelValue results_p36();
ExcelValue results_q36();
ExcelValue results_r36();
ExcelValue results_s36();
ExcelValue results_t36();
ExcelValue results_u36();
ExcelValue results_v36();
ExcelValue results_w36();
ExcelValue results_e37();
ExcelValue results_f37();
ExcelValue results_g37();
ExcelValue results_h37();
ExcelValue results_i37();
ExcelValue results_j37();
ExcelValue results_k37();
ExcelValue results_l37();
ExcelValue results_m37();
ExcelValue results_n37();
ExcelValue results_o37();
ExcelValue results_p37();
ExcelValue results_q37();
ExcelValue results_r37();
ExcelValue results_s37();
ExcelValue results_t37();
ExcelValue results_u37();
ExcelValue results_v37();
ExcelValue results_w37();
ExcelValue results_e38();
ExcelValue results_f38();
ExcelValue results_g38();
ExcelValue results_h38();
ExcelValue results_i38();
ExcelValue results_j38();
ExcelValue results_k38();
ExcelValue results_l38();
ExcelValue results_m38();
ExcelValue results_n38();
ExcelValue results_o38();
ExcelValue results_p38();
ExcelValue results_q38();
ExcelValue results_r38();
ExcelValue results_s38();
ExcelValue results_t38();
ExcelValue results_u38();
ExcelValue results_v38();
ExcelValue results_w38();
ExcelValue results_e39();
ExcelValue results_f39();
ExcelValue results_g39();
ExcelValue results_h39();
ExcelValue results_i39();
ExcelValue results_j39();
ExcelValue results_k39();
ExcelValue results_l39();
ExcelValue results_m39();
ExcelValue results_n39();
ExcelValue results_o39();
ExcelValue results_p39();
ExcelValue results_q39();
ExcelValue results_r39();
ExcelValue results_s39();
ExcelValue results_t39();
ExcelValue results_u39();
ExcelValue results_v39();
ExcelValue results_w39();
ExcelValue results_e40();
ExcelValue results_f40();
ExcelValue results_g40();
ExcelValue results_h40();
ExcelValue results_i40();
ExcelValue results_j40();
ExcelValue results_k40();
ExcelValue results_l40();
ExcelValue results_m40();
ExcelValue results_n40();
ExcelValue results_o40();
ExcelValue results_p40();
ExcelValue results_q40();
ExcelValue results_r40();
ExcelValue results_s40();
ExcelValue results_t40();
ExcelValue results_u40();
ExcelValue results_v40();
ExcelValue results_w40();
ExcelValue results_e41();
ExcelValue results_f41();
ExcelValue results_g41();
ExcelValue results_h41();
ExcelValue results_i41();
ExcelValue results_j41();
ExcelValue results_k41();
ExcelValue results_l41();
ExcelValue results_m41();
ExcelValue results_n41();
ExcelValue results_o41();
ExcelValue results_p41();
ExcelValue results_q41();
ExcelValue results_r41();
ExcelValue results_s41();
ExcelValue results_t41();
ExcelValue results_u41();
ExcelValue results_v41();
ExcelValue results_w41();
ExcelValue results_e42();
ExcelValue results_f42();
ExcelValue results_g42();
ExcelValue results_h42();
ExcelValue results_i42();
ExcelValue results_j42();
ExcelValue results_k42();
ExcelValue results_l42();
ExcelValue results_m42();
ExcelValue results_n42();
ExcelValue results_o42();
ExcelValue results_p42();
ExcelValue results_q42();
ExcelValue results_r42();
ExcelValue results_s42();
ExcelValue results_t42();
ExcelValue results_u42();
ExcelValue results_v42();
ExcelValue results_w42();
ExcelValue results_e43();
ExcelValue results_f43();
ExcelValue results_g43();
ExcelValue results_h43();
ExcelValue results_i43();
ExcelValue results_j43();
ExcelValue results_k43();
ExcelValue results_l43();
ExcelValue results_m43();
ExcelValue results_n43();
ExcelValue results_o43();
ExcelValue results_p43();
ExcelValue results_q43();
ExcelValue results_r43();
ExcelValue results_s43();
ExcelValue results_t43();
ExcelValue results_u43();
ExcelValue results_v43();
ExcelValue results_w43();
ExcelValue results_e44();
ExcelValue results_f44();
ExcelValue results_g44();
ExcelValue results_h44();
ExcelValue results_i44();
ExcelValue results_j44();
ExcelValue results_k44();
ExcelValue results_l44();
ExcelValue results_m44();
ExcelValue results_n44();
ExcelValue results_o44();
ExcelValue results_p44();
ExcelValue results_q44();
ExcelValue results_r44();
ExcelValue results_s44();
ExcelValue results_t44();
ExcelValue results_u44();
ExcelValue results_v44();
ExcelValue results_w44();
ExcelValue results_e45();
ExcelValue results_f45();
ExcelValue results_g45();
ExcelValue results_h45();
ExcelValue results_i45();
ExcelValue results_j45();
ExcelValue results_k45();
ExcelValue results_l45();
ExcelValue results_m45();
ExcelValue results_n45();
ExcelValue results_o45();
ExcelValue results_p45();
ExcelValue results_q45();
ExcelValue results_r45();
ExcelValue results_s45();
ExcelValue results_t45();
ExcelValue results_u45();
ExcelValue results_v45();
ExcelValue results_w45();
ExcelValue results_e46();
ExcelValue results_f46();
ExcelValue results_g46();
ExcelValue results_h46();
ExcelValue results_i46();
ExcelValue results_j46();
ExcelValue results_k46();
ExcelValue results_l46();
ExcelValue results_m46();
ExcelValue results_n46();
ExcelValue results_o46();
ExcelValue results_p46();
ExcelValue results_q46();
ExcelValue results_r46();
ExcelValue results_s46();
ExcelValue results_t46();
ExcelValue results_u46();
ExcelValue results_v46();
ExcelValue results_w46();
ExcelValue results_e47();
ExcelValue results_f47();
ExcelValue results_g47();
ExcelValue results_h47();
ExcelValue results_i47();
ExcelValue results_j47();
ExcelValue results_k47();
ExcelValue results_l47();
ExcelValue results_m47();
ExcelValue results_n47();
ExcelValue results_o47();
ExcelValue results_p47();
ExcelValue results_q47();
ExcelValue results_r47();
ExcelValue results_s47();
ExcelValue results_t47();
ExcelValue results_u47();
ExcelValue results_v47();
ExcelValue results_w47();
ExcelValue results_e48();
ExcelValue results_f48();
ExcelValue results_g48();
ExcelValue results_h48();
ExcelValue results_i48();
ExcelValue results_j48();
ExcelValue results_k48();
ExcelValue results_l48();
ExcelValue results_m48();
ExcelValue results_n48();
ExcelValue results_o48();
ExcelValue results_p48();
ExcelValue results_q48();
ExcelValue results_r48();
ExcelValue results_s48();
ExcelValue results_t48();
ExcelValue results_u48();
ExcelValue results_v48();
ExcelValue results_w48();
ExcelValue results_e49();
ExcelValue results_f49();
ExcelValue results_g49();
ExcelValue results_h49();
ExcelValue results_i49();
ExcelValue results_j49();
ExcelValue results_k49();
ExcelValue results_l49();
ExcelValue results_m49();
ExcelValue results_n49();
ExcelValue results_o49();
ExcelValue results_p49();
ExcelValue results_q49();
ExcelValue results_r49();
ExcelValue results_s49();
ExcelValue results_t49();
ExcelValue results_u49();
ExcelValue results_v49();
ExcelValue results_w49();
ExcelValue results_e50();
ExcelValue results_f50();
ExcelValue results_g50();
ExcelValue results_h50();
ExcelValue results_i50();
ExcelValue results_j50();
ExcelValue results_k50();
ExcelValue results_l50();
ExcelValue results_m50();
ExcelValue results_n50();
ExcelValue results_o50();
ExcelValue results_p50();
ExcelValue results_q50();
ExcelValue results_r50();
ExcelValue results_s50();
ExcelValue results_t50();
ExcelValue results_u50();
ExcelValue results_v50();
ExcelValue results_w50();
ExcelValue results_e51();
ExcelValue results_f51();
ExcelValue results_g51();
ExcelValue results_h51();
ExcelValue results_i51();
ExcelValue results_j51();
ExcelValue results_k51();
ExcelValue results_l51();
ExcelValue results_m51();
ExcelValue results_n51();
ExcelValue results_o51();
ExcelValue results_p51();
ExcelValue results_q51();
ExcelValue results_r51();
ExcelValue results_s51();
ExcelValue results_t51();
ExcelValue results_u51();
ExcelValue results_v51();
ExcelValue results_w51();
ExcelValue results_e52();
ExcelValue results_f52();
ExcelValue results_g52();
ExcelValue results_h52();
ExcelValue results_i52();
ExcelValue results_j52();
ExcelValue results_k52();
ExcelValue results_l52();
ExcelValue results_m52();
ExcelValue results_n52();
ExcelValue results_o52();
ExcelValue results_p52();
ExcelValue results_q52();
ExcelValue results_r52();
ExcelValue results_s52();
ExcelValue results_t52();
ExcelValue results_u52();
ExcelValue results_v52();
ExcelValue results_w52();
ExcelValue results_e53();
ExcelValue results_f53();
ExcelValue results_g53();
ExcelValue results_h53();
ExcelValue results_i53();
ExcelValue results_j53();
ExcelValue results_k53();
ExcelValue results_l53();
ExcelValue results_m53();
ExcelValue results_n53();
ExcelValue results_o53();
ExcelValue results_p53();
ExcelValue results_q53();
ExcelValue results_r53();
ExcelValue results_s53();
ExcelValue results_t53();
ExcelValue results_u53();
ExcelValue results_v53();
ExcelValue results_w53();
ExcelValue results_e54();
ExcelValue results_f54();
ExcelValue results_g54();
ExcelValue results_h54();
ExcelValue results_i54();
ExcelValue results_j54();
ExcelValue results_k54();
ExcelValue results_l54();
ExcelValue results_m54();
ExcelValue results_n54();
ExcelValue results_o54();
ExcelValue results_p54();
ExcelValue results_q54();
ExcelValue results_r54();
ExcelValue results_s54();
ExcelValue results_t54();
ExcelValue results_u54();
ExcelValue results_v54();
ExcelValue results_w54();
ExcelValue results_e55();
ExcelValue results_f55();
ExcelValue results_g55();
ExcelValue results_h55();
ExcelValue results_i55();
ExcelValue results_j55();
ExcelValue results_k55();
ExcelValue results_l55();
ExcelValue results_m55();
ExcelValue results_n55();
ExcelValue results_o55();
ExcelValue results_p55();
ExcelValue results_q55();
ExcelValue results_r55();
ExcelValue results_s55();
ExcelValue results_t55();
ExcelValue results_u55();
ExcelValue results_v55();
ExcelValue results_w55();
ExcelValue results_e56();
ExcelValue results_f56();
ExcelValue results_g56();
ExcelValue results_h56();
ExcelValue results_i56();
ExcelValue results_j56();
ExcelValue results_k56();
ExcelValue results_l56();
ExcelValue results_m56();
ExcelValue results_n56();
ExcelValue results_o56();
ExcelValue results_p56();
ExcelValue results_q56();
ExcelValue results_r56();
ExcelValue results_s56();
ExcelValue results_t56();
ExcelValue results_u56();
ExcelValue results_v56();
ExcelValue results_w56();
ExcelValue results_e57();
ExcelValue results_f57();
ExcelValue results_g57();
ExcelValue results_h57();
ExcelValue results_i57();
ExcelValue results_j57();
ExcelValue results_k57();
ExcelValue results_l57();
ExcelValue results_m57();
ExcelValue results_n57();
ExcelValue results_o57();
ExcelValue results_p57();
ExcelValue results_q57();
ExcelValue results_r57();
ExcelValue results_s57();
ExcelValue results_t57();
ExcelValue results_u57();
ExcelValue results_v57();
ExcelValue results_w57();
ExcelValue results_e58();
ExcelValue results_f58();
ExcelValue results_g58();
ExcelValue results_h58();
ExcelValue results_i58();
ExcelValue results_j58();
ExcelValue results_k58();
ExcelValue results_l58();
ExcelValue results_m58();
ExcelValue results_n58();
ExcelValue results_o58();
ExcelValue results_p58();
ExcelValue results_q58();
ExcelValue results_r58();
ExcelValue results_s58();
ExcelValue results_t58();
ExcelValue results_u58();
ExcelValue results_v58();
ExcelValue results_w58();
ExcelValue results_e59();
ExcelValue results_f59();
ExcelValue results_g59();
ExcelValue results_h59();
ExcelValue results_i59();
ExcelValue results_j59();
ExcelValue results_k59();
ExcelValue results_l59();
ExcelValue results_m59();
ExcelValue results_n59();
ExcelValue results_o59();
ExcelValue results_p59();
ExcelValue results_q59();
ExcelValue results_r59();
ExcelValue results_s59();
ExcelValue results_t59();
ExcelValue results_u59();
ExcelValue results_v59();
ExcelValue results_w59();
ExcelValue results_e60();
ExcelValue results_f60();
ExcelValue results_g60();
ExcelValue results_h60();
ExcelValue results_i60();
ExcelValue results_j60();
ExcelValue results_k60();
ExcelValue results_l60();
ExcelValue results_m60();
ExcelValue results_n60();
ExcelValue results_o60();
ExcelValue results_p60();
ExcelValue results_q60();
ExcelValue results_r60();
ExcelValue results_s60();
ExcelValue results_t60();
ExcelValue results_u60();
ExcelValue results_v60();
ExcelValue results_w60();
ExcelValue results_e61();
ExcelValue results_f61();
ExcelValue results_g61();
ExcelValue results_h61();
ExcelValue results_i61();
ExcelValue results_j61();
ExcelValue results_k61();
ExcelValue results_l61();
ExcelValue results_m61();
ExcelValue results_n61();
ExcelValue results_o61();
ExcelValue results_p61();
ExcelValue results_q61();
ExcelValue results_r61();
ExcelValue results_s61();
ExcelValue results_t61();
ExcelValue results_u61();
ExcelValue results_v61();
ExcelValue results_w61();
ExcelValue results_e62();
ExcelValue results_f62();
ExcelValue results_g62();
ExcelValue results_h62();
ExcelValue results_i62();
ExcelValue results_j62();
ExcelValue results_k62();
ExcelValue results_l62();
ExcelValue results_m62();
ExcelValue results_n62();
ExcelValue results_o62();
ExcelValue results_p62();
ExcelValue results_q62();
ExcelValue results_r62();
ExcelValue results_s62();
ExcelValue results_t62();
ExcelValue results_u62();
ExcelValue results_v62();
ExcelValue results_w62();
ExcelValue results_e63();
ExcelValue results_f63();
ExcelValue results_g63();
ExcelValue results_h63();
ExcelValue results_i63();
ExcelValue results_j63();
ExcelValue results_k63();
ExcelValue results_l63();
ExcelValue results_m63();
ExcelValue results_n63();
ExcelValue results_o63();
ExcelValue results_p63();
ExcelValue results_q63();
ExcelValue results_r63();
ExcelValue results_s63();
ExcelValue results_t63();
ExcelValue results_u63();
ExcelValue results_v63();
ExcelValue results_w63();
ExcelValue results_e64();
ExcelValue results_f64();
ExcelValue results_g64();
ExcelValue results_h64();
ExcelValue results_i64();
ExcelValue results_j64();
ExcelValue results_k64();
ExcelValue results_l64();
ExcelValue results_m64();
ExcelValue results_n64();
ExcelValue results_o64();
ExcelValue results_p64();
ExcelValue results_q64();
ExcelValue results_r64();
ExcelValue results_s64();
ExcelValue results_t64();
ExcelValue results_u64();
ExcelValue results_v64();
ExcelValue results_w64();
ExcelValue results_e65();
ExcelValue results_f65();
ExcelValue results_g65();
ExcelValue results_h65();
ExcelValue results_i65();
ExcelValue results_j65();
ExcelValue results_k65();
ExcelValue results_l65();
ExcelValue results_m65();
ExcelValue results_n65();
ExcelValue results_o65();
ExcelValue results_p65();
ExcelValue results_q65();
ExcelValue results_r65();
ExcelValue results_s65();
ExcelValue results_t65();
ExcelValue results_u65();
ExcelValue results_v65();
ExcelValue results_w65();
ExcelValue results_e66();
ExcelValue results_f66();
ExcelValue results_g66();
ExcelValue results_h66();
ExcelValue results_i66();
ExcelValue results_j66();
ExcelValue results_k66();
ExcelValue results_l66();
ExcelValue results_m66();
ExcelValue results_n66();
ExcelValue results_o66();
ExcelValue results_p66();
ExcelValue results_q66();
ExcelValue results_r66();
ExcelValue results_s66();
ExcelValue results_t66();
ExcelValue results_u66();
ExcelValue results_v66();
ExcelValue results_w66();
ExcelValue results_e67();
ExcelValue results_f67();
ExcelValue results_g67();
ExcelValue results_h67();
ExcelValue results_i67();
ExcelValue results_j67();
ExcelValue results_k67();
ExcelValue results_l67();
ExcelValue results_m67();
ExcelValue results_n67();
ExcelValue results_o67();
ExcelValue results_p67();
ExcelValue results_q67();
ExcelValue results_r67();
ExcelValue results_s67();
ExcelValue results_t67();
ExcelValue results_u67();
ExcelValue results_v67();
ExcelValue results_w67();
ExcelValue results_e68();
ExcelValue results_f68();
ExcelValue results_g68();
ExcelValue results_h68();
ExcelValue results_i68();
ExcelValue results_j68();
ExcelValue results_k68();
ExcelValue results_l68();
ExcelValue results_m68();
ExcelValue results_n68();
ExcelValue results_o68();
ExcelValue results_p68();
ExcelValue results_q68();
ExcelValue results_r68();
ExcelValue results_s68();
ExcelValue results_t68();
ExcelValue results_u68();
ExcelValue results_v68();
ExcelValue results_w68();
ExcelValue results_e69();
ExcelValue results_f69();
ExcelValue results_g69();
ExcelValue results_h69();
ExcelValue results_i69();
ExcelValue results_j69();
ExcelValue results_k69();
ExcelValue results_l69();
ExcelValue results_m69();
ExcelValue results_n69();
ExcelValue results_o69();
ExcelValue results_p69();
ExcelValue results_q69();
ExcelValue results_r69();
ExcelValue results_s69();
ExcelValue results_t69();
ExcelValue results_u69();
ExcelValue results_v69();
ExcelValue results_w69();
ExcelValue results_e70();
ExcelValue results_f70();
ExcelValue results_g70();
ExcelValue results_h70();
ExcelValue results_i70();
ExcelValue results_j70();
ExcelValue results_k70();
ExcelValue results_l70();
ExcelValue results_m70();
ExcelValue results_n70();
ExcelValue results_o70();
ExcelValue results_p70();
ExcelValue results_q70();
ExcelValue results_r70();
ExcelValue results_s70();
ExcelValue results_t70();
ExcelValue results_u70();
ExcelValue results_v70();
ExcelValue results_w70();
ExcelValue results_e71();
ExcelValue results_f71();
ExcelValue results_g71();
ExcelValue results_h71();
ExcelValue results_i71();
ExcelValue results_j71();
ExcelValue results_k71();
ExcelValue results_l71();
ExcelValue results_m71();
ExcelValue results_n71();
ExcelValue results_o71();
ExcelValue results_p71();
ExcelValue results_q71();
ExcelValue results_r71();
ExcelValue results_s71();
ExcelValue results_t71();
ExcelValue results_u71();
ExcelValue results_v71();
ExcelValue results_w71();
ExcelValue results_e72();
ExcelValue results_f72();
ExcelValue results_g72();
ExcelValue results_h72();
ExcelValue results_i72();
ExcelValue results_j72();
ExcelValue results_k72();
ExcelValue results_l72();
ExcelValue results_m72();
ExcelValue results_n72();
ExcelValue results_o72();
ExcelValue results_p72();
ExcelValue results_q72();
ExcelValue results_r72();
ExcelValue results_s72();
ExcelValue results_t72();
ExcelValue results_u72();
ExcelValue results_v72();
ExcelValue results_w72();
ExcelValue results_e73();
ExcelValue results_f73();
ExcelValue results_g73();
ExcelValue results_h73();
ExcelValue results_i73();
ExcelValue results_j73();
ExcelValue results_k73();
ExcelValue results_l73();
ExcelValue results_m73();
ExcelValue results_n73();
ExcelValue results_o73();
ExcelValue results_p73();
ExcelValue results_q73();
ExcelValue results_r73();
ExcelValue results_s73();
ExcelValue results_t73();
ExcelValue results_u73();
ExcelValue results_v73();
ExcelValue results_w73();
ExcelValue results_e74();
ExcelValue results_f74();
ExcelValue results_g74();
ExcelValue results_h74();
ExcelValue results_i74();
ExcelValue results_j74();
ExcelValue results_k74();
ExcelValue results_l74();
ExcelValue results_m74();
ExcelValue results_n74();
ExcelValue results_o74();
ExcelValue results_p74();
ExcelValue results_q74();
ExcelValue results_r74();
ExcelValue results_s74();
ExcelValue results_t74();
ExcelValue results_u74();
ExcelValue results_v74();
ExcelValue results_w74();
ExcelValue results_e75();
ExcelValue results_f75();
ExcelValue results_g75();
ExcelValue results_h75();
ExcelValue results_i75();
ExcelValue results_j75();
ExcelValue results_k75();
ExcelValue results_l75();
ExcelValue results_m75();
ExcelValue results_n75();
ExcelValue results_o75();
ExcelValue results_p75();
ExcelValue results_q75();
ExcelValue results_r75();
ExcelValue results_s75();
ExcelValue results_t75();
ExcelValue results_u75();
ExcelValue results_v75();
ExcelValue results_w75();
ExcelValue results_e76();
ExcelValue results_f76();
ExcelValue results_g76();
ExcelValue results_h76();
ExcelValue results_i76();
ExcelValue results_j76();
ExcelValue results_k76();
ExcelValue results_l76();
ExcelValue results_m76();
ExcelValue results_n76();
ExcelValue results_o76();
ExcelValue results_p76();
ExcelValue results_q76();
ExcelValue results_r76();
ExcelValue results_s76();
ExcelValue results_t76();
ExcelValue results_u76();
ExcelValue results_v76();
ExcelValue results_w76();
ExcelValue results_e77();
ExcelValue results_f77();
ExcelValue results_g77();
ExcelValue results_h77();
ExcelValue results_i77();
ExcelValue results_j77();
ExcelValue results_k77();
ExcelValue results_l77();
ExcelValue results_m77();
ExcelValue results_n77();
ExcelValue results_o77();
ExcelValue results_p77();
ExcelValue results_q77();
ExcelValue results_r77();
ExcelValue results_s77();
ExcelValue results_t77();
ExcelValue results_u77();
ExcelValue results_v77();
ExcelValue results_w77();
ExcelValue results_e78();
ExcelValue results_f78();
ExcelValue results_g78();
ExcelValue results_h78();
ExcelValue results_i78();
ExcelValue results_j78();
ExcelValue results_k78();
ExcelValue results_l78();
ExcelValue results_m78();
ExcelValue results_n78();
ExcelValue results_o78();
ExcelValue results_p78();
ExcelValue results_q78();
ExcelValue results_r78();
ExcelValue results_s78();
ExcelValue results_t78();
ExcelValue results_u78();
ExcelValue results_v78();
ExcelValue results_w78();
ExcelValue results_e79();
ExcelValue results_f79();
ExcelValue results_g79();
ExcelValue results_h79();
ExcelValue results_i79();
ExcelValue results_j79();
ExcelValue results_k79();
ExcelValue results_l79();
ExcelValue results_m79();
ExcelValue results_n79();
ExcelValue results_o79();
ExcelValue results_p79();
ExcelValue results_q79();
ExcelValue results_r79();
ExcelValue results_s79();
ExcelValue results_t79();
ExcelValue results_u79();
ExcelValue results_v79();
ExcelValue results_w79();
ExcelValue results_e80();
ExcelValue results_f80();
ExcelValue results_g80();
ExcelValue results_h80();
ExcelValue results_i80();
ExcelValue results_j80();
ExcelValue results_k80();
ExcelValue results_l80();
ExcelValue results_m80();
ExcelValue results_n80();
ExcelValue results_o80();
ExcelValue results_p80();
ExcelValue results_q80();
ExcelValue results_r80();
ExcelValue results_s80();
ExcelValue results_t80();
ExcelValue results_u80();
ExcelValue results_v80();
ExcelValue results_w80();
ExcelValue results_e81();
ExcelValue results_f81();
ExcelValue results_g81();
ExcelValue results_h81();
ExcelValue results_i81();
ExcelValue results_j81();
ExcelValue results_k81();
ExcelValue results_l81();
ExcelValue results_m81();
ExcelValue results_n81();
ExcelValue results_o81();
ExcelValue results_p81();
ExcelValue results_q81();
ExcelValue results_r81();
ExcelValue results_s81();
ExcelValue results_t81();
ExcelValue results_u81();
ExcelValue results_v81();
ExcelValue results_w81();
ExcelValue results_e82();
ExcelValue results_f82();
ExcelValue results_g82();
ExcelValue results_h82();
ExcelValue results_i82();
ExcelValue results_j82();
ExcelValue results_k82();
ExcelValue results_l82();
ExcelValue results_m82();
ExcelValue results_n82();
ExcelValue results_o82();
ExcelValue results_p82();
ExcelValue results_q82();
ExcelValue results_r82();
ExcelValue results_s82();
ExcelValue results_t82();
ExcelValue results_u82();
ExcelValue results_v82();
ExcelValue results_w82();
ExcelValue results_e83();
ExcelValue results_f83();
ExcelValue results_g83();
ExcelValue results_h83();
ExcelValue results_i83();
ExcelValue results_j83();
ExcelValue results_k83();
ExcelValue results_l83();
ExcelValue results_m83();
ExcelValue results_n83();
ExcelValue results_o83();
ExcelValue results_p83();
ExcelValue results_q83();
ExcelValue results_r83();
ExcelValue results_s83();
ExcelValue results_t83();
ExcelValue results_u83();
ExcelValue results_v83();
ExcelValue results_w83();
ExcelValue results_e84();
ExcelValue results_f84();
ExcelValue results_g84();
ExcelValue results_h84();
ExcelValue results_i84();
ExcelValue results_j84();
ExcelValue results_k84();
ExcelValue results_l84();
ExcelValue results_m84();
ExcelValue results_n84();
ExcelValue results_o84();
ExcelValue results_p84();
ExcelValue results_q84();
ExcelValue results_r84();
ExcelValue results_s84();
ExcelValue results_t84();
ExcelValue results_u84();
ExcelValue results_v84();
ExcelValue results_w84();
ExcelValue results_e85();
ExcelValue results_f85();
ExcelValue results_g85();
ExcelValue results_h85();
ExcelValue results_i85();
ExcelValue results_j85();
ExcelValue results_k85();
ExcelValue results_l85();
ExcelValue results_m85();
ExcelValue results_n85();
ExcelValue results_o85();
ExcelValue results_p85();
ExcelValue results_q85();
ExcelValue results_r85();
ExcelValue results_s85();
ExcelValue results_t85();
ExcelValue results_u85();
ExcelValue results_v85();
ExcelValue results_w85();
ExcelValue results_e86();
ExcelValue results_f86();
ExcelValue results_g86();
ExcelValue results_h86();
ExcelValue results_i86();
ExcelValue results_j86();
ExcelValue results_k86();
ExcelValue results_l86();
ExcelValue results_m86();
ExcelValue results_n86();
ExcelValue results_o86();
ExcelValue results_p86();
ExcelValue results_q86();
ExcelValue results_r86();
ExcelValue results_s86();
ExcelValue results_t86();
ExcelValue results_u86();
ExcelValue results_v86();
ExcelValue results_w86();
ExcelValue results_e87();
ExcelValue results_f87();
ExcelValue results_g87();
ExcelValue results_h87();
ExcelValue results_i87();
ExcelValue results_j87();
ExcelValue results_k87();
ExcelValue results_l87();
ExcelValue results_m87();
ExcelValue results_n87();
ExcelValue results_o87();
ExcelValue results_p87();
ExcelValue results_q87();
ExcelValue results_r87();
ExcelValue results_s87();
ExcelValue results_t87();
ExcelValue results_u87();
ExcelValue results_v87();
ExcelValue results_w87();
ExcelValue results_e88();
ExcelValue results_f88();
ExcelValue results_g88();
ExcelValue results_h88();
ExcelValue results_i88();
ExcelValue results_j88();
ExcelValue results_k88();
ExcelValue results_l88();
ExcelValue results_m88();
ExcelValue results_n88();
ExcelValue results_o88();
ExcelValue results_p88();
ExcelValue results_q88();
ExcelValue results_r88();
ExcelValue results_s88();
ExcelValue results_t88();
ExcelValue results_u88();
ExcelValue results_v88();
ExcelValue results_w88();
ExcelValue results_e89();
ExcelValue results_f89();
ExcelValue results_g89();
ExcelValue results_h89();
ExcelValue results_i89();
ExcelValue results_j89();
ExcelValue results_k89();
ExcelValue results_l89();
ExcelValue results_m89();
ExcelValue results_n89();
ExcelValue results_o89();
ExcelValue results_p89();
ExcelValue results_q89();
ExcelValue results_r89();
ExcelValue results_s89();
ExcelValue results_t89();
ExcelValue results_u89();
ExcelValue results_v89();
ExcelValue results_w89();
ExcelValue results_e90();
ExcelValue results_f90();
ExcelValue results_g90();
ExcelValue results_h90();
ExcelValue results_i90();
ExcelValue results_j90();
ExcelValue results_k90();
ExcelValue results_l90();
ExcelValue results_m90();
ExcelValue results_n90();
ExcelValue results_o90();
ExcelValue results_p90();
ExcelValue results_q90();
ExcelValue results_r90();
ExcelValue results_s90();
ExcelValue results_t90();
ExcelValue results_u90();
ExcelValue results_v90();
ExcelValue results_w90();
ExcelValue results_e91();
ExcelValue results_f91();
ExcelValue results_g91();
ExcelValue results_h91();
ExcelValue results_i91();
ExcelValue results_j91();
ExcelValue results_k91();
ExcelValue results_l91();
ExcelValue results_m91();
ExcelValue results_n91();
ExcelValue results_o91();
ExcelValue results_p91();
ExcelValue results_q91();
ExcelValue results_r91();
ExcelValue results_s91();
ExcelValue results_t91();
ExcelValue results_u91();
ExcelValue results_v91();
ExcelValue results_w91();
ExcelValue results_e92();
ExcelValue results_f92();
ExcelValue results_g92();
ExcelValue results_h92();
ExcelValue results_i92();
ExcelValue results_j92();
ExcelValue results_k92();
ExcelValue results_l92();
ExcelValue results_m92();
ExcelValue results_n92();
ExcelValue results_o92();
ExcelValue results_p92();
ExcelValue results_q92();
ExcelValue results_r92();
ExcelValue results_s92();
ExcelValue results_t92();
ExcelValue results_u92();
ExcelValue results_v92();
ExcelValue results_w92();
ExcelValue results_e93();
ExcelValue results_f93();
ExcelValue results_g93();
ExcelValue results_h93();
ExcelValue results_i93();
ExcelValue results_j93();
ExcelValue results_k93();
ExcelValue results_l93();
ExcelValue results_m93();
ExcelValue results_n93();
ExcelValue results_o93();
ExcelValue results_p93();
ExcelValue results_q93();
ExcelValue results_r93();
ExcelValue results_s93();
ExcelValue results_t93();
ExcelValue results_u93();
ExcelValue results_v93();
ExcelValue results_w93();
ExcelValue results_e94();
ExcelValue results_f94();
ExcelValue results_g94();
ExcelValue results_h94();
ExcelValue results_i94();
ExcelValue results_j94();
ExcelValue results_k94();
ExcelValue results_l94();
ExcelValue results_m94();
ExcelValue results_n94();
ExcelValue results_o94();
ExcelValue results_p94();
ExcelValue results_q94();
ExcelValue results_r94();
ExcelValue results_s94();
ExcelValue results_t94();
ExcelValue results_u94();
ExcelValue results_v94();
ExcelValue results_w94();
ExcelValue results_e95();
ExcelValue results_f95();
ExcelValue results_g95();
ExcelValue results_h95();
ExcelValue results_i95();
ExcelValue results_j95();
ExcelValue results_k95();
ExcelValue results_l95();
ExcelValue results_m95();
ExcelValue results_n95();
ExcelValue results_o95();
ExcelValue results_p95();
ExcelValue results_q95();
ExcelValue results_r95();
ExcelValue results_s95();
ExcelValue results_t95();
ExcelValue results_u95();
ExcelValue results_v95();
ExcelValue results_w95();
ExcelValue results_e96();
ExcelValue results_f96();
ExcelValue results_g96();
ExcelValue results_h96();
ExcelValue results_i96();
ExcelValue results_j96();
ExcelValue results_k96();
ExcelValue results_l96();
ExcelValue results_m96();
ExcelValue results_n96();
ExcelValue results_o96();
ExcelValue results_p96();
ExcelValue results_q96();
ExcelValue results_r96();
ExcelValue results_s96();
ExcelValue results_t96();
ExcelValue results_u96();
ExcelValue results_v96();
ExcelValue results_w96();
ExcelValue results_e97();
ExcelValue results_f97();
ExcelValue results_g97();
ExcelValue results_h97();
ExcelValue results_i97();
ExcelValue results_j97();
ExcelValue results_k97();
ExcelValue results_l97();
ExcelValue results_m97();
ExcelValue results_n97();
ExcelValue results_o97();
ExcelValue results_p97();
ExcelValue results_q97();
ExcelValue results_r97();
ExcelValue results_s97();
ExcelValue results_t97();
ExcelValue results_u97();
ExcelValue results_v97();
ExcelValue results_w97();
ExcelValue results_e98();
ExcelValue results_f98();
ExcelValue results_g98();
ExcelValue results_h98();
ExcelValue results_i98();
ExcelValue results_j98();
ExcelValue results_k98();
ExcelValue results_l98();
ExcelValue results_m98();
ExcelValue results_n98();
ExcelValue results_o98();
ExcelValue results_p98();
ExcelValue results_q98();
ExcelValue results_r98();
ExcelValue results_s98();
ExcelValue results_t98();
ExcelValue results_u98();
ExcelValue results_v98();
ExcelValue results_w98();
ExcelValue results_e99();
ExcelValue results_f99();
ExcelValue results_g99();
ExcelValue results_h99();
ExcelValue results_i99();
ExcelValue results_j99();
ExcelValue results_k99();
ExcelValue results_l99();
ExcelValue results_m99();
ExcelValue results_n99();
ExcelValue results_o99();
ExcelValue results_p99();
ExcelValue results_q99();
ExcelValue results_r99();
ExcelValue results_s99();
ExcelValue results_t99();
ExcelValue results_u99();
ExcelValue results_v99();
ExcelValue results_w99();
ExcelValue results_e100();
ExcelValue results_f100();
ExcelValue results_g100();
ExcelValue results_h100();
ExcelValue results_i100();
ExcelValue results_j100();
ExcelValue results_k100();
ExcelValue results_l100();
ExcelValue results_m100();
ExcelValue results_n100();
ExcelValue results_o100();
ExcelValue results_p100();
ExcelValue results_q100();
ExcelValue results_r100();
ExcelValue results_s100();
ExcelValue results_t100();
ExcelValue results_u100();
ExcelValue results_v100();
ExcelValue results_w100();
ExcelValue results_e101();
ExcelValue results_f101();
ExcelValue results_g101();
ExcelValue results_h101();
ExcelValue results_i101();
ExcelValue results_j101();
ExcelValue results_k101();
ExcelValue results_l101();
ExcelValue results_m101();
ExcelValue results_n101();
ExcelValue results_o101();
ExcelValue results_p101();
ExcelValue results_q101();
ExcelValue results_r101();
ExcelValue results_s101();
ExcelValue results_t101();
ExcelValue results_u101();
ExcelValue results_v101();
ExcelValue results_w101();
ExcelValue results_e102();
ExcelValue results_f102();
ExcelValue results_g102();
ExcelValue results_h102();
ExcelValue results_i102();
ExcelValue results_j102();
ExcelValue results_k102();
ExcelValue results_l102();
ExcelValue results_m102();
ExcelValue results_n102();
ExcelValue results_o102();
ExcelValue results_p102();
ExcelValue results_q102();
ExcelValue results_r102();
ExcelValue results_s102();
ExcelValue results_t102();
ExcelValue results_u102();
ExcelValue results_v102();
ExcelValue results_w102();
ExcelValue results_e103();
ExcelValue results_f103();
ExcelValue results_g103();
ExcelValue results_h103();
ExcelValue results_i103();
ExcelValue results_j103();
ExcelValue results_k103();
ExcelValue results_l103();
ExcelValue results_m103();
ExcelValue results_n103();
ExcelValue results_o103();
ExcelValue results_p103();
ExcelValue results_q103();
ExcelValue results_r103();
ExcelValue results_s103();
ExcelValue results_t103();
ExcelValue results_u103();
ExcelValue results_v103();
ExcelValue results_w103();
ExcelValue results_e104();
ExcelValue results_f104();
ExcelValue results_g104();
ExcelValue results_h104();
ExcelValue results_i104();
ExcelValue results_j104();
ExcelValue results_k104();
ExcelValue results_l104();
ExcelValue results_m104();
ExcelValue results_n104();
ExcelValue results_o104();
ExcelValue results_p104();
ExcelValue results_q104();
ExcelValue results_r104();
ExcelValue results_s104();
ExcelValue results_t104();
ExcelValue results_u104();
ExcelValue results_v104();
ExcelValue results_w104();
ExcelValue results_e105();
ExcelValue results_f105();
ExcelValue results_g105();
ExcelValue results_h105();
ExcelValue results_i105();
ExcelValue results_j105();
ExcelValue results_k105();
ExcelValue results_l105();
ExcelValue results_m105();
ExcelValue results_n105();
ExcelValue results_o105();
ExcelValue results_p105();
ExcelValue results_q105();
ExcelValue results_r105();
ExcelValue results_s105();
ExcelValue results_t105();
ExcelValue results_u105();
ExcelValue results_v105();
ExcelValue results_w105();
ExcelValue results_e106();
ExcelValue results_f106();
ExcelValue results_g106();
ExcelValue results_h106();
ExcelValue results_i106();
ExcelValue results_j106();
ExcelValue results_k106();
ExcelValue results_l106();
ExcelValue results_m106();
ExcelValue results_n106();
ExcelValue results_o106();
ExcelValue results_p106();
ExcelValue results_q106();
ExcelValue results_r106();
ExcelValue results_s106();
ExcelValue results_t106();
ExcelValue results_u106();
ExcelValue results_v106();
ExcelValue results_w106();
ExcelValue results_e107();
ExcelValue results_f107();
ExcelValue results_g107();
ExcelValue results_h107();
ExcelValue results_i107();
ExcelValue results_j107();
ExcelValue results_k107();
ExcelValue results_l107();
ExcelValue results_m107();
ExcelValue results_n107();
ExcelValue results_o107();
ExcelValue results_p107();
ExcelValue results_q107();
ExcelValue results_r107();
ExcelValue results_s107();
ExcelValue results_t107();
ExcelValue results_u107();
ExcelValue results_v107();
ExcelValue results_w107();
ExcelValue results_e108();
ExcelValue results_f108();
ExcelValue results_g108();
ExcelValue results_h108();
ExcelValue results_i108();
ExcelValue results_j108();
ExcelValue results_k108();
ExcelValue results_l108();
ExcelValue results_m108();
ExcelValue results_n108();
ExcelValue results_o108();
ExcelValue results_p108();
ExcelValue results_q108();
ExcelValue results_r108();
ExcelValue results_s108();
ExcelValue results_t108();
ExcelValue results_u108();
ExcelValue results_v108();
ExcelValue results_w108();
ExcelValue results_e109();
ExcelValue results_f109();
ExcelValue results_g109();
ExcelValue results_h109();
ExcelValue results_i109();
ExcelValue results_j109();
ExcelValue results_k109();
ExcelValue results_l109();
ExcelValue results_m109();
ExcelValue results_n109();
ExcelValue results_o109();
ExcelValue results_p109();
ExcelValue results_q109();
ExcelValue results_r109();
ExcelValue results_s109();
ExcelValue results_t109();
ExcelValue results_u109();
ExcelValue results_v109();
ExcelValue results_w109();
ExcelValue results_e110();
ExcelValue results_f110();
ExcelValue results_g110();
ExcelValue results_h110();
ExcelValue results_i110();
ExcelValue results_j110();
ExcelValue results_k110();
ExcelValue results_l110();
ExcelValue results_m110();
ExcelValue results_n110();
ExcelValue results_o110();
ExcelValue results_p110();
ExcelValue results_q110();
ExcelValue results_r110();
ExcelValue results_s110();
ExcelValue results_t110();
ExcelValue results_u110();
ExcelValue results_v110();
ExcelValue results_w110();
ExcelValue results_e111();
ExcelValue results_f111();
ExcelValue results_g111();
ExcelValue results_h111();
ExcelValue results_i111();
ExcelValue results_j111();
ExcelValue results_k111();
ExcelValue results_l111();
ExcelValue results_m111();
ExcelValue results_n111();
ExcelValue results_o111();
ExcelValue results_p111();
ExcelValue results_q111();
ExcelValue results_r111();
ExcelValue results_s111();
ExcelValue results_t111();
ExcelValue results_u111();
ExcelValue results_v111();
ExcelValue results_w111();
ExcelValue results_e112();
ExcelValue results_f112();
ExcelValue results_g112();
ExcelValue results_h112();
ExcelValue results_i112();
ExcelValue results_j112();
ExcelValue results_k112();
ExcelValue results_l112();
ExcelValue results_m112();
ExcelValue results_n112();
ExcelValue results_o112();
ExcelValue results_p112();
ExcelValue results_q112();
ExcelValue results_r112();
ExcelValue results_s112();
ExcelValue results_t112();
ExcelValue results_u112();
ExcelValue results_v112();
ExcelValue results_w112();
ExcelValue results_e113();
ExcelValue results_f113();
ExcelValue results_g113();
ExcelValue results_h113();
ExcelValue results_i113();
ExcelValue results_j113();
ExcelValue results_k113();
ExcelValue results_l113();
ExcelValue results_m113();
ExcelValue results_n113();
ExcelValue results_o113();
ExcelValue results_p113();
ExcelValue results_q113();
ExcelValue results_r113();
ExcelValue results_s113();
ExcelValue results_t113();
ExcelValue results_u113();
ExcelValue results_v113();
ExcelValue results_w113();
ExcelValue results_e114();
ExcelValue results_f114();
ExcelValue results_g114();
ExcelValue results_h114();
ExcelValue results_i114();
ExcelValue results_j114();
ExcelValue results_k114();
ExcelValue results_l114();
ExcelValue results_m114();
ExcelValue results_n114();
ExcelValue results_o114();
ExcelValue results_p114();
ExcelValue results_q114();
ExcelValue results_r114();
ExcelValue results_s114();
ExcelValue results_t114();
ExcelValue results_u114();
ExcelValue results_v114();
ExcelValue results_w114();
ExcelValue results_e115();
ExcelValue results_f115();
ExcelValue results_g115();
ExcelValue results_h115();
ExcelValue results_i115();
ExcelValue results_j115();
ExcelValue results_k115();
ExcelValue results_l115();
ExcelValue results_m115();
ExcelValue results_n115();
ExcelValue results_o115();
ExcelValue results_p115();
ExcelValue results_q115();
ExcelValue results_r115();
ExcelValue results_s115();
ExcelValue results_t115();
ExcelValue results_u115();
ExcelValue results_v115();
ExcelValue results_w115();
ExcelValue results_e116();
ExcelValue results_f116();
ExcelValue results_g116();
ExcelValue results_h116();
ExcelValue results_i116();
ExcelValue results_j116();
ExcelValue results_k116();
ExcelValue results_l116();
ExcelValue results_m116();
ExcelValue results_n116();
ExcelValue results_o116();
ExcelValue results_p116();
ExcelValue results_q116();
ExcelValue results_r116();
ExcelValue results_s116();
ExcelValue results_t116();
ExcelValue results_u116();
ExcelValue results_v116();
ExcelValue results_w116();
ExcelValue results_e117();
ExcelValue results_f117();
ExcelValue results_g117();
ExcelValue results_h117();
ExcelValue results_i117();
ExcelValue results_j117();
ExcelValue results_k117();
ExcelValue results_l117();
ExcelValue results_m117();
ExcelValue results_n117();
ExcelValue results_o117();
ExcelValue results_p117();
ExcelValue results_q117();
ExcelValue results_r117();
ExcelValue results_s117();
ExcelValue results_t117();
ExcelValue results_u117();
ExcelValue results_v117();
ExcelValue results_w117();
ExcelValue results_e118();
ExcelValue results_f118();
ExcelValue results_g118();
ExcelValue results_h118();
ExcelValue results_i118();
ExcelValue results_j118();
ExcelValue results_k118();
ExcelValue results_l118();
ExcelValue results_m118();
ExcelValue results_n118();
ExcelValue results_o118();
ExcelValue results_p118();
ExcelValue results_q118();
ExcelValue results_r118();
ExcelValue results_s118();
ExcelValue results_t118();
ExcelValue results_u118();
ExcelValue results_v118();
ExcelValue results_w118();
ExcelValue results_e119();
ExcelValue results_f119();
ExcelValue results_g119();
ExcelValue results_h119();
ExcelValue results_i119();
ExcelValue results_j119();
ExcelValue results_k119();
ExcelValue results_l119();
ExcelValue results_m119();
ExcelValue results_n119();
ExcelValue results_o119();
ExcelValue results_p119();
ExcelValue results_q119();
ExcelValue results_r119();
ExcelValue results_s119();
ExcelValue results_t119();
ExcelValue results_u119();
ExcelValue results_v119();
ExcelValue results_w119();
ExcelValue results_e120();
ExcelValue results_f120();
ExcelValue results_g120();
ExcelValue results_h120();
ExcelValue results_i120();
ExcelValue results_j120();
ExcelValue results_k120();
ExcelValue results_l120();
ExcelValue results_m120();
ExcelValue results_n120();
ExcelValue results_o120();
ExcelValue results_p120();
ExcelValue results_q120();
ExcelValue results_r120();
ExcelValue results_s120();
ExcelValue results_t120();
ExcelValue results_u120();
ExcelValue results_v120();
ExcelValue results_w120();
ExcelValue results_e121();
ExcelValue results_f121();
ExcelValue results_g121();
ExcelValue results_h121();
ExcelValue results_i121();
ExcelValue results_j121();
ExcelValue results_k121();
ExcelValue results_l121();
ExcelValue results_m121();
ExcelValue results_n121();
ExcelValue results_o121();
ExcelValue results_p121();
ExcelValue results_q121();
ExcelValue results_r121();
ExcelValue results_s121();
ExcelValue results_t121();
ExcelValue results_u121();
ExcelValue results_v121();
ExcelValue results_w121();
ExcelValue results_e122();
ExcelValue results_f122();
ExcelValue results_g122();
ExcelValue results_h122();
ExcelValue results_i122();
ExcelValue results_j122();
ExcelValue results_k122();
ExcelValue results_l122();
ExcelValue results_m122();
ExcelValue results_n122();
ExcelValue results_o122();
ExcelValue results_p122();
ExcelValue results_q122();
ExcelValue results_r122();
ExcelValue results_s122();
ExcelValue results_t122();
ExcelValue results_u122();
ExcelValue results_v122();
ExcelValue results_w122();
ExcelValue results_e123();
ExcelValue results_f123();
ExcelValue results_g123();
ExcelValue results_h123();
ExcelValue results_i123();
ExcelValue results_j123();
ExcelValue results_k123();
ExcelValue results_l123();
ExcelValue results_m123();
ExcelValue results_n123();
ExcelValue results_o123();
ExcelValue results_p123();
ExcelValue results_q123();
ExcelValue results_r123();
ExcelValue results_s123();
ExcelValue results_t123();
ExcelValue results_u123();
ExcelValue results_v123();
ExcelValue results_w123();
ExcelValue results_e124();
ExcelValue results_f124();
ExcelValue results_g124();
ExcelValue results_h124();
ExcelValue results_i124();
ExcelValue results_j124();
ExcelValue results_k124();
ExcelValue results_l124();
ExcelValue results_m124();
ExcelValue results_n124();
ExcelValue results_o124();
ExcelValue results_p124();
ExcelValue results_q124();
ExcelValue results_r124();
ExcelValue results_s124();
ExcelValue results_t124();
ExcelValue results_u124();
ExcelValue results_v124();
ExcelValue results_w124();
ExcelValue results_e125();
ExcelValue results_f125();
ExcelValue results_g125();
ExcelValue results_h125();
ExcelValue results_i125();
ExcelValue results_j125();
ExcelValue results_k125();
ExcelValue results_l125();
ExcelValue results_m125();
ExcelValue results_n125();
ExcelValue results_o125();
ExcelValue results_p125();
ExcelValue results_q125();
ExcelValue results_r125();
ExcelValue results_s125();
ExcelValue results_t125();
ExcelValue results_u125();
ExcelValue results_v125();
ExcelValue results_w125();
ExcelValue results_e126();
ExcelValue results_f126();
ExcelValue results_g126();
ExcelValue results_h126();
ExcelValue results_i126();
ExcelValue results_j126();
ExcelValue results_k126();
ExcelValue results_l126();
ExcelValue results_m126();
ExcelValue results_n126();
ExcelValue results_o126();
ExcelValue results_p126();
ExcelValue results_q126();
ExcelValue results_r126();
ExcelValue results_s126();
ExcelValue results_t126();
ExcelValue results_u126();
ExcelValue results_v126();
ExcelValue results_w126();
ExcelValue results_e127();
ExcelValue results_f127();
ExcelValue results_g127();
ExcelValue results_h127();
ExcelValue results_i127();
ExcelValue results_j127();
ExcelValue results_k127();
ExcelValue results_l127();
ExcelValue results_m127();
ExcelValue results_n127();
ExcelValue results_o127();
ExcelValue results_p127();
ExcelValue results_q127();
ExcelValue results_r127();
ExcelValue results_s127();
ExcelValue results_t127();
ExcelValue results_u127();
ExcelValue results_v127();
ExcelValue results_w127();
ExcelValue results_e128();
ExcelValue results_f128();
ExcelValue results_g128();
ExcelValue results_h128();
ExcelValue results_i128();
ExcelValue results_j128();
ExcelValue results_k128();
ExcelValue results_l128();
ExcelValue results_m128();
ExcelValue results_n128();
ExcelValue results_o128();
ExcelValue results_p128();
ExcelValue results_q128();
ExcelValue results_r128();
ExcelValue results_s128();
ExcelValue results_t128();
ExcelValue results_u128();
ExcelValue results_v128();
ExcelValue results_w128();
ExcelValue results_e129();
ExcelValue results_f129();
ExcelValue results_g129();
ExcelValue results_h129();
ExcelValue results_i129();
ExcelValue results_j129();
ExcelValue results_k129();
ExcelValue results_l129();
ExcelValue results_m129();
ExcelValue results_n129();
ExcelValue results_o129();
ExcelValue results_p129();
ExcelValue results_q129();
ExcelValue results_r129();
ExcelValue results_s129();
ExcelValue results_t129();
ExcelValue results_u129();
ExcelValue results_v129();
ExcelValue results_w129();
ExcelValue results_e130();
ExcelValue results_f130();
ExcelValue results_g130();
ExcelValue results_h130();
ExcelValue results_i130();
ExcelValue results_j130();
ExcelValue results_k130();
ExcelValue results_l130();
ExcelValue results_m130();
ExcelValue results_n130();
ExcelValue results_o130();
ExcelValue results_p130();
ExcelValue results_q130();
ExcelValue results_r130();
ExcelValue results_s130();
ExcelValue results_t130();
ExcelValue results_u130();
ExcelValue results_v130();
ExcelValue results_w130();
ExcelValue results_e131();
ExcelValue results_f131();
ExcelValue results_g131();
ExcelValue results_h131();
ExcelValue results_i131();
ExcelValue results_j131();
ExcelValue results_k131();
ExcelValue results_l131();
ExcelValue results_m131();
ExcelValue results_n131();
ExcelValue results_o131();
ExcelValue results_p131();
ExcelValue results_q131();
ExcelValue results_r131();
ExcelValue results_s131();
ExcelValue results_t131();
ExcelValue results_u131();
ExcelValue results_v131();
ExcelValue results_w131();
ExcelValue results_e132();
ExcelValue results_f132();
ExcelValue results_g132();
ExcelValue results_h132();
ExcelValue results_i132();
ExcelValue results_j132();
ExcelValue results_k132();
ExcelValue results_l132();
ExcelValue results_m132();
ExcelValue results_n132();
ExcelValue results_o132();
ExcelValue results_p132();
ExcelValue results_q132();
ExcelValue results_r132();
ExcelValue results_s132();
ExcelValue results_t132();
ExcelValue results_u132();
ExcelValue results_v132();
ExcelValue results_w132();
ExcelValue results_e133();
ExcelValue results_f133();
ExcelValue results_g133();
ExcelValue results_h133();
ExcelValue results_i133();
ExcelValue results_j133();
ExcelValue results_k133();
ExcelValue results_l133();
ExcelValue results_m133();
ExcelValue results_n133();
ExcelValue results_o133();
ExcelValue results_p133();
ExcelValue results_q133();
ExcelValue results_r133();
ExcelValue results_s133();
ExcelValue results_t133();
ExcelValue results_u133();
ExcelValue results_v133();
ExcelValue results_w133();
ExcelValue results_e134();
ExcelValue results_f134();
ExcelValue results_g134();
ExcelValue results_h134();
ExcelValue results_i134();
ExcelValue results_j134();
ExcelValue results_k134();
ExcelValue results_l134();
ExcelValue results_m134();
ExcelValue results_n134();
ExcelValue results_o134();
ExcelValue results_p134();
ExcelValue results_q134();
ExcelValue results_r134();
ExcelValue results_s134();
ExcelValue results_t134();
ExcelValue results_u134();
ExcelValue results_v134();
ExcelValue results_w134();
ExcelValue results_e135();
ExcelValue results_f135();
ExcelValue results_g135();
ExcelValue results_h135();
ExcelValue results_i135();
ExcelValue results_j135();
ExcelValue results_k135();
ExcelValue results_l135();
ExcelValue results_m135();
ExcelValue results_n135();
ExcelValue results_o135();
ExcelValue results_p135();
ExcelValue results_q135();
ExcelValue results_r135();
ExcelValue results_s135();
ExcelValue results_t135();
ExcelValue results_u135();
ExcelValue results_v135();
ExcelValue results_w135();
ExcelValue results_e136();
ExcelValue results_f136();
ExcelValue results_g136();
ExcelValue results_h136();
ExcelValue results_i136();
ExcelValue results_j136();
ExcelValue results_k136();
ExcelValue results_l136();
ExcelValue results_m136();
ExcelValue results_n136();
ExcelValue results_o136();
ExcelValue results_p136();
ExcelValue results_q136();
ExcelValue results_r136();
ExcelValue results_s136();
ExcelValue results_t136();
ExcelValue results_u136();
ExcelValue results_v136();
ExcelValue results_w136();
ExcelValue results_e137();
ExcelValue results_f137();
ExcelValue results_g137();
ExcelValue results_h137();
ExcelValue results_i137();
ExcelValue results_j137();
ExcelValue results_k137();
ExcelValue results_l137();
ExcelValue results_m137();
ExcelValue results_n137();
ExcelValue results_o137();
ExcelValue results_p137();
ExcelValue results_q137();
ExcelValue results_r137();
ExcelValue results_s137();
ExcelValue results_t137();
ExcelValue results_u137();
ExcelValue results_v137();
ExcelValue results_w137();
ExcelValue results_e138();
ExcelValue results_f138();
ExcelValue results_g138();
ExcelValue results_h138();
ExcelValue results_i138();
ExcelValue results_j138();
ExcelValue results_k138();
ExcelValue results_l138();
ExcelValue results_m138();
ExcelValue results_n138();
ExcelValue results_o138();
ExcelValue results_p138();
ExcelValue results_q138();
ExcelValue results_r138();
ExcelValue results_s138();
ExcelValue results_t138();
ExcelValue results_u138();
ExcelValue results_v138();
ExcelValue results_w138();
ExcelValue results_e139();
ExcelValue results_f139();
ExcelValue results_g139();
ExcelValue results_h139();
ExcelValue results_i139();
ExcelValue results_j139();
ExcelValue results_k139();
ExcelValue results_l139();
ExcelValue results_m139();
ExcelValue results_n139();
ExcelValue results_o139();
ExcelValue results_p139();
ExcelValue results_q139();
ExcelValue results_r139();
ExcelValue results_s139();
ExcelValue results_t139();
ExcelValue results_u139();
ExcelValue results_v139();
ExcelValue results_w139();
ExcelValue results_e140();
ExcelValue results_f140();
ExcelValue results_g140();
ExcelValue results_h140();
ExcelValue results_i140();
ExcelValue results_j140();
ExcelValue results_k140();
ExcelValue results_l140();
ExcelValue results_m140();
ExcelValue results_n140();
ExcelValue results_o140();
ExcelValue results_p140();
ExcelValue results_q140();
ExcelValue results_r140();
ExcelValue results_s140();
ExcelValue results_t140();
ExcelValue results_u140();
ExcelValue results_v140();
ExcelValue results_w140();
ExcelValue results_e141();
ExcelValue results_f141();
ExcelValue results_g141();
ExcelValue results_h141();
ExcelValue results_i141();
ExcelValue results_j141();
ExcelValue results_k141();
ExcelValue results_l141();
ExcelValue results_m141();
ExcelValue results_n141();
ExcelValue results_o141();
ExcelValue results_p141();
ExcelValue results_q141();
ExcelValue results_r141();
ExcelValue results_s141();
ExcelValue results_t141();
ExcelValue results_u141();
ExcelValue results_v141();
ExcelValue results_w141();
ExcelValue results_e142();
ExcelValue results_f142();
ExcelValue results_g142();
ExcelValue results_h142();
ExcelValue results_i142();
ExcelValue results_j142();
ExcelValue results_k142();
ExcelValue results_l142();
ExcelValue results_m142();
ExcelValue results_n142();
ExcelValue results_o142();
ExcelValue results_p142();
ExcelValue results_q142();
ExcelValue results_r142();
ExcelValue results_s142();
ExcelValue results_t142();
ExcelValue results_u142();
ExcelValue results_v142();
ExcelValue results_w142();
ExcelValue results_e143();
ExcelValue results_f143();
ExcelValue results_g143();
ExcelValue results_h143();
ExcelValue results_i143();
ExcelValue results_j143();
ExcelValue results_k143();
ExcelValue results_l143();
ExcelValue results_m143();
ExcelValue results_n143();
ExcelValue results_o143();
ExcelValue results_p143();
ExcelValue results_q143();
ExcelValue results_r143();
ExcelValue results_s143();
ExcelValue results_t143();
ExcelValue results_u143();
ExcelValue results_v143();
ExcelValue results_w143();
ExcelValue results_e144();
ExcelValue results_f144();
ExcelValue results_g144();
ExcelValue results_h144();
ExcelValue results_i144();
ExcelValue results_j144();
ExcelValue results_k144();
ExcelValue results_l144();
ExcelValue results_m144();
ExcelValue results_n144();
ExcelValue results_o144();
ExcelValue results_p144();
ExcelValue results_q144();
ExcelValue results_r144();
ExcelValue results_s144();
ExcelValue results_t144();
ExcelValue results_u144();
ExcelValue results_v144();
ExcelValue results_w144();
ExcelValue results_e145();
ExcelValue results_f145();
ExcelValue results_g145();
ExcelValue results_h145();
ExcelValue results_i145();
ExcelValue results_j145();
ExcelValue results_k145();
ExcelValue results_l145();
ExcelValue results_m145();
ExcelValue results_n145();
ExcelValue results_o145();
ExcelValue results_p145();
ExcelValue results_q145();
ExcelValue results_r145();
ExcelValue results_s145();
ExcelValue results_t145();
ExcelValue results_u145();
ExcelValue results_v145();
ExcelValue results_w145();
ExcelValue results_e146();
ExcelValue results_f146();
ExcelValue results_g146();
ExcelValue results_h146();
ExcelValue results_i146();
ExcelValue results_j146();
ExcelValue results_k146();
ExcelValue results_l146();
ExcelValue results_m146();
ExcelValue results_n146();
ExcelValue results_o146();
ExcelValue results_p146();
ExcelValue results_q146();
ExcelValue results_r146();
ExcelValue results_s146();
ExcelValue results_t146();
ExcelValue results_u146();
ExcelValue results_v146();
ExcelValue results_w146();
ExcelValue results_e147();
ExcelValue results_f147();
ExcelValue results_g147();
ExcelValue results_h147();
ExcelValue results_i147();
ExcelValue results_j147();
ExcelValue results_k147();
ExcelValue results_l147();
ExcelValue results_m147();
ExcelValue results_n147();
ExcelValue results_o147();
ExcelValue results_p147();
ExcelValue results_q147();
ExcelValue results_r147();
ExcelValue results_s147();
ExcelValue results_t147();
ExcelValue results_u147();
ExcelValue results_v147();
ExcelValue results_w147();
ExcelValue results_e148();
ExcelValue results_f148();
ExcelValue results_g148();
ExcelValue results_h148();
ExcelValue results_i148();
ExcelValue results_j148();
ExcelValue results_k148();
ExcelValue results_l148();
ExcelValue results_m148();
ExcelValue results_n148();
ExcelValue results_o148();
ExcelValue results_p148();
ExcelValue results_q148();
ExcelValue results_r148();
ExcelValue results_s148();
ExcelValue results_t148();
ExcelValue results_u148();
ExcelValue results_v148();
ExcelValue results_w148();
ExcelValue results_e149();
ExcelValue results_f149();
ExcelValue results_g149();
ExcelValue results_h149();
ExcelValue results_i149();
ExcelValue results_j149();
ExcelValue results_k149();
ExcelValue results_l149();
ExcelValue results_m149();
ExcelValue results_n149();
ExcelValue results_o149();
ExcelValue results_p149();
ExcelValue results_q149();
ExcelValue results_r149();
ExcelValue results_s149();
ExcelValue results_t149();
ExcelValue results_u149();
ExcelValue results_v149();
ExcelValue results_w149();
ExcelValue results_e150();
ExcelValue results_f150();
ExcelValue results_g150();
ExcelValue results_h150();
ExcelValue results_i150();
ExcelValue results_j150();
ExcelValue results_k150();
ExcelValue results_l150();
ExcelValue results_m150();
ExcelValue results_n150();
ExcelValue results_o150();
ExcelValue results_p150();
ExcelValue results_q150();
ExcelValue results_r150();
ExcelValue results_s150();
ExcelValue results_t150();
ExcelValue results_u150();
ExcelValue results_v150();
ExcelValue results_w150();
ExcelValue results_e151();
ExcelValue results_f151();
ExcelValue results_g151();
ExcelValue results_h151();
ExcelValue results_i151();
ExcelValue results_j151();
ExcelValue results_k151();
ExcelValue results_l151();
ExcelValue results_m151();
ExcelValue results_n151();
ExcelValue results_o151();
ExcelValue results_p151();
ExcelValue results_q151();
ExcelValue results_r151();
ExcelValue results_s151();
ExcelValue results_t151();
ExcelValue results_u151();
ExcelValue results_v151();
ExcelValue results_w151();
ExcelValue results_e152();
ExcelValue results_f152();
ExcelValue results_g152();
ExcelValue results_h152();
ExcelValue results_i152();
ExcelValue results_j152();
ExcelValue results_k152();
ExcelValue results_l152();
ExcelValue results_m152();
ExcelValue results_n152();
ExcelValue results_o152();
ExcelValue results_p152();
ExcelValue results_q152();
ExcelValue results_r152();
ExcelValue results_s152();
ExcelValue results_t152();
ExcelValue results_u152();
ExcelValue results_v152();
ExcelValue results_w152();
ExcelValue results_e153();
ExcelValue results_f153();
ExcelValue results_g153();
ExcelValue results_h153();
ExcelValue results_i153();
ExcelValue results_j153();
ExcelValue results_k153();
ExcelValue results_l153();
ExcelValue results_m153();
ExcelValue results_n153();
ExcelValue results_o153();
ExcelValue results_p153();
ExcelValue results_q153();
ExcelValue results_r153();
ExcelValue results_s153();
ExcelValue results_t153();
ExcelValue results_u153();
ExcelValue results_v153();
ExcelValue results_w153();
ExcelValue results_e154();
ExcelValue results_f154();
ExcelValue results_g154();
ExcelValue results_h154();
ExcelValue results_i154();
ExcelValue results_j154();
ExcelValue results_k154();
ExcelValue results_l154();
ExcelValue results_m154();
ExcelValue results_n154();
ExcelValue results_o154();
ExcelValue results_p154();
ExcelValue results_q154();
ExcelValue results_r154();
ExcelValue results_s154();
ExcelValue results_t154();
ExcelValue results_u154();
ExcelValue results_v154();
ExcelValue results_w154();
ExcelValue results_e155();
ExcelValue results_f155();
ExcelValue results_g155();
ExcelValue results_h155();
ExcelValue results_i155();
ExcelValue results_j155();
ExcelValue results_k155();
ExcelValue results_l155();
ExcelValue results_m155();
ExcelValue results_n155();
ExcelValue results_o155();
ExcelValue results_p155();
ExcelValue results_q155();
ExcelValue results_r155();
ExcelValue results_s155();
ExcelValue results_t155();
ExcelValue results_u155();
ExcelValue results_v155();
ExcelValue results_w155();
ExcelValue results_e156();
ExcelValue results_f156();
ExcelValue results_g156();
ExcelValue results_h156();
ExcelValue results_i156();
ExcelValue results_j156();
ExcelValue results_k156();
ExcelValue results_l156();
ExcelValue results_m156();
ExcelValue results_n156();
ExcelValue results_o156();
ExcelValue results_p156();
ExcelValue results_q156();
ExcelValue results_r156();
ExcelValue results_s156();
ExcelValue results_t156();
ExcelValue results_u156();
ExcelValue results_v156();
ExcelValue results_w156();
ExcelValue results_e157();
ExcelValue results_f157();
ExcelValue results_g157();
ExcelValue results_h157();
ExcelValue results_i157();
ExcelValue results_j157();
ExcelValue results_k157();
ExcelValue results_l157();
ExcelValue results_m157();
ExcelValue results_n157();
ExcelValue results_o157();
ExcelValue results_p157();
ExcelValue results_q157();
ExcelValue results_r157();
ExcelValue results_s157();
ExcelValue results_t157();
ExcelValue results_u157();
ExcelValue results_v157();
ExcelValue results_w157();
ExcelValue results_e158();
ExcelValue results_f158();
ExcelValue results_g158();
ExcelValue results_h158();
ExcelValue results_i158();
ExcelValue results_j158();
ExcelValue results_k158();
ExcelValue results_l158();
ExcelValue results_m158();
ExcelValue results_n158();
ExcelValue results_o158();
ExcelValue results_p158();
ExcelValue results_q158();
ExcelValue results_r158();
ExcelValue results_s158();
ExcelValue results_t158();
ExcelValue results_u158();
ExcelValue results_v158();
ExcelValue results_w158();
ExcelValue results_e159();
ExcelValue results_f159();
ExcelValue results_g159();
ExcelValue results_h159();
ExcelValue results_i159();
ExcelValue results_j159();
ExcelValue results_k159();
ExcelValue results_l159();
ExcelValue results_m159();
ExcelValue results_n159();
ExcelValue results_o159();
ExcelValue results_p159();
ExcelValue results_q159();
ExcelValue results_r159();
ExcelValue results_s159();
ExcelValue results_t159();
ExcelValue results_u159();
ExcelValue results_v159();
ExcelValue results_w159();
ExcelValue results_e160();
ExcelValue results_f160();
ExcelValue results_g160();
ExcelValue results_h160();
ExcelValue results_i160();
ExcelValue results_j160();
ExcelValue results_k160();
ExcelValue results_l160();
ExcelValue results_m160();
ExcelValue results_n160();
ExcelValue results_o160();
ExcelValue results_p160();
ExcelValue results_q160();
ExcelValue results_r160();
ExcelValue results_s160();
ExcelValue results_t160();
ExcelValue results_u160();
ExcelValue results_v160();
ExcelValue results_w160();
ExcelValue results_e161();
ExcelValue results_f161();
ExcelValue results_g161();
ExcelValue results_h161();
ExcelValue results_i161();
ExcelValue results_j161();
ExcelValue results_k161();
ExcelValue results_l161();
ExcelValue results_m161();
ExcelValue results_n161();
ExcelValue results_o161();
ExcelValue results_p161();
ExcelValue results_q161();
ExcelValue results_r161();
ExcelValue results_s161();
ExcelValue results_t161();
ExcelValue results_u161();
ExcelValue results_v161();
ExcelValue results_w161();
ExcelValue results_e644();
ExcelValue results_f644();
ExcelValue results_g644();
ExcelValue results_h644();
ExcelValue results_i644();
ExcelValue results_j644();
ExcelValue results_k644();
ExcelValue results_l644();
ExcelValue results_m644();
ExcelValue results_n644();
ExcelValue results_o644();
ExcelValue results_p644();
ExcelValue results_q644();
ExcelValue results_r644();
ExcelValue results_s644();
ExcelValue results_t644();
ExcelValue results_u644();
ExcelValue results_v644();
ExcelValue results_w644();
ExcelValue results_e645();
ExcelValue results_f645();
ExcelValue results_g645();
ExcelValue results_h645();
ExcelValue results_i645();
ExcelValue results_j645();
ExcelValue results_k645();
ExcelValue results_l645();
ExcelValue results_m645();
ExcelValue results_n645();
ExcelValue results_o645();
ExcelValue results_p645();
ExcelValue results_q645();
ExcelValue results_r645();
ExcelValue results_s645();
ExcelValue results_t645();
ExcelValue results_u645();
ExcelValue results_v645();
ExcelValue results_w645();
ExcelValue results_e646();
ExcelValue results_f646();
ExcelValue results_g646();
ExcelValue results_h646();
ExcelValue results_i646();
ExcelValue results_j646();
ExcelValue results_k646();
ExcelValue results_l646();
ExcelValue results_m646();
ExcelValue results_n646();
ExcelValue results_o646();
ExcelValue results_p646();
ExcelValue results_q646();
ExcelValue results_r646();
ExcelValue results_s646();
ExcelValue results_t646();
ExcelValue results_u646();
ExcelValue results_v646();
ExcelValue results_w646();
ExcelValue results_e647();
ExcelValue results_f647();
ExcelValue results_g647();
ExcelValue results_h647();
ExcelValue results_i647();
ExcelValue results_j647();
ExcelValue results_k647();
ExcelValue results_l647();
ExcelValue results_m647();
ExcelValue results_n647();
ExcelValue results_o647();
ExcelValue results_p647();
ExcelValue results_q647();
ExcelValue results_r647();
ExcelValue results_s647();
ExcelValue results_t647();
ExcelValue results_u647();
ExcelValue results_v647();
ExcelValue results_w647();
ExcelValue results_e689();
ExcelValue results_f689();
ExcelValue results_g689();
ExcelValue results_h689();
ExcelValue results_i689();
ExcelValue results_j689();
ExcelValue results_k689();
ExcelValue results_l689();
ExcelValue results_m689();
ExcelValue results_n689();
ExcelValue results_o689();
ExcelValue results_p689();
ExcelValue results_q689();
ExcelValue results_r689();
ExcelValue results_s689();
ExcelValue results_t689();
ExcelValue results_u689();
ExcelValue results_v689();
ExcelValue results_w689();
ExcelValue results_e690();
ExcelValue results_f690();
ExcelValue results_g690();
ExcelValue results_h690();
ExcelValue results_i690();
ExcelValue results_j690();
ExcelValue results_k690();
ExcelValue results_l690();
ExcelValue results_m690();
ExcelValue results_n690();
ExcelValue results_o690();
ExcelValue results_p690();
ExcelValue results_q690();
ExcelValue results_r690();
ExcelValue results_s690();
ExcelValue results_t690();
ExcelValue results_u690();
ExcelValue results_v690();
ExcelValue results_w690();
ExcelValue results_e691();
ExcelValue results_f691();
ExcelValue results_g691();
ExcelValue results_h691();
ExcelValue results_i691();
ExcelValue results_j691();
ExcelValue results_k691();
ExcelValue results_l691();
ExcelValue results_m691();
ExcelValue results_n691();
ExcelValue results_o691();
ExcelValue results_p691();
ExcelValue results_q691();
ExcelValue results_r691();
ExcelValue results_s691();
ExcelValue results_t691();
ExcelValue results_u691();
ExcelValue results_v691();
ExcelValue results_w691();
ExcelValue results_e692();
ExcelValue results_f692();
ExcelValue results_g692();
ExcelValue results_h692();
ExcelValue results_i692();
ExcelValue results_j692();
ExcelValue results_k692();
ExcelValue results_l692();
ExcelValue results_m692();
ExcelValue results_n692();
ExcelValue results_o692();
ExcelValue results_p692();
ExcelValue results_q692();
ExcelValue results_r692();
ExcelValue results_s692();
ExcelValue results_t692();
ExcelValue results_u692();
ExcelValue results_v692();
ExcelValue results_w692();
// starting the value constants
static ExcelValue constant1 = {.type = ExcelNumber, .number = 1.5};
static ExcelValue constant2 = {.type = ExcelNumber, .number = 2050.0};
static ExcelValue constant3 = {.type = ExcelNumber, .number = 2040.0};
static ExcelValue constant4 = {.type = ExcelNumber, .number = 2060.0};
static ExcelValue constant5 = {.type = ExcelNumber, .number = 2070.0};
static ExcelValue constant6 = {.type = ExcelNumber, .number = 2065.0};
static ExcelValue constant7 = {.type = ExcelNumber, .number = 2020.0};
static ExcelValue constant8 = {.type = ExcelNumber, .number = 2030.0};
static ExcelValue constant9 = {.type = ExcelNumber, .number = 2035.0};
static ExcelValue constant10 = {.type = ExcelString, .string = "bau"};
static ExcelValue constant11 = {.type = ExcelString, .string = "jean-claude junker"};
static ExcelValue constant12 = {.type = ExcelNumber, .number = 66.0};
static ExcelValue constant13 = {.type = ExcelNumber, .number = 12.0};
static ExcelValue constant14 = {.type = ExcelString, .string = "Buildings Temperature"};
static ExcelValue constant15 = {.type = ExcelString, .string = "Buildings Insulation"};
static ExcelValue constant16 = {.type = ExcelString, .string = "District Heat Share"};
static ExcelValue constant17 = {.type = ExcelString, .string = "Heat Pump Share"};
static ExcelValue constant18 = {.type = ExcelString, .string = "Hybrid Heat Share"};
static ExcelValue constant19 = {.type = ExcelString, .string = "Heat Network - Heat Pump"};
static ExcelValue constant20 = {.type = ExcelString, .string = "Heat Network - Biomass CHP"};
static ExcelValue constant21 = {.type = ExcelString, .string = "Lighting and Applicances"};
static ExcelValue constant22 = {.type = ExcelString, .string = "UK Transport Demand"};
static ExcelValue constant23 = {.type = ExcelString, .string = "International Aviation"};
static ExcelValue constant24 = {.type = ExcelString, .string = "Light Vehicles - Electric"};
static ExcelValue constant25 = {.type = ExcelString, .string = "Light Vehicles - Hydrogen"};
static ExcelValue constant26 = {.type = ExcelString, .string = "Light Vehicles - Hybrid"};
static ExcelValue constant27 = {.type = ExcelString, .string = "Light Vehicles - Biofuel"};
static ExcelValue constant28 = {.type = ExcelString, .string = "Heavy Vehicles - Electric"};
static ExcelValue constant29 = {.type = ExcelString, .string = "Heavy Vehicles - Hydrogen"};
static ExcelValue constant30 = {.type = ExcelString, .string = "Heavy Vehicles - Hybrid"};
static ExcelValue constant31 = {.type = ExcelString, .string = "Heavy Vehicles - Biofuel"};
static ExcelValue constant32 = {.type = ExcelString, .string = "Aviation Efficiency"};
static ExcelValue constant33 = {.type = ExcelString, .string = "Aviation Biofuel"};
static ExcelValue constant34 = {.type = ExcelString, .string = "Industrial Efficiency"};
static ExcelValue constant35 = {.type = ExcelString, .string = "Industrial Electrification"};
static ExcelValue constant36 = {.type = ExcelString, .string = "Industry Shift to Biomass"};
static ExcelValue constant37 = {.type = ExcelString, .string = "Industry Shift to Gas"};
static ExcelValue constant38 = {.type = ExcelString, .string = "Industry CCS"};
static ExcelValue constant39 = {.type = ExcelString, .string = "Hydrogen Gas Grid Share"};
static ExcelValue constant40 = {.type = ExcelString, .string = "Biomethane Gas Grid Share"};
static ExcelValue constant41 = {.type = ExcelString, .string = "Hydrogen from Biomass CCS"};
static ExcelValue constant42 = {.type = ExcelString, .string = "Hydrogen from Methane CCS"};
static ExcelValue constant43 = {.type = ExcelString, .string = "Zero Carbon Hydrogen Imports"};
static ExcelValue constant44 = {.type = ExcelString, .string = "Greenhouse Gas Removal"};
static ExcelValue constant45 = {.type = ExcelString, .string = "Bio Transformation with CCS"};
static ExcelValue constant46 = {.type = ExcelString, .string = "CCS Capture Rate"};
static ExcelValue constant47 = {.type = ExcelString, .string = "Seasonal Storage"};
static ExcelValue constant48 = {.type = ExcelString, .string = "DSR, Batteries & Interconnectors"};
static ExcelValue constant49 = {.type = ExcelString, .string = "Biomass with CCS"};
static ExcelValue constant50 = {.type = ExcelString, .string = "Nuclear"};
static ExcelValue constant51 = {.type = ExcelString, .string = "Offshore & Onshore Wind"};
static ExcelValue constant52 = {.type = ExcelString, .string = "Solar"};
static ExcelValue constant53 = {.type = ExcelString, .string = "Wave & Tidal"};
static ExcelValue constant54 = {.type = ExcelString, .string = "Gas with CCS"};
static ExcelValue constant55 = {.type = ExcelString, .string = "Farming Yield & Efficiency"};
static ExcelValue constant56 = {.type = ExcelString, .string = "Forestry"};
static ExcelValue constant57 = {.type = ExcelString, .string = "Land for Bioenergy"};
static ExcelValue constant58 = {.type = ExcelString, .string = "Waste Reduction"};
static ExcelValue constant59 = {.type = ExcelString, .string = "Fossil Fuel"};
static ExcelValue constant60 = {.type = ExcelNumber, .number = 53.0};
static ExcelValue constant61 = {.type = ExcelNumber, .number = 66.43750816947981};
static ExcelValue constant62 = {.type = ExcelNumber, .number = 71.8765016678253};
static ExcelValue constant63 = {.type = ExcelNumber, .number = 76.31551417040463};
static ExcelValue constant64 = {.type = ExcelNumber, .number = 79.32554980259958};
static ExcelValue constant65 = {.type = ExcelNumber, .number = 78.02565570175432};
static ExcelValue constant66 = {.type = ExcelNumber, .number = 73.02565570175432};
static ExcelValue constant67 = {.type = ExcelNumber, .number = 55.29257845628436};
static ExcelValue constant68 = {.type = ExcelNumber, .number = 61.68677158376656};
static ExcelValue constant69 = {.type = ExcelNumber, .number = 68.38784047318114};
static ExcelValue constant70 = {.type = ExcelNumber, .number = 69.22383888002562};
static ExcelValue constant71 = {.type = ExcelNumber, .number = 70.45649593824434};
static ExcelValue constant72 = {.type = ExcelNumber, .number = 71.93753693990158};
static ExcelValue constant73 = {.type = ExcelNumber, .number = 73.4522603820611};
static ExcelValue constant74 = {.type = ExcelNumber, .number = 74.86874322350724};
static ExcelValue constant75 = {.type = ExcelNumber, .number = 76.28891738993318};
static ExcelValue constant76 = {.type = ExcelNumber, .number = 77.67477081615992};
static ExcelValue constant77 = {.type = ExcelNumber, .number = 79.48889489886054};
static ExcelValue constant78 = {.type = ExcelNumber, .number = 9.956666666666665};
static ExcelValue constant79 = {.type = ExcelNumber, .number = 16.173333333333325};
static ExcelValue constant80 = {.type = ExcelNumber, .number = 22.13};
static ExcelValue constant81 = {.type = ExcelNumber, .number = 28.086666666666662};
static ExcelValue constant82 = {.type = ExcelNumber, .number = 34.04333333333333};
static ExcelValue constant83 = {.type = ExcelNumber, .number = 39.99999999999999};
static ExcelValue constant84 = {.type = ExcelString, .string = "Gas CCS"};
static ExcelValue constant85 = {.type = ExcelNumber, .number = 0.3333333333333332};
static ExcelValue constant86 = {.type = ExcelNumber, .number = 0.6666666666666664};
static ExcelValue constant87 = {.type = ExcelNumber, .number = 0.9999999999999999};
static ExcelValue constant88 = {.type = ExcelNumber, .number = 1.3333333333333328};
static ExcelValue constant89 = {.type = ExcelNumber, .number = 1.6666666666666665};
static ExcelValue constant90 = {.type = ExcelNumber, .number = 1.9999999999999998};
static ExcelValue constant91 = {.type = ExcelString, .string = "Biomass CCS"};
static ExcelValue constant92 = {.type = ExcelString, .string = "Wind Onshore"};
static ExcelValue constant93 = {.type = ExcelNumber, .number = 6.666666666666666};
static ExcelValue constant94 = {.type = ExcelNumber, .number = 7.333333333333332};
static ExcelValue constant95 = {.type = ExcelNumber, .number = 7.999999999999999};
static ExcelValue constant96 = {.type = ExcelNumber, .number = 10.666666666666664};
static ExcelValue constant97 = {.type = ExcelNumber, .number = 13.333333333333332};
static ExcelValue constant98 = {.type = ExcelNumber, .number = 15.999999999999998};
static ExcelValue constant99 = {.type = ExcelString, .string = "Wind Offshore"};
static ExcelValue constant100 = {.type = ExcelNumber, .number = 5.333333333333332};
static ExcelValue constant101 = {.type = ExcelNumber, .number = 8.166666666666666};
static ExcelValue constant102 = {.type = ExcelNumber, .number = 9.999999999999998};
static ExcelValue constant103 = {.type = ExcelNumber, .number = 16.66666666666666};
static ExcelValue constant104 = {.type = ExcelNumber, .number = 19.999999999999996};
static ExcelValue constant105 = {.type = ExcelNumber, .number = 1.6};
static ExcelValue constant106 = {.type = ExcelNumber, .number = 1.733333333333333};
static ExcelValue constant107 = {.type = ExcelNumber, .number = 2.0666666666666664};
static ExcelValue constant108 = {.type = ExcelNumber, .number = 2.3999999999999995};
static ExcelValue constant109 = {.type = ExcelNumber, .number = 2.733333333333333};
static ExcelValue constant110 = {.type = ExcelNumber, .number = 3.066666666666666};
static ExcelValue constant111 = {.type = ExcelString, .string = "Tidal & Wave"};
static ExcelValue constant112 = {.type = ExcelNumber, .number = 0.9999999999999998};
static ExcelValue constant113 = {.type = ExcelNumber, .number = 1.9999999999999996};
static ExcelValue constant114 = {.type = ExcelNumber, .number = 2.999999999999999};
static ExcelValue constant115 = {.type = ExcelNumber, .number = 3.999999999999999};
static ExcelValue constant116 = {.type = ExcelNumber, .number = 5.999999999999998};
static ExcelValue constant117 = {.type = ExcelString, .string = "Other Renewbles"};
static ExcelValue constant118 = {.type = ExcelNumber, .number = 4.5};
static ExcelValue constant119 = {.type = ExcelNumber, .number = 3.3};
static ExcelValue constant120 = {.type = ExcelNumber, .number = 1.5000000000000002};
static ExcelValue constant121 = {.type = ExcelNumber, .number = 241.47548240108796};
static ExcelValue constant122 = {.type = ExcelNumber, .number = 249.62729132125457};
static ExcelValue constant123 = {.type = ExcelNumber, .number = 251.72226659141342};
static ExcelValue constant124 = {.type = ExcelNumber, .number = 244.51162741174724};
static ExcelValue constant125 = {.type = ExcelNumber, .number = 271.2342418942791};
static ExcelValue constant126 = {.type = ExcelNumber, .number = 274.8909939282381};
static ExcelValue constant127 = {.type = ExcelNumber, .number = 240.85409434318953};
static ExcelValue constant128 = {.type = ExcelNumber, .number = 172.7896088928918};
static ExcelValue constant129 = {.type = ExcelNumber, .number = 211.17332363575292};
static ExcelValue constant130 = {.type = ExcelNumber, .number = 248.42564131361553};
static ExcelValue constant131 = {.type = ExcelNumber, .number = 251.72135317015756};
static ExcelValue constant132 = {.type = ExcelNumber, .number = 255.24233461677017};
static ExcelValue constant133 = {.type = ExcelNumber, .number = 262.5545728941863};
static ExcelValue constant134 = {.type = ExcelNumber, .number = 269.9841454037849};
static ExcelValue constant135 = {.type = ExcelNumber, .number = 277.3880949965715};
static ExcelValue constant136 = {.type = ExcelNumber, .number = 284.8205051506565};
static ExcelValue constant137 = {.type = ExcelNumber, .number = 291.8898639346455};
static ExcelValue constant138 = {.type = ExcelNumber, .number = 299.17882432036777};
static ExcelValue constant139 = {.type = ExcelNumber, .number = 62.008473986199114};
static ExcelValue constant140 = {.type = ExcelNumber, .number = 54.092579151642866};
static ExcelValue constant141 = {.type = ExcelNumber, .number = 67.45248879918904};
static ExcelValue constant142 = {.type = ExcelNumber, .number = 108.80054917229447};
static ExcelValue constant143 = {.type = ExcelNumber, .number = 149.78483082672815};
static ExcelValue constant144 = {.type = ExcelNumber, .number = 190.918137436062};
static ExcelValue constant145 = {.type = ExcelNumber, .number = 232.52464570086767};
static ExcelValue constant146 = {.type = ExcelNumber, .number = 275.1603593950432};
static ExcelValue constant147 = {.type = ExcelNumber, .number = 273.7364534337893};
static ExcelValue constant148 = {.type = ExcelNumber, .number = 273.7501827318159};
static ExcelValue constant149 = {.type = ExcelNumber, .number = 273.706237912385};
static ExcelValue constant150 = {.type = ExcelNumber, .number = 273.5971085939215};
static ExcelValue constant151 = {.type = ExcelNumber, .number = 273.57101254192423};
static ExcelValue constant152 = {.type = ExcelNumber, .number = 273.54327744667034};
static ExcelValue constant153 = {.type = ExcelNumber, .number = 273.53285054647586};
static ExcelValue constant154 = {.type = ExcelNumber, .number = 273.5229163155353};
static ExcelValue constant155 = {.type = ExcelNumber, .number = 273.5075279488801};
static ExcelValue constant156 = {.type = ExcelNumber, .number = 273.4252423237401};
static ExcelValue constant157 = {.type = ExcelNumber, .number = 2.5404770639132126};
static ExcelValue constant158 = {.type = ExcelNumber, .number = 5.045367593521487};
static ExcelValue constant159 = {.type = ExcelNumber, .number = 7.614457057391285};
static ExcelValue constant160 = {.type = ExcelNumber, .number = 10.196197703088998};
static ExcelValue constant161 = {.type = ExcelNumber, .number = 12.806728013976112};
static ExcelValue constant162 = {.type = ExcelNumber, .number = 15.477770215971182};
static ExcelValue constant163 = {.type = ExcelNumber, .number = 15.39767550565065};
static ExcelValue constant164 = {.type = ExcelNumber, .number = 15.398447778664648};
static ExcelValue constant165 = {.type = ExcelNumber, .number = 15.395975882571657};
static ExcelValue constant166 = {.type = ExcelNumber, .number = 15.389837358408085};
static ExcelValue constant167 = {.type = ExcelNumber, .number = 15.388369455483236};
static ExcelValue constant168 = {.type = ExcelNumber, .number = 15.386809356375206};
static ExcelValue constant169 = {.type = ExcelNumber, .number = 15.386222843239267};
static ExcelValue constant170 = {.type = ExcelNumber, .number = 15.38566404274886};
static ExcelValue constant171 = {.type = ExcelNumber, .number = 15.384798447124506};
static ExcelValue constant172 = {.type = ExcelNumber, .number = 15.380169880710383};
static ExcelValue constant173 = {.type = ExcelNumber, .number = 9.182431191733249};
static ExcelValue constant174 = {.type = ExcelNumber, .number = 13.134077915552865};
static ExcelValue constant175 = {.type = ExcelNumber, .number = 16.918792502168987};
static ExcelValue constant176 = {.type = ExcelNumber, .number = 20.82870695169617};
static ExcelValue constant177 = {.type = ExcelNumber, .number = 21.82206394947867};
static ExcelValue constant178 = {.type = ExcelNumber, .number = 22.781269842046363};
static ExcelValue constant179 = {.type = ExcelNumber, .number = 22.62316911911715};
static ExcelValue constant180 = {.type = ExcelNumber, .number = 22.502401103002963};
static ExcelValue constant181 = {.type = ExcelNumber, .number = 22.316772661686098};
static ExcelValue constant182 = {.type = ExcelNumber, .number = 22.15372150887955};
static ExcelValue constant183 = {.type = ExcelNumber, .number = 22.010548144375548};
static ExcelValue constant184 = {.type = ExcelNumber, .number = 21.881369965234505};
static ExcelValue constant185 = {.type = ExcelNumber, .number = 21.861621363791173};
static ExcelValue constant186 = {.type = ExcelNumber, .number = 21.85699279737705};
static ExcelValue constant187 = {.type = ExcelNumber, .number = 10.280460885822023};
static ExcelValue constant188 = {.type = ExcelNumber, .number = 10.023944787910716};
static ExcelValue constant189 = {.type = ExcelNumber, .number = 16.75001834589522};
static ExcelValue constant190 = {.type = ExcelNumber, .number = 18.25104156832727};
static ExcelValue constant191 = {.type = ExcelNumber, .number = 20.07522509294679};
static ExcelValue constant192 = {.type = ExcelNumber, .number = 26.921947388760884};
static ExcelValue constant193 = {.type = ExcelNumber, .number = 33.871032938581735};
static ExcelValue constant194 = {.type = ExcelNumber, .number = 41.03527187900865};
static ExcelValue constant195 = {.type = ExcelNumber, .number = 40.750490686757864};
static ExcelValue constant196 = {.type = ExcelNumber, .number = 40.75323654636319};
static ExcelValue constant197 = {.type = ExcelNumber, .number = 40.744447582477};
static ExcelValue constant198 = {.type = ExcelNumber, .number = 40.7226217187843};
static ExcelValue constant199 = {.type = ExcelNumber, .number = 40.71740250838484};
static ExcelValue constant200 = {.type = ExcelNumber, .number = 40.71185548933407};
static ExcelValue constant201 = {.type = ExcelNumber, .number = 40.70977010929517};
static ExcelValue constant202 = {.type = ExcelNumber, .number = 40.70778326310706};
static ExcelValue constant203 = {.type = ExcelNumber, .number = 40.70470558977602};
static ExcelValue constant204 = {.type = ExcelNumber, .number = 40.68824846474803};
static ExcelValue constant205 = {.type = ExcelNumber, .number = 5.140230442911012};
static ExcelValue constant206 = {.type = ExcelNumber, .number = 5.011972393955358};
static ExcelValue constant207 = {.type = ExcelNumber, .number = 13.400014676716175};
static ExcelValue constant208 = {.type = ExcelNumber, .number = 20.3250235647281};
static ExcelValue constant209 = {.type = ExcelNumber, .number = 25.094031366183486};
static ExcelValue constant210 = {.type = ExcelNumber, .number = 33.652434235951105};
static ExcelValue constant211 = {.type = ExcelNumber, .number = 42.33879117322715};
static ExcelValue constant212 = {.type = ExcelNumber, .number = 51.294089848760805};
static ExcelValue constant213 = {.type = ExcelNumber, .number = 50.938113358447325};
static ExcelValue constant214 = {.type = ExcelNumber, .number = 50.941545682953986};
static ExcelValue constant215 = {.type = ExcelNumber, .number = 50.93055947809625};
static ExcelValue constant216 = {.type = ExcelNumber, .number = 50.90327714848038};
static ExcelValue constant217 = {.type = ExcelNumber, .number = 50.89675313548105};
static ExcelValue constant218 = {.type = ExcelNumber, .number = 50.889819361667584};
static ExcelValue constant219 = {.type = ExcelNumber, .number = 50.88721263661896};
static ExcelValue constant220 = {.type = ExcelNumber, .number = 50.88472907888382};
static ExcelValue constant221 = {.type = ExcelNumber, .number = 50.88088198722002};
static ExcelValue constant222 = {.type = ExcelNumber, .number = 50.86031058093503};
static ExcelValue constant223 = {.type = ExcelNumber, .number = 1.5266196531493215};
static ExcelValue constant224 = {.type = ExcelNumber, .number = 1.416776894319978};
static ExcelValue constant225 = {.type = ExcelNumber, .number = 1.6388587154899135};
static ExcelValue constant226 = {.type = ExcelNumber, .number = 1.9466693026611073};
static ExcelValue constant227 = {.type = ExcelNumber, .number = 2.2680731291826053};
static ExcelValue constant228 = {.type = ExcelNumber, .number = 2.5890403527554966};
static ExcelValue constant229 = {.type = ExcelNumber, .number = 2.9123186363810696};
static ExcelValue constant230 = {.type = ExcelNumber, .number = 1.9066513477314122};
static ExcelValue constant231 = {.type = ExcelNumber, .number = 1.9013117003767102};
static ExcelValue constant232 = {.type = ExcelNumber, .number = 1.90136318524431};
static ExcelValue constant233 = {.type = ExcelNumber, .number = 1.901198392171444};
static ExcelValue constant234 = {.type = ExcelNumber, .number = 1.900789157227206};
static ExcelValue constant235 = {.type = ExcelNumber, .number = 1.9006912970322158};
static ExcelValue constant236 = {.type = ExcelNumber, .number = 1.900587290425014};
static ExcelValue constant237 = {.type = ExcelNumber, .number = 1.9005481895492846};
static ExcelValue constant238 = {.type = ExcelNumber, .number = 1.9005109361832575};
static ExcelValue constant239 = {.type = ExcelNumber, .number = 1.9004532298083006};
static ExcelValue constant240 = {.type = ExcelNumber, .number = 1.9001446587140256};
static ExcelValue constant241 = {.type = ExcelNumber, .number = 2.540477063913212};
static ExcelValue constant242 = {.type = ExcelNumber, .number = 7.614457057391284};
static ExcelValue constant243 = {.type = ExcelNumber, .number = 15.47777021597118};
static ExcelValue constant244 = {.type = ExcelNumber, .number = 15.397675505650648};
static ExcelValue constant245 = {.type = ExcelNumber, .number = 15.398447778664647};
static ExcelValue constant246 = {.type = ExcelNumber, .number = 15.386809356375208};
static ExcelValue constant247 = {.type = ExcelNumber, .number = 15.385664042748859};
static ExcelValue constant248 = {.type = ExcelNumber, .number = 15.384798447124503};
static ExcelValue constant249 = {.type = ExcelNumber, .number = 15.38016988071038};
static ExcelValue constant250 = {.type = ExcelNumber, .number = 63.55102101001474};
static ExcelValue constant251 = {.type = ExcelNumber, .number = 83.93252689803668};
static ExcelValue constant252 = {.type = ExcelNumber, .number = 86.53560912065194};
static ExcelValue constant253 = {.type = ExcelNumber, .number = 81.5294182922757};
static ExcelValue constant254 = {.type = ExcelNumber, .number = 66.57554911570435};
static ExcelValue constant255 = {.type = ExcelNumber, .number = 45.82195266870805};
static ExcelValue constant256 = {.type = ExcelNumber, .number = 40.85059879792871};
static ExcelValue constant257 = {.type = ExcelNumber, .number = 38.650531336205205};
static ExcelValue constant258 = {.type = ExcelNumber, .number = 35.97136109555277};
static ExcelValue constant259 = {.type = ExcelNumber, .number = 33.38090019819302};
static ExcelValue constant260 = {.type = ExcelNumber, .number = 32.757766916824885};
static ExcelValue constant261 = {.type = ExcelNumber, .number = 32.29771431798154};
static ExcelValue constant262 = {.type = ExcelNumber, .number = 31.560705188682263};
static ExcelValue constant263 = {.type = ExcelNumber, .number = 30.91435094911117};
static ExcelValue constant264 = {.type = ExcelNumber, .number = 30.343856915354937};
static ExcelValue constant265 = {.type = ExcelNumber, .number = 29.829239700629795};
static ExcelValue constant266 = {.type = ExcelNumber, .number = 29.753491278447793};
static ExcelValue constant267 = {.type = ExcelNumber, .number = 29.75233413684426};
static ExcelValue constant268 = {.type = ExcelString, .string = "Interconnectors"};
static ExcelValue constant269 = {.type = ExcelNumber, .number = 29.473949533329105};
static ExcelValue constant270 = {.type = ExcelNumber, .number = 24.104396225314275};
static ExcelValue constant271 = {.type = ExcelNumber, .number = 19.320686469793998};
static ExcelValue constant272 = {.type = ExcelNumber, .number = 14.389024622180832};
static ExcelValue constant273 = {.type = ExcelNumber, .number = 9.651503355630702};
static ExcelValue constant274 = {.type = ExcelNumber, .number = 4.846470122770267};
static ExcelValue constant275 = {.type = ExcelString, .string = "Domestic District Heat Network "};
static ExcelValue constant276 = {.type = ExcelString, .string = "Domestic Heat Pump Air Source "};
static ExcelValue constant277 = {.type = ExcelString, .string = "Domestic Heat Pump Ground Source "};
static ExcelValue constant278 = {.type = ExcelString, .string = "Domestic Hybrid Heat Pump / Gas Boiler "};
static ExcelValue constant279 = {.type = ExcelNumber, .number = 1.185186608838176};
static ExcelValue constant280 = {.type = ExcelNumber, .number = 2.4899324244003727};
static ExcelValue constant281 = {.type = ExcelNumber, .number = 3.4629595709036702};
static ExcelValue constant282 = {.type = ExcelNumber, .number = 4.1845607700366845};
static ExcelValue constant283 = {.type = ExcelNumber, .number = 4.636232455118452};
static ExcelValue constant284 = {.type = ExcelNumber, .number = 4.873983270159889};
static ExcelValue constant285 = {.type = ExcelNumber, .number = 4.511293360215118};
static ExcelValue constant286 = {.type = ExcelNumber, .number = 4.146564312035282};
static ExcelValue constant287 = {.type = ExcelNumber, .number = 4.149401726414248};
static ExcelValue constant288 = {.type = ExcelNumber, .number = 4.153614831212371};
static ExcelValue constant289 = {.type = ExcelNumber, .number = 4.159453065740231};
static ExcelValue constant290 = {.type = ExcelNumber, .number = 4.166069698092355};
static ExcelValue constant291 = {.type = ExcelNumber, .number = 4.172689232224778};
static ExcelValue constant292 = {.type = ExcelNumber, .number = 4.179307287018227};
static ExcelValue constant293 = {.type = ExcelNumber, .number = 4.186358826063691};
static ExcelValue constant294 = {.type = ExcelNumber, .number = 4.193989374591568};
static ExcelValue constant295 = {.type = ExcelString, .string = "Domestic Oil Fuel Boiler"};
static ExcelValue constant296 = {.type = ExcelNumber, .number = 4.100681825715016};
static ExcelValue constant297 = {.type = ExcelNumber, .number = 5.068432976331472};
static ExcelValue constant298 = {.type = ExcelNumber, .number = 2.55189197636061};
static ExcelValue constant299 = {.type = ExcelNumber, .number = 2.3378632047734884};
static ExcelValue constant300 = {.type = ExcelNumber, .number = 1.8968416968526116};
static ExcelValue constant301 = {.type = ExcelNumber, .number = 1.4750578572497206};
static ExcelValue constant302 = {.type = ExcelNumber, .number = 1.0921587785397118};
static ExcelValue constant303 = {.type = ExcelNumber, .number = 0.7757743647908056};
static ExcelValue constant304 = {.type = ExcelNumber, .number = 0.7370916002293653};
static ExcelValue constant305 = {.type = ExcelNumber, .number = 0.6989682962534598};
static ExcelValue constant306 = {.type = ExcelNumber, .number = 0.6930345303113793};
static ExcelValue constant307 = {.type = ExcelNumber, .number = 0.6870838411584774};
static ExcelValue constant308 = {.type = ExcelNumber, .number = 0.681113160288947};
static ExcelValue constant309 = {.type = ExcelNumber, .number = 0.6751329038708054};
static ExcelValue constant310 = {.type = ExcelNumber, .number = 0.6691526117560862};
static ExcelValue constant311 = {.type = ExcelNumber, .number = 0.6631723378396219};
static ExcelValue constant312 = {.type = ExcelNumber, .number = 0.6571867313677491};
static ExcelValue constant313 = {.type = ExcelNumber, .number = 0.6511940021454373};
static ExcelValue constant314 = {.type = ExcelString, .string = "Domestic Solid Fuel Boiler "};
static ExcelValue constant315 = {.type = ExcelNumber, .number = 5.033438855160621};
static ExcelValue constant316 = {.type = ExcelNumber, .number = 6.221318444621313};
static ExcelValue constant317 = {.type = ExcelNumber, .number = 3.1450635041937227};
static ExcelValue constant318 = {.type = ExcelNumber, .number = 2.8958885787153497};
static ExcelValue constant319 = {.type = ExcelNumber, .number = 2.37051800079658};
static ExcelValue constant320 = {.type = ExcelNumber, .number = 1.8655612576541174};
static ExcelValue constant321 = {.type = ExcelNumber, .number = 1.4044643474760672};
static ExcelValue constant322 = {.type = ExcelNumber, .number = 1.021964667813046};
static ExcelValue constant323 = {.type = ExcelNumber, .number = 0.9751943659752176};
static ExcelValue constant324 = {.type = ExcelNumber, .number = 0.9290905563517873};
static ExcelValue constant325 = {.type = ExcelNumber, .number = 0.9212032087711614};
static ExcelValue constant326 = {.type = ExcelNumber, .number = 0.9132933663285489};
static ExcelValue constant327 = {.type = ExcelNumber, .number = 0.9053569502699021};
static ExcelValue constant328 = {.type = ExcelNumber, .number = 0.8974078060920627};
static ExcelValue constant329 = {.type = ExcelNumber, .number = 0.8894586144652141};
static ExcelValue constant330 = {.type = ExcelNumber, .number = 0.881509447028056};
static ExcelValue constant331 = {.type = ExcelNumber, .number = 0.8735531913911929};
static ExcelValue constant332 = {.type = ExcelNumber, .number = 0.865587467971308};
static ExcelValue constant333 = {.type = ExcelString, .string = "Domestic Gas Boiler "};
static ExcelValue constant334 = {.type = ExcelNumber, .number = 42.047138385937494};
static ExcelValue constant335 = {.type = ExcelNumber, .number = 51.97016296637424};
static ExcelValue constant336 = {.type = ExcelNumber, .number = 52.84857957051086};
static ExcelValue constant337 = {.type = ExcelNumber, .number = 48.827549919601935};
static ExcelValue constant338 = {.type = ExcelNumber, .number = 37.77602053938245};
static ExcelValue constant339 = {.type = ExcelNumber, .number = 27.407726808872436};
static ExcelValue constant340 = {.type = ExcelNumber, .number = 18.214635022480994};
static ExcelValue constant341 = {.type = ExcelNumber, .number = 10.732348331723077};
static ExcelValue constant342 = {.type = ExcelNumber, .number = 9.840755770644467};
static ExcelValue constant343 = {.type = ExcelNumber, .number = 8.962903764672525};
static ExcelValue constant344 = {.type = ExcelNumber, .number = 8.886814801287446};
static ExcelValue constant345 = {.type = ExcelNumber, .number = 8.810508830763712};
static ExcelValue constant346 = {.type = ExcelNumber, .number = 8.733946505505163};
static ExcelValue constant347 = {.type = ExcelNumber, .number = 8.657261392530552};
static ExcelValue constant348 = {.type = ExcelNumber, .number = 8.580575821817026};
static ExcelValue constant349 = {.type = ExcelNumber, .number = 8.503890484460587};
static ExcelValue constant350 = {.type = ExcelNumber, .number = 8.427136767492085};
static ExcelValue constant351 = {.type = ExcelNumber, .number = 8.35029171515534};
static ExcelValue constant352 = {.type = ExcelString, .string = "Domestic Electric Heater "};
static ExcelValue constant353 = {.type = ExcelString, .string = "Non-Domestic District Heat Network "};
static ExcelValue constant354 = {.type = ExcelString, .string = "Non-Domestic Heat Pump Air Source "};
static ExcelValue constant355 = {.type = ExcelString, .string = "Non-Domestic Heat Pump Ground Source "};
static ExcelValue constant356 = {.type = ExcelString, .string = "Non-Domestic Hybrid Heat Pump / Gas Boiler "};
static ExcelValue constant357 = {.type = ExcelNumber, .number = 0.35446921390671504};
static ExcelValue constant358 = {.type = ExcelNumber, .number = 0.7056004405852192};
static ExcelValue constant359 = {.type = ExcelNumber, .number = 0.9736662658392147};
static ExcelValue constant360 = {.type = ExcelNumber, .number = 1.1764068616754073};
static ExcelValue constant361 = {.type = ExcelNumber, .number = 1.317013970650231};
static ExcelValue constant362 = {.type = ExcelNumber, .number = 1.4034107990307993};
static ExcelValue constant363 = {.type = ExcelNumber, .number = 1.312783384091302};
static ExcelValue constant364 = {.type = ExcelNumber, .number = 1.220271936132702};
static ExcelValue constant365 = {.type = ExcelNumber, .number = 1.2352904227046257};
static ExcelValue constant366 = {.type = ExcelNumber, .number = 1.2508776714930974};
static ExcelValue constant367 = {.type = ExcelNumber, .number = 1.2670551444340648};
static ExcelValue constant368 = {.type = ExcelNumber, .number = 1.2838646126640698};
static ExcelValue constant369 = {.type = ExcelNumber, .number = 1.301342442341282};
static ExcelValue constant370 = {.type = ExcelNumber, .number = 1.3195085638527948};
static ExcelValue constant371 = {.type = ExcelNumber, .number = 1.3383754641343175};
static ExcelValue constant372 = {.type = ExcelNumber, .number = 1.357961415064884};
static ExcelValue constant373 = {.type = ExcelString, .string = "Non-Domestic Oil Fuel Boiler"};
static ExcelValue constant374 = {.type = ExcelNumber, .number = 1.7305632511744031};
static ExcelValue constant375 = {.type = ExcelNumber, .number = 1.8609068755747662};
static ExcelValue constant376 = {.type = ExcelNumber, .number = 0.8526338871633609};
static ExcelValue constant377 = {.type = ExcelNumber, .number = 0.7114151532008094};
static ExcelValue constant378 = {.type = ExcelNumber, .number = 0.5608322701544937};
static ExcelValue constant379 = {.type = ExcelNumber, .number = 0.4252169838781159};
static ExcelValue constant380 = {.type = ExcelNumber, .number = 0.3083039658618848};
static ExcelValue constant381 = {.type = ExcelNumber, .number = 0.2111719920983056};
static ExcelValue constant382 = {.type = ExcelNumber, .number = 0.20369943348936603};
static ExcelValue constant383 = {.type = ExcelNumber, .number = 0.19620890033423719};
static ExcelValue constant384 = {.type = ExcelNumber, .number = 0.19747781935874642};
static ExcelValue constant385 = {.type = ExcelNumber, .number = 0.1988230045283175};
static ExcelValue constant386 = {.type = ExcelNumber, .number = 0.20024803837525304};
static ExcelValue constant387 = {.type = ExcelNumber, .number = 0.2017612064096223};
static ExcelValue constant388 = {.type = ExcelNumber, .number = 0.20336949603532847};
static ExcelValue constant389 = {.type = ExcelNumber, .number = 0.2050760216808647};
static ExcelValue constant390 = {.type = ExcelNumber, .number = 0.2068821221218041};
static ExcelValue constant391 = {.type = ExcelNumber, .number = 0.2087904462296081};
static ExcelValue constant392 = {.type = ExcelString, .string = "Non-Domestic Solid Fuel Boiler "};
static ExcelValue constant393 = {.type = ExcelNumber, .number = 2.124203895837612};
static ExcelValue constant394 = {.type = ExcelNumber, .number = 2.284195987753901};
static ExcelValue constant395 = {.type = ExcelNumber, .number = 1.051224806479803};
static ExcelValue constant396 = {.type = ExcelNumber, .number = 0.8820993635680753};
static ExcelValue constant397 = {.type = ExcelNumber, .number = 0.7023713047026572};
static ExcelValue constant398 = {.type = ExcelNumber, .number = 0.5400556713848293};
static ExcelValue constant399 = {.type = ExcelNumber, .number = 0.39975953722157137};
static ExcelValue constant400 = {.type = ExcelNumber, .number = 0.2829185486341696};
static ExcelValue constant401 = {.type = ExcelNumber, .number = 0.2742311878937669};
static ExcelValue constant402 = {.type = ExcelNumber, .number = 0.26553591006625604};
static ExcelValue constant403 = {.type = ExcelNumber, .number = 0.26725317960601425};
static ExcelValue constant404 = {.type = ExcelNumber, .number = 0.26907366260959475};
static ExcelValue constant405 = {.type = ExcelNumber, .number = 0.2710022074349144};
static ExcelValue constant406 = {.type = ExcelNumber, .number = 0.27305002713323057};
static ExcelValue constant407 = {.type = ExcelNumber, .number = 0.27522657798634953};
static ExcelValue constant408 = {.type = ExcelNumber, .number = 0.2775360748520215};
static ExcelValue constant409 = {.type = ExcelNumber, .number = 0.279980329538934};
static ExcelValue constant410 = {.type = ExcelNumber, .number = 0.28256292685130824};
static ExcelValue constant411 = {.type = ExcelString, .string = "Non-Domestic Gas Boiler "};
static ExcelValue constant412 = {.type = ExcelNumber, .number = 17.744666765278794};
static ExcelValue constant413 = {.type = ExcelNumber, .number = 19.081170460473704};
static ExcelValue constant414 = {.type = ExcelNumber, .number = 17.69288560657506};
static ExcelValue constant415 = {.type = ExcelNumber, .number = 14.919768402687055};
static ExcelValue constant416 = {.type = ExcelNumber, .number = 11.240142326323836};
static ExcelValue constant417 = {.type = ExcelNumber, .number = 7.96000212445711};
static ExcelValue constant418 = {.type = ExcelNumber, .number = 5.159440413355561};
static ExcelValue constant419 = {.type = ExcelNumber, .number = 2.8537617916700504};
static ExcelValue constant420 = {.type = ExcelNumber, .number = 2.6538708629840846};
static ExcelValue constant421 = {.type = ExcelNumber, .number = 2.452529520311291};
static ExcelValue constant422 = {.type = ExcelNumber, .number = 2.4683904795297167};
static ExcelValue constant423 = {.type = ExcelNumber, .number = 2.485204733791568};
static ExcelValue constant424 = {.type = ExcelNumber, .number = 2.503017063258267};
static ExcelValue constant425 = {.type = ExcelNumber, .number = 2.521931033354223};
static ExcelValue constant426 = {.type = ExcelNumber, .number = 2.5420339837175123};
static ExcelValue constant427 = {.type = ExcelNumber, .number = 2.5633648434069367};
static ExcelValue constant428 = {.type = ExcelNumber, .number = 2.5859403465594717};
static ExcelValue constant429 = {.type = ExcelNumber, .number = 2.6097936029649578};
static ExcelValue constant430 = {.type = ExcelString, .string = "Non-Domestic Electric Heater "};
static ExcelValue constant431 = {.type = ExcelString, .string = "Domestic Gas Stove "};
static ExcelValue constant432 = {.type = ExcelNumber, .number = 2.9274765301711088};
static ExcelValue constant433 = {.type = ExcelNumber, .number = 3.0292251769810115};
static ExcelValue constant434 = {.type = ExcelNumber, .number = 3.107049838302372};
static ExcelValue constant435 = {.type = ExcelNumber, .number = 2.9784713983966014};
static ExcelValue constant436 = {.type = ExcelNumber, .number = 2.663917187565171};
static ExcelValue constant437 = {.type = ExcelNumber, .number = 2.353321686300182};
static ExcelValue constant438 = {.type = ExcelNumber, .number = 2.052648515689095};
static ExcelValue constant439 = {.type = ExcelNumber, .number = 1.7645033085119957};
static ExcelValue constant440 = {.type = ExcelNumber, .number = 1.6591277334826475};
static ExcelValue constant441 = {.type = ExcelNumber, .number = 1.5479293584325513};
static ExcelValue constant442 = {.type = ExcelNumber, .number = 1.5719205365612399};
static ExcelValue constant443 = {.type = ExcelNumber, .number = 1.597315131298313};
static ExcelValue constant444 = {.type = ExcelNumber, .number = 1.6243676092493187};
static ExcelValue constant445 = {.type = ExcelNumber, .number = 1.6522141731511244};
static ExcelValue constant446 = {.type = ExcelNumber, .number = 1.6800636973168175};
static ExcelValue constant447 = {.type = ExcelNumber, .number = 1.707911712328372};
static ExcelValue constant448 = {.type = ExcelNumber, .number = 1.7362019481988393};
static ExcelValue constant449 = {.type = ExcelNumber, .number = 1.765082863129586};
static ExcelValue constant450 = {.type = ExcelString, .string = "Domestic Electric Stove "};
static ExcelValue constant451 = {.type = ExcelString, .string = "Non-Domestic Gas Stove "};
static ExcelValue constant452 = {.type = ExcelNumber, .number = 3.450351871026432};
static ExcelValue constant453 = {.type = ExcelNumber, .number = 3.537477579502817};
static ExcelValue constant454 = {.type = ExcelNumber, .number = 3.5802580703545055};
static ExcelValue constant455 = {.type = ExcelNumber, .number = 3.394229944693024};
static ExcelValue constant456 = {.type = ExcelNumber, .number = 3.0090448168652415};
static ExcelValue constant457 = {.type = ExcelNumber, .number = 2.636803497151705};
static ExcelValue constant458 = {.type = ExcelNumber, .number = 2.2792779895732944};
static ExcelValue constant459 = {.type = ExcelNumber, .number = 1.9383435723916995};
static ExcelValue constant460 = {.type = ExcelNumber, .number = 1.8375747164632683};
static ExcelValue constant461 = {.type = ExcelNumber, .number = 1.7304808429168201};
static ExcelValue constant462 = {.type = ExcelNumber, .number = 1.7741776527146829};
static ExcelValue constant463 = {.type = ExcelNumber, .number = 1.8189778617176429};
static ExcelValue constant464 = {.type = ExcelNumber, .number = 1.8649093321383288};
static ExcelValue constant465 = {.type = ExcelNumber, .number = 1.9120006297451546};
static ExcelValue constant466 = {.type = ExcelNumber, .number = 1.9602810416279823};
static ExcelValue constant467 = {.type = ExcelNumber, .number = 2.0097805944123937};
static ExcelValue constant468 = {.type = ExcelNumber, .number = 2.0605300729338922};
static ExcelValue constant469 = {.type = ExcelNumber, .number = 2.112561039383658};
static ExcelValue constant470 = {.type = ExcelString, .string = "Non-Domestic Electric Stove "};
static ExcelValue constant471 = {.type = ExcelString, .string = "Domestic Lighting "};
static ExcelValue constant472 = {.type = ExcelString, .string = "Non-Domestic Lighting "};
static ExcelValue constant473 = {.type = ExcelString, .string = "Domestic Appliances "};
static ExcelValue constant474 = {.type = ExcelNumber, .number = 1.88328331556766};
static ExcelValue constant475 = {.type = ExcelNumber, .number = 1.04871338463667};
static ExcelValue constant476 = {.type = ExcelNumber, .number = 1.11824754259859};
static ExcelValue constant477 = {.type = ExcelNumber, .number = 1.18245997541111};
static ExcelValue constant478 = {.type = ExcelNumber, .number = 1.23957686452896};
static ExcelValue constant479 = {.type = ExcelString, .string = "Non-Domestic Appliances "};
static ExcelValue constant480 = {.type = ExcelNumber, .number = 14.6911816051795};
static ExcelValue constant481 = {.type = ExcelNumber, .number = 10.2904069370086};
static ExcelValue constant482 = {.type = ExcelNumber, .number = 6.52949922146348};
static ExcelValue constant483 = {.type = ExcelNumber, .number = 3.54680132146847};
static ExcelValue constant484 = {.type = ExcelNumber, .number = 1.91416249173958};
static ExcelValue constant485 = {.type = ExcelString, .string = "Domestic Air Conditioning "};
static ExcelValue constant486 = {.type = ExcelString, .string = "Non-Domestic Air Conditioning "};
static ExcelValue constant487 = {.type = ExcelString, .string = "Heat Network Waste Heat Offtake from Power "};
static ExcelValue constant488 = {.type = ExcelString, .string = "Heat Network Heat Pump Water Source "};
static ExcelValue constant489 = {.type = ExcelString, .string = "Heat Network Heat Pump Ground Source "};
static ExcelValue constant490 = {.type = ExcelString, .string = "Heat Network Biomass CHP "};
static ExcelValue constant491 = {.type = ExcelNumber, .number = 0.09924086000047816};
static ExcelValue constant492 = {.type = ExcelNumber, .number = 0.43180372592632443};
static ExcelValue constant493 = {.type = ExcelNumber, .number = 0.5178361304012347};
static ExcelValue constant494 = {.type = ExcelNumber, .number = 0.27346402391975316};
static ExcelValue constant495 = {.type = ExcelNumber, .number = 0.029091917438271688};
static ExcelValue constant496 = {.type = ExcelNumber, .number = 7.441578217834731e-17};
static ExcelValue constant497 = {.type = ExcelNumber, .number = -7.441578217834731e-17};
static ExcelValue constant498 = {.type = ExcelString, .string = "Heat Network Gas CHP "};
static ExcelValue constant499 = {.type = ExcelNumber, .number = 11.21235872695641};
static ExcelValue constant500 = {.type = ExcelNumber, .number = 12.89460214891471};
static ExcelValue constant501 = {.type = ExcelNumber, .number = 4.6715982089885015};
static ExcelValue constant502 = {.type = ExcelString, .string = "Gas Grid Distribution All Types "};
static ExcelValue constant503 = {.type = ExcelNumber, .number = 14.526829661964548};
static ExcelValue constant504 = {.type = ExcelNumber, .number = 16.31334161813321};
static ExcelValue constant505 = {.type = ExcelNumber, .number = 14.978200731814326};
static ExcelValue constant506 = {.type = ExcelNumber, .number = 13.146506217951032};
static ExcelValue constant507 = {.type = ExcelNumber, .number = 10.60267258478709};
static ExcelValue constant508 = {.type = ExcelNumber, .number = 8.191253854028666};
static ExcelValue constant509 = {.type = ExcelNumber, .number = 6.097629795742718};
static ExcelValue constant510 = {.type = ExcelNumber, .number = 4.360954417054294};
static ExcelValue constant511 = {.type = ExcelNumber, .number = 3.9748668521101664};
static ExcelValue constant512 = {.type = ExcelNumber, .number = 3.6389254252324994};
static ExcelValue constant513 = {.type = ExcelNumber, .number = 3.6049695400003694};
static ExcelValue constant514 = {.type = ExcelNumber, .number = 3.571881078157531};
static ExcelValue constant515 = {.type = ExcelNumber, .number = 3.5779424683062384};
static ExcelValue constant516 = {.type = ExcelNumber, .number = 3.584730088221861};
static ExcelValue constant517 = {.type = ExcelNumber, .number = 3.592028016810906};
static ExcelValue constant518 = {.type = ExcelNumber, .number = 3.599849752561418};
static ExcelValue constant519 = {.type = ExcelNumber, .number = 3.6083407846722446};
static ExcelValue constant520 = {.type = ExcelNumber, .number = 3.6175603015489486};
static ExcelValue constant521 = {.type = ExcelString, .string = "Gas Grid Transmission All Types "};
static ExcelValue constant522 = {.type = ExcelNumber, .number = 12.477180032913884};
static ExcelValue constant523 = {.type = ExcelNumber, .number = 12.830963143295822};
static ExcelValue constant524 = {.type = ExcelNumber, .number = 11.526650803657589};
static ExcelValue constant525 = {.type = ExcelNumber, .number = 9.958589305459673};
static ExcelValue constant526 = {.type = ExcelNumber, .number = 7.88133765757809};
static ExcelValue constant527 = {.type = ExcelNumber, .number = 5.939884796717406};
static ExcelValue constant528 = {.type = ExcelNumber, .number = 4.431473576012041};
static ExcelValue constant529 = {.type = ExcelNumber, .number = 3.244780020731158};
static ExcelValue constant530 = {.type = ExcelNumber, .number = 2.8016501078117657};
static ExcelValue constant531 = {.type = ExcelNumber, .number = 2.4910906434780893};
static ExcelValue constant532 = {.type = ExcelNumber, .number = 2.393305944290221};
static ExcelValue constant533 = {.type = ExcelNumber, .number = 2.2959819852447185};
static ExcelValue constant534 = {.type = ExcelNumber, .number = 2.2992015499860727};
static ExcelValue constant535 = {.type = ExcelNumber, .number = 2.302806858540991};
static ExcelValue constant536 = {.type = ExcelNumber, .number = 2.3066832223772815};
static ExcelValue constant537 = {.type = ExcelNumber, .number = 2.3108378113445935};
static ExcelValue constant538 = {.type = ExcelNumber, .number = 2.3153479034077895};
static ExcelValue constant539 = {.type = ExcelNumber, .number = 2.320244937048614};
static ExcelValue constant540 = {.type = ExcelString, .string = "Car Electric "};
static ExcelValue constant541 = {.type = ExcelString, .string = "Car Hydrogen "};
static ExcelValue constant542 = {.type = ExcelString, .string = "Car Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant543 = {.type = ExcelNumber, .number = 0.43574906731164714};
static ExcelValue constant544 = {.type = ExcelNumber, .number = 0.4094104115618428};
static ExcelValue constant545 = {.type = ExcelNumber, .number = 3.053955201446044};
static ExcelValue constant546 = {.type = ExcelNumber, .number = 5.643805311162076};
static ExcelValue constant547 = {.type = ExcelNumber, .number = 7.867757561249739};
static ExcelValue constant548 = {.type = ExcelNumber, .number = 3.664759158023485};
static ExcelValue constant549 = {.type = ExcelNumber, .number = 1.8624901788336932};
static ExcelValue constant550 = {.type = ExcelString, .string = "Car Internal Combustion Engine "};
static ExcelValue constant551 = {.type = ExcelNumber, .number = 79.44548893302635};
static ExcelValue constant552 = {.type = ExcelNumber, .number = 71.5862552651352};
static ExcelValue constant553 = {.type = ExcelNumber, .number = 44.62653040729231};
static ExcelValue constant554 = {.type = ExcelNumber, .number = 19.978994330547334};
static ExcelValue constant555 = {.type = ExcelString, .string = "LGV (van) Electric "};
static ExcelValue constant556 = {.type = ExcelString, .string = "LGV (van) Hydrogen "};
static ExcelValue constant557 = {.type = ExcelString, .string = "LGV (van) Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant558 = {.type = ExcelNumber, .number = 0.07634151997965176};
static ExcelValue constant559 = {.type = ExcelNumber, .number = 0.06300519973783565};
static ExcelValue constant560 = {.type = ExcelNumber, .number = 0.5170193400958447};
static ExcelValue constant561 = {.type = ExcelNumber, .number = 0.9959616690399034};
static ExcelValue constant562 = {.type = ExcelNumber, .number = 1.4596538878667837};
static ExcelValue constant563 = {.type = ExcelNumber, .number = 0.7162544035453671};
static ExcelValue constant564 = {.type = ExcelNumber, .number = 0.38278077208699746};
static ExcelValue constant565 = {.type = ExcelString, .string = "LGV (van) Internal Combustion Engine "};
static ExcelValue constant566 = {.type = ExcelNumber, .number = 14.149449355738513};
static ExcelValue constant567 = {.type = ExcelNumber, .number = 11.251752235017547};
static ExcelValue constant568 = {.type = ExcelNumber, .number = 7.814638563338151};
static ExcelValue constant569 = {.type = ExcelNumber, .number = 3.6827634854323876};
static ExcelValue constant570 = {.type = ExcelString, .string = "HGV _Rigid Electric "};
static ExcelValue constant571 = {.type = ExcelString, .string = "HGV _Rigid Hydrogen "};
static ExcelValue constant572 = {.type = ExcelString, .string = "HGV _Rigid Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant573 = {.type = ExcelNumber, .number = 0.17824196230604433};
static ExcelValue constant574 = {.type = ExcelNumber, .number = 0.17930241018142545};
static ExcelValue constant575 = {.type = ExcelNumber, .number = 1.2579039338232918};
static ExcelValue constant576 = {.type = ExcelNumber, .number = 2.225806387255339};
static ExcelValue constant577 = {.type = ExcelNumber, .number = 2.8668004311580373};
static ExcelValue constant578 = {.type = ExcelNumber, .number = 1.2440059333351572};
static ExcelValue constant579 = {.type = ExcelNumber, .number = 0.6230246084682721};
static ExcelValue constant580 = {.type = ExcelString, .string = "HGV _Rigid Internal Combustion Engine "};
static ExcelValue constant581 = {.type = ExcelNumber, .number = 9.811532203799343};
static ExcelValue constant582 = {.type = ExcelNumber, .number = 9.716387590837018};
static ExcelValue constant583 = {.type = ExcelNumber, .number = 5.976361695930594};
static ExcelValue constant584 = {.type = ExcelNumber, .number = 2.677549350260118};
static ExcelValue constant585 = {.type = ExcelString, .string = "HGV _Articulated Electric "};
static ExcelValue constant586 = {.type = ExcelString, .string = "HGV _Articulated Hydrogen "};
static ExcelValue constant587 = {.type = ExcelString, .string = "HGV _Articulated Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant588 = {.type = ExcelNumber, .number = 0.1745732030481716};
static ExcelValue constant589 = {.type = ExcelNumber, .number = 0.17191952777367178};
static ExcelValue constant590 = {.type = ExcelNumber, .number = 1.197014124134419};
static ExcelValue constant591 = {.type = ExcelNumber, .number = 2.099014192874656};
static ExcelValue constant592 = {.type = ExcelNumber, .number = 3.0851377744625834};
static ExcelValue constant593 = {.type = ExcelNumber, .number = 2.498365259808508};
static ExcelValue constant594 = {.type = ExcelNumber, .number = 1.2341399650372704};
static ExcelValue constant595 = {.type = ExcelString, .string = "HGV _Articulated Internal Combustion Engine "};
static ExcelValue constant596 = {.type = ExcelNumber, .number = 8.938455784971504};
static ExcelValue constant597 = {.type = ExcelNumber, .number = 8.745013991731396};
static ExcelValue constant598 = {.type = ExcelNumber, .number = 5.664282288499209};
static ExcelValue constant599 = {.type = ExcelNumber, .number = 3.001781695281233};
static ExcelValue constant600 = {.type = ExcelNumber, .number = 0.6751202848695618};
static ExcelValue constant601 = {.type = ExcelString, .string = "Bus Electric "};
static ExcelValue constant602 = {.type = ExcelString, .string = "Bus Hydrogen "};
static ExcelValue constant603 = {.type = ExcelString, .string = "Bus Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant604 = {.type = ExcelNumber, .number = 0.07031708679958541};
static ExcelValue constant605 = {.type = ExcelNumber, .number = 0.0745974198836928};
static ExcelValue constant606 = {.type = ExcelNumber, .number = 0.5768076405076508};
static ExcelValue constant607 = {.type = ExcelNumber, .number = 1.1025198175095712};
static ExcelValue constant608 = {.type = ExcelNumber, .number = 1.687413773304529};
static ExcelValue constant609 = {.type = ExcelNumber, .number = 1.4041250157819314};
static ExcelValue constant610 = {.type = ExcelNumber, .number = 0.7301236127520518};
static ExcelValue constant611 = {.type = ExcelString, .string = "Bus Internal Combustion Engine "};
static ExcelValue constant612 = {.type = ExcelNumber, .number = 3.8706842804327124};
static ExcelValue constant613 = {.type = ExcelNumber, .number = 4.042430016043687};
static ExcelValue constant614 = {.type = ExcelNumber, .number = 2.9168947909396428};
static ExcelValue constant615 = {.type = ExcelNumber, .number = 1.6893448816678913};
static ExcelValue constant616 = {.type = ExcelNumber, .number = 0.39696373196219903};
static ExcelValue constant617 = {.type = ExcelString, .string = "Train Passenger Electric "};
static ExcelValue constant618 = {.type = ExcelString, .string = "Train Passenger Internal Combustion Engine "};
static ExcelValue constant619 = {.type = ExcelNumber, .number = 0.5500731381216691};
static ExcelValue constant620 = {.type = ExcelNumber, .number = 0.5639531562312852};
static ExcelValue constant621 = {.type = ExcelNumber, .number = 0.5500108033965385};
static ExcelValue constant622 = {.type = ExcelNumber, .number = 0.5306154218649904};
static ExcelValue constant623 = {.type = ExcelNumber, .number = 0.5093498224833923};
static ExcelValue constant624 = {.type = ExcelNumber, .number = 0.4833625965734646};
static ExcelValue constant625 = {.type = ExcelNumber, .number = 0.457105718029231};
static ExcelValue constant626 = {.type = ExcelNumber, .number = 0.4307649619037583};
static ExcelValue constant627 = {.type = ExcelNumber, .number = 0.43804000710534197};
static ExcelValue constant628 = {.type = ExcelNumber, .number = 0.444931968738943};
static ExcelValue constant629 = {.type = ExcelNumber, .number = 0.45182791787190163};
static ExcelValue constant630 = {.type = ExcelNumber, .number = 0.4591272606811464};
static ExcelValue constant631 = {.type = ExcelNumber, .number = 0.4669031402511262};
static ExcelValue constant632 = {.type = ExcelNumber, .number = 0.4749072694007868};
static ExcelValue constant633 = {.type = ExcelNumber, .number = 0.4829122494394315};
static ExcelValue constant634 = {.type = ExcelNumber, .number = 0.4909167956915352};
static ExcelValue constant635 = {.type = ExcelNumber, .number = 0.4990484525228793};
static ExcelValue constant636 = {.type = ExcelNumber, .number = 0.5073498922940914};
static ExcelValue constant637 = {.type = ExcelString, .string = "Train Freight Electric "};
static ExcelValue constant638 = {.type = ExcelString, .string = "Train Freight Internal Combustion Engine "};
static ExcelValue constant639 = {.type = ExcelNumber, .number = 1.5215763635840474};
static ExcelValue constant640 = {.type = ExcelNumber, .number = 1.7794943549207438};
static ExcelValue constant641 = {.type = ExcelNumber, .number = 1.8941480588050619};
static ExcelValue constant642 = {.type = ExcelNumber, .number = 1.99301313976178};
static ExcelValue constant643 = {.type = ExcelNumber, .number = 2.0412289969603363};
static ExcelValue constant644 = {.type = ExcelNumber, .number = 2.002174654553639};
static ExcelValue constant645 = {.type = ExcelNumber, .number = 1.9634921739624789};
static ExcelValue constant646 = {.type = ExcelNumber, .number = 1.9251815551868539};
static ExcelValue constant647 = {.type = ExcelString, .string = "Non-Road Mobile Machinery Internal Combustion Engine "};
static ExcelValue constant648 = {.type = ExcelNumber, .number = 0.00013969824239987847};
static ExcelValue constant649 = {.type = ExcelNumber, .number = 0.00013854416554654614};
static ExcelValue constant650 = {.type = ExcelNumber, .number = 0.00013739008869321384};
static ExcelValue constant651 = {.type = ExcelNumber, .number = 0.0001362360118398815};
static ExcelValue constant652 = {.type = ExcelNumber, .number = 0.00013508193498654918};
static ExcelValue constant653 = {.type = ExcelNumber, .number = 0.00013392785813321688};
static ExcelValue constant654 = {.type = ExcelNumber, .number = 0.00013277378127988452};
static ExcelValue constant655 = {.type = ExcelString, .string = "Shipping Domestic Internal Combustion Engine "};
static ExcelValue constant656 = {.type = ExcelNumber, .number = 1.235606157630203};
static ExcelValue constant657 = {.type = ExcelNumber, .number = 1.1079989585619847};
static ExcelValue constant658 = {.type = ExcelNumber, .number = 0.9768752323581723};
static ExcelValue constant659 = {.type = ExcelNumber, .number = 0.8177634882138837};
static ExcelValue constant660 = {.type = ExcelNumber, .number = 0.737127450333529};
static ExcelValue constant661 = {.type = ExcelNumber, .number = 0.6877904209400816};
static ExcelValue constant662 = {.type = ExcelNumber, .number = 0.6434838955447321};
static ExcelValue constant663 = {.type = ExcelNumber, .number = 0.5986476442278306};
static ExcelValue constant664 = {.type = ExcelString, .string = "Shipping International Internal Combustion Engine "};
static ExcelValue constant665 = {.type = ExcelNumber, .number = 9.0972596970779};
static ExcelValue constant666 = {.type = ExcelNumber, .number = 8.829462686634699};
static ExcelValue constant667 = {.type = ExcelNumber, .number = 8.2060091636953};
static ExcelValue constant668 = {.type = ExcelNumber, .number = 8.234243065357111};
static ExcelValue constant669 = {.type = ExcelNumber, .number = 7.7773205715859675};
static ExcelValue constant670 = {.type = ExcelNumber, .number = 7.456785598016119};
static ExcelValue constant671 = {.type = ExcelNumber, .number = 7.103557918766534};
static ExcelValue constant672 = {.type = ExcelNumber, .number = 7.139048366719577};
static ExcelValue constant673 = {.type = ExcelString, .string = "Light Duty Vehicles Electric Refuelling Infrastructure "};
static ExcelValue constant674 = {.type = ExcelString, .string = "HGV _Articulated Electric Refuelling Infrastructure "};
static ExcelValue constant675 = {.type = ExcelString, .string = "Bus Electric Refuelling Infrastructure "};
static ExcelValue constant676 = {.type = ExcelString, .string = "Light Duty Vehicles Hydrogen Refuelling Infrastructure "};
static ExcelValue constant677 = {.type = ExcelString, .string = "HGV _Articulated Hydrogen Refuelling Infrastructure "};
static ExcelValue constant678 = {.type = ExcelString, .string = "Bus Hydrogen Refuelling Infrastructure "};
static ExcelValue constant679 = {.type = ExcelString, .string = "Light Duty Vehicles Internal Combustion Engine Refuelling Infrastructure "};
static ExcelValue constant680 = {.type = ExcelString, .string = "HGV _Articulated Internal Combustion Engine Refuelling Infrastructure "};
static ExcelValue constant681 = {.type = ExcelString, .string = "Bus Internal Combustion Engine Refuelling Infrastructure "};
static ExcelValue constant682 = {.type = ExcelString, .string = "Aviation Domestic Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant683 = {.type = ExcelString, .string = "Aviation Domestic Conventional "};
static ExcelValue constant684 = {.type = ExcelNumber, .number = 2.214710411365551};
static ExcelValue constant685 = {.type = ExcelNumber, .number = 2.5304855752768733};
static ExcelValue constant686 = {.type = ExcelNumber, .number = 2.7806581632047287};
static ExcelValue constant687 = {.type = ExcelNumber, .number = 3.0028202119606697};
static ExcelValue constant688 = {.type = ExcelNumber, .number = 3.2097702949928384};
static ExcelValue constant689 = {.type = ExcelNumber, .number = 3.3787694178588565};
static ExcelValue constant690 = {.type = ExcelNumber, .number = 3.533545648570639};
static ExcelValue constant691 = {.type = ExcelNumber, .number = 3.638235968174833};
static ExcelValue constant692 = {.type = ExcelNumber, .number = 3.6986718498219058};
static ExcelValue constant693 = {.type = ExcelNumber, .number = 3.755925346681803};
static ExcelValue constant694 = {.type = ExcelNumber, .number = 3.8132119688395583};
static ExcelValue constant695 = {.type = ExcelNumber, .number = 3.8738496976642387};
static ExcelValue constant696 = {.type = ExcelNumber, .number = 3.9384461536822566};
static ExcelValue constant697 = {.type = ExcelNumber, .number = 4.004938744253578};
static ExcelValue constant698 = {.type = ExcelNumber, .number = 4.071438403403103};
static ExcelValue constant699 = {.type = ExcelNumber, .number = 4.137934458963742};
static ExcelValue constant700 = {.type = ExcelNumber, .number = 4.205486458468395};
static ExcelValue constant701 = {.type = ExcelNumber, .number = 4.274448893423998};
static ExcelValue constant702 = {.type = ExcelString, .string = "Aviation International Plug-in Hybrid Electric Vehicle "};
static ExcelValue constant703 = {.type = ExcelString, .string = "Aviation International Conventional "};
static ExcelValue constant704 = {.type = ExcelNumber, .number = 36.942919818629925};
static ExcelValue constant705 = {.type = ExcelNumber, .number = 40.62725670063316};
static ExcelValue constant706 = {.type = ExcelNumber, .number = 42.94247605370944};
static ExcelValue constant707 = {.type = ExcelNumber, .number = 44.72965788127872};
static ExcelValue constant708 = {.type = ExcelNumber, .number = 45.98139866684657};
static ExcelValue constant709 = {.type = ExcelNumber, .number = 48.15805609925605};
static ExcelValue constant710 = {.type = ExcelNumber, .number = 51.120500703326925};
static ExcelValue constant711 = {.type = ExcelNumber, .number = 52.48336309664013};
static ExcelValue constant712 = {.type = ExcelNumber, .number = 53.369165237281834};
static ExcelValue constant713 = {.type = ExcelNumber, .number = 54.208323510406416};
static ExcelValue constant714 = {.type = ExcelNumber, .number = 55.047967297417};
static ExcelValue constant715 = {.type = ExcelNumber, .number = 55.936727890175796};
static ExcelValue constant716 = {.type = ExcelNumber, .number = 56.88351111661688};
static ExcelValue constant717 = {.type = ExcelNumber, .number = 57.85808578086754};
static ExcelValue constant718 = {.type = ExcelNumber, .number = 58.83276404849973};
static ExcelValue constant719 = {.type = ExcelNumber, .number = 59.80738949872174};
static ExcelValue constant720 = {.type = ExcelNumber, .number = 60.797491804422904};
static ExcelValue constant721 = {.type = ExcelNumber, .number = 61.8082667090444};
static ExcelValue constant722 = {.type = ExcelString, .string = "Woodland "};
static ExcelValue constant723 = {.type = ExcelNumber, .number = -7.29159883};
static ExcelValue constant724 = {.type = ExcelNumber, .number = -4.04741327};
static ExcelValue constant725 = {.type = ExcelNumber, .number = -1.1500830640694388};
static ExcelValue constant726 = {.type = ExcelNumber, .number = 0.4167896294888984};
static ExcelValue constant727 = {.type = ExcelNumber, .number = 0.5773353706750121};
static ExcelValue constant728 = {.type = ExcelNumber, .number = -0.5538703005111023};
static ExcelValue constant729 = {.type = ExcelNumber, .number = -1.9448040740694406};
static ExcelValue constant730 = {.type = ExcelNumber, .number = -2.568644469999998};
static ExcelValue constant731 = {.type = ExcelNumber, .number = -2.674060469999996};
static ExcelValue constant732 = {.type = ExcelNumber, .number = -2.7794764699999988};
static ExcelValue constant733 = {.type = ExcelNumber, .number = -2.884892469999997};
static ExcelValue constant734 = {.type = ExcelNumber, .number = -2.9903084699999996};
static ExcelValue constant735 = {.type = ExcelString, .string = "Arable Land for Food "};
static ExcelValue constant736 = {.type = ExcelNumber, .number = 15.28835};
static ExcelValue constant737 = {.type = ExcelNumber, .number = 15.211908249999997};
static ExcelValue constant738 = {.type = ExcelNumber, .number = 15.135466499999998};
static ExcelValue constant739 = {.type = ExcelNumber, .number = 15.059024749999997};
static ExcelValue constant740 = {.type = ExcelNumber, .number = 14.982582999999998};
static ExcelValue constant741 = {.type = ExcelNumber, .number = 14.906141249999997};
static ExcelValue constant742 = {.type = ExcelNumber, .number = 14.829699499999998};
static ExcelValue constant743 = {.type = ExcelString, .string = "Livestock Grazing Land"};
static ExcelValue constant744 = {.type = ExcelNumber, .number = -7.96705};
static ExcelValue constant745 = {.type = ExcelNumber, .number = -7.927294340869316};
static ExcelValue constant746 = {.type = ExcelNumber, .number = -7.887737062875045};
static ExcelValue constant747 = {.type = ExcelNumber, .number = -7.848377176093409};
static ExcelValue constant748 = {.type = ExcelNumber, .number = -7.809213695540161};
static ExcelValue constant749 = {.type = ExcelNumber, .number = -7.7702456411463245};
static ExcelValue constant750 = {.type = ExcelNumber, .number = -7.731472037733385};
static ExcelValue constant751 = {.type = ExcelNumber, .number = -7.692891914988979};
static ExcelValue constant752 = {.type = ExcelString, .string = "Settlement Land"};
static ExcelValue constant753 = {.type = ExcelNumber, .number = 6.32981};
static ExcelValue constant754 = {.type = ExcelString, .string = "Other Land "};
static ExcelValue constant755 = {.type = ExcelNumber, .number = -1.29274};
static ExcelValue constant756 = {.type = ExcelString, .string = "Manure Management "};
static ExcelValue constant757 = {.type = ExcelNumber, .number = 4.55687};
static ExcelValue constant758 = {.type = ExcelNumber, .number = 2.8416110022298655};
static ExcelValue constant759 = {.type = ExcelNumber, .number = 2.8367807703635703};
static ExcelValue constant760 = {.type = ExcelNumber, .number = 2.831777220095185};
static ExcelValue constant761 = {.type = ExcelNumber, .number = 2.8265983613754173};
static ExcelValue constant762 = {.type = ExcelNumber, .number = 2.8212421859722543};
static ExcelValue constant763 = {.type = ExcelNumber, .number = 2.815706667320935};
static ExcelValue constant764 = {.type = ExcelNumber, .number = 2.8099897603727397};
static ExcelValue constant765 = {.type = ExcelString, .string = "Enteric Fermentation "};
static ExcelValue constant766 = {.type = ExcelNumber, .number = 15.561};
static ExcelValue constant767 = {.type = ExcelNumber, .number = 15.37152176275053};
static ExcelValue constant768 = {.type = ExcelNumber, .number = 15.314493320969015};
static ExcelValue constant769 = {.type = ExcelNumber, .number = 15.257676455062827};
static ExcelValue constant770 = {.type = ExcelNumber, .number = 15.201070380084142};
static ExcelValue constant771 = {.type = ExcelNumber, .number = 15.144674313997307};
static ExcelValue constant772 = {.type = ExcelNumber, .number = 15.088487477668016};
static ExcelValue constant773 = {.type = ExcelNumber, .number = 15.032509094852562};
static ExcelValue constant774 = {.type = ExcelString, .string = "Soil Management "};
static ExcelValue constant775 = {.type = ExcelNumber, .number = 23.28};
static ExcelValue constant776 = {.type = ExcelNumber, .number = 22.545258398704277};
static ExcelValue constant777 = {.type = ExcelNumber, .number = 22.473715332253565};
static ExcelValue constant778 = {.type = ExcelNumber, .number = 22.40217226580285};
static ExcelValue constant779 = {.type = ExcelNumber, .number = 22.33062919935214};
static ExcelValue constant780 = {.type = ExcelNumber, .number = 22.259086132901427};
static ExcelValue constant781 = {.type = ExcelNumber, .number = 22.18754306645071};
static ExcelValue constant782 = {.type = ExcelNumber, .number = 22.116};
static ExcelValue constant783 = {.type = ExcelString, .string = "Agricultural Fuel Use"};
static ExcelValue constant784 = {.type = ExcelNumber, .number = 1.5732495789301777};
static ExcelValue constant785 = {.type = ExcelNumber, .number = 1.6343324755419286};
static ExcelValue constant786 = {.type = ExcelNumber, .number = 1.5809618733018966};
static ExcelValue constant787 = {.type = ExcelNumber, .number = 1.5347353610463577};
static ExcelValue constant788 = {.type = ExcelNumber, .number = 1.478305747775109};
static ExcelValue constant789 = {.type = ExcelNumber, .number = 1.4212210130469556};
static ExcelValue constant790 = {.type = ExcelNumber, .number = 1.3641362783188022};
static ExcelValue constant791 = {.type = ExcelNumber, .number = 1.3070515435906491};
static ExcelValue constant792 = {.type = ExcelNumber, .number = 1.297922557278242};
static ExcelValue constant793 = {.type = ExcelNumber, .number = 1.2887935709658351};
static ExcelValue constant794 = {.type = ExcelString, .string = "Woodland Dry Waste "};
static ExcelValue constant795 = {.type = ExcelNumber, .number = 0.011065203140800001};
static ExcelValue constant796 = {.type = ExcelNumber, .number = 0.011229087492800001};
static ExcelValue constant797 = {.type = ExcelNumber, .number = 0.012284390720583025};
static ExcelValue constant798 = {.type = ExcelNumber, .number = 0.01336533951421283};
static ExcelValue constant799 = {.type = ExcelNumber, .number = 0.01447193387368942};
static ExcelValue constant800 = {.type = ExcelNumber, .number = 0.01560417379901284};
static ExcelValue constant801 = {.type = ExcelNumber, .number = 0.016762059290183037};
static ExcelValue constant802 = {.type = ExcelNumber, .number = 0.017945590347199978};
static ExcelValue constant803 = {.type = ExcelNumber, .number = 0.01890112313759996};
static ExcelValue constant804 = {.type = ExcelNumber, .number = 0.019856655927999987};
static ExcelValue constant805 = {.type = ExcelNumber, .number = 0.020812188718399967};
static ExcelValue constant806 = {.type = ExcelNumber, .number = 0.0217677215088};
static ExcelValue constant807 = {.type = ExcelString, .string = "Arable Land for Food Dry Waste "};
static ExcelValue constant808 = {.type = ExcelNumber, .number = 0.03239204344};
static ExcelValue constant809 = {.type = ExcelString, .string = "Bioenergy LandBioenergy Solid "};
static ExcelValue constant810 = {.type = ExcelNumber, .number = 0.2935333333333333};
static ExcelValue constant811 = {.type = ExcelNumber, .number = 0.37683363061829644};
static ExcelValue constant812 = {.type = ExcelNumber, .number = 0.46985141336857034};
static ExcelValue constant813 = {.type = ExcelNumber, .number = 0.5678563243308695};
static ExcelValue constant814 = {.type = ExcelNumber, .number = 0.6709930250733092};
static ExcelValue constant815 = {.type = ExcelNumber, .number = 0.779409452375009};
static ExcelValue constant816 = {.type = ExcelNumber, .number = 0.8932568830582244};
static ExcelValue constant817 = {.type = ExcelNumber, .number = 1.01269};
static ExcelValue constant818 = {.type = ExcelString, .string = "Manure Management Wet Waste "};
static ExcelValue constant819 = {.type = ExcelNumber, .number = 0.10075278779654606};
static ExcelValue constant820 = {.type = ExcelNumber, .number = 0.2544108846357746};
static ExcelValue constant821 = {.type = ExcelNumber, .number = 0.2569651902907495};
static ExcelValue constant822 = {.type = ExcelNumber, .number = 0.25954514137904927};
static ExcelValue constant823 = {.type = ExcelNumber, .number = 0.26215099538287817};
static ExcelValue constant824 = {.type = ExcelNumber, .number = 0.26478301236958224};
static ExcelValue constant825 = {.type = ExcelNumber, .number = 0.2674414550176049};
static ExcelValue constant826 = {.type = ExcelNumber, .number = 0.27012658864270195};
static ExcelValue constant827 = {.type = ExcelString, .string = "Biomass Pelletisation Plant"};
static ExcelValue constant828 = {.type = ExcelString, .string = "Landfill Gas Collection "};
static ExcelValue constant829 = {.type = ExcelString, .string = "Anaerobic Digestion Plant "};
static ExcelValue constant830 = {.type = ExcelNumber, .number = 0.014654620121185033};
static ExcelValue constant831 = {.type = ExcelNumber, .number = 0.046774268694524786};
static ExcelValue constant832 = {.type = ExcelNumber, .number = 0.0656785036115686};
static ExcelValue constant833 = {.type = ExcelNumber, .number = 0.06995144486118635};
static ExcelValue constant834 = {.type = ExcelNumber, .number = 0.0735010044374063};
static ExcelValue constant835 = {.type = ExcelNumber, .number = 0.07638125069133124};
static ExcelValue constant836 = {.type = ExcelNumber, .number = 0.08666439549899153};
static ExcelValue constant837 = {.type = ExcelNumber, .number = 0.09805154516279835};
static ExcelValue constant838 = {.type = ExcelNumber, .number = 0.10482703665518577};
static ExcelValue constant839 = {.type = ExcelNumber, .number = 0.10816442953486158};
static ExcelValue constant840 = {.type = ExcelNumber, .number = 0.11620966533864702};
static ExcelValue constant841 = {.type = ExcelNumber, .number = 0.12233017541340892};
static ExcelValue constant842 = {.type = ExcelNumber, .number = 0.12700581386011792};
static ExcelValue constant843 = {.type = ExcelNumber, .number = 0.13060324777767532};
static ExcelValue constant844 = {.type = ExcelNumber, .number = 0.13340270611504645};
static ExcelValue constant845 = {.type = ExcelNumber, .number = 0.13561243204469065};
static ExcelValue constant846 = {.type = ExcelString, .string = "Biomass Gasification Plant (BioSNG) "};
static ExcelValue constant847 = {.type = ExcelString, .string = "Biofuel Plant "};
static ExcelValue constant848 = {.type = ExcelNumber, .number = 0.00840076988652702};
static ExcelValue constant849 = {.type = ExcelNumber, .number = 0.008606123673125078};
static ExcelValue constant850 = {.type = ExcelNumber, .number = 0.016422575043824265};
static ExcelValue constant851 = {.type = ExcelNumber, .number = 0.021852672249321727};
static ExcelValue constant852 = {.type = ExcelNumber, .number = 0.02493873163370769};
static ExcelValue constant853 = {.type = ExcelNumber, .number = 0.08922020546930881};
static ExcelValue constant854 = {.type = ExcelNumber, .number = 0.15703781578042303};
static ExcelValue constant855 = {.type = ExcelNumber, .number = 0.19682047134992434};
static ExcelValue constant856 = {.type = ExcelNumber, .number = 0.19849447076328897};
static ExcelValue constant857 = {.type = ExcelNumber, .number = 0.19914948468042415};
static ExcelValue constant858 = {.type = ExcelNumber, .number = 0.2027486975048282};
static ExcelValue constant859 = {.type = ExcelNumber, .number = 0.20664237152561923};
static ExcelValue constant860 = {.type = ExcelNumber, .number = 0.21088070190915076};
static ExcelValue constant861 = {.type = ExcelNumber, .number = 0.21529967764528357};
static ExcelValue constant862 = {.type = ExcelNumber, .number = 0.2197488985275161};
static ExcelValue constant863 = {.type = ExcelNumber, .number = 0.22422847414398725};
static ExcelValue constant864 = {.type = ExcelNumber, .number = 0.2231788172646479};
static ExcelValue constant865 = {.type = ExcelNumber, .number = 0.22151779071431854};
static ExcelValue constant866 = {.type = ExcelString, .string = "Energy from Waste "};
static ExcelValue constant867 = {.type = ExcelNumber, .number = 0.447196557322895};
static ExcelValue constant868 = {.type = ExcelNumber, .number = 0.6741201731037033};
static ExcelValue constant869 = {.type = ExcelNumber, .number = 0.6822309377199879};
static ExcelValue constant870 = {.type = ExcelNumber, .number = 0.6802131284002014};
static ExcelValue constant871 = {.type = ExcelNumber, .number = 0.6717653745845423};
static ExcelValue constant872 = {.type = ExcelNumber, .number = 0.6145354138542063};
static ExcelValue constant873 = {.type = ExcelNumber, .number = 0.5529517305887596};
static ExcelValue constant874 = {.type = ExcelNumber, .number = 0.5196703853342325};
static ExcelValue constant875 = {.type = ExcelNumber, .number = 0.479939607274211};
static ExcelValue constant876 = {.type = ExcelNumber, .number = 0.44123218919975415};
static ExcelValue constant877 = {.type = ExcelNumber, .number = 0.43193107813707177};
static ExcelValue constant878 = {.type = ExcelNumber, .number = 0.4250802802651999};
static ExcelValue constant879 = {.type = ExcelNumber, .number = 0.41407397985306527};
static ExcelValue constant880 = {.type = ExcelNumber, .number = 0.40442251452614186};
static ExcelValue constant881 = {.type = ExcelNumber, .number = 0.39590085329143887};
static ExcelValue constant882 = {.type = ExcelNumber, .number = 0.3882139539556127};
static ExcelValue constant883 = {.type = ExcelNumber, .number = 0.3870854166666667};
static ExcelValue constant884 = {.type = ExcelString, .string = "Biomass Gasification Plant (BioSNG) with CCS "};
static ExcelValue constant885 = {.type = ExcelString, .string = "Biofuel Plant with CCS "};
static ExcelValue constant886 = {.type = ExcelNumber, .number = -0.0892912812676131};
static ExcelValue constant887 = {.type = ExcelNumber, .number = -0.271164844035539};
static ExcelValue constant888 = {.type = ExcelNumber, .number = -0.559452310051154};
static ExcelValue constant889 = {.type = ExcelNumber, .number = -0.8911437059561661};
static ExcelValue constant890 = {.type = ExcelNumber, .number = -1.1582987399950146};
static ExcelValue constant891 = {.type = ExcelNumber, .number = -1.442758113696935};
static ExcelValue constant892 = {.type = ExcelNumber, .number = -1.474205426911114};
static ExcelValue constant893 = {.type = ExcelNumber, .number = -1.4922034382558074};
static ExcelValue constant894 = {.type = ExcelNumber, .number = -1.5117945796605676};
static ExcelValue constant895 = {.type = ExcelNumber, .number = -1.5322207311581413};
static ExcelValue constant896 = {.type = ExcelNumber, .number = -1.5527866869844074};
static ExcelValue constant897 = {.type = ExcelNumber, .number = -1.573492953696555};
static ExcelValue constant898 = {.type = ExcelNumber, .number = -1.5686410487036078};
static ExcelValue constant899 = {.type = ExcelNumber, .number = -1.5609631654147056};
static ExcelValue constant900 = {.type = ExcelString, .string = "Energy from Waste with CCS "};
static ExcelValue constant901 = {.type = ExcelNumber, .number = -0.517205369180875};
static ExcelValue constant902 = {.type = ExcelNumber, .number = -0.9831135579912621};
static ExcelValue constant903 = {.type = ExcelNumber, .number = -1.3908877777606776};
static ExcelValue constant904 = {.type = ExcelNumber, .number = -1.829258310708213};
static ExcelValue constant905 = {.type = ExcelNumber, .number = -2.219430624825458};
static ExcelValue constant906 = {.type = ExcelNumber, .number = -2.577206802022186};
static ExcelValue constant907 = {.type = ExcelNumber, .number = -2.5489840665859513};
static ExcelValue constant908 = {.type = ExcelNumber, .number = -2.5085549900441135};
static ExcelValue constant909 = {.type = ExcelNumber, .number = -2.443602765481827};
static ExcelValue constant910 = {.type = ExcelNumber, .number = -2.3866459207841944};
static ExcelValue constant911 = {.type = ExcelNumber, .number = -2.336356465341935};
static ExcelValue constant912 = {.type = ExcelNumber, .number = -2.2909932467169183};
static ExcelValue constant913 = {.type = ExcelNumber, .number = -2.2843333333333335};
static ExcelValue constant914 = {.type = ExcelString, .string = "Landfill Gas Release "};
static ExcelValue constant915 = {.type = ExcelNumber, .number = 13.770505546875004};
static ExcelValue constant916 = {.type = ExcelNumber, .number = 13.367937515625004};
static ExcelValue constant917 = {.type = ExcelNumber, .number = 12.729509665039066};
static ExcelValue constant918 = {.type = ExcelNumber, .number = 11.908507748779298};
static ExcelValue constant919 = {.type = ExcelNumber, .number = 10.944896082092287};
static ExcelValue constant920 = {.type = ExcelNumber, .number = 9.868647901412963};
static ExcelValue constant921 = {.type = ExcelNumber, .number = 8.702243134067537};
static ExcelValue constant922 = {.type = ExcelNumber, .number = 7.462541725550651};
static ExcelValue constant923 = {.type = ExcelNumber, .number = 6.532765669162988};
static ExcelValue constant924 = {.type = ExcelNumber, .number = 5.835433626872242};
static ExcelValue constant925 = {.type = ExcelNumber, .number = 5.312434595154181};
static ExcelValue constant926 = {.type = ExcelNumber, .number = 4.920185321365636};
static ExcelValue constant927 = {.type = ExcelNumber, .number = 4.625998366024227};
static ExcelValue constant928 = {.type = ExcelNumber, .number = 4.40535814951817};
static ExcelValue constant929 = {.type = ExcelNumber, .number = 4.239877987138627};
static ExcelValue constant930 = {.type = ExcelNumber, .number = 4.11576786535397};
static ExcelValue constant931 = {.type = ExcelNumber, .number = 4.022685274015478};
static ExcelValue constant932 = {.type = ExcelNumber, .number = 3.952873330511607};
static ExcelValue constant933 = {.type = ExcelString, .string = "Direct Air Capture "};
static ExcelValue constant934 = {.type = ExcelNumber, .number = -0.8333333333333333};
static ExcelValue constant935 = {.type = ExcelNumber, .number = -1.6666666666666665};
static ExcelValue constant936 = {.type = ExcelNumber, .number = -2.5};
static ExcelValue constant937 = {.type = ExcelNumber, .number = -3.333333333333333};
static ExcelValue constant938 = {.type = ExcelNumber, .number = -4.166666666666666};
static ExcelValue constant939 = {.type = ExcelNumber, .number = -5.0};
static ExcelValue constant940 = {.type = ExcelString, .string = "Enhanced Weathering"};
static ExcelValue constant941 = {.type = ExcelNumber, .number = -0.3333333333333333};
static ExcelValue constant942 = {.type = ExcelNumber, .number = -0.6666666666666666};
static ExcelValue constant943 = {.type = ExcelNumber, .number = -1.0};
static ExcelValue constant944 = {.type = ExcelNumber, .number = -1.3333333333333333};
static ExcelValue constant945 = {.type = ExcelNumber, .number = -1.6666666666666667};
static ExcelValue constant946 = {.type = ExcelNumber, .number = -2.0};
static ExcelValue constant947 = {.type = ExcelString, .string = "Hydrogen Production - Biomass Gasification with CCS"};
static ExcelValue constant948 = {.type = ExcelNumber, .number = -5.56875};
static ExcelValue constant949 = {.type = ExcelNumber, .number = -11.1375};
static ExcelValue constant950 = {.type = ExcelNumber, .number = -16.70625};
static ExcelValue constant951 = {.type = ExcelNumber, .number = -22.555648302726766};
static ExcelValue constant952 = {.type = ExcelNumber, .number = -28.54654403567447};
static ExcelValue constant953 = {.type = ExcelNumber, .number = -34.679648241206024};
static ExcelValue constant954 = {.type = ExcelNumber, .number = -35.10486577181208};
static ExcelValue constant955 = {.type = ExcelNumber, .number = -35.53151260504201};
static ExcelValue constant956 = {.type = ExcelNumber, .number = -35.95959595959595};
static ExcelValue constant957 = {.type = ExcelString, .string = "Hydrogen Production - Steam Methane Reformation with CCS"};
static ExcelValue constant958 = {.type = ExcelNumber, .number = 0.34410645643964977};
static ExcelValue constant959 = {.type = ExcelNumber, .number = 0.5718503454869444};
static ExcelValue constant960 = {.type = ExcelNumber, .number = 1.15992267928649};
static ExcelValue constant961 = {.type = ExcelNumber, .number = 1.4852166838080363};
static ExcelValue constant962 = {.type = ExcelNumber, .number = 1.77958077711713};
static ExcelValue constant963 = {.type = ExcelNumber, .number = 2.0428595325242624};
static ExcelValue constant964 = {.type = ExcelNumber, .number = 1.949911268749523};
static ExcelValue constant965 = {.type = ExcelNumber, .number = 1.8566505738360455};
static ExcelValue constant966 = {.type = ExcelNumber, .number = 1.7630758698487863};
static ExcelValue constant967 = {.type = ExcelString, .string = "Zero Carbon hydrogen imports"};
static ExcelValue constant968 = {.type = ExcelString, .string = "Hydrogen Production - Electrolysis"};
static ExcelValue constant969 = {.type = ExcelString, .string = "Seasonal Hydrogen Storage"};
static ExcelValue constant970 = {.type = ExcelString, .string = "Electricity - Biomass CCS "};
static ExcelValue constant971 = {.type = ExcelNumber, .number = -7.769468013989542};
static ExcelValue constant972 = {.type = ExcelNumber, .number = -11.437252529159817};
static ExcelValue constant973 = {.type = ExcelNumber, .number = -11.459298136230032};
static ExcelValue constant974 = {.type = ExcelNumber, .number = -9.10323061480098};
static ExcelValue constant975 = {.type = ExcelNumber, .number = -6.106033114322072};
static ExcelValue constant976 = {.type = ExcelString, .string = "Electricity - Nuclear "};
static ExcelValue constant977 = {.type = ExcelString, .string = "Electricity - Hydro "};
static ExcelValue constant978 = {.type = ExcelString, .string = "Electricity - Wind (onshore) "};
static ExcelValue constant979 = {.type = ExcelString, .string = "Electricity - Wind (offshore) "};
static ExcelValue constant980 = {.type = ExcelString, .string = "Electricity - Solar PV "};
static ExcelValue constant981 = {.type = ExcelString, .string = "Electricity - Tidal Stream "};
static ExcelValue constant982 = {.type = ExcelString, .string = "Electricity - Tidal Range "};
static ExcelValue constant983 = {.type = ExcelString, .string = "Electricity - Wave "};
static ExcelValue constant984 = {.type = ExcelString, .string = "Electricity - Biomass "};
static ExcelValue constant985 = {.type = ExcelNumber, .number = 0.3691441966990915};
static ExcelValue constant986 = {.type = ExcelNumber, .number = 0.5434085563798341};
static ExcelValue constant987 = {.type = ExcelNumber, .number = 0.5444559907599005};
static ExcelValue constant988 = {.type = ExcelNumber, .number = 0.4325141369546295};
static ExcelValue constant989 = {.type = ExcelNumber, .number = 0.290110813886608};
static ExcelValue constant990 = {.type = ExcelString, .string = "Electricity - Gas CCS "};
static ExcelValue constant991 = {.type = ExcelNumber, .number = 0.23574055701120716};
static ExcelValue constant992 = {.type = ExcelNumber, .number = 0.468178903765014};
static ExcelValue constant993 = {.type = ExcelNumber, .number = 0.7065745145056979};
static ExcelValue constant994 = {.type = ExcelNumber, .number = 0.9086137764019775};
static ExcelValue constant995 = {.type = ExcelNumber, .number = 1.0939491355890874};
static ExcelValue constant996 = {.type = ExcelNumber, .number = 1.2647564171166734};
static ExcelValue constant997 = {.type = ExcelNumber, .number = 1.2009640392406684};
static ExcelValue constant998 = {.type = ExcelNumber, .number = 1.143581476177684};
static ExcelValue constant999 = {.type = ExcelNumber, .number = 1.0857709428534388};
static ExcelValue constant1000 = {.type = ExcelNumber, .number = 1.0853380355002675};
static ExcelValue constant1001 = {.type = ExcelNumber, .number = 1.0852345145312243};
static ExcelValue constant1002 = {.type = ExcelNumber, .number = 1.0851244916075466};
static ExcelValue constant1003 = {.type = ExcelNumber, .number = 1.0850831289212535};
static ExcelValue constant1004 = {.type = ExcelNumber, .number = 1.0850437206148258};
static ExcelValue constant1005 = {.type = ExcelNumber, .number = 1.0849826761844918};
static ExcelValue constant1006 = {.type = ExcelNumber, .number = 1.084656255634223};
static ExcelValue constant1007 = {.type = ExcelString, .string = "Electricity - Coal Unabated "};
static ExcelValue constant1008 = {.type = ExcelNumber, .number = 23.92396881716982};
static ExcelValue constant1009 = {.type = ExcelNumber, .number = 13.043672639325644};
static ExcelValue constant1010 = {.type = ExcelNumber, .number = 13.06881466369084};
static ExcelValue constant1011 = {.type = ExcelString, .string = "Electricity - Gas Unabated (CCGT)"};
static ExcelValue constant1012 = {.type = ExcelNumber, .number = 83.16796434074912};
static ExcelValue constant1013 = {.type = ExcelNumber, .number = 88.79284209672988};
static ExcelValue constant1014 = {.type = ExcelNumber, .number = 97.0748846055302};
static ExcelValue constant1015 = {.type = ExcelNumber, .number = 103.70448065043479};
static ExcelValue constant1016 = {.type = ExcelNumber, .number = 116.07906995820392};
static ExcelValue constant1017 = {.type = ExcelNumber, .number = 118.04497608717537};
static ExcelValue constant1018 = {.type = ExcelNumber, .number = 103.4411665919102};
static ExcelValue constant1019 = {.type = ExcelNumber, .number = 74.08084216742674};
static ExcelValue constant1020 = {.type = ExcelNumber, .number = 89.8958894096335};
static ExcelValue constant1021 = {.type = ExcelNumber, .number = 106.64839460343987};
static ExcelValue constant1022 = {.type = ExcelNumber, .number = 108.07952991873326};
static ExcelValue constant1023 = {.type = ExcelNumber, .number = 109.5373701619457};
static ExcelValue constant1024 = {.type = ExcelNumber, .number = 112.79106939530735};
static ExcelValue constant1025 = {.type = ExcelNumber, .number = 116.09492604327296};
static ExcelValue constant1026 = {.type = ExcelNumber, .number = 119.40687951009856};
static ExcelValue constant1027 = {.type = ExcelNumber, .number = 122.73194019939751};
static ExcelValue constant1028 = {.type = ExcelNumber, .number = 125.88725970196136};
static ExcelValue constant1029 = {.type = ExcelNumber, .number = 129.0600786297309};
static ExcelValue constant1030 = {.type = ExcelString, .string = "Electricity - Gas Unabated Peaking plant (OCGT)"};
static ExcelValue constant1031 = {.type = ExcelNumber, .number = 2.8963573814688535};
static ExcelValue constant1032 = {.type = ExcelNumber, .number = 5.890387827541659};
static ExcelValue constant1033 = {.type = ExcelNumber, .number = 6.4837580603037095};
static ExcelValue constant1034 = {.type = ExcelNumber, .number = 8.871111049435404};
static ExcelValue constant1035 = {.type = ExcelNumber, .number = 8.799879945865964};
static ExcelValue constant1036 = {.type = ExcelNumber, .number = 8.517578920996296};
static ExcelValue constant1037 = {.type = ExcelNumber, .number = 7.450460407740003};
static ExcelValue constant1038 = {.type = ExcelNumber, .number = 5.473216944659102};
static ExcelValue constant1039 = {.type = ExcelNumber, .number = 7.330414759085669};
static ExcelValue constant1040 = {.type = ExcelNumber, .number = 7.729248130111249};
static ExcelValue constant1041 = {.type = ExcelNumber, .number = 7.815491420693785};
static ExcelValue constant1042 = {.type = ExcelNumber, .number = 7.978746149170363};
static ExcelValue constant1043 = {.type = ExcelNumber, .number = 8.091674341441795};
static ExcelValue constant1044 = {.type = ExcelNumber, .number = 8.208466969744336};
static ExcelValue constant1045 = {.type = ExcelNumber, .number = 8.30536573275721};
static ExcelValue constant1046 = {.type = ExcelNumber, .number = 8.402260799546932};
static ExcelValue constant1047 = {.type = ExcelNumber, .number = 8.501744581679601};
static ExcelValue constant1048 = {.type = ExcelNumber, .number = 8.684835706063991};
static ExcelValue constant1049 = {.type = ExcelString, .string = "Electricity - Interconnectors "};
static ExcelValue constant1050 = {.type = ExcelString, .string = "Electricity - Seasonal Storage "};
static ExcelValue constant1051 = {.type = ExcelString, .string = "Electricity - Demand Shift Vehicle to Grid "};
static ExcelValue constant1052 = {.type = ExcelString, .string = "Electricity - Demand Shift Battery "};
static ExcelValue constant1053 = {.type = ExcelString, .string = "Electricity - Distribution "};
static ExcelValue constant1054 = {.type = ExcelString, .string = "Electricity - Transmission "};
static ExcelValue constant1055 = {.type = ExcelString, .string = "Industry Ferrous and non-ferrous metal production "};
static ExcelValue constant1056 = {.type = ExcelNumber, .number = 7.322434342226452};
static ExcelValue constant1057 = {.type = ExcelNumber, .number = 7.263176538198852};
static ExcelValue constant1058 = {.type = ExcelNumber, .number = 6.868965688850404};
static ExcelValue constant1059 = {.type = ExcelNumber, .number = 6.492488936597583};
static ExcelValue constant1060 = {.type = ExcelNumber, .number = 5.990609180914439};
static ExcelValue constant1061 = {.type = ExcelNumber, .number = 5.5198312368880575};
static ExcelValue constant1062 = {.type = ExcelNumber, .number = 5.0783823027555375};
static ExcelValue constant1063 = {.type = ExcelNumber, .number = 4.622980869101785};
static ExcelValue constant1064 = {.type = ExcelNumber, .number = 4.503052721088645};
static ExcelValue constant1065 = {.type = ExcelNumber, .number = 4.383170435745344};
static ExcelValue constant1066 = {.type = ExcelNumber, .number = 4.383013648293453};
static ExcelValue constant1067 = {.type = ExcelString, .string = "Industry Other Industry"};
static ExcelValue constant1068 = {.type = ExcelNumber, .number = 67.82434342226452};
static ExcelValue constant1069 = {.type = ExcelNumber, .number = 67.1237653819885};
static ExcelValue constant1070 = {.type = ExcelNumber, .number = 63.41115688850402};
static ExcelValue constant1071 = {.type = ExcelNumber, .number = 59.87588936597586};
static ExcelValue constant1072 = {.type = ExcelNumber, .number = 55.0865918091444};
static ExcelValue constant1073 = {.type = ExcelNumber, .number = 50.60831236888058};
static ExcelValue constant1074 = {.type = ExcelNumber, .number = 46.42332302755537};
static ExcelValue constant1075 = {.type = ExcelNumber, .number = 42.09880869101785};
static ExcelValue constant1076 = {.type = ExcelNumber, .number = 40.899527210886454};
static ExcelValue constant1077 = {.type = ExcelNumber, .number = 39.70070435745344};
static ExcelValue constant1078 = {.type = ExcelNumber, .number = 39.699136482934534};
static ExcelValue constant1079 = {.type = ExcelString, .string = "Industry Cement, ceramics and glass"};
static ExcelValue constant1080 = {.type = ExcelNumber, .number = 14.561979018470323};
static ExcelValue constant1081 = {.type = ExcelNumber, .number = 14.425982040874539};
static ExcelValue constant1082 = {.type = ExcelNumber, .number = 13.634697969150835};
static ExcelValue constant1083 = {.type = ExcelNumber, .number = 12.880246252933444};
static ExcelValue constant1084 = {.type = ExcelNumber, .number = 11.865342144976145};
static ExcelValue constant1085 = {.type = ExcelNumber, .number = 10.915034107382889};
static ExcelValue constant1086 = {.type = ExcelNumber, .number = 10.0256401672615};
static ExcelValue constant1087 = {.type = ExcelNumber, .number = 9.107267958903707};
static ExcelValue constant1088 = {.type = ExcelNumber, .number = 8.85818642072257};
static ExcelValue constant1089 = {.type = ExcelNumber, .number = 8.60920013577879};
static ExcelValue constant1090 = {.type = ExcelNumber, .number = 8.608874500301788};
static ExcelValue constant1091 = {.type = ExcelString, .string = "Industry Refineries"};
static ExcelValue constant1092 = {.type = ExcelNumber, .number = 23.869965030783874};
static ExcelValue constant1093 = {.type = ExcelNumber, .number = 22.432535841594323};
static ExcelValue constant1094 = {.type = ExcelNumber, .number = 17.15047442974452};
static ExcelValue constant1095 = {.type = ExcelNumber, .number = 12.966313116134401};
static ExcelValue constant1096 = {.type = ExcelNumber, .number = 9.388481413971485};
static ExcelValue constant1097 = {.type = ExcelNumber, .number = 7.921143684499443};
static ExcelValue constant1098 = {.type = ExcelNumber, .number = 7.015092599996487};
static ExcelValue constant1099 = {.type = ExcelNumber, .number = 6.027369159418858};
static ExcelValue constant1100 = {.type = ExcelNumber, .number = 5.928092240502379};
static ExcelValue constant1101 = {.type = ExcelNumber, .number = 5.82130293615036};
static ExcelValue constant1102 = {.type = ExcelNumber, .number = 5.887511350664208};
static ExcelValue constant1103 = {.type = ExcelNumber, .number = 5.957108905715392};
static ExcelValue constant1104 = {.type = ExcelNumber, .number = 6.031275913835689};
static ExcelValue constant1105 = {.type = ExcelNumber, .number = 6.107635127928271};
static ExcelValue constant1106 = {.type = ExcelNumber, .number = 6.184009256803236};
static ExcelValue constant1107 = {.type = ExcelNumber, .number = 6.2603862186988195};
static ExcelValue constant1108 = {.type = ExcelNumber, .number = 6.337987596145276};
static ExcelValue constant1109 = {.type = ExcelNumber, .number = 6.4172222490489155};
static ExcelValue constant1110 = {.type = ExcelString, .string = "Industry Chemicals"};
static ExcelValue constant1111 = {.type = ExcelNumber, .number = 12.493537682400646};
static ExcelValue constant1112 = {.type = ExcelNumber, .number = 12.379466182967198};
static ExcelValue constant1113 = {.type = ExcelNumber, .number = 11.701631603350712};
static ExcelValue constant1114 = {.type = ExcelNumber, .number = 11.055172733980344};
static ExcelValue constant1115 = {.type = ExcelNumber, .number = 10.186847012387089};
static ExcelValue constant1116 = {.type = ExcelNumber, .number = 9.373547572955795};
static ExcelValue constant1117 = {.type = ExcelNumber, .number = 8.6121379202598};
static ExcelValue constant1118 = {.type = ExcelNumber, .number = 7.826043076103158};
static ExcelValue constant1119 = {.type = ExcelNumber, .number = 7.61386250654145};
static ExcelValue constant1120 = {.type = ExcelNumber, .number = 7.401763078626377};
static ExcelValue constant1121 = {.type = ExcelNumber, .number = 7.401485685442263};
static ExcelValue constant1122 = {.type = ExcelString, .string = "Hydrogen"};
static ExcelValue constant1123 = {.type = ExcelNumber, .number = 0.08333333333333333};
static ExcelValue constant1124 = {.type = ExcelNumber, .number = 0.16666666666666666};
static ExcelValue constant1125 = {.type = ExcelNumber, .number = 0.25};
static ExcelValue constant1126 = {.type = ExcelNumber, .number = 0.3333333333333333};
static ExcelValue constant1127 = {.type = ExcelNumber, .number = 0.4166666666666667};
static ExcelValue constant1128 = {.type = ExcelNumber, .number = 0.5};
static ExcelValue constant1129 = {.type = ExcelString, .string = "Electric"};
static ExcelValue constant1130 = {.type = ExcelNumber, .number = 0.01};
static ExcelValue constant1131 = {.type = ExcelNumber, .number = 0.1325};
static ExcelValue constant1132 = {.type = ExcelNumber, .number = 0.255};
static ExcelValue constant1133 = {.type = ExcelNumber, .number = 0.3775};
static ExcelValue constant1134 = {.type = ExcelString, .string = "Plug-in Hybrid Electric"};
static ExcelValue constant1135 = {.type = ExcelNumber, .number = 0.02};
static ExcelValue constant1136 = {.type = ExcelNumber, .number = 0.15};
static ExcelValue constant1137 = {.type = ExcelNumber, .number = 0.28};
static ExcelValue constant1138 = {.type = ExcelNumber, .number = 0.37250000000000005};
static ExcelValue constant1139 = {.type = ExcelNumber, .number = 0.16666666666666669};
static ExcelValue constant1140 = {.type = ExcelNumber, .number = 0.08333333333333331};
static ExcelValue constant1141 = {.type = ExcelString, .string = "Conventional"};
static ExcelValue constant1142 = {.type = ExcelNumber, .number = 0.97};
static ExcelValue constant1143 = {.type = ExcelNumber, .number = 0.6341666666666665};
static ExcelValue constant1144 = {.type = ExcelNumber, .number = 0.29833333333333334};
static ExcelValue constant1145 = {.type = ExcelString, .string = "Electricity "};
static ExcelValue constant1146 = {.type = ExcelNumber, .number = 8.172120758330962};
static ExcelValue constant1147 = {.type = ExcelNumber, .number = 8.26148824841383};
static ExcelValue constant1148 = {.type = ExcelNumber, .number = 28.4618926506027};
static ExcelValue constant1149 = {.type = ExcelNumber, .number = 49.01760049941427};
static ExcelValue constant1150 = {.type = ExcelNumber, .number = 68.67079785727623};
static ExcelValue constant1151 = {.type = ExcelNumber, .number = 71.70297758222762};
static ExcelValue constant1152 = {.type = ExcelNumber, .number = 69.4436907757069};
static ExcelValue constant1153 = {.type = ExcelNumber, .number = 67.03608196686832};
static ExcelValue constant1154 = {.type = ExcelNumber, .number = 67.84600265545374};
static ExcelValue constant1155 = {.type = ExcelNumber, .number = 68.6132751819221};
static ExcelValue constant1156 = {.type = ExcelNumber, .number = 69.3809916311615};
static ExcelValue constant1157 = {.type = ExcelNumber, .number = 70.19361733869633};
static ExcelValue constant1158 = {.type = ExcelNumber, .number = 71.05929522263642};
static ExcelValue constant1159 = {.type = ExcelNumber, .number = 71.9503838155992};
static ExcelValue constant1160 = {.type = ExcelNumber, .number = 72.84156713685203};
static ExcelValue constant1161 = {.type = ExcelNumber, .number = 73.7327021652511};
static ExcelValue constant1162 = {.type = ExcelNumber, .number = 74.63798823807839};
static ExcelValue constant1163 = {.type = ExcelNumber, .number = 75.56217601003712};
static ExcelValue constant1164 = {.type = ExcelString, .string = "Hydrogen Gaseous "};
static ExcelValue constant1165 = {.type = ExcelNumber, .number = 24.08324844803424};
static ExcelValue constant1166 = {.type = ExcelNumber, .number = 45.658515772190164};
static ExcelValue constant1167 = {.type = ExcelNumber, .number = 68.13111261201458};
static ExcelValue constant1168 = {.type = ExcelNumber, .number = 89.65505217421071};
static ExcelValue constant1169 = {.type = ExcelNumber, .number = 111.16297007163408};
static ExcelValue constant1170 = {.type = ExcelNumber, .number = 132.2739607700418};
static ExcelValue constant1171 = {.type = ExcelNumber, .number = 133.6744384853355};
static ExcelValue constant1172 = {.type = ExcelNumber, .number = 135.00117095435385};
static ExcelValue constant1173 = {.type = ExcelNumber, .number = 136.32867103427157};
static ExcelValue constant1174 = {.type = ExcelNumber, .number = 137.73382614429218};
static ExcelValue constant1175 = {.type = ExcelNumber, .number = 139.23071664501288};
static ExcelValue constant1176 = {.type = ExcelNumber, .number = 140.77154617855194};
static ExcelValue constant1177 = {.type = ExcelNumber, .number = 142.31253951190638};
static ExcelValue constant1178 = {.type = ExcelNumber, .number = 143.8534493394726};
static ExcelValue constant1179 = {.type = ExcelNumber, .number = 145.41882850415925};
static ExcelValue constant1180 = {.type = ExcelNumber, .number = 147.01689162023507};
static ExcelValue constant1181 = {.type = ExcelString, .string = "Fossil Fuel Liquid "};
static ExcelValue constant1182 = {.type = ExcelNumber, .number = 670.9290481724032};
static ExcelValue constant1183 = {.type = ExcelNumber, .number = 642.9536578705774};
static ExcelValue constant1184 = {.type = ExcelNumber, .number = 520.7208599508731};
static ExcelValue constant1185 = {.type = ExcelNumber, .number = 407.17885135616507};
static ExcelValue constant1186 = {.type = ExcelNumber, .number = 311.2882526100534};
static ExcelValue constant1187 = {.type = ExcelNumber, .number = 285.0254524634644};
static ExcelValue constant1188 = {.type = ExcelNumber, .number = 276.895713072283};
static ExcelValue constant1189 = {.type = ExcelNumber, .number = 263.20710912253276};
static ExcelValue constant1190 = {.type = ExcelNumber, .number = 266.99733790860085};
static ExcelValue constant1191 = {.type = ExcelNumber, .number = 270.5879838293252};
static ExcelValue constant1192 = {.type = ExcelNumber, .number = 274.18070719897673};
static ExcelValue constant1193 = {.type = ExcelNumber, .number = 277.9835948056346};
static ExcelValue constant1194 = {.type = ExcelNumber, .number = 282.0347534948921};
static ExcelValue constant1195 = {.type = ExcelNumber, .number = 286.20482802636656};
static ExcelValue constant1196 = {.type = ExcelNumber, .number = 290.3753458628422};
static ExcelValue constant1197 = {.type = ExcelNumber, .number = 294.5456377006899};
static ExcelValue constant1198 = {.type = ExcelNumber, .number = 298.78215293139635};
static ExcelValue constant1199 = {.type = ExcelNumber, .number = 303.1071234456428};
static ExcelValue constant1200 = {.type = ExcelString, .string = "Bioenergy Liquid "};
static ExcelValue constant1201 = {.type = ExcelNumber, .number = 1.5728227961365546};
static ExcelValue constant1202 = {.type = ExcelNumber, .number = 1.7334930761295322};
static ExcelValue constant1203 = {.type = ExcelNumber, .number = 5.929483925229272};
static ExcelValue constant1204 = {.type = ExcelNumber, .number = 8.21875072952473};
static ExcelValue constant1205 = {.type = ExcelNumber, .number = 9.01570383496548};
static ExcelValue constant1206 = {.type = ExcelNumber, .number = 10.561856595832731};
static ExcelValue constant1207 = {.type = ExcelNumber, .number = 12.433910463190706};
static ExcelValue constant1208 = {.type = ExcelNumber, .number = 13.853005743291199};
static ExcelValue constant1209 = {.type = ExcelNumber, .number = 14.052491468873729};
static ExcelValue constant1210 = {.type = ExcelNumber, .number = 14.24147283312238};
static ExcelValue constant1211 = {.type = ExcelNumber, .number = 14.430563536788252};
static ExcelValue constant1212 = {.type = ExcelNumber, .number = 14.630715516086033};
static ExcelValue constant1213 = {.type = ExcelNumber, .number = 14.843934394468006};
static ExcelValue constant1214 = {.type = ExcelNumber, .number = 15.063412001387716};
static ExcelValue constant1215 = {.type = ExcelNumber, .number = 15.282912940149595};
static ExcelValue constant1216 = {.type = ExcelNumber, .number = 15.50240198424684};
static ExcelValue constant1217 = {.type = ExcelNumber, .number = 15.725376470073495};
static ExcelValue constant1218 = {.type = ExcelNumber, .number = 15.953006497139096};
static ExcelValue constant1219 = {.type = ExcelString, .string = "Mha."};
static ExcelValue constant1220 = {.type = ExcelString, .string = "GBP.2016."};
static ExcelValue constant1221 = {.type = ExcelString, .string = "km."};
static ExcelValue constant1222 = {.type = ExcelString, .string = "TWh."};
static ExcelValue constant1223 = {.type = ExcelString, .string = "kt."};
static ExcelValue constant1224 = {.type = ExcelString, .string = "Mt."};
static ExcelValue constant1225 = {.type = ExcelString, .string = "GW."};
static ExcelValue constant1226 = {.type = ExcelNumber, .number = 30.0};
static ExcelValue constant1227 = {.type = ExcelNumber, .number = 20.0};
static ExcelValue constant1228 = {.type = ExcelNumber, .number = 50.0};
// ending the value constants

ExcelValue control_d17_default() {
  return THREE;
}
static ExcelValue control_d17_variable;
ExcelValue control_d17() { if(variable_set[0] == 1) { return control_d17_variable; } else { return control_d17_default(); } }
void set_control_d17(ExcelValue newValue) { variable_set[0] = 1; control_d17_variable = newValue; }

ExcelValue control_d18_default() {
  return constant1;
}
static ExcelValue control_d18_variable;
ExcelValue control_d18() { if(variable_set[1] == 1) { return control_d18_variable; } else { return control_d18_default(); } }
void set_control_d18(ExcelValue newValue) { variable_set[1] = 1; control_d18_variable = newValue; }

ExcelValue control_d19_default() {
  return constant1;
}
static ExcelValue control_d19_variable;
ExcelValue control_d19() { if(variable_set[2] == 1) { return control_d19_variable; } else { return control_d19_default(); } }
void set_control_d19(ExcelValue newValue) { variable_set[2] = 1; control_d19_variable = newValue; }

ExcelValue control_d20_default() {
  return TWO;
}
static ExcelValue control_d20_variable;
ExcelValue control_d20() { if(variable_set[3] == 1) { return control_d20_variable; } else { return control_d20_default(); } }
void set_control_d20(ExcelValue newValue) { variable_set[3] = 1; control_d20_variable = newValue; }

ExcelValue control_d21_default() {
  return TWO;
}
static ExcelValue control_d21_variable;
ExcelValue control_d21() { if(variable_set[4] == 1) { return control_d21_variable; } else { return control_d21_default(); } }
void set_control_d21(ExcelValue newValue) { variable_set[4] = 1; control_d21_variable = newValue; }

ExcelValue control_d22_default() {
  return constant1;
}
static ExcelValue control_d22_variable;
ExcelValue control_d22() { if(variable_set[5] == 1) { return control_d22_variable; } else { return control_d22_default(); } }
void set_control_d22(ExcelValue newValue) { variable_set[5] = 1; control_d22_variable = newValue; }

ExcelValue control_d23_default() {
  return TWO;
}
static ExcelValue control_d23_variable;
ExcelValue control_d23() { if(variable_set[6] == 1) { return control_d23_variable; } else { return control_d23_default(); } }
void set_control_d23(ExcelValue newValue) { variable_set[6] = 1; control_d23_variable = newValue; }

ExcelValue control_d24_default() {
  return THREE;
}
static ExcelValue control_d24_variable;
ExcelValue control_d24() { if(variable_set[7] == 1) { return control_d24_variable; } else { return control_d24_default(); } }
void set_control_d24(ExcelValue newValue) { variable_set[7] = 1; control_d24_variable = newValue; }

ExcelValue control_d25_default() {
  return TWO;
}
static ExcelValue control_d25_variable;
ExcelValue control_d25() { if(variable_set[8] == 1) { return control_d25_variable; } else { return control_d25_default(); } }
void set_control_d25(ExcelValue newValue) { variable_set[8] = 1; control_d25_variable = newValue; }

ExcelValue control_d26_default() {
  return TWO;
}
static ExcelValue control_d26_variable;
ExcelValue control_d26() { if(variable_set[9] == 1) { return control_d26_variable; } else { return control_d26_default(); } }
void set_control_d26(ExcelValue newValue) { variable_set[9] = 1; control_d26_variable = newValue; }

ExcelValue control_d27_default() {
  return TWO;
}
static ExcelValue control_d27_variable;
ExcelValue control_d27() { if(variable_set[10] == 1) { return control_d27_variable; } else { return control_d27_default(); } }
void set_control_d27(ExcelValue newValue) { variable_set[10] = 1; control_d27_variable = newValue; }

ExcelValue control_d28_default() {
  return THREE;
}
static ExcelValue control_d28_variable;
ExcelValue control_d28() { if(variable_set[11] == 1) { return control_d28_variable; } else { return control_d28_default(); } }
void set_control_d28(ExcelValue newValue) { variable_set[11] = 1; control_d28_variable = newValue; }

ExcelValue control_d29_default() {
  return TWO;
}
static ExcelValue control_d29_variable;
ExcelValue control_d29() { if(variable_set[12] == 1) { return control_d29_variable; } else { return control_d29_default(); } }
void set_control_d29(ExcelValue newValue) { variable_set[12] = 1; control_d29_variable = newValue; }

ExcelValue control_d30_default() {
  return TWO;
}
static ExcelValue control_d30_variable;
ExcelValue control_d30() { if(variable_set[13] == 1) { return control_d30_variable; } else { return control_d30_default(); } }
void set_control_d30(ExcelValue newValue) { variable_set[13] = 1; control_d30_variable = newValue; }

ExcelValue control_d31_default() {
  return TWO;
}
static ExcelValue control_d31_variable;
ExcelValue control_d31() { if(variable_set[14] == 1) { return control_d31_variable; } else { return control_d31_default(); } }
void set_control_d31(ExcelValue newValue) { variable_set[14] = 1; control_d31_variable = newValue; }

ExcelValue control_d32_default() {
  return THREE;
}
static ExcelValue control_d32_variable;
ExcelValue control_d32() { if(variable_set[15] == 1) { return control_d32_variable; } else { return control_d32_default(); } }
void set_control_d32(ExcelValue newValue) { variable_set[15] = 1; control_d32_variable = newValue; }

ExcelValue control_d33_default() {
  return TWO;
}
static ExcelValue control_d33_variable;
ExcelValue control_d33() { if(variable_set[16] == 1) { return control_d33_variable; } else { return control_d33_default(); } }
void set_control_d33(ExcelValue newValue) { variable_set[16] = 1; control_d33_variable = newValue; }

ExcelValue control_d34_default() {
  return TWO;
}
static ExcelValue control_d34_variable;
ExcelValue control_d34() { if(variable_set[17] == 1) { return control_d34_variable; } else { return control_d34_default(); } }
void set_control_d34(ExcelValue newValue) { variable_set[17] = 1; control_d34_variable = newValue; }

ExcelValue control_d35_default() {
  return TWO;
}
static ExcelValue control_d35_variable;
ExcelValue control_d35() { if(variable_set[18] == 1) { return control_d35_variable; } else { return control_d35_default(); } }
void set_control_d35(ExcelValue newValue) { variable_set[18] = 1; control_d35_variable = newValue; }

ExcelValue control_d36_default() {
  return TWO;
}
static ExcelValue control_d36_variable;
ExcelValue control_d36() { if(variable_set[19] == 1) { return control_d36_variable; } else { return control_d36_default(); } }
void set_control_d36(ExcelValue newValue) { variable_set[19] = 1; control_d36_variable = newValue; }

ExcelValue control_d37_default() {
  return TWO;
}
static ExcelValue control_d37_variable;
ExcelValue control_d37() { if(variable_set[20] == 1) { return control_d37_variable; } else { return control_d37_default(); } }
void set_control_d37(ExcelValue newValue) { variable_set[20] = 1; control_d37_variable = newValue; }

ExcelValue control_d38_default() {
  return ONE;
}
static ExcelValue control_d38_variable;
ExcelValue control_d38() { if(variable_set[21] == 1) { return control_d38_variable; } else { return control_d38_default(); } }
void set_control_d38(ExcelValue newValue) { variable_set[21] = 1; control_d38_variable = newValue; }

ExcelValue control_d39_default() {
  return ONE;
}
static ExcelValue control_d39_variable;
ExcelValue control_d39() { if(variable_set[22] == 1) { return control_d39_variable; } else { return control_d39_default(); } }
void set_control_d39(ExcelValue newValue) { variable_set[22] = 1; control_d39_variable = newValue; }

ExcelValue control_d40_default() {
  return ONE;
}
static ExcelValue control_d40_variable;
ExcelValue control_d40() { if(variable_set[23] == 1) { return control_d40_variable; } else { return control_d40_default(); } }
void set_control_d40(ExcelValue newValue) { variable_set[23] = 1; control_d40_variable = newValue; }

ExcelValue control_d41_default() {
  return TWO;
}
static ExcelValue control_d41_variable;
ExcelValue control_d41() { if(variable_set[24] == 1) { return control_d41_variable; } else { return control_d41_default(); } }
void set_control_d41(ExcelValue newValue) { variable_set[24] = 1; control_d41_variable = newValue; }

ExcelValue control_d42_default() {
  return TWO;
}
static ExcelValue control_d42_variable;
ExcelValue control_d42() { if(variable_set[25] == 1) { return control_d42_variable; } else { return control_d42_default(); } }
void set_control_d42(ExcelValue newValue) { variable_set[25] = 1; control_d42_variable = newValue; }

ExcelValue control_d43_default() {
  return TWO;
}
static ExcelValue control_d43_variable;
ExcelValue control_d43() { if(variable_set[26] == 1) { return control_d43_variable; } else { return control_d43_default(); } }
void set_control_d43(ExcelValue newValue) { variable_set[26] = 1; control_d43_variable = newValue; }

ExcelValue control_d44_default() {
  return THREE;
}
static ExcelValue control_d44_variable;
ExcelValue control_d44() { if(variable_set[27] == 1) { return control_d44_variable; } else { return control_d44_default(); } }
void set_control_d44(ExcelValue newValue) { variable_set[27] = 1; control_d44_variable = newValue; }

ExcelValue control_d45_default() {
  return TWO;
}
static ExcelValue control_d45_variable;
ExcelValue control_d45() { if(variable_set[28] == 1) { return control_d45_variable; } else { return control_d45_default(); } }
void set_control_d45(ExcelValue newValue) { variable_set[28] = 1; control_d45_variable = newValue; }

ExcelValue control_d46_default() {
  return TWO;
}
static ExcelValue control_d46_variable;
ExcelValue control_d46() { if(variable_set[29] == 1) { return control_d46_variable; } else { return control_d46_default(); } }
void set_control_d46(ExcelValue newValue) { variable_set[29] = 1; control_d46_variable = newValue; }

ExcelValue control_d47_default() {
  return TWO;
}
static ExcelValue control_d47_variable;
ExcelValue control_d47() { if(variable_set[30] == 1) { return control_d47_variable; } else { return control_d47_default(); } }
void set_control_d47(ExcelValue newValue) { variable_set[30] = 1; control_d47_variable = newValue; }

ExcelValue control_d48_default() {
  return TWO;
}
static ExcelValue control_d48_variable;
ExcelValue control_d48() { if(variable_set[31] == 1) { return control_d48_variable; } else { return control_d48_default(); } }
void set_control_d48(ExcelValue newValue) { variable_set[31] = 1; control_d48_variable = newValue; }

ExcelValue control_d49_default() {
  return TWO;
}
static ExcelValue control_d49_variable;
ExcelValue control_d49() { if(variable_set[32] == 1) { return control_d49_variable; } else { return control_d49_default(); } }
void set_control_d49(ExcelValue newValue) { variable_set[32] = 1; control_d49_variable = newValue; }

ExcelValue control_d50_default() {
  return TWO;
}
static ExcelValue control_d50_variable;
ExcelValue control_d50() { if(variable_set[33] == 1) { return control_d50_variable; } else { return control_d50_default(); } }
void set_control_d50(ExcelValue newValue) { variable_set[33] = 1; control_d50_variable = newValue; }

ExcelValue control_d51_default() {
  return THREE;
}
static ExcelValue control_d51_variable;
ExcelValue control_d51() { if(variable_set[34] == 1) { return control_d51_variable; } else { return control_d51_default(); } }
void set_control_d51(ExcelValue newValue) { variable_set[34] = 1; control_d51_variable = newValue; }

ExcelValue control_d52_default() {
  return TWO;
}
static ExcelValue control_d52_variable;
ExcelValue control_d52() { if(variable_set[35] == 1) { return control_d52_variable; } else { return control_d52_default(); } }
void set_control_d52(ExcelValue newValue) { variable_set[35] = 1; control_d52_variable = newValue; }

ExcelValue control_d53_default() {
  return TWO;
}
static ExcelValue control_d53_variable;
ExcelValue control_d53() { if(variable_set[36] == 1) { return control_d53_variable; } else { return control_d53_default(); } }
void set_control_d53(ExcelValue newValue) { variable_set[36] = 1; control_d53_variable = newValue; }

ExcelValue control_d54_default() {
  return TWO;
}
static ExcelValue control_d54_variable;
ExcelValue control_d54() { if(variable_set[37] == 1) { return control_d54_variable; } else { return control_d54_default(); } }
void set_control_d54(ExcelValue newValue) { variable_set[37] = 1; control_d54_variable = newValue; }

ExcelValue control_d55_default() {
  return TWO;
}
static ExcelValue control_d55_variable;
ExcelValue control_d55() { if(variable_set[38] == 1) { return control_d55_variable; } else { return control_d55_default(); } }
void set_control_d55(ExcelValue newValue) { variable_set[38] = 1; control_d55_variable = newValue; }

ExcelValue control_d56_default() {
  return TWO;
}
static ExcelValue control_d56_variable;
ExcelValue control_d56() { if(variable_set[39] == 1) { return control_d56_variable; } else { return control_d56_default(); } }
void set_control_d56(ExcelValue newValue) { variable_set[39] = 1; control_d56_variable = newValue; }

ExcelValue control_d57_default() {
  return TWO;
}
static ExcelValue control_d57_variable;
ExcelValue control_d57() { if(variable_set[40] == 1) { return control_d57_variable; } else { return control_d57_default(); } }
void set_control_d57(ExcelValue newValue) { variable_set[40] = 1; control_d57_variable = newValue; }

ExcelValue control_d58_default() {
  return TWO;
}
static ExcelValue control_d58_variable;
ExcelValue control_d58() { if(variable_set[41] == 1) { return control_d58_variable; } else { return control_d58_default(); } }
void set_control_d58(ExcelValue newValue) { variable_set[41] = 1; control_d58_variable = newValue; }

ExcelValue control_d59_default() {
  return TWO;
}
static ExcelValue control_d59_variable;
ExcelValue control_d59() { if(variable_set[42] == 1) { return control_d59_variable; } else { return control_d59_default(); } }
void set_control_d59(ExcelValue newValue) { variable_set[42] = 1; control_d59_variable = newValue; }

ExcelValue control_d60_default() {
  return ONE;
}
static ExcelValue control_d60_variable;
ExcelValue control_d60() { if(variable_set[43] == 1) { return control_d60_variable; } else { return control_d60_default(); } }
void set_control_d60(ExcelValue newValue) { variable_set[43] = 1; control_d60_variable = newValue; }

ExcelValue control_d61_default() {
  return TWO;
}
static ExcelValue control_d61_variable;
ExcelValue control_d61() { if(variable_set[44] == 1) { return control_d61_variable; } else { return control_d61_default(); } }
void set_control_d61(ExcelValue newValue) { variable_set[44] = 1; control_d61_variable = newValue; }

ExcelValue control_g17_default() {
  return constant2;
}
static ExcelValue control_g17_variable;
ExcelValue control_g17() { if(variable_set[45] == 1) { return control_g17_variable; } else { return control_g17_default(); } }
void set_control_g17(ExcelValue newValue) { variable_set[45] = 1; control_g17_variable = newValue; }

ExcelValue control_g18_default() {
  return constant2;
}
static ExcelValue control_g18_variable;
ExcelValue control_g18() { if(variable_set[46] == 1) { return control_g18_variable; } else { return control_g18_default(); } }
void set_control_g18(ExcelValue newValue) { variable_set[46] = 1; control_g18_variable = newValue; }

ExcelValue control_g19_default() {
  return constant2;
}
static ExcelValue control_g19_variable;
ExcelValue control_g19() { if(variable_set[47] == 1) { return control_g19_variable; } else { return control_g19_default(); } }
void set_control_g19(ExcelValue newValue) { variable_set[47] = 1; control_g19_variable = newValue; }

ExcelValue control_g20_default() {
  return constant2;
}
static ExcelValue control_g20_variable;
ExcelValue control_g20() { if(variable_set[48] == 1) { return control_g20_variable; } else { return control_g20_default(); } }
void set_control_g20(ExcelValue newValue) { variable_set[48] = 1; control_g20_variable = newValue; }

ExcelValue control_g21_default() {
  return constant2;
}
static ExcelValue control_g21_variable;
ExcelValue control_g21() { if(variable_set[49] == 1) { return control_g21_variable; } else { return control_g21_default(); } }
void set_control_g21(ExcelValue newValue) { variable_set[49] = 1; control_g21_variable = newValue; }

ExcelValue control_g22_default() {
  return constant2;
}
static ExcelValue control_g22_variable;
ExcelValue control_g22() { if(variable_set[50] == 1) { return control_g22_variable; } else { return control_g22_default(); } }
void set_control_g22(ExcelValue newValue) { variable_set[50] = 1; control_g22_variable = newValue; }

ExcelValue control_g23_default() {
  return constant2;
}
static ExcelValue control_g23_variable;
ExcelValue control_g23() { if(variable_set[51] == 1) { return control_g23_variable; } else { return control_g23_default(); } }
void set_control_g23(ExcelValue newValue) { variable_set[51] = 1; control_g23_variable = newValue; }

ExcelValue control_g24_default() {
  return constant2;
}
static ExcelValue control_g24_variable;
ExcelValue control_g24() { if(variable_set[52] == 1) { return control_g24_variable; } else { return control_g24_default(); } }
void set_control_g24(ExcelValue newValue) { variable_set[52] = 1; control_g24_variable = newValue; }

ExcelValue control_g25_default() {
  return constant2;
}
static ExcelValue control_g25_variable;
ExcelValue control_g25() { if(variable_set[53] == 1) { return control_g25_variable; } else { return control_g25_default(); } }
void set_control_g25(ExcelValue newValue) { variable_set[53] = 1; control_g25_variable = newValue; }

ExcelValue control_g26_default() {
  return constant2;
}
static ExcelValue control_g26_variable;
ExcelValue control_g26() { if(variable_set[54] == 1) { return control_g26_variable; } else { return control_g26_default(); } }
void set_control_g26(ExcelValue newValue) { variable_set[54] = 1; control_g26_variable = newValue; }

ExcelValue control_g27_default() {
  return constant3;
}
static ExcelValue control_g27_variable;
ExcelValue control_g27() { if(variable_set[55] == 1) { return control_g27_variable; } else { return control_g27_default(); } }
void set_control_g27(ExcelValue newValue) { variable_set[55] = 1; control_g27_variable = newValue; }

ExcelValue control_g28_default() {
  return constant2;
}
static ExcelValue control_g28_variable;
ExcelValue control_g28() { if(variable_set[56] == 1) { return control_g28_variable; } else { return control_g28_default(); } }
void set_control_g28(ExcelValue newValue) { variable_set[56] = 1; control_g28_variable = newValue; }

ExcelValue control_g29_default() {
  return constant2;
}
static ExcelValue control_g29_variable;
ExcelValue control_g29() { if(variable_set[57] == 1) { return control_g29_variable; } else { return control_g29_default(); } }
void set_control_g29(ExcelValue newValue) { variable_set[57] = 1; control_g29_variable = newValue; }

ExcelValue control_g30_default() {
  return constant2;
}
static ExcelValue control_g30_variable;
ExcelValue control_g30() { if(variable_set[58] == 1) { return control_g30_variable; } else { return control_g30_default(); } }
void set_control_g30(ExcelValue newValue) { variable_set[58] = 1; control_g30_variable = newValue; }

ExcelValue control_g31_default() {
  return constant2;
}
static ExcelValue control_g31_variable;
ExcelValue control_g31() { if(variable_set[59] == 1) { return control_g31_variable; } else { return control_g31_default(); } }
void set_control_g31(ExcelValue newValue) { variable_set[59] = 1; control_g31_variable = newValue; }

ExcelValue control_g32_default() {
  return constant2;
}
static ExcelValue control_g32_variable;
ExcelValue control_g32() { if(variable_set[60] == 1) { return control_g32_variable; } else { return control_g32_default(); } }
void set_control_g32(ExcelValue newValue) { variable_set[60] = 1; control_g32_variable = newValue; }

ExcelValue control_g33_default() {
  return constant2;
}
static ExcelValue control_g33_variable;
ExcelValue control_g33() { if(variable_set[61] == 1) { return control_g33_variable; } else { return control_g33_default(); } }
void set_control_g33(ExcelValue newValue) { variable_set[61] = 1; control_g33_variable = newValue; }

ExcelValue control_g34_default() {
  return constant2;
}
static ExcelValue control_g34_variable;
ExcelValue control_g34() { if(variable_set[62] == 1) { return control_g34_variable; } else { return control_g34_default(); } }
void set_control_g34(ExcelValue newValue) { variable_set[62] = 1; control_g34_variable = newValue; }

ExcelValue control_g35_default() {
  return constant2;
}
static ExcelValue control_g35_variable;
ExcelValue control_g35() { if(variable_set[63] == 1) { return control_g35_variable; } else { return control_g35_default(); } }
void set_control_g35(ExcelValue newValue) { variable_set[63] = 1; control_g35_variable = newValue; }

ExcelValue control_g36_default() {
  return constant2;
}
static ExcelValue control_g36_variable;
ExcelValue control_g36() { if(variable_set[64] == 1) { return control_g36_variable; } else { return control_g36_default(); } }
void set_control_g36(ExcelValue newValue) { variable_set[64] = 1; control_g36_variable = newValue; }

ExcelValue control_g37_default() {
  return constant2;
}
static ExcelValue control_g37_variable;
ExcelValue control_g37() { if(variable_set[65] == 1) { return control_g37_variable; } else { return control_g37_default(); } }
void set_control_g37(ExcelValue newValue) { variable_set[65] = 1; control_g37_variable = newValue; }

ExcelValue control_g38_default() {
  return constant2;
}
static ExcelValue control_g38_variable;
ExcelValue control_g38() { if(variable_set[66] == 1) { return control_g38_variable; } else { return control_g38_default(); } }
void set_control_g38(ExcelValue newValue) { variable_set[66] = 1; control_g38_variable = newValue; }

ExcelValue control_g39_default() {
  return constant2;
}
static ExcelValue control_g39_variable;
ExcelValue control_g39() { if(variable_set[67] == 1) { return control_g39_variable; } else { return control_g39_default(); } }
void set_control_g39(ExcelValue newValue) { variable_set[67] = 1; control_g39_variable = newValue; }

ExcelValue control_g40_default() {
  return constant2;
}
static ExcelValue control_g40_variable;
ExcelValue control_g40() { if(variable_set[68] == 1) { return control_g40_variable; } else { return control_g40_default(); } }
void set_control_g40(ExcelValue newValue) { variable_set[68] = 1; control_g40_variable = newValue; }

ExcelValue control_g41_default() {
  return constant2;
}
static ExcelValue control_g41_variable;
ExcelValue control_g41() { if(variable_set[69] == 1) { return control_g41_variable; } else { return control_g41_default(); } }
void set_control_g41(ExcelValue newValue) { variable_set[69] = 1; control_g41_variable = newValue; }

ExcelValue control_g42_default() {
  return constant4;
}
static ExcelValue control_g42_variable;
ExcelValue control_g42() { if(variable_set[70] == 1) { return control_g42_variable; } else { return control_g42_default(); } }
void set_control_g42(ExcelValue newValue) { variable_set[70] = 1; control_g42_variable = newValue; }

ExcelValue control_g43_default() {
  return constant2;
}
static ExcelValue control_g43_variable;
ExcelValue control_g43() { if(variable_set[71] == 1) { return control_g43_variable; } else { return control_g43_default(); } }
void set_control_g43(ExcelValue newValue) { variable_set[71] = 1; control_g43_variable = newValue; }

ExcelValue control_g44_default() {
  return constant2;
}
static ExcelValue control_g44_variable;
ExcelValue control_g44() { if(variable_set[72] == 1) { return control_g44_variable; } else { return control_g44_default(); } }
void set_control_g44(ExcelValue newValue) { variable_set[72] = 1; control_g44_variable = newValue; }

ExcelValue control_g45_default() {
  return constant2;
}
static ExcelValue control_g45_variable;
ExcelValue control_g45() { if(variable_set[73] == 1) { return control_g45_variable; } else { return control_g45_default(); } }
void set_control_g45(ExcelValue newValue) { variable_set[73] = 1; control_g45_variable = newValue; }

ExcelValue control_g46_default() {
  return constant2;
}
static ExcelValue control_g46_variable;
ExcelValue control_g46() { if(variable_set[74] == 1) { return control_g46_variable; } else { return control_g46_default(); } }
void set_control_g46(ExcelValue newValue) { variable_set[74] = 1; control_g46_variable = newValue; }

ExcelValue control_g47_default() {
  return constant5;
}
static ExcelValue control_g47_variable;
ExcelValue control_g47() { if(variable_set[75] == 1) { return control_g47_variable; } else { return control_g47_default(); } }
void set_control_g47(ExcelValue newValue) { variable_set[75] = 1; control_g47_variable = newValue; }

ExcelValue control_g48_default() {
  return constant4;
}
static ExcelValue control_g48_variable;
ExcelValue control_g48() { if(variable_set[76] == 1) { return control_g48_variable; } else { return control_g48_default(); } }
void set_control_g48(ExcelValue newValue) { variable_set[76] = 1; control_g48_variable = newValue; }

ExcelValue control_g49_default() {
  return constant6;
}
static ExcelValue control_g49_variable;
ExcelValue control_g49() { if(variable_set[77] == 1) { return control_g49_variable; } else { return control_g49_default(); } }
void set_control_g49(ExcelValue newValue) { variable_set[77] = 1; control_g49_variable = newValue; }

ExcelValue control_g50_default() {
  return constant2;
}
static ExcelValue control_g50_variable;
ExcelValue control_g50() { if(variable_set[78] == 1) { return control_g50_variable; } else { return control_g50_default(); } }
void set_control_g50(ExcelValue newValue) { variable_set[78] = 1; control_g50_variable = newValue; }

ExcelValue control_g51_default() {
  return constant2;
}
static ExcelValue control_g51_variable;
ExcelValue control_g51() { if(variable_set[79] == 1) { return control_g51_variable; } else { return control_g51_default(); } }
void set_control_g51(ExcelValue newValue) { variable_set[79] = 1; control_g51_variable = newValue; }

ExcelValue control_g52_default() {
  return constant2;
}
static ExcelValue control_g52_variable;
ExcelValue control_g52() { if(variable_set[80] == 1) { return control_g52_variable; } else { return control_g52_default(); } }
void set_control_g52(ExcelValue newValue) { variable_set[80] = 1; control_g52_variable = newValue; }

ExcelValue control_g53_default() {
  return constant2;
}
static ExcelValue control_g53_variable;
ExcelValue control_g53() { if(variable_set[81] == 1) { return control_g53_variable; } else { return control_g53_default(); } }
void set_control_g53(ExcelValue newValue) { variable_set[81] = 1; control_g53_variable = newValue; }

ExcelValue control_g54_default() {
  return constant2;
}
static ExcelValue control_g54_variable;
ExcelValue control_g54() { if(variable_set[82] == 1) { return control_g54_variable; } else { return control_g54_default(); } }
void set_control_g54(ExcelValue newValue) { variable_set[82] = 1; control_g54_variable = newValue; }

ExcelValue control_g55_default() {
  return constant2;
}
static ExcelValue control_g55_variable;
ExcelValue control_g55() { if(variable_set[83] == 1) { return control_g55_variable; } else { return control_g55_default(); } }
void set_control_g55(ExcelValue newValue) { variable_set[83] = 1; control_g55_variable = newValue; }

ExcelValue control_g56_default() {
  return constant2;
}
static ExcelValue control_g56_variable;
ExcelValue control_g56() { if(variable_set[84] == 1) { return control_g56_variable; } else { return control_g56_default(); } }
void set_control_g56(ExcelValue newValue) { variable_set[84] = 1; control_g56_variable = newValue; }

ExcelValue control_g57_default() {
  return constant2;
}
static ExcelValue control_g57_variable;
ExcelValue control_g57() { if(variable_set[85] == 1) { return control_g57_variable; } else { return control_g57_default(); } }
void set_control_g57(ExcelValue newValue) { variable_set[85] = 1; control_g57_variable = newValue; }

ExcelValue control_g58_default() {
  return constant2;
}
static ExcelValue control_g58_variable;
ExcelValue control_g58() { if(variable_set[86] == 1) { return control_g58_variable; } else { return control_g58_default(); } }
void set_control_g58(ExcelValue newValue) { variable_set[86] = 1; control_g58_variable = newValue; }

ExcelValue control_g59_default() {
  return constant5;
}
static ExcelValue control_g59_variable;
ExcelValue control_g59() { if(variable_set[87] == 1) { return control_g59_variable; } else { return control_g59_default(); } }
void set_control_g59(ExcelValue newValue) { variable_set[87] = 1; control_g59_variable = newValue; }

ExcelValue control_g60_default() {
  return constant2;
}
static ExcelValue control_g60_variable;
ExcelValue control_g60() { if(variable_set[88] == 1) { return control_g60_variable; } else { return control_g60_default(); } }
void set_control_g60(ExcelValue newValue) { variable_set[88] = 1; control_g60_variable = newValue; }

ExcelValue control_g61_default() {
  return constant2;
}
static ExcelValue control_g61_variable;
ExcelValue control_g61() { if(variable_set[89] == 1) { return control_g61_variable; } else { return control_g61_default(); } }
void set_control_g61(ExcelValue newValue) { variable_set[89] = 1; control_g61_variable = newValue; }

ExcelValue control_e17_default() {
  return constant7;
}
static ExcelValue control_e17_variable;
ExcelValue control_e17() { if(variable_set[90] == 1) { return control_e17_variable; } else { return control_e17_default(); } }
void set_control_e17(ExcelValue newValue) { variable_set[90] = 1; control_e17_variable = newValue; }

ExcelValue control_e18_default() {
  return constant7;
}
static ExcelValue control_e18_variable;
ExcelValue control_e18() { if(variable_set[91] == 1) { return control_e18_variable; } else { return control_e18_default(); } }
void set_control_e18(ExcelValue newValue) { variable_set[91] = 1; control_e18_variable = newValue; }

ExcelValue control_e19_default() {
  return constant7;
}
static ExcelValue control_e19_variable;
ExcelValue control_e19() { if(variable_set[92] == 1) { return control_e19_variable; } else { return control_e19_default(); } }
void set_control_e19(ExcelValue newValue) { variable_set[92] = 1; control_e19_variable = newValue; }

ExcelValue control_e20_default() {
  return constant7;
}
static ExcelValue control_e20_variable;
ExcelValue control_e20() { if(variable_set[93] == 1) { return control_e20_variable; } else { return control_e20_default(); } }
void set_control_e20(ExcelValue newValue) { variable_set[93] = 1; control_e20_variable = newValue; }

ExcelValue control_e21_default() {
  return constant7;
}
static ExcelValue control_e21_variable;
ExcelValue control_e21() { if(variable_set[94] == 1) { return control_e21_variable; } else { return control_e21_default(); } }
void set_control_e21(ExcelValue newValue) { variable_set[94] = 1; control_e21_variable = newValue; }

ExcelValue control_e22_default() {
  return constant7;
}
static ExcelValue control_e22_variable;
ExcelValue control_e22() { if(variable_set[95] == 1) { return control_e22_variable; } else { return control_e22_default(); } }
void set_control_e22(ExcelValue newValue) { variable_set[95] = 1; control_e22_variable = newValue; }

ExcelValue control_e23_default() {
  return constant7;
}
static ExcelValue control_e23_variable;
ExcelValue control_e23() { if(variable_set[96] == 1) { return control_e23_variable; } else { return control_e23_default(); } }
void set_control_e23(ExcelValue newValue) { variable_set[96] = 1; control_e23_variable = newValue; }

ExcelValue control_e24_default() {
  return constant7;
}
static ExcelValue control_e24_variable;
ExcelValue control_e24() { if(variable_set[97] == 1) { return control_e24_variable; } else { return control_e24_default(); } }
void set_control_e24(ExcelValue newValue) { variable_set[97] = 1; control_e24_variable = newValue; }

ExcelValue control_e25_default() {
  return constant7;
}
static ExcelValue control_e25_variable;
ExcelValue control_e25() { if(variable_set[98] == 1) { return control_e25_variable; } else { return control_e25_default(); } }
void set_control_e25(ExcelValue newValue) { variable_set[98] = 1; control_e25_variable = newValue; }

ExcelValue control_e26_default() {
  return constant7;
}
static ExcelValue control_e26_variable;
ExcelValue control_e26() { if(variable_set[99] == 1) { return control_e26_variable; } else { return control_e26_default(); } }
void set_control_e26(ExcelValue newValue) { variable_set[99] = 1; control_e26_variable = newValue; }

ExcelValue control_e27_default() {
  return constant7;
}
static ExcelValue control_e27_variable;
ExcelValue control_e27() { if(variable_set[100] == 1) { return control_e27_variable; } else { return control_e27_default(); } }
void set_control_e27(ExcelValue newValue) { variable_set[100] = 1; control_e27_variable = newValue; }

ExcelValue control_e28_default() {
  return constant7;
}
static ExcelValue control_e28_variable;
ExcelValue control_e28() { if(variable_set[101] == 1) { return control_e28_variable; } else { return control_e28_default(); } }
void set_control_e28(ExcelValue newValue) { variable_set[101] = 1; control_e28_variable = newValue; }

ExcelValue control_e29_default() {
  return constant7;
}
static ExcelValue control_e29_variable;
ExcelValue control_e29() { if(variable_set[102] == 1) { return control_e29_variable; } else { return control_e29_default(); } }
void set_control_e29(ExcelValue newValue) { variable_set[102] = 1; control_e29_variable = newValue; }

ExcelValue control_e30_default() {
  return constant7;
}
static ExcelValue control_e30_variable;
ExcelValue control_e30() { if(variable_set[103] == 1) { return control_e30_variable; } else { return control_e30_default(); } }
void set_control_e30(ExcelValue newValue) { variable_set[103] = 1; control_e30_variable = newValue; }

ExcelValue control_e31_default() {
  return constant7;
}
static ExcelValue control_e31_variable;
ExcelValue control_e31() { if(variable_set[104] == 1) { return control_e31_variable; } else { return control_e31_default(); } }
void set_control_e31(ExcelValue newValue) { variable_set[104] = 1; control_e31_variable = newValue; }

ExcelValue control_e32_default() {
  return constant7;
}
static ExcelValue control_e32_variable;
ExcelValue control_e32() { if(variable_set[105] == 1) { return control_e32_variable; } else { return control_e32_default(); } }
void set_control_e32(ExcelValue newValue) { variable_set[105] = 1; control_e32_variable = newValue; }

ExcelValue control_e33_default() {
  return constant7;
}
static ExcelValue control_e33_variable;
ExcelValue control_e33() { if(variable_set[106] == 1) { return control_e33_variable; } else { return control_e33_default(); } }
void set_control_e33(ExcelValue newValue) { variable_set[106] = 1; control_e33_variable = newValue; }

ExcelValue control_e34_default() {
  return constant7;
}
static ExcelValue control_e34_variable;
ExcelValue control_e34() { if(variable_set[107] == 1) { return control_e34_variable; } else { return control_e34_default(); } }
void set_control_e34(ExcelValue newValue) { variable_set[107] = 1; control_e34_variable = newValue; }

ExcelValue control_e35_default() {
  return constant7;
}
static ExcelValue control_e35_variable;
ExcelValue control_e35() { if(variable_set[108] == 1) { return control_e35_variable; } else { return control_e35_default(); } }
void set_control_e35(ExcelValue newValue) { variable_set[108] = 1; control_e35_variable = newValue; }

ExcelValue control_e36_default() {
  return constant7;
}
static ExcelValue control_e36_variable;
ExcelValue control_e36() { if(variable_set[109] == 1) { return control_e36_variable; } else { return control_e36_default(); } }
void set_control_e36(ExcelValue newValue) { variable_set[109] = 1; control_e36_variable = newValue; }

ExcelValue control_e37_default() {
  return constant7;
}
static ExcelValue control_e37_variable;
ExcelValue control_e37() { if(variable_set[110] == 1) { return control_e37_variable; } else { return control_e37_default(); } }
void set_control_e37(ExcelValue newValue) { variable_set[110] = 1; control_e37_variable = newValue; }

ExcelValue control_e38_default() {
  return constant7;
}
static ExcelValue control_e38_variable;
ExcelValue control_e38() { if(variable_set[111] == 1) { return control_e38_variable; } else { return control_e38_default(); } }
void set_control_e38(ExcelValue newValue) { variable_set[111] = 1; control_e38_variable = newValue; }

ExcelValue control_e39_default() {
  return constant7;
}
static ExcelValue control_e39_variable;
ExcelValue control_e39() { if(variable_set[112] == 1) { return control_e39_variable; } else { return control_e39_default(); } }
void set_control_e39(ExcelValue newValue) { variable_set[112] = 1; control_e39_variable = newValue; }

ExcelValue control_e40_default() {
  return constant7;
}
static ExcelValue control_e40_variable;
ExcelValue control_e40() { if(variable_set[113] == 1) { return control_e40_variable; } else { return control_e40_default(); } }
void set_control_e40(ExcelValue newValue) { variable_set[113] = 1; control_e40_variable = newValue; }

ExcelValue control_e41_default() {
  return constant7;
}
static ExcelValue control_e41_variable;
ExcelValue control_e41() { if(variable_set[114] == 1) { return control_e41_variable; } else { return control_e41_default(); } }
void set_control_e41(ExcelValue newValue) { variable_set[114] = 1; control_e41_variable = newValue; }

ExcelValue control_e42_default() {
  return constant8;
}
static ExcelValue control_e42_variable;
ExcelValue control_e42() { if(variable_set[115] == 1) { return control_e42_variable; } else { return control_e42_default(); } }
void set_control_e42(ExcelValue newValue) { variable_set[115] = 1; control_e42_variable = newValue; }

ExcelValue control_e43_default() {
  return constant7;
}
static ExcelValue control_e43_variable;
ExcelValue control_e43() { if(variable_set[116] == 1) { return control_e43_variable; } else { return control_e43_default(); } }
void set_control_e43(ExcelValue newValue) { variable_set[116] = 1; control_e43_variable = newValue; }

ExcelValue control_e44_default() {
  return constant7;
}
static ExcelValue control_e44_variable;
ExcelValue control_e44() { if(variable_set[117] == 1) { return control_e44_variable; } else { return control_e44_default(); } }
void set_control_e44(ExcelValue newValue) { variable_set[117] = 1; control_e44_variable = newValue; }

ExcelValue control_e45_default() {
  return constant7;
}
static ExcelValue control_e45_variable;
ExcelValue control_e45() { if(variable_set[118] == 1) { return control_e45_variable; } else { return control_e45_default(); } }
void set_control_e45(ExcelValue newValue) { variable_set[118] = 1; control_e45_variable = newValue; }

ExcelValue control_e46_default() {
  return constant7;
}
static ExcelValue control_e46_variable;
ExcelValue control_e46() { if(variable_set[119] == 1) { return control_e46_variable; } else { return control_e46_default(); } }
void set_control_e46(ExcelValue newValue) { variable_set[119] = 1; control_e46_variable = newValue; }

ExcelValue control_e47_default() {
  return constant3;
}
static ExcelValue control_e47_variable;
ExcelValue control_e47() { if(variable_set[120] == 1) { return control_e47_variable; } else { return control_e47_default(); } }
void set_control_e47(ExcelValue newValue) { variable_set[120] = 1; control_e47_variable = newValue; }

ExcelValue control_e48_default() {
  return constant8;
}
static ExcelValue control_e48_variable;
ExcelValue control_e48() { if(variable_set[121] == 1) { return control_e48_variable; } else { return control_e48_default(); } }
void set_control_e48(ExcelValue newValue) { variable_set[121] = 1; control_e48_variable = newValue; }

ExcelValue control_e49_default() {
  return constant9;
}
static ExcelValue control_e49_variable;
ExcelValue control_e49() { if(variable_set[122] == 1) { return control_e49_variable; } else { return control_e49_default(); } }
void set_control_e49(ExcelValue newValue) { variable_set[122] = 1; control_e49_variable = newValue; }

ExcelValue control_e50_default() {
  return constant7;
}
static ExcelValue control_e50_variable;
ExcelValue control_e50() { if(variable_set[123] == 1) { return control_e50_variable; } else { return control_e50_default(); } }
void set_control_e50(ExcelValue newValue) { variable_set[123] = 1; control_e50_variable = newValue; }

ExcelValue control_e51_default() {
  return constant7;
}
static ExcelValue control_e51_variable;
ExcelValue control_e51() { if(variable_set[124] == 1) { return control_e51_variable; } else { return control_e51_default(); } }
void set_control_e51(ExcelValue newValue) { variable_set[124] = 1; control_e51_variable = newValue; }

ExcelValue control_e52_default() {
  return constant7;
}
static ExcelValue control_e52_variable;
ExcelValue control_e52() { if(variable_set[125] == 1) { return control_e52_variable; } else { return control_e52_default(); } }
void set_control_e52(ExcelValue newValue) { variable_set[125] = 1; control_e52_variable = newValue; }

ExcelValue control_e53_default() {
  return constant7;
}
static ExcelValue control_e53_variable;
ExcelValue control_e53() { if(variable_set[126] == 1) { return control_e53_variable; } else { return control_e53_default(); } }
void set_control_e53(ExcelValue newValue) { variable_set[126] = 1; control_e53_variable = newValue; }

ExcelValue control_e54_default() {
  return constant7;
}
static ExcelValue control_e54_variable;
ExcelValue control_e54() { if(variable_set[127] == 1) { return control_e54_variable; } else { return control_e54_default(); } }
void set_control_e54(ExcelValue newValue) { variable_set[127] = 1; control_e54_variable = newValue; }

ExcelValue control_e55_default() {
  return constant7;
}
static ExcelValue control_e55_variable;
ExcelValue control_e55() { if(variable_set[128] == 1) { return control_e55_variable; } else { return control_e55_default(); } }
void set_control_e55(ExcelValue newValue) { variable_set[128] = 1; control_e55_variable = newValue; }

ExcelValue control_e56_default() {
  return constant7;
}
static ExcelValue control_e56_variable;
ExcelValue control_e56() { if(variable_set[129] == 1) { return control_e56_variable; } else { return control_e56_default(); } }
void set_control_e56(ExcelValue newValue) { variable_set[129] = 1; control_e56_variable = newValue; }

ExcelValue control_e57_default() {
  return constant7;
}
static ExcelValue control_e57_variable;
ExcelValue control_e57() { if(variable_set[130] == 1) { return control_e57_variable; } else { return control_e57_default(); } }
void set_control_e57(ExcelValue newValue) { variable_set[130] = 1; control_e57_variable = newValue; }

ExcelValue control_e58_default() {
  return constant7;
}
static ExcelValue control_e58_variable;
ExcelValue control_e58() { if(variable_set[131] == 1) { return control_e58_variable; } else { return control_e58_default(); } }
void set_control_e58(ExcelValue newValue) { variable_set[131] = 1; control_e58_variable = newValue; }

ExcelValue control_e59_default() {
  return constant7;
}
static ExcelValue control_e59_variable;
ExcelValue control_e59() { if(variable_set[132] == 1) { return control_e59_variable; } else { return control_e59_default(); } }
void set_control_e59(ExcelValue newValue) { variable_set[132] = 1; control_e59_variable = newValue; }

ExcelValue control_e60_default() {
  return constant7;
}
static ExcelValue control_e60_variable;
ExcelValue control_e60() { if(variable_set[133] == 1) { return control_e60_variable; } else { return control_e60_default(); } }
void set_control_e60(ExcelValue newValue) { variable_set[133] = 1; control_e60_variable = newValue; }

ExcelValue control_e61_default() {
  return constant7;
}
static ExcelValue control_e61_variable;
ExcelValue control_e61() { if(variable_set[134] == 1) { return control_e61_variable; } else { return control_e61_default(); } }
void set_control_e61(ExcelValue newValue) { variable_set[134] = 1; control_e61_variable = newValue; }

ExcelValue control_e62_default() {
  return BLANK;
}
static ExcelValue control_e62_variable;
ExcelValue control_e62() { if(variable_set[135] == 1) { return control_e62_variable; } else { return control_e62_default(); } }
void set_control_e62(ExcelValue newValue) { variable_set[135] = 1; control_e62_variable = newValue; }

ExcelValue control_i16() { return constant10; }
ExcelValue control_i17() {
  static ExcelValue result;
  if(variable_set[137] == 1) { return result;}
  result = ONE;
  variable_set[137] = 1;
  return result;
}

ExcelValue control_i18() {
  static ExcelValue result;
  if(variable_set[138] == 1) { return result;}
  result = ONE;
  variable_set[138] = 1;
  return result;
}

ExcelValue control_i19() {
  static ExcelValue result;
  if(variable_set[139] == 1) { return result;}
  result = ONE;
  variable_set[139] = 1;
  return result;
}

ExcelValue control_i20() {
  static ExcelValue result;
  if(variable_set[140] == 1) { return result;}
  result = ONE;
  variable_set[140] = 1;
  return result;
}

ExcelValue control_i21() {
  static ExcelValue result;
  if(variable_set[141] == 1) { return result;}
  result = ONE;
  variable_set[141] = 1;
  return result;
}

ExcelValue control_i22() {
  static ExcelValue result;
  if(variable_set[142] == 1) { return result;}
  result = ONE;
  variable_set[142] = 1;
  return result;
}

ExcelValue control_i23() {
  static ExcelValue result;
  if(variable_set[143] == 1) { return result;}
  result = ONE;
  variable_set[143] = 1;
  return result;
}

ExcelValue control_i24() {
  static ExcelValue result;
  if(variable_set[144] == 1) { return result;}
  result = ONE;
  variable_set[144] = 1;
  return result;
}

ExcelValue control_i25() {
  static ExcelValue result;
  if(variable_set[145] == 1) { return result;}
  result = ONE;
  variable_set[145] = 1;
  return result;
}

ExcelValue control_i26() {
  static ExcelValue result;
  if(variable_set[146] == 1) { return result;}
  result = ONE;
  variable_set[146] = 1;
  return result;
}

ExcelValue control_i27() {
  static ExcelValue result;
  if(variable_set[147] == 1) { return result;}
  result = ONE;
  variable_set[147] = 1;
  return result;
}

ExcelValue control_i28() {
  static ExcelValue result;
  if(variable_set[148] == 1) { return result;}
  result = ONE;
  variable_set[148] = 1;
  return result;
}

ExcelValue control_i29() {
  static ExcelValue result;
  if(variable_set[149] == 1) { return result;}
  result = ONE;
  variable_set[149] = 1;
  return result;
}

ExcelValue control_i30() {
  static ExcelValue result;
  if(variable_set[150] == 1) { return result;}
  result = ONE;
  variable_set[150] = 1;
  return result;
}

ExcelValue control_i31() {
  static ExcelValue result;
  if(variable_set[151] == 1) { return result;}
  result = ONE;
  variable_set[151] = 1;
  return result;
}

ExcelValue control_i32() {
  static ExcelValue result;
  if(variable_set[152] == 1) { return result;}
  result = ONE;
  variable_set[152] = 1;
  return result;
}

ExcelValue control_i33() {
  static ExcelValue result;
  if(variable_set[153] == 1) { return result;}
  result = ONE;
  variable_set[153] = 1;
  return result;
}

ExcelValue control_i34() {
  static ExcelValue result;
  if(variable_set[154] == 1) { return result;}
  result = ONE;
  variable_set[154] = 1;
  return result;
}

ExcelValue control_i35() {
  static ExcelValue result;
  if(variable_set[155] == 1) { return result;}
  result = ONE;
  variable_set[155] = 1;
  return result;
}

ExcelValue control_i36() {
  static ExcelValue result;
  if(variable_set[156] == 1) { return result;}
  result = ONE;
  variable_set[156] = 1;
  return result;
}

ExcelValue control_i37() {
  static ExcelValue result;
  if(variable_set[157] == 1) { return result;}
  result = ONE;
  variable_set[157] = 1;
  return result;
}

ExcelValue control_i38() {
  static ExcelValue result;
  if(variable_set[158] == 1) { return result;}
  result = ONE;
  variable_set[158] = 1;
  return result;
}

ExcelValue control_i39() {
  static ExcelValue result;
  if(variable_set[159] == 1) { return result;}
  result = ONE;
  variable_set[159] = 1;
  return result;
}

ExcelValue control_i40() {
  static ExcelValue result;
  if(variable_set[160] == 1) { return result;}
  result = ONE;
  variable_set[160] = 1;
  return result;
}

ExcelValue control_i41() {
  static ExcelValue result;
  if(variable_set[161] == 1) { return result;}
  result = ONE;
  variable_set[161] = 1;
  return result;
}

ExcelValue control_i42() {
  static ExcelValue result;
  if(variable_set[162] == 1) { return result;}
  result = ONE;
  variable_set[162] = 1;
  return result;
}

ExcelValue control_i43() {
  static ExcelValue result;
  if(variable_set[163] == 1) { return result;}
  result = ONE;
  variable_set[163] = 1;
  return result;
}

ExcelValue control_i44() {
  static ExcelValue result;
  if(variable_set[164] == 1) { return result;}
  result = ONE;
  variable_set[164] = 1;
  return result;
}

ExcelValue control_i45() {
  static ExcelValue result;
  if(variable_set[165] == 1) { return result;}
  result = ONE;
  variable_set[165] = 1;
  return result;
}

ExcelValue control_i46() {
  static ExcelValue result;
  if(variable_set[166] == 1) { return result;}
  result = ONE;
  variable_set[166] = 1;
  return result;
}

ExcelValue control_i47() {
  static ExcelValue result;
  if(variable_set[167] == 1) { return result;}
  result = ONE;
  variable_set[167] = 1;
  return result;
}

ExcelValue control_i48() {
  static ExcelValue result;
  if(variable_set[168] == 1) { return result;}
  result = ONE;
  variable_set[168] = 1;
  return result;
}

ExcelValue control_i49() {
  static ExcelValue result;
  if(variable_set[169] == 1) { return result;}
  result = ONE;
  variable_set[169] = 1;
  return result;
}

ExcelValue control_i50() {
  static ExcelValue result;
  if(variable_set[170] == 1) { return result;}
  result = ONE;
  variable_set[170] = 1;
  return result;
}

ExcelValue control_i51() {
  static ExcelValue result;
  if(variable_set[171] == 1) { return result;}
  result = ONE;
  variable_set[171] = 1;
  return result;
}

ExcelValue control_i52() {
  static ExcelValue result;
  if(variable_set[172] == 1) { return result;}
  result = ONE;
  variable_set[172] = 1;
  return result;
}

ExcelValue control_i53() {
  static ExcelValue result;
  if(variable_set[173] == 1) { return result;}
  result = ONE;
  variable_set[173] = 1;
  return result;
}

ExcelValue control_i54() {
  static ExcelValue result;
  if(variable_set[174] == 1) { return result;}
  result = ONE;
  variable_set[174] = 1;
  return result;
}

ExcelValue control_i55() {
  static ExcelValue result;
  if(variable_set[175] == 1) { return result;}
  result = ONE;
  variable_set[175] = 1;
  return result;
}

ExcelValue control_i56() {
  static ExcelValue result;
  if(variable_set[176] == 1) { return result;}
  result = ONE;
  variable_set[176] = 1;
  return result;
}

ExcelValue control_i57() {
  static ExcelValue result;
  if(variable_set[177] == 1) { return result;}
  result = ONE;
  variable_set[177] = 1;
  return result;
}

ExcelValue control_i58() {
  static ExcelValue result;
  if(variable_set[178] == 1) { return result;}
  result = ONE;
  variable_set[178] = 1;
  return result;
}

ExcelValue control_i59() {
  static ExcelValue result;
  if(variable_set[179] == 1) { return result;}
  result = ONE;
  variable_set[179] = 1;
  return result;
}

ExcelValue control_i60() {
  static ExcelValue result;
  if(variable_set[180] == 1) { return result;}
  result = ONE;
  variable_set[180] = 1;
  return result;
}

ExcelValue control_i61() {
  static ExcelValue result;
  if(variable_set[181] == 1) { return result;}
  result = ONE;
  variable_set[181] = 1;
  return result;
}

ExcelValue control_i62() { return constant11; }
ExcelValue control_i63() { return constant12; }
ExcelValue control_i64() { return constant13; }
ExcelValue control_ef17() {
  static ExcelValue result;
  if(variable_set[185] == 1) { return result;}
  result = ONE;
  variable_set[185] = 1;
  return result;
}

ExcelValue control_eg17() {
  static ExcelValue result;
  if(variable_set[186] == 1) { return result;}
  result = TWO;
  variable_set[186] = 1;
  return result;
}

ExcelValue control_eh17() {
  static ExcelValue result;
  if(variable_set[187] == 1) { return result;}
  result = THREE;
  variable_set[187] = 1;
  return result;
}

ExcelValue control_ei17() {
  static ExcelValue result;
  if(variable_set[188] == 1) { return result;}
  result = FOUR;
  variable_set[188] = 1;
  return result;
}

ExcelValue control_ef18() {
  static ExcelValue result;
  if(variable_set[189] == 1) { return result;}
  result = ONE;
  variable_set[189] = 1;
  return result;
}

ExcelValue control_eg18() {
  static ExcelValue result;
  if(variable_set[190] == 1) { return result;}
  result = TWO;
  variable_set[190] = 1;
  return result;
}

ExcelValue control_eh18() {
  static ExcelValue result;
  if(variable_set[191] == 1) { return result;}
  result = THREE;
  variable_set[191] = 1;
  return result;
}

ExcelValue control_ei18() {
  static ExcelValue result;
  if(variable_set[192] == 1) { return result;}
  result = FOUR;
  variable_set[192] = 1;
  return result;
}

ExcelValue control_ef19() {
  static ExcelValue result;
  if(variable_set[193] == 1) { return result;}
  result = ONE;
  variable_set[193] = 1;
  return result;
}

ExcelValue control_eg19() {
  static ExcelValue result;
  if(variable_set[194] == 1) { return result;}
  result = TWO;
  variable_set[194] = 1;
  return result;
}

ExcelValue control_eh19() {
  static ExcelValue result;
  if(variable_set[195] == 1) { return result;}
  result = THREE;
  variable_set[195] = 1;
  return result;
}

ExcelValue control_ei19() {
  static ExcelValue result;
  if(variable_set[196] == 1) { return result;}
  result = FOUR;
  variable_set[196] = 1;
  return result;
}

ExcelValue control_ef20() {
  static ExcelValue result;
  if(variable_set[197] == 1) { return result;}
  result = ONE;
  variable_set[197] = 1;
  return result;
}

ExcelValue control_eg20() {
  static ExcelValue result;
  if(variable_set[198] == 1) { return result;}
  result = TWO;
  variable_set[198] = 1;
  return result;
}

ExcelValue control_eh20() {
  static ExcelValue result;
  if(variable_set[199] == 1) { return result;}
  result = THREE;
  variable_set[199] = 1;
  return result;
}

ExcelValue control_ei20() {
  static ExcelValue result;
  if(variable_set[200] == 1) { return result;}
  result = FOUR;
  variable_set[200] = 1;
  return result;
}

ExcelValue control_ef21() {
  static ExcelValue result;
  if(variable_set[201] == 1) { return result;}
  result = ONE;
  variable_set[201] = 1;
  return result;
}

ExcelValue control_eg21() {
  static ExcelValue result;
  if(variable_set[202] == 1) { return result;}
  result = TWO;
  variable_set[202] = 1;
  return result;
}

ExcelValue control_eh21() {
  static ExcelValue result;
  if(variable_set[203] == 1) { return result;}
  result = THREE;
  variable_set[203] = 1;
  return result;
}

ExcelValue control_ei21() {
  static ExcelValue result;
  if(variable_set[204] == 1) { return result;}
  result = FOUR;
  variable_set[204] = 1;
  return result;
}

ExcelValue control_ef22() {
  static ExcelValue result;
  if(variable_set[205] == 1) { return result;}
  result = ONE;
  variable_set[205] = 1;
  return result;
}

ExcelValue control_eg22() {
  static ExcelValue result;
  if(variable_set[206] == 1) { return result;}
  result = TWO;
  variable_set[206] = 1;
  return result;
}

ExcelValue control_eh22() {
  static ExcelValue result;
  if(variable_set[207] == 1) { return result;}
  result = THREE;
  variable_set[207] = 1;
  return result;
}

ExcelValue control_ei22() {
  static ExcelValue result;
  if(variable_set[208] == 1) { return result;}
  result = FOUR;
  variable_set[208] = 1;
  return result;
}

ExcelValue control_ef23() {
  static ExcelValue result;
  if(variable_set[209] == 1) { return result;}
  result = ONE;
  variable_set[209] = 1;
  return result;
}

ExcelValue control_eg23() {
  static ExcelValue result;
  if(variable_set[210] == 1) { return result;}
  result = TWO;
  variable_set[210] = 1;
  return result;
}

ExcelValue control_eh23() {
  static ExcelValue result;
  if(variable_set[211] == 1) { return result;}
  result = THREE;
  variable_set[211] = 1;
  return result;
}

ExcelValue control_ei23() {
  static ExcelValue result;
  if(variable_set[212] == 1) { return result;}
  result = FOUR;
  variable_set[212] = 1;
  return result;
}

ExcelValue control_ef24() {
  static ExcelValue result;
  if(variable_set[213] == 1) { return result;}
  result = ONE;
  variable_set[213] = 1;
  return result;
}

ExcelValue control_eg24() {
  static ExcelValue result;
  if(variable_set[214] == 1) { return result;}
  result = TWO;
  variable_set[214] = 1;
  return result;
}

ExcelValue control_eh24() {
  static ExcelValue result;
  if(variable_set[215] == 1) { return result;}
  result = THREE;
  variable_set[215] = 1;
  return result;
}

ExcelValue control_ei24() {
  static ExcelValue result;
  if(variable_set[216] == 1) { return result;}
  result = FOUR;
  variable_set[216] = 1;
  return result;
}

ExcelValue control_ef25() {
  static ExcelValue result;
  if(variable_set[217] == 1) { return result;}
  result = ONE;
  variable_set[217] = 1;
  return result;
}

ExcelValue control_eg25() {
  static ExcelValue result;
  if(variable_set[218] == 1) { return result;}
  result = TWO;
  variable_set[218] = 1;
  return result;
}

ExcelValue control_eh25() {
  static ExcelValue result;
  if(variable_set[219] == 1) { return result;}
  result = THREE;
  variable_set[219] = 1;
  return result;
}

ExcelValue control_ei25() {
  static ExcelValue result;
  if(variable_set[220] == 1) { return result;}
  result = FOUR;
  variable_set[220] = 1;
  return result;
}

ExcelValue control_ef26() {
  static ExcelValue result;
  if(variable_set[221] == 1) { return result;}
  result = ONE;
  variable_set[221] = 1;
  return result;
}

ExcelValue control_eg26() {
  static ExcelValue result;
  if(variable_set[222] == 1) { return result;}
  result = TWO;
  variable_set[222] = 1;
  return result;
}

ExcelValue control_eh26() {
  static ExcelValue result;
  if(variable_set[223] == 1) { return result;}
  result = THREE;
  variable_set[223] = 1;
  return result;
}

ExcelValue control_ei26() {
  static ExcelValue result;
  if(variable_set[224] == 1) { return result;}
  result = FOUR;
  variable_set[224] = 1;
  return result;
}

ExcelValue control_ef27() {
  static ExcelValue result;
  if(variable_set[225] == 1) { return result;}
  result = ONE;
  variable_set[225] = 1;
  return result;
}

ExcelValue control_eg27() {
  static ExcelValue result;
  if(variable_set[226] == 1) { return result;}
  result = TWO;
  variable_set[226] = 1;
  return result;
}

ExcelValue control_eh27() {
  static ExcelValue result;
  if(variable_set[227] == 1) { return result;}
  result = THREE;
  variable_set[227] = 1;
  return result;
}

ExcelValue control_ei27() {
  static ExcelValue result;
  if(variable_set[228] == 1) { return result;}
  result = FOUR;
  variable_set[228] = 1;
  return result;
}

ExcelValue control_ef28() {
  static ExcelValue result;
  if(variable_set[229] == 1) { return result;}
  result = ONE;
  variable_set[229] = 1;
  return result;
}

ExcelValue control_eg28() {
  static ExcelValue result;
  if(variable_set[230] == 1) { return result;}
  result = TWO;
  variable_set[230] = 1;
  return result;
}

ExcelValue control_eh28() {
  static ExcelValue result;
  if(variable_set[231] == 1) { return result;}
  result = THREE;
  variable_set[231] = 1;
  return result;
}

ExcelValue control_ei28() {
  static ExcelValue result;
  if(variable_set[232] == 1) { return result;}
  result = FOUR;
  variable_set[232] = 1;
  return result;
}

ExcelValue control_ef29() {
  static ExcelValue result;
  if(variable_set[233] == 1) { return result;}
  result = ONE;
  variable_set[233] = 1;
  return result;
}

ExcelValue control_eg29() {
  static ExcelValue result;
  if(variable_set[234] == 1) { return result;}
  result = TWO;
  variable_set[234] = 1;
  return result;
}

ExcelValue control_eh29() {
  static ExcelValue result;
  if(variable_set[235] == 1) { return result;}
  result = THREE;
  variable_set[235] = 1;
  return result;
}

ExcelValue control_ei29() {
  static ExcelValue result;
  if(variable_set[236] == 1) { return result;}
  result = FOUR;
  variable_set[236] = 1;
  return result;
}

ExcelValue control_ef30() {
  static ExcelValue result;
  if(variable_set[237] == 1) { return result;}
  result = ONE;
  variable_set[237] = 1;
  return result;
}

ExcelValue control_eg30() {
  static ExcelValue result;
  if(variable_set[238] == 1) { return result;}
  result = TWO;
  variable_set[238] = 1;
  return result;
}

ExcelValue control_eh30() {
  static ExcelValue result;
  if(variable_set[239] == 1) { return result;}
  result = THREE;
  variable_set[239] = 1;
  return result;
}

ExcelValue control_ei30() {
  static ExcelValue result;
  if(variable_set[240] == 1) { return result;}
  result = FOUR;
  variable_set[240] = 1;
  return result;
}

ExcelValue control_ef31() {
  static ExcelValue result;
  if(variable_set[241] == 1) { return result;}
  result = ONE;
  variable_set[241] = 1;
  return result;
}

ExcelValue control_eg31() {
  static ExcelValue result;
  if(variable_set[242] == 1) { return result;}
  result = TWO;
  variable_set[242] = 1;
  return result;
}

ExcelValue control_eh31() {
  static ExcelValue result;
  if(variable_set[243] == 1) { return result;}
  result = THREE;
  variable_set[243] = 1;
  return result;
}

ExcelValue control_ei31() {
  static ExcelValue result;
  if(variable_set[244] == 1) { return result;}
  result = FOUR;
  variable_set[244] = 1;
  return result;
}

ExcelValue control_ef32() {
  static ExcelValue result;
  if(variable_set[245] == 1) { return result;}
  result = ONE;
  variable_set[245] = 1;
  return result;
}

ExcelValue control_eg32() {
  static ExcelValue result;
  if(variable_set[246] == 1) { return result;}
  result = TWO;
  variable_set[246] = 1;
  return result;
}

ExcelValue control_eh32() {
  static ExcelValue result;
  if(variable_set[247] == 1) { return result;}
  result = THREE;
  variable_set[247] = 1;
  return result;
}

ExcelValue control_ei32() {
  static ExcelValue result;
  if(variable_set[248] == 1) { return result;}
  result = FOUR;
  variable_set[248] = 1;
  return result;
}

ExcelValue control_ef33() {
  static ExcelValue result;
  if(variable_set[249] == 1) { return result;}
  result = ONE;
  variable_set[249] = 1;
  return result;
}

ExcelValue control_eg33() {
  static ExcelValue result;
  if(variable_set[250] == 1) { return result;}
  result = TWO;
  variable_set[250] = 1;
  return result;
}

ExcelValue control_eh33() {
  static ExcelValue result;
  if(variable_set[251] == 1) { return result;}
  result = THREE;
  variable_set[251] = 1;
  return result;
}

ExcelValue control_ei33() {
  static ExcelValue result;
  if(variable_set[252] == 1) { return result;}
  result = FOUR;
  variable_set[252] = 1;
  return result;
}

ExcelValue control_ef34() {
  static ExcelValue result;
  if(variable_set[253] == 1) { return result;}
  result = ONE;
  variable_set[253] = 1;
  return result;
}

ExcelValue control_eg34() {
  static ExcelValue result;
  if(variable_set[254] == 1) { return result;}
  result = TWO;
  variable_set[254] = 1;
  return result;
}

ExcelValue control_eh34() {
  static ExcelValue result;
  if(variable_set[255] == 1) { return result;}
  result = THREE;
  variable_set[255] = 1;
  return result;
}

ExcelValue control_ei34() {
  static ExcelValue result;
  if(variable_set[256] == 1) { return result;}
  result = FOUR;
  variable_set[256] = 1;
  return result;
}

ExcelValue control_ef35() {
  static ExcelValue result;
  if(variable_set[257] == 1) { return result;}
  result = ONE;
  variable_set[257] = 1;
  return result;
}

ExcelValue control_eg35() {
  static ExcelValue result;
  if(variable_set[258] == 1) { return result;}
  result = TWO;
  variable_set[258] = 1;
  return result;
}

ExcelValue control_eh35() {
  static ExcelValue result;
  if(variable_set[259] == 1) { return result;}
  result = THREE;
  variable_set[259] = 1;
  return result;
}

ExcelValue control_ei35() {
  static ExcelValue result;
  if(variable_set[260] == 1) { return result;}
  result = FOUR;
  variable_set[260] = 1;
  return result;
}

ExcelValue control_ef36() {
  static ExcelValue result;
  if(variable_set[261] == 1) { return result;}
  result = ONE;
  variable_set[261] = 1;
  return result;
}

ExcelValue control_eg36() {
  static ExcelValue result;
  if(variable_set[262] == 1) { return result;}
  result = TWO;
  variable_set[262] = 1;
  return result;
}

ExcelValue control_eh36() {
  static ExcelValue result;
  if(variable_set[263] == 1) { return result;}
  result = THREE;
  variable_set[263] = 1;
  return result;
}

ExcelValue control_ei36() {
  static ExcelValue result;
  if(variable_set[264] == 1) { return result;}
  result = FOUR;
  variable_set[264] = 1;
  return result;
}

ExcelValue control_ef37() {
  static ExcelValue result;
  if(variable_set[265] == 1) { return result;}
  result = ONE;
  variable_set[265] = 1;
  return result;
}

ExcelValue control_eg37() {
  static ExcelValue result;
  if(variable_set[266] == 1) { return result;}
  result = TWO;
  variable_set[266] = 1;
  return result;
}

ExcelValue control_eh37() {
  static ExcelValue result;
  if(variable_set[267] == 1) { return result;}
  result = THREE;
  variable_set[267] = 1;
  return result;
}

ExcelValue control_ei37() {
  static ExcelValue result;
  if(variable_set[268] == 1) { return result;}
  result = FOUR;
  variable_set[268] = 1;
  return result;
}

ExcelValue control_ef38() {
  static ExcelValue result;
  if(variable_set[269] == 1) { return result;}
  result = ONE;
  variable_set[269] = 1;
  return result;
}

ExcelValue control_eg38() {
  static ExcelValue result;
  if(variable_set[270] == 1) { return result;}
  result = TWO;
  variable_set[270] = 1;
  return result;
}

ExcelValue control_eh38() {
  static ExcelValue result;
  if(variable_set[271] == 1) { return result;}
  result = THREE;
  variable_set[271] = 1;
  return result;
}

ExcelValue control_ei38() {
  static ExcelValue result;
  if(variable_set[272] == 1) { return result;}
  result = FOUR;
  variable_set[272] = 1;
  return result;
}

ExcelValue control_ef39() {
  static ExcelValue result;
  if(variable_set[273] == 1) { return result;}
  result = ONE;
  variable_set[273] = 1;
  return result;
}

ExcelValue control_eg39() {
  static ExcelValue result;
  if(variable_set[274] == 1) { return result;}
  result = TWO;
  variable_set[274] = 1;
  return result;
}

ExcelValue control_eh39() {
  static ExcelValue result;
  if(variable_set[275] == 1) { return result;}
  result = THREE;
  variable_set[275] = 1;
  return result;
}

ExcelValue control_ei39() {
  static ExcelValue result;
  if(variable_set[276] == 1) { return result;}
  result = FOUR;
  variable_set[276] = 1;
  return result;
}

ExcelValue control_ef40() {
  static ExcelValue result;
  if(variable_set[277] == 1) { return result;}
  result = ONE;
  variable_set[277] = 1;
  return result;
}

ExcelValue control_eg40() {
  static ExcelValue result;
  if(variable_set[278] == 1) { return result;}
  result = TWO;
  variable_set[278] = 1;
  return result;
}

ExcelValue control_eh40() {
  static ExcelValue result;
  if(variable_set[279] == 1) { return result;}
  result = THREE;
  variable_set[279] = 1;
  return result;
}

ExcelValue control_ei40() {
  static ExcelValue result;
  if(variable_set[280] == 1) { return result;}
  result = FOUR;
  variable_set[280] = 1;
  return result;
}

ExcelValue control_ef41() {
  static ExcelValue result;
  if(variable_set[281] == 1) { return result;}
  result = ONE;
  variable_set[281] = 1;
  return result;
}

ExcelValue control_eg41() {
  static ExcelValue result;
  if(variable_set[282] == 1) { return result;}
  result = TWO;
  variable_set[282] = 1;
  return result;
}

ExcelValue control_eh41() {
  static ExcelValue result;
  if(variable_set[283] == 1) { return result;}
  result = THREE;
  variable_set[283] = 1;
  return result;
}

ExcelValue control_ei41() {
  static ExcelValue result;
  if(variable_set[284] == 1) { return result;}
  result = FOUR;
  variable_set[284] = 1;
  return result;
}

ExcelValue control_ef42() {
  static ExcelValue result;
  if(variable_set[285] == 1) { return result;}
  result = ONE;
  variable_set[285] = 1;
  return result;
}

ExcelValue control_eg42() {
  static ExcelValue result;
  if(variable_set[286] == 1) { return result;}
  result = TWO;
  variable_set[286] = 1;
  return result;
}

ExcelValue control_eh42() {
  static ExcelValue result;
  if(variable_set[287] == 1) { return result;}
  result = THREE;
  variable_set[287] = 1;
  return result;
}

ExcelValue control_ei42() {
  static ExcelValue result;
  if(variable_set[288] == 1) { return result;}
  result = FOUR;
  variable_set[288] = 1;
  return result;
}

ExcelValue control_ef43() {
  static ExcelValue result;
  if(variable_set[289] == 1) { return result;}
  result = ONE;
  variable_set[289] = 1;
  return result;
}

ExcelValue control_eg43() {
  static ExcelValue result;
  if(variable_set[290] == 1) { return result;}
  result = TWO;
  variable_set[290] = 1;
  return result;
}

ExcelValue control_eh43() {
  static ExcelValue result;
  if(variable_set[291] == 1) { return result;}
  result = THREE;
  variable_set[291] = 1;
  return result;
}

ExcelValue control_ei43() {
  static ExcelValue result;
  if(variable_set[292] == 1) { return result;}
  result = FOUR;
  variable_set[292] = 1;
  return result;
}

ExcelValue control_ef44() {
  static ExcelValue result;
  if(variable_set[293] == 1) { return result;}
  result = ONE;
  variable_set[293] = 1;
  return result;
}

ExcelValue control_eg44() {
  static ExcelValue result;
  if(variable_set[294] == 1) { return result;}
  result = TWO;
  variable_set[294] = 1;
  return result;
}

ExcelValue control_eh44() {
  static ExcelValue result;
  if(variable_set[295] == 1) { return result;}
  result = THREE;
  variable_set[295] = 1;
  return result;
}

ExcelValue control_ei44() {
  static ExcelValue result;
  if(variable_set[296] == 1) { return result;}
  result = FOUR;
  variable_set[296] = 1;
  return result;
}

ExcelValue control_ef45() {
  static ExcelValue result;
  if(variable_set[297] == 1) { return result;}
  result = ONE;
  variable_set[297] = 1;
  return result;
}

ExcelValue control_eg45() {
  static ExcelValue result;
  if(variable_set[298] == 1) { return result;}
  result = TWO;
  variable_set[298] = 1;
  return result;
}

ExcelValue control_eh45() {
  static ExcelValue result;
  if(variable_set[299] == 1) { return result;}
  result = THREE;
  variable_set[299] = 1;
  return result;
}

ExcelValue control_ei45() {
  static ExcelValue result;
  if(variable_set[300] == 1) { return result;}
  result = FOUR;
  variable_set[300] = 1;
  return result;
}

ExcelValue control_ef46() {
  static ExcelValue result;
  if(variable_set[301] == 1) { return result;}
  result = ONE;
  variable_set[301] = 1;
  return result;
}

ExcelValue control_eg46() {
  static ExcelValue result;
  if(variable_set[302] == 1) { return result;}
  result = TWO;
  variable_set[302] = 1;
  return result;
}

ExcelValue control_eh46() {
  static ExcelValue result;
  if(variable_set[303] == 1) { return result;}
  result = THREE;
  variable_set[303] = 1;
  return result;
}

ExcelValue control_ei46() {
  static ExcelValue result;
  if(variable_set[304] == 1) { return result;}
  result = FOUR;
  variable_set[304] = 1;
  return result;
}

ExcelValue control_ef47() {
  static ExcelValue result;
  if(variable_set[305] == 1) { return result;}
  result = ONE;
  variable_set[305] = 1;
  return result;
}

ExcelValue control_eg47() {
  static ExcelValue result;
  if(variable_set[306] == 1) { return result;}
  result = TWO;
  variable_set[306] = 1;
  return result;
}

ExcelValue control_eh47() {
  static ExcelValue result;
  if(variable_set[307] == 1) { return result;}
  result = THREE;
  variable_set[307] = 1;
  return result;
}

ExcelValue control_ei47() {
  static ExcelValue result;
  if(variable_set[308] == 1) { return result;}
  result = FOUR;
  variable_set[308] = 1;
  return result;
}

ExcelValue control_ef48() {
  static ExcelValue result;
  if(variable_set[309] == 1) { return result;}
  result = ONE;
  variable_set[309] = 1;
  return result;
}

ExcelValue control_eg48() {
  static ExcelValue result;
  if(variable_set[310] == 1) { return result;}
  result = TWO;
  variable_set[310] = 1;
  return result;
}

ExcelValue control_eh48() {
  static ExcelValue result;
  if(variable_set[311] == 1) { return result;}
  result = THREE;
  variable_set[311] = 1;
  return result;
}

ExcelValue control_ei48() {
  static ExcelValue result;
  if(variable_set[312] == 1) { return result;}
  result = FOUR;
  variable_set[312] = 1;
  return result;
}

ExcelValue control_ef49() {
  static ExcelValue result;
  if(variable_set[313] == 1) { return result;}
  result = ONE;
  variable_set[313] = 1;
  return result;
}

ExcelValue control_eg49() {
  static ExcelValue result;
  if(variable_set[314] == 1) { return result;}
  result = TWO;
  variable_set[314] = 1;
  return result;
}

ExcelValue control_eh49() {
  static ExcelValue result;
  if(variable_set[315] == 1) { return result;}
  result = THREE;
  variable_set[315] = 1;
  return result;
}

ExcelValue control_ei49() {
  static ExcelValue result;
  if(variable_set[316] == 1) { return result;}
  result = FOUR;
  variable_set[316] = 1;
  return result;
}

ExcelValue control_ef50() {
  static ExcelValue result;
  if(variable_set[317] == 1) { return result;}
  result = ONE;
  variable_set[317] = 1;
  return result;
}

ExcelValue control_eg50() {
  static ExcelValue result;
  if(variable_set[318] == 1) { return result;}
  result = TWO;
  variable_set[318] = 1;
  return result;
}

ExcelValue control_eh50() {
  static ExcelValue result;
  if(variable_set[319] == 1) { return result;}
  result = THREE;
  variable_set[319] = 1;
  return result;
}

ExcelValue control_ei50() {
  static ExcelValue result;
  if(variable_set[320] == 1) { return result;}
  result = FOUR;
  variable_set[320] = 1;
  return result;
}

ExcelValue control_ef51() {
  static ExcelValue result;
  if(variable_set[321] == 1) { return result;}
  result = ONE;
  variable_set[321] = 1;
  return result;
}

ExcelValue control_eg51() {
  static ExcelValue result;
  if(variable_set[322] == 1) { return result;}
  result = TWO;
  variable_set[322] = 1;
  return result;
}

ExcelValue control_eh51() {
  static ExcelValue result;
  if(variable_set[323] == 1) { return result;}
  result = THREE;
  variable_set[323] = 1;
  return result;
}

ExcelValue control_ei51() {
  static ExcelValue result;
  if(variable_set[324] == 1) { return result;}
  result = FOUR;
  variable_set[324] = 1;
  return result;
}

ExcelValue control_ef52() {
  static ExcelValue result;
  if(variable_set[325] == 1) { return result;}
  result = ONE;
  variable_set[325] = 1;
  return result;
}

ExcelValue control_eg52() {
  static ExcelValue result;
  if(variable_set[326] == 1) { return result;}
  result = TWO;
  variable_set[326] = 1;
  return result;
}

ExcelValue control_eh52() {
  static ExcelValue result;
  if(variable_set[327] == 1) { return result;}
  result = THREE;
  variable_set[327] = 1;
  return result;
}

ExcelValue control_ei52() {
  static ExcelValue result;
  if(variable_set[328] == 1) { return result;}
  result = FOUR;
  variable_set[328] = 1;
  return result;
}

ExcelValue control_ef53() {
  static ExcelValue result;
  if(variable_set[329] == 1) { return result;}
  result = ONE;
  variable_set[329] = 1;
  return result;
}

ExcelValue control_eg53() {
  static ExcelValue result;
  if(variable_set[330] == 1) { return result;}
  result = TWO;
  variable_set[330] = 1;
  return result;
}

ExcelValue control_eh53() {
  static ExcelValue result;
  if(variable_set[331] == 1) { return result;}
  result = THREE;
  variable_set[331] = 1;
  return result;
}

ExcelValue control_ei53() {
  static ExcelValue result;
  if(variable_set[332] == 1) { return result;}
  result = FOUR;
  variable_set[332] = 1;
  return result;
}

ExcelValue control_ef54() {
  static ExcelValue result;
  if(variable_set[333] == 1) { return result;}
  result = ONE;
  variable_set[333] = 1;
  return result;
}

ExcelValue control_eg54() {
  static ExcelValue result;
  if(variable_set[334] == 1) { return result;}
  result = TWO;
  variable_set[334] = 1;
  return result;
}

ExcelValue control_eh54() {
  static ExcelValue result;
  if(variable_set[335] == 1) { return result;}
  result = THREE;
  variable_set[335] = 1;
  return result;
}

ExcelValue control_ei54() {
  static ExcelValue result;
  if(variable_set[336] == 1) { return result;}
  result = FOUR;
  variable_set[336] = 1;
  return result;
}

ExcelValue control_ef55() {
  static ExcelValue result;
  if(variable_set[337] == 1) { return result;}
  result = ONE;
  variable_set[337] = 1;
  return result;
}

ExcelValue control_eg55() {
  static ExcelValue result;
  if(variable_set[338] == 1) { return result;}
  result = TWO;
  variable_set[338] = 1;
  return result;
}

ExcelValue control_eh55() {
  static ExcelValue result;
  if(variable_set[339] == 1) { return result;}
  result = THREE;
  variable_set[339] = 1;
  return result;
}

ExcelValue control_ei55() {
  static ExcelValue result;
  if(variable_set[340] == 1) { return result;}
  result = FOUR;
  variable_set[340] = 1;
  return result;
}

ExcelValue control_ef56() {
  static ExcelValue result;
  if(variable_set[341] == 1) { return result;}
  result = ONE;
  variable_set[341] = 1;
  return result;
}

ExcelValue control_eg56() {
  static ExcelValue result;
  if(variable_set[342] == 1) { return result;}
  result = TWO;
  variable_set[342] = 1;
  return result;
}

ExcelValue control_eh56() {
  static ExcelValue result;
  if(variable_set[343] == 1) { return result;}
  result = THREE;
  variable_set[343] = 1;
  return result;
}

ExcelValue control_ei56() {
  static ExcelValue result;
  if(variable_set[344] == 1) { return result;}
  result = FOUR;
  variable_set[344] = 1;
  return result;
}

ExcelValue control_ef57() {
  static ExcelValue result;
  if(variable_set[345] == 1) { return result;}
  result = ONE;
  variable_set[345] = 1;
  return result;
}

ExcelValue control_eg57() {
  static ExcelValue result;
  if(variable_set[346] == 1) { return result;}
  result = TWO;
  variable_set[346] = 1;
  return result;
}

ExcelValue control_eh57() {
  static ExcelValue result;
  if(variable_set[347] == 1) { return result;}
  result = THREE;
  variable_set[347] = 1;
  return result;
}

ExcelValue control_ei57() {
  static ExcelValue result;
  if(variable_set[348] == 1) { return result;}
  result = FOUR;
  variable_set[348] = 1;
  return result;
}

ExcelValue control_ef58() {
  static ExcelValue result;
  if(variable_set[349] == 1) { return result;}
  result = ONE;
  variable_set[349] = 1;
  return result;
}

ExcelValue control_eg58() {
  static ExcelValue result;
  if(variable_set[350] == 1) { return result;}
  result = TWO;
  variable_set[350] = 1;
  return result;
}

ExcelValue control_eh58() {
  static ExcelValue result;
  if(variable_set[351] == 1) { return result;}
  result = THREE;
  variable_set[351] = 1;
  return result;
}

ExcelValue control_ei58() {
  static ExcelValue result;
  if(variable_set[352] == 1) { return result;}
  result = FOUR;
  variable_set[352] = 1;
  return result;
}

ExcelValue control_ef59() {
  static ExcelValue result;
  if(variable_set[353] == 1) { return result;}
  result = ONE;
  variable_set[353] = 1;
  return result;
}

ExcelValue control_eg59() {
  static ExcelValue result;
  if(variable_set[354] == 1) { return result;}
  result = TWO;
  variable_set[354] = 1;
  return result;
}

ExcelValue control_eh59() {
  static ExcelValue result;
  if(variable_set[355] == 1) { return result;}
  result = THREE;
  variable_set[355] = 1;
  return result;
}

ExcelValue control_ei59() {
  static ExcelValue result;
  if(variable_set[356] == 1) { return result;}
  result = FOUR;
  variable_set[356] = 1;
  return result;
}

ExcelValue control_ef60() {
  static ExcelValue result;
  if(variable_set[357] == 1) { return result;}
  result = ONE;
  variable_set[357] = 1;
  return result;
}

ExcelValue control_eg60() {
  static ExcelValue result;
  if(variable_set[358] == 1) { return result;}
  result = TWO;
  variable_set[358] = 1;
  return result;
}

ExcelValue control_eh60() {
  static ExcelValue result;
  if(variable_set[359] == 1) { return result;}
  result = THREE;
  variable_set[359] = 1;
  return result;
}

ExcelValue control_ei60() {
  static ExcelValue result;
  if(variable_set[360] == 1) { return result;}
  result = FOUR;
  variable_set[360] = 1;
  return result;
}

ExcelValue control_ef61() {
  static ExcelValue result;
  if(variable_set[361] == 1) { return result;}
  result = ONE;
  variable_set[361] = 1;
  return result;
}

ExcelValue control_eg61() {
  static ExcelValue result;
  if(variable_set[362] == 1) { return result;}
  result = TWO;
  variable_set[362] = 1;
  return result;
}

ExcelValue control_eh61() {
  static ExcelValue result;
  if(variable_set[363] == 1) { return result;}
  result = THREE;
  variable_set[363] = 1;
  return result;
}

ExcelValue control_ei61() {
  static ExcelValue result;
  if(variable_set[364] == 1) { return result;}
  result = FOUR;
  variable_set[364] = 1;
  return result;
}

ExcelValue control_ef62() { return BLANK; }
ExcelValue control_eg62() { return BLANK; }
ExcelValue control_eh62() { return BLANK; }
ExcelValue control_ei62() { return BLANK; }
ExcelValue control_c17() { return constant14; }
ExcelValue control_c18() { return constant15; }
ExcelValue control_c19() { return constant16; }
ExcelValue control_c20() { return constant17; }
ExcelValue control_c21() { return constant18; }
ExcelValue control_c22() { return constant19; }
ExcelValue control_c23() { return constant20; }
ExcelValue control_c24() { return constant21; }
ExcelValue control_c25() { return constant22; }
ExcelValue control_c26() { return constant23; }
ExcelValue control_c27() { return constant24; }
ExcelValue control_c28() { return constant25; }
ExcelValue control_c29() { return constant26; }
ExcelValue control_c30() { return constant27; }
ExcelValue control_c31() { return constant28; }
ExcelValue control_c32() { return constant29; }
ExcelValue control_c33() { return constant30; }
ExcelValue control_c34() { return constant31; }
ExcelValue control_c35() { return constant32; }
ExcelValue control_c36() { return constant33; }
ExcelValue control_c37() { return constant34; }
ExcelValue control_c38() { return constant35; }
ExcelValue control_c39() { return constant36; }
ExcelValue control_c40() { return constant37; }
ExcelValue control_c41() { return constant38; }
ExcelValue control_c42() { return constant39; }
ExcelValue control_c43() { return constant40; }
ExcelValue control_c44() { return constant41; }
ExcelValue control_c45() { return constant42; }
ExcelValue control_c46() { return constant43; }
ExcelValue control_c47() { return constant44; }
ExcelValue control_c48() { return constant45; }
ExcelValue control_c49() { return constant46; }
ExcelValue control_c50() { return constant47; }
ExcelValue control_c51() { return constant48; }
ExcelValue control_c52() { return constant49; }
ExcelValue control_c53() { return constant50; }
ExcelValue control_c54() { return constant51; }
ExcelValue control_c55() { return constant52; }
ExcelValue control_c56() { return constant53; }
ExcelValue control_c57() { return constant54; }
ExcelValue control_c58() { return constant55; }
ExcelValue control_c59() { return constant56; }
ExcelValue control_c60() { return constant57; }
ExcelValue control_c61() { return constant58; }
ExcelValue control_c62() { return BLANK; }
ExcelValue control_h17() {
  static ExcelValue result;
  if(variable_set[415] == 1) { return result;}
  result = FOUR;
  variable_set[415] = 1;
  return result;
}

ExcelValue control_h18() {
  static ExcelValue result;
  if(variable_set[416] == 1) { return result;}
  result = FOUR;
  variable_set[416] = 1;
  return result;
}

ExcelValue control_h19() {
  static ExcelValue result;
  if(variable_set[417] == 1) { return result;}
  result = FOUR;
  variable_set[417] = 1;
  return result;
}

ExcelValue control_h20() {
  static ExcelValue result;
  if(variable_set[418] == 1) { return result;}
  result = FOUR;
  variable_set[418] = 1;
  return result;
}

ExcelValue control_h21() {
  static ExcelValue result;
  if(variable_set[419] == 1) { return result;}
  result = FOUR;
  variable_set[419] = 1;
  return result;
}

ExcelValue control_h22() {
  static ExcelValue result;
  if(variable_set[420] == 1) { return result;}
  result = FOUR;
  variable_set[420] = 1;
  return result;
}

ExcelValue control_h23() {
  static ExcelValue result;
  if(variable_set[421] == 1) { return result;}
  result = FOUR;
  variable_set[421] = 1;
  return result;
}

ExcelValue control_h24() {
  static ExcelValue result;
  if(variable_set[422] == 1) { return result;}
  result = FOUR;
  variable_set[422] = 1;
  return result;
}

ExcelValue control_h25() {
  static ExcelValue result;
  if(variable_set[423] == 1) { return result;}
  result = FOUR;
  variable_set[423] = 1;
  return result;
}

ExcelValue control_h26() {
  static ExcelValue result;
  if(variable_set[424] == 1) { return result;}
  result = FOUR;
  variable_set[424] = 1;
  return result;
}

ExcelValue control_h27() {
  static ExcelValue result;
  if(variable_set[425] == 1) { return result;}
  result = FOUR;
  variable_set[425] = 1;
  return result;
}

ExcelValue control_h28() {
  static ExcelValue result;
  if(variable_set[426] == 1) { return result;}
  result = FOUR;
  variable_set[426] = 1;
  return result;
}

ExcelValue control_h29() {
  static ExcelValue result;
  if(variable_set[427] == 1) { return result;}
  result = FOUR;
  variable_set[427] = 1;
  return result;
}

ExcelValue control_h30() {
  static ExcelValue result;
  if(variable_set[428] == 1) { return result;}
  result = FOUR;
  variable_set[428] = 1;
  return result;
}

ExcelValue control_h31() {
  static ExcelValue result;
  if(variable_set[429] == 1) { return result;}
  result = FOUR;
  variable_set[429] = 1;
  return result;
}

ExcelValue control_h32() {
  static ExcelValue result;
  if(variable_set[430] == 1) { return result;}
  result = FOUR;
  variable_set[430] = 1;
  return result;
}

ExcelValue control_h33() {
  static ExcelValue result;
  if(variable_set[431] == 1) { return result;}
  result = FOUR;
  variable_set[431] = 1;
  return result;
}

ExcelValue control_h34() {
  static ExcelValue result;
  if(variable_set[432] == 1) { return result;}
  result = FOUR;
  variable_set[432] = 1;
  return result;
}

ExcelValue control_h35() {
  static ExcelValue result;
  if(variable_set[433] == 1) { return result;}
  result = FOUR;
  variable_set[433] = 1;
  return result;
}

ExcelValue control_h36() {
  static ExcelValue result;
  if(variable_set[434] == 1) { return result;}
  result = FOUR;
  variable_set[434] = 1;
  return result;
}

ExcelValue control_h37() {
  static ExcelValue result;
  if(variable_set[435] == 1) { return result;}
  result = FOUR;
  variable_set[435] = 1;
  return result;
}

ExcelValue control_h38() {
  static ExcelValue result;
  if(variable_set[436] == 1) { return result;}
  result = FOUR;
  variable_set[436] = 1;
  return result;
}

ExcelValue control_h39() {
  static ExcelValue result;
  if(variable_set[437] == 1) { return result;}
  result = FOUR;
  variable_set[437] = 1;
  return result;
}

ExcelValue control_h40() {
  static ExcelValue result;
  if(variable_set[438] == 1) { return result;}
  result = FOUR;
  variable_set[438] = 1;
  return result;
}

ExcelValue control_h41() {
  static ExcelValue result;
  if(variable_set[439] == 1) { return result;}
  result = FOUR;
  variable_set[439] = 1;
  return result;
}

ExcelValue control_h42() {
  static ExcelValue result;
  if(variable_set[440] == 1) { return result;}
  result = FOUR;
  variable_set[440] = 1;
  return result;
}

ExcelValue control_h43() {
  static ExcelValue result;
  if(variable_set[441] == 1) { return result;}
  result = FOUR;
  variable_set[441] = 1;
  return result;
}

ExcelValue control_h44() {
  static ExcelValue result;
  if(variable_set[442] == 1) { return result;}
  result = FOUR;
  variable_set[442] = 1;
  return result;
}

ExcelValue control_h45() {
  static ExcelValue result;
  if(variable_set[443] == 1) { return result;}
  result = FOUR;
  variable_set[443] = 1;
  return result;
}

ExcelValue control_h46() {
  static ExcelValue result;
  if(variable_set[444] == 1) { return result;}
  result = FOUR;
  variable_set[444] = 1;
  return result;
}

ExcelValue control_h47() {
  static ExcelValue result;
  if(variable_set[445] == 1) { return result;}
  result = FOUR;
  variable_set[445] = 1;
  return result;
}

ExcelValue control_h48() {
  static ExcelValue result;
  if(variable_set[446] == 1) { return result;}
  result = FOUR;
  variable_set[446] = 1;
  return result;
}

ExcelValue control_h49() {
  static ExcelValue result;
  if(variable_set[447] == 1) { return result;}
  result = FOUR;
  variable_set[447] = 1;
  return result;
}

ExcelValue control_h50() {
  static ExcelValue result;
  if(variable_set[448] == 1) { return result;}
  result = FOUR;
  variable_set[448] = 1;
  return result;
}

ExcelValue control_h51() {
  static ExcelValue result;
  if(variable_set[449] == 1) { return result;}
  result = FOUR;
  variable_set[449] = 1;
  return result;
}

ExcelValue control_h52() {
  static ExcelValue result;
  if(variable_set[450] == 1) { return result;}
  result = FOUR;
  variable_set[450] = 1;
  return result;
}

ExcelValue control_h53() {
  static ExcelValue result;
  if(variable_set[451] == 1) { return result;}
  result = FOUR;
  variable_set[451] = 1;
  return result;
}

ExcelValue control_h54() {
  static ExcelValue result;
  if(variable_set[452] == 1) { return result;}
  result = FOUR;
  variable_set[452] = 1;
  return result;
}

ExcelValue control_h55() {
  static ExcelValue result;
  if(variable_set[453] == 1) { return result;}
  result = FOUR;
  variable_set[453] = 1;
  return result;
}

ExcelValue control_h56() {
  static ExcelValue result;
  if(variable_set[454] == 1) { return result;}
  result = FOUR;
  variable_set[454] = 1;
  return result;
}

ExcelValue control_h57() {
  static ExcelValue result;
  if(variable_set[455] == 1) { return result;}
  result = FOUR;
  variable_set[455] = 1;
  return result;
}

ExcelValue control_h58() {
  static ExcelValue result;
  if(variable_set[456] == 1) { return result;}
  result = FOUR;
  variable_set[456] = 1;
  return result;
}

ExcelValue control_h59() {
  static ExcelValue result;
  if(variable_set[457] == 1) { return result;}
  result = FOUR;
  variable_set[457] = 1;
  return result;
}

ExcelValue control_h60() {
  static ExcelValue result;
  if(variable_set[458] == 1) { return result;}
  result = FOUR;
  variable_set[458] = 1;
  return result;
}

ExcelValue control_h61() {
  static ExcelValue result;
  if(variable_set[459] == 1) { return result;}
  result = FOUR;
  variable_set[459] = 1;
  return result;
}

ExcelValue control_h62() { return BLANK; }
ExcelValue results_e583() { return constant59; }
ExcelValue results_f583() { return constant60; }
ExcelValue results_g583() { return constant61; }
ExcelValue results_h583() { return constant62; }
ExcelValue results_i583() { return constant63; }
ExcelValue results_j583() { return constant64; }
ExcelValue results_k583() { return constant65; }
ExcelValue results_l583() { return constant66; }
ExcelValue results_m583() { return constant67; }
ExcelValue results_n583() { return constant68; }
ExcelValue results_o583() { return constant69; }
ExcelValue results_p583() { return constant70; }
ExcelValue results_q583() { return constant71; }
ExcelValue results_r583() { return constant72; }
ExcelValue results_s583() { return constant73; }
ExcelValue results_t583() { return constant74; }
ExcelValue results_u583() { return constant75; }
ExcelValue results_v583() { return constant76; }
ExcelValue results_w583() { return constant77; }
ExcelValue results_e584() { return constant50; }
ExcelValue results_f584() {
  static ExcelValue result;
  if(variable_set[481] == 1) { return result;}
  result = NINE;
  variable_set[481] = 1;
  return result;
}

ExcelValue results_g584() {
  static ExcelValue result;
  if(variable_set[482] == 1) { return result;}
  result = EIGHT;
  variable_set[482] = 1;
  return result;
}

ExcelValue results_h584() { return constant78; }
ExcelValue results_i584() { return constant79; }
ExcelValue results_j584() { return constant80; }
ExcelValue results_k584() { return constant81; }
ExcelValue results_l584() { return constant82; }
ExcelValue results_m584() { return constant83; }
ExcelValue results_n584() { return constant83; }
ExcelValue results_o584() { return constant83; }
ExcelValue results_p584() { return constant83; }
ExcelValue results_q584() { return constant83; }
ExcelValue results_r584() { return constant83; }
ExcelValue results_s584() { return constant83; }
ExcelValue results_t584() { return constant83; }
ExcelValue results_u584() { return constant83; }
ExcelValue results_v584() { return constant83; }
ExcelValue results_w584() { return constant83; }
ExcelValue results_e585() { return constant84; }
ExcelValue results_f585() {
  static ExcelValue result;
  if(variable_set[500] == 1) { return result;}
  result = ZERO;
  variable_set[500] = 1;
  return result;
}

ExcelValue results_g585() {
  static ExcelValue result;
  if(variable_set[501] == 1) { return result;}
  result = ZERO;
  variable_set[501] = 1;
  return result;
}

ExcelValue results_h585() { return constant85; }
ExcelValue results_i585() { return constant86; }
ExcelValue results_j585() { return constant87; }
ExcelValue results_k585() { return constant88; }
ExcelValue results_l585() { return constant89; }
ExcelValue results_m585() { return constant90; }
ExcelValue results_n585() { return constant90; }
ExcelValue results_o585() { return constant90; }
ExcelValue results_p585() { return constant90; }
ExcelValue results_q585() { return constant90; }
ExcelValue results_r585() { return constant90; }
ExcelValue results_s585() { return constant90; }
ExcelValue results_t585() { return constant90; }
ExcelValue results_u585() { return constant90; }
ExcelValue results_v585() { return constant90; }
ExcelValue results_w585() { return constant90; }
ExcelValue results_e586() { return constant91; }
ExcelValue results_f586() {
  static ExcelValue result;
  if(variable_set[519] == 1) { return result;}
  result = ZERO;
  variable_set[519] = 1;
  return result;
}

ExcelValue results_g586() {
  static ExcelValue result;
  if(variable_set[520] == 1) { return result;}
  result = ZERO;
  variable_set[520] = 1;
  return result;
}

ExcelValue results_h586() { return constant85; }
ExcelValue results_i586() { return constant86; }
ExcelValue results_j586() { return constant87; }
ExcelValue results_k586() { return constant88; }
ExcelValue results_l586() { return constant89; }
ExcelValue results_m586() { return constant90; }
ExcelValue results_n586() { return constant90; }
ExcelValue results_o586() { return constant90; }
ExcelValue results_p586() { return constant90; }
ExcelValue results_q586() { return constant90; }
ExcelValue results_r586() { return constant90; }
ExcelValue results_s586() { return constant90; }
ExcelValue results_t586() { return constant90; }
ExcelValue results_u586() { return constant90; }
ExcelValue results_v586() { return constant90; }
ExcelValue results_w586() { return constant90; }
ExcelValue results_e587() { return constant92; }
ExcelValue results_f587() {
  static ExcelValue result;
  if(variable_set[538] == 1) { return result;}
  result = FOUR;
  variable_set[538] = 1;
  return result;
}

ExcelValue results_g587() {
  static ExcelValue result;
  if(variable_set[539] == 1) { return result;}
  result = FOUR;
  variable_set[539] = 1;
  return result;
}

ExcelValue results_h587() { return constant93; }
ExcelValue results_i587() { return constant94; }
ExcelValue results_j587() { return constant95; }
ExcelValue results_k587() { return constant96; }
ExcelValue results_l587() { return constant97; }
ExcelValue results_m587() { return constant98; }
ExcelValue results_n587() { return constant98; }
ExcelValue results_o587() { return constant98; }
ExcelValue results_p587() { return constant98; }
ExcelValue results_q587() { return constant98; }
ExcelValue results_r587() { return constant98; }
ExcelValue results_s587() { return constant98; }
ExcelValue results_t587() { return constant98; }
ExcelValue results_u587() { return constant98; }
ExcelValue results_v587() { return constant98; }
ExcelValue results_w587() { return constant98; }
ExcelValue results_e588() { return constant99; }
ExcelValue results_f588() {
  static ExcelValue result;
  if(variable_set[557] == 1) { return result;}
  result = TWO;
  variable_set[557] = 1;
  return result;
}

ExcelValue results_g588() {
  static ExcelValue result;
  if(variable_set[558] == 1) { return result;}
  result = TWO;
  variable_set[558] = 1;
  return result;
}

ExcelValue results_h588() { return constant100; }
ExcelValue results_i588() { return constant101; }
ExcelValue results_j588() { return constant102; }
ExcelValue results_k588() { return constant97; }
ExcelValue results_l588() { return constant103; }
ExcelValue results_m588() { return constant104; }
ExcelValue results_n588() { return constant104; }
ExcelValue results_o588() { return constant104; }
ExcelValue results_p588() { return constant104; }
ExcelValue results_q588() { return constant104; }
ExcelValue results_r588() { return constant104; }
ExcelValue results_s588() { return constant104; }
ExcelValue results_t588() { return constant104; }
ExcelValue results_u588() { return constant104; }
ExcelValue results_v588() { return constant104; }
ExcelValue results_w588() { return constant104; }
ExcelValue results_e589() { return constant52; }
ExcelValue results_f589() { return constant105; }
ExcelValue results_g589() { return constant1; }
ExcelValue results_h589() { return constant106; }
ExcelValue results_i589() { return constant107; }
ExcelValue results_j589() { return constant108; }
ExcelValue results_k589() { return constant109; }
ExcelValue results_l589() { return constant110; }
ExcelValue results_m589() {
  static ExcelValue result;
  if(variable_set[583] == 1) { return result;}
  result = TWO;
  variable_set[583] = 1;
  return result;
}

ExcelValue results_n589() {
  static ExcelValue result;
  if(variable_set[584] == 1) { return result;}
  result = TWO;
  variable_set[584] = 1;
  return result;
}

ExcelValue results_o589() {
  static ExcelValue result;
  if(variable_set[585] == 1) { return result;}
  result = TWO;
  variable_set[585] = 1;
  return result;
}

ExcelValue results_p589() {
  static ExcelValue result;
  if(variable_set[586] == 1) { return result;}
  result = TWO;
  variable_set[586] = 1;
  return result;
}

ExcelValue results_q589() {
  static ExcelValue result;
  if(variable_set[587] == 1) { return result;}
  result = TWO;
  variable_set[587] = 1;
  return result;
}

ExcelValue results_r589() {
  static ExcelValue result;
  if(variable_set[588] == 1) { return result;}
  result = TWO;
  variable_set[588] = 1;
  return result;
}

ExcelValue results_s589() {
  static ExcelValue result;
  if(variable_set[589] == 1) { return result;}
  result = TWO;
  variable_set[589] = 1;
  return result;
}

ExcelValue results_t589() {
  static ExcelValue result;
  if(variable_set[590] == 1) { return result;}
  result = TWO;
  variable_set[590] = 1;
  return result;
}

ExcelValue results_u589() {
  static ExcelValue result;
  if(variable_set[591] == 1) { return result;}
  result = TWO;
  variable_set[591] = 1;
  return result;
}

ExcelValue results_v589() {
  static ExcelValue result;
  if(variable_set[592] == 1) { return result;}
  result = TWO;
  variable_set[592] = 1;
  return result;
}

ExcelValue results_w589() {
  static ExcelValue result;
  if(variable_set[593] == 1) { return result;}
  result = TWO;
  variable_set[593] = 1;
  return result;
}

ExcelValue results_e590() { return constant111; }
ExcelValue results_f590() {
  static ExcelValue result;
  if(variable_set[595] == 1) { return result;}
  result = ZERO;
  variable_set[595] = 1;
  return result;
}

ExcelValue results_g590() {
  static ExcelValue result;
  if(variable_set[596] == 1) { return result;}
  result = ZERO;
  variable_set[596] = 1;
  return result;
}

ExcelValue results_h590() { return constant112; }
ExcelValue results_i590() { return constant113; }
ExcelValue results_j590() { return constant114; }
ExcelValue results_k590() { return constant115; }
ExcelValue results_l590() {
  static ExcelValue result;
  if(variable_set[601] == 1) { return result;}
  result = FIVE;
  variable_set[601] = 1;
  return result;
}

ExcelValue results_m590() { return constant116; }
ExcelValue results_n590() { return constant116; }
ExcelValue results_o590() { return constant116; }
ExcelValue results_p590() { return constant116; }
ExcelValue results_q590() { return constant116; }
ExcelValue results_r590() { return constant116; }
ExcelValue results_s590() { return constant116; }
ExcelValue results_t590() { return constant116; }
ExcelValue results_u590() { return constant116; }
ExcelValue results_v590() { return constant116; }
ExcelValue results_w590() { return constant116; }
ExcelValue results_e591() { return constant117; }
ExcelValue results_f591() { return constant118; }
ExcelValue results_g591() { return constant118; }
ExcelValue results_h591() { return constant118; }
ExcelValue results_i591() { return constant119; }
ExcelValue results_j591() { return constant119; }
ExcelValue results_k591() { return constant119; }
ExcelValue results_l591() { return constant119; }
ExcelValue results_m591() { return constant119; }
ExcelValue results_n591() { return constant119; }
ExcelValue results_o591() { return constant120; }
ExcelValue results_p591() { return constant120; }
ExcelValue results_q591() { return constant120; }
ExcelValue results_r591() { return constant1; }
ExcelValue results_s591() { return constant1; }
ExcelValue results_t591() { return constant120; }
ExcelValue results_u591() { return constant120; }
ExcelValue results_v591() { return constant120; }
ExcelValue results_w591() { return constant120; }
ExcelValue results_e526() { return constant59; }
ExcelValue results_f526() { return constant121; }
ExcelValue results_g526() { return constant122; }
ExcelValue results_h526() { return constant123; }
ExcelValue results_i526() { return constant124; }
ExcelValue results_j526() { return constant125; }
ExcelValue results_k526() { return constant126; }
ExcelValue results_l526() { return constant127; }
ExcelValue results_m526() { return constant128; }
ExcelValue results_n526() { return constant129; }
ExcelValue results_o526() { return constant130; }
ExcelValue results_p526() { return constant131; }
ExcelValue results_q526() { return constant132; }
ExcelValue results_r526() { return constant133; }
ExcelValue results_s526() { return constant134; }
ExcelValue results_t526() { return constant135; }
ExcelValue results_u526() { return constant136; }
ExcelValue results_v526() { return constant137; }
ExcelValue results_w526() { return constant138; }
ExcelValue results_e527() { return constant50; }
ExcelValue results_f527() { return constant139; }
ExcelValue results_g527() { return constant140; }
ExcelValue results_h527() { return constant141; }
ExcelValue results_i527() { return constant142; }
ExcelValue results_j527() { return constant143; }
ExcelValue results_k527() { return constant144; }
ExcelValue results_l527() { return constant145; }
ExcelValue results_m527() { return constant146; }
ExcelValue results_n527() { return constant147; }
ExcelValue results_o527() { return constant148; }
ExcelValue results_p527() { return constant149; }
ExcelValue results_q527() { return constant150; }
ExcelValue results_r527() { return constant151; }
ExcelValue results_s527() { return constant152; }
ExcelValue results_t527() { return constant153; }
ExcelValue results_u527() { return constant154; }
ExcelValue results_v527() { return constant155; }
ExcelValue results_w527() { return constant156; }
ExcelValue results_e528() { return constant84; }
ExcelValue results_f528() {
  static ExcelValue result;
  if(variable_set[671] == 1) { return result;}
  result = ZERO;
  variable_set[671] = 1;
  return result;
}

ExcelValue results_g528() {
  static ExcelValue result;
  if(variable_set[672] == 1) { return result;}
  result = ZERO;
  variable_set[672] = 1;
  return result;
}

ExcelValue results_h528() { return constant157; }
ExcelValue results_i528() { return constant158; }
ExcelValue results_j528() { return constant159; }
ExcelValue results_k528() { return constant160; }
ExcelValue results_l528() { return constant161; }
ExcelValue results_m528() { return constant162; }
ExcelValue results_n528() { return constant163; }
ExcelValue results_o528() { return constant164; }
ExcelValue results_p528() { return constant165; }
ExcelValue results_q528() { return constant166; }
ExcelValue results_r528() { return constant167; }
ExcelValue results_s528() { return constant168; }
ExcelValue results_t528() { return constant169; }
ExcelValue results_u528() { return constant170; }
ExcelValue results_v528() { return constant171; }
ExcelValue results_w528() { return constant172; }
ExcelValue results_e529() { return constant91; }
ExcelValue results_f529() {
  static ExcelValue result;
  if(variable_set[690] == 1) { return result;}
  result = ZERO;
  variable_set[690] = 1;
  return result;
}

ExcelValue results_g529() {
  static ExcelValue result;
  if(variable_set[691] == 1) { return result;}
  result = ZERO;
  variable_set[691] = 1;
  return result;
}

ExcelValue results_h529() { return constant157; }
ExcelValue results_i529() { return constant158; }
ExcelValue results_j529() { return constant173; }
ExcelValue results_k529() { return constant174; }
ExcelValue results_l529() { return constant175; }
ExcelValue results_m529() { return constant176; }
ExcelValue results_n529() { return constant177; }
ExcelValue results_o529() { return constant178; }
ExcelValue results_p529() { return constant179; }
ExcelValue results_q529() { return constant180; }
ExcelValue results_r529() { return constant181; }
ExcelValue results_s529() { return constant182; }
ExcelValue results_t529() { return constant183; }
ExcelValue results_u529() { return constant184; }
ExcelValue results_v529() { return constant185; }
ExcelValue results_w529() { return constant186; }
ExcelValue results_e530() { return constant92; }
ExcelValue results_f530() { return constant187; }
ExcelValue results_g530() { return constant188; }
ExcelValue results_h530() { return constant189; }
ExcelValue results_i530() { return constant190; }
ExcelValue results_j530() { return constant191; }
ExcelValue results_k530() { return constant192; }
ExcelValue results_l530() { return constant193; }
ExcelValue results_m530() { return constant194; }
ExcelValue results_n530() { return constant195; }
ExcelValue results_o530() { return constant196; }
ExcelValue results_p530() { return constant197; }
ExcelValue results_q530() { return constant198; }
ExcelValue results_r530() { return constant199; }
ExcelValue results_s530() { return constant200; }
ExcelValue results_t530() { return constant201; }
ExcelValue results_u530() { return constant202; }
ExcelValue results_v530() { return constant203; }
ExcelValue results_w530() { return constant204; }
ExcelValue results_e531() { return constant99; }
ExcelValue results_f531() { return constant205; }
ExcelValue results_g531() { return constant206; }
ExcelValue results_h531() { return constant207; }
ExcelValue results_i531() { return constant208; }
ExcelValue results_j531() { return constant209; }
ExcelValue results_k531() { return constant210; }
ExcelValue results_l531() { return constant211; }
ExcelValue results_m531() { return constant212; }
ExcelValue results_n531() { return constant213; }
ExcelValue results_o531() { return constant214; }
ExcelValue results_p531() { return constant215; }
ExcelValue results_q531() { return constant216; }
ExcelValue results_r531() { return constant217; }
ExcelValue results_s531() { return constant218; }
ExcelValue results_t531() { return constant219; }
ExcelValue results_u531() { return constant220; }
ExcelValue results_v531() { return constant221; }
ExcelValue results_w531() { return constant222; }
ExcelValue results_e532() { return constant52; }
ExcelValue results_f532() { return constant223; }
ExcelValue results_g532() { return constant224; }
ExcelValue results_h532() { return constant225; }
ExcelValue results_i532() { return constant226; }
ExcelValue results_j532() { return constant227; }
ExcelValue results_k532() { return constant228; }
ExcelValue results_l532() { return constant229; }
ExcelValue results_m532() { return constant230; }
ExcelValue results_n532() { return constant231; }
ExcelValue results_o532() { return constant232; }
ExcelValue results_p532() { return constant233; }
ExcelValue results_q532() { return constant234; }
ExcelValue results_r532() { return constant235; }
ExcelValue results_s532() { return constant236; }
ExcelValue results_t532() { return constant237; }
ExcelValue results_u532() { return constant238; }
ExcelValue results_v532() { return constant239; }
ExcelValue results_w532() { return constant240; }
ExcelValue results_e533() { return constant111; }
ExcelValue results_f533() {
  static ExcelValue result;
  if(variable_set[766] == 1) { return result;}
  result = ZERO;
  variable_set[766] = 1;
  return result;
}

ExcelValue results_g533() {
  static ExcelValue result;
  if(variable_set[767] == 1) { return result;}
  result = ZERO;
  variable_set[767] = 1;
  return result;
}

ExcelValue results_h533() { return constant241; }
ExcelValue results_i533() { return constant158; }
ExcelValue results_j533() { return constant242; }
ExcelValue results_k533() { return constant160; }
ExcelValue results_l533() { return constant161; }
ExcelValue results_m533() { return constant243; }
ExcelValue results_n533() { return constant244; }
ExcelValue results_o533() { return constant245; }
ExcelValue results_p533() { return constant165; }
ExcelValue results_q533() { return constant166; }
ExcelValue results_r533() { return constant167; }
ExcelValue results_s533() { return constant246; }
ExcelValue results_t533() { return constant169; }
ExcelValue results_u533() { return constant247; }
ExcelValue results_v533() { return constant248; }
ExcelValue results_w533() { return constant249; }
ExcelValue results_e534() { return constant117; }
ExcelValue results_f534() { return constant250; }
ExcelValue results_g534() { return constant251; }
ExcelValue results_h534() { return constant252; }
ExcelValue results_i534() { return constant253; }
ExcelValue results_j534() { return constant254; }
ExcelValue results_k534() { return constant255; }
ExcelValue results_l534() { return constant256; }
ExcelValue results_m534() { return constant257; }
ExcelValue results_n534() { return constant258; }
ExcelValue results_o534() { return constant259; }
ExcelValue results_p534() { return constant260; }
ExcelValue results_q534() { return constant261; }
ExcelValue results_r534() { return constant262; }
ExcelValue results_s534() { return constant263; }
ExcelValue results_t534() { return constant264; }
ExcelValue results_u534() { return constant265; }
ExcelValue results_v534() { return constant266; }
ExcelValue results_w534() { return constant267; }
ExcelValue results_e535() { return constant268; }
ExcelValue results_f535() { return constant269; }
ExcelValue results_g535() { return constant270; }
ExcelValue results_h535() { return constant271; }
ExcelValue results_i535() { return constant272; }
ExcelValue results_j535() { return constant273; }
ExcelValue results_k535() { return constant274; }
ExcelValue results_l535() {
  static ExcelValue result;
  if(variable_set[810] == 1) { return result;}
  result = ZERO;
  variable_set[810] = 1;
  return result;
}

ExcelValue results_m535() {
  static ExcelValue result;
  if(variable_set[811] == 1) { return result;}
  result = ZERO;
  variable_set[811] = 1;
  return result;
}

ExcelValue results_n535() {
  static ExcelValue result;
  if(variable_set[812] == 1) { return result;}
  result = ZERO;
  variable_set[812] = 1;
  return result;
}

ExcelValue results_o535() {
  static ExcelValue result;
  if(variable_set[813] == 1) { return result;}
  result = ZERO;
  variable_set[813] = 1;
  return result;
}

ExcelValue results_p535() {
  static ExcelValue result;
  if(variable_set[814] == 1) { return result;}
  result = ZERO;
  variable_set[814] = 1;
  return result;
}

ExcelValue results_q535() {
  static ExcelValue result;
  if(variable_set[815] == 1) { return result;}
  result = ZERO;
  variable_set[815] = 1;
  return result;
}

ExcelValue results_r535() {
  static ExcelValue result;
  if(variable_set[816] == 1) { return result;}
  result = ZERO;
  variable_set[816] = 1;
  return result;
}

ExcelValue results_s535() {
  static ExcelValue result;
  if(variable_set[817] == 1) { return result;}
  result = ZERO;
  variable_set[817] = 1;
  return result;
}

ExcelValue results_t535() {
  static ExcelValue result;
  if(variable_set[818] == 1) { return result;}
  result = ZERO;
  variable_set[818] = 1;
  return result;
}

ExcelValue results_u535() {
  static ExcelValue result;
  if(variable_set[819] == 1) { return result;}
  result = ZERO;
  variable_set[819] = 1;
  return result;
}

ExcelValue results_v535() {
  static ExcelValue result;
  if(variable_set[820] == 1) { return result;}
  result = ZERO;
  variable_set[820] = 1;
  return result;
}

ExcelValue results_w535() {
  static ExcelValue result;
  if(variable_set[821] == 1) { return result;}
  result = ZERO;
  variable_set[821] = 1;
  return result;
}

ExcelValue results_e34() { return constant275; }
ExcelValue results_f34() {
  static ExcelValue result;
  if(variable_set[823] == 1) { return result;}
  result = ZERO;
  variable_set[823] = 1;
  return result;
}

ExcelValue results_g34() {
  static ExcelValue result;
  if(variable_set[824] == 1) { return result;}
  result = ZERO;
  variable_set[824] = 1;
  return result;
}

ExcelValue results_h34() {
  static ExcelValue result;
  if(variable_set[825] == 1) { return result;}
  result = ZERO;
  variable_set[825] = 1;
  return result;
}

ExcelValue results_i34() {
  static ExcelValue result;
  if(variable_set[826] == 1) { return result;}
  result = ZERO;
  variable_set[826] = 1;
  return result;
}

ExcelValue results_j34() {
  static ExcelValue result;
  if(variable_set[827] == 1) { return result;}
  result = ZERO;
  variable_set[827] = 1;
  return result;
}

ExcelValue results_k34() {
  static ExcelValue result;
  if(variable_set[828] == 1) { return result;}
  result = ZERO;
  variable_set[828] = 1;
  return result;
}

ExcelValue results_l34() {
  static ExcelValue result;
  if(variable_set[829] == 1) { return result;}
  result = ZERO;
  variable_set[829] = 1;
  return result;
}

ExcelValue results_m34() {
  static ExcelValue result;
  if(variable_set[830] == 1) { return result;}
  result = ZERO;
  variable_set[830] = 1;
  return result;
}

ExcelValue results_n34() {
  static ExcelValue result;
  if(variable_set[831] == 1) { return result;}
  result = ZERO;
  variable_set[831] = 1;
  return result;
}

ExcelValue results_o34() {
  static ExcelValue result;
  if(variable_set[832] == 1) { return result;}
  result = ZERO;
  variable_set[832] = 1;
  return result;
}

ExcelValue results_p34() {
  static ExcelValue result;
  if(variable_set[833] == 1) { return result;}
  result = ZERO;
  variable_set[833] = 1;
  return result;
}

ExcelValue results_q34() {
  static ExcelValue result;
  if(variable_set[834] == 1) { return result;}
  result = ZERO;
  variable_set[834] = 1;
  return result;
}

ExcelValue results_r34() {
  static ExcelValue result;
  if(variable_set[835] == 1) { return result;}
  result = ZERO;
  variable_set[835] = 1;
  return result;
}

ExcelValue results_s34() {
  static ExcelValue result;
  if(variable_set[836] == 1) { return result;}
  result = ZERO;
  variable_set[836] = 1;
  return result;
}

ExcelValue results_t34() {
  static ExcelValue result;
  if(variable_set[837] == 1) { return result;}
  result = ZERO;
  variable_set[837] = 1;
  return result;
}

ExcelValue results_u34() {
  static ExcelValue result;
  if(variable_set[838] == 1) { return result;}
  result = ZERO;
  variable_set[838] = 1;
  return result;
}

ExcelValue results_v34() {
  static ExcelValue result;
  if(variable_set[839] == 1) { return result;}
  result = ZERO;
  variable_set[839] = 1;
  return result;
}

ExcelValue results_w34() {
  static ExcelValue result;
  if(variable_set[840] == 1) { return result;}
  result = ZERO;
  variable_set[840] = 1;
  return result;
}

ExcelValue results_e35() { return constant276; }
ExcelValue results_f35() {
  static ExcelValue result;
  if(variable_set[842] == 1) { return result;}
  result = ZERO;
  variable_set[842] = 1;
  return result;
}

ExcelValue results_g35() {
  static ExcelValue result;
  if(variable_set[843] == 1) { return result;}
  result = ZERO;
  variable_set[843] = 1;
  return result;
}

ExcelValue results_h35() {
  static ExcelValue result;
  if(variable_set[844] == 1) { return result;}
  result = ZERO;
  variable_set[844] = 1;
  return result;
}

ExcelValue results_i35() {
  static ExcelValue result;
  if(variable_set[845] == 1) { return result;}
  result = ZERO;
  variable_set[845] = 1;
  return result;
}

ExcelValue results_j35() {
  static ExcelValue result;
  if(variable_set[846] == 1) { return result;}
  result = ZERO;
  variable_set[846] = 1;
  return result;
}

ExcelValue results_k35() {
  static ExcelValue result;
  if(variable_set[847] == 1) { return result;}
  result = ZERO;
  variable_set[847] = 1;
  return result;
}

ExcelValue results_l35() {
  static ExcelValue result;
  if(variable_set[848] == 1) { return result;}
  result = ZERO;
  variable_set[848] = 1;
  return result;
}

ExcelValue results_m35() {
  static ExcelValue result;
  if(variable_set[849] == 1) { return result;}
  result = ZERO;
  variable_set[849] = 1;
  return result;
}

ExcelValue results_n35() {
  static ExcelValue result;
  if(variable_set[850] == 1) { return result;}
  result = ZERO;
  variable_set[850] = 1;
  return result;
}

ExcelValue results_o35() {
  static ExcelValue result;
  if(variable_set[851] == 1) { return result;}
  result = ZERO;
  variable_set[851] = 1;
  return result;
}

ExcelValue results_p35() {
  static ExcelValue result;
  if(variable_set[852] == 1) { return result;}
  result = ZERO;
  variable_set[852] = 1;
  return result;
}

ExcelValue results_q35() {
  static ExcelValue result;
  if(variable_set[853] == 1) { return result;}
  result = ZERO;
  variable_set[853] = 1;
  return result;
}

ExcelValue results_r35() {
  static ExcelValue result;
  if(variable_set[854] == 1) { return result;}
  result = ZERO;
  variable_set[854] = 1;
  return result;
}

ExcelValue results_s35() {
  static ExcelValue result;
  if(variable_set[855] == 1) { return result;}
  result = ZERO;
  variable_set[855] = 1;
  return result;
}

ExcelValue results_t35() {
  static ExcelValue result;
  if(variable_set[856] == 1) { return result;}
  result = ZERO;
  variable_set[856] = 1;
  return result;
}

ExcelValue results_u35() {
  static ExcelValue result;
  if(variable_set[857] == 1) { return result;}
  result = ZERO;
  variable_set[857] = 1;
  return result;
}

ExcelValue results_v35() {
  static ExcelValue result;
  if(variable_set[858] == 1) { return result;}
  result = ZERO;
  variable_set[858] = 1;
  return result;
}

ExcelValue results_w35() {
  static ExcelValue result;
  if(variable_set[859] == 1) { return result;}
  result = ZERO;
  variable_set[859] = 1;
  return result;
}

ExcelValue results_e36() { return constant277; }
ExcelValue results_f36() {
  static ExcelValue result;
  if(variable_set[861] == 1) { return result;}
  result = ZERO;
  variable_set[861] = 1;
  return result;
}

ExcelValue results_g36() {
  static ExcelValue result;
  if(variable_set[862] == 1) { return result;}
  result = ZERO;
  variable_set[862] = 1;
  return result;
}

ExcelValue results_h36() {
  static ExcelValue result;
  if(variable_set[863] == 1) { return result;}
  result = ZERO;
  variable_set[863] = 1;
  return result;
}

ExcelValue results_i36() {
  static ExcelValue result;
  if(variable_set[864] == 1) { return result;}
  result = ZERO;
  variable_set[864] = 1;
  return result;
}

ExcelValue results_j36() {
  static ExcelValue result;
  if(variable_set[865] == 1) { return result;}
  result = ZERO;
  variable_set[865] = 1;
  return result;
}

ExcelValue results_k36() {
  static ExcelValue result;
  if(variable_set[866] == 1) { return result;}
  result = ZERO;
  variable_set[866] = 1;
  return result;
}

ExcelValue results_l36() {
  static ExcelValue result;
  if(variable_set[867] == 1) { return result;}
  result = ZERO;
  variable_set[867] = 1;
  return result;
}

ExcelValue results_m36() {
  static ExcelValue result;
  if(variable_set[868] == 1) { return result;}
  result = ZERO;
  variable_set[868] = 1;
  return result;
}

ExcelValue results_n36() {
  static ExcelValue result;
  if(variable_set[869] == 1) { return result;}
  result = ZERO;
  variable_set[869] = 1;
  return result;
}

ExcelValue results_o36() {
  static ExcelValue result;
  if(variable_set[870] == 1) { return result;}
  result = ZERO;
  variable_set[870] = 1;
  return result;
}

ExcelValue results_p36() {
  static ExcelValue result;
  if(variable_set[871] == 1) { return result;}
  result = ZERO;
  variable_set[871] = 1;
  return result;
}

ExcelValue results_q36() {
  static ExcelValue result;
  if(variable_set[872] == 1) { return result;}
  result = ZERO;
  variable_set[872] = 1;
  return result;
}

ExcelValue results_r36() {
  static ExcelValue result;
  if(variable_set[873] == 1) { return result;}
  result = ZERO;
  variable_set[873] = 1;
  return result;
}

ExcelValue results_s36() {
  static ExcelValue result;
  if(variable_set[874] == 1) { return result;}
  result = ZERO;
  variable_set[874] = 1;
  return result;
}

ExcelValue results_t36() {
  static ExcelValue result;
  if(variable_set[875] == 1) { return result;}
  result = ZERO;
  variable_set[875] = 1;
  return result;
}

ExcelValue results_u36() {
  static ExcelValue result;
  if(variable_set[876] == 1) { return result;}
  result = ZERO;
  variable_set[876] = 1;
  return result;
}

ExcelValue results_v36() {
  static ExcelValue result;
  if(variable_set[877] == 1) { return result;}
  result = ZERO;
  variable_set[877] = 1;
  return result;
}

ExcelValue results_w36() {
  static ExcelValue result;
  if(variable_set[878] == 1) { return result;}
  result = ZERO;
  variable_set[878] = 1;
  return result;
}

ExcelValue results_e37() { return constant278; }
ExcelValue results_f37() {
  static ExcelValue result;
  if(variable_set[880] == 1) { return result;}
  result = ZERO;
  variable_set[880] = 1;
  return result;
}

ExcelValue results_g37() {
  static ExcelValue result;
  if(variable_set[881] == 1) { return result;}
  result = ZERO;
  variable_set[881] = 1;
  return result;
}

ExcelValue results_h37() { return constant279; }
ExcelValue results_i37() { return constant280; }
ExcelValue results_j37() { return constant281; }
ExcelValue results_k37() { return constant282; }
ExcelValue results_l37() { return constant283; }
ExcelValue results_m37() { return constant284; }
ExcelValue results_n37() { return constant285; }
ExcelValue results_o37() { return constant286; }
ExcelValue results_p37() { return constant287; }
ExcelValue results_q37() { return constant288; }
ExcelValue results_r37() { return constant289; }
ExcelValue results_s37() { return constant290; }
ExcelValue results_t37() { return constant291; }
ExcelValue results_u37() { return constant292; }
ExcelValue results_v37() { return constant293; }
ExcelValue results_w37() { return constant294; }
ExcelValue results_e38() { return constant295; }
ExcelValue results_f38() { return constant296; }
ExcelValue results_g38() { return constant297; }
ExcelValue results_h38() { return constant298; }
ExcelValue results_i38() { return constant299; }
ExcelValue results_j38() { return constant300; }
ExcelValue results_k38() { return constant301; }
ExcelValue results_l38() { return constant302; }
ExcelValue results_m38() { return constant303; }
ExcelValue results_n38() { return constant304; }
ExcelValue results_o38() { return constant305; }
ExcelValue results_p38() { return constant306; }
ExcelValue results_q38() { return constant307; }
ExcelValue results_r38() { return constant308; }
ExcelValue results_s38() { return constant309; }
ExcelValue results_t38() { return constant310; }
ExcelValue results_u38() { return constant311; }
ExcelValue results_v38() { return constant312; }
ExcelValue results_w38() { return constant313; }
ExcelValue results_e39() { return constant314; }
ExcelValue results_f39() { return constant315; }
ExcelValue results_g39() { return constant316; }
ExcelValue results_h39() { return constant317; }
ExcelValue results_i39() { return constant318; }
ExcelValue results_j39() { return constant319; }
ExcelValue results_k39() { return constant320; }
ExcelValue results_l39() { return constant321; }
ExcelValue results_m39() { return constant322; }
ExcelValue results_n39() { return constant323; }
ExcelValue results_o39() { return constant324; }
ExcelValue results_p39() { return constant325; }
ExcelValue results_q39() { return constant326; }
ExcelValue results_r39() { return constant327; }
ExcelValue results_s39() { return constant328; }
ExcelValue results_t39() { return constant329; }
ExcelValue results_u39() { return constant330; }
ExcelValue results_v39() { return constant331; }
ExcelValue results_w39() { return constant332; }
ExcelValue results_e40() { return constant333; }
ExcelValue results_f40() { return constant334; }
ExcelValue results_g40() { return constant335; }
ExcelValue results_h40() { return constant336; }
ExcelValue results_i40() { return constant337; }
ExcelValue results_j40() { return constant338; }
ExcelValue results_k40() { return constant339; }
ExcelValue results_l40() { return constant340; }
ExcelValue results_m40() { return constant341; }
ExcelValue results_n40() { return constant342; }
ExcelValue results_o40() { return constant343; }
ExcelValue results_p40() { return constant344; }
ExcelValue results_q40() { return constant345; }
ExcelValue results_r40() { return constant346; }
ExcelValue results_s40() { return constant347; }
ExcelValue results_t40() { return constant348; }
ExcelValue results_u40() { return constant349; }
ExcelValue results_v40() { return constant350; }
ExcelValue results_w40() { return constant351; }
ExcelValue results_e41() { return constant352; }
ExcelValue results_f41() {
  static ExcelValue result;
  if(variable_set[956] == 1) { return result;}
  result = ZERO;
  variable_set[956] = 1;
  return result;
}

ExcelValue results_g41() {
  static ExcelValue result;
  if(variable_set[957] == 1) { return result;}
  result = ZERO;
  variable_set[957] = 1;
  return result;
}

ExcelValue results_h41() {
  static ExcelValue result;
  if(variable_set[958] == 1) { return result;}
  result = ZERO;
  variable_set[958] = 1;
  return result;
}

ExcelValue results_i41() {
  static ExcelValue result;
  if(variable_set[959] == 1) { return result;}
  result = ZERO;
  variable_set[959] = 1;
  return result;
}

ExcelValue results_j41() {
  static ExcelValue result;
  if(variable_set[960] == 1) { return result;}
  result = ZERO;
  variable_set[960] = 1;
  return result;
}

ExcelValue results_k41() {
  static ExcelValue result;
  if(variable_set[961] == 1) { return result;}
  result = ZERO;
  variable_set[961] = 1;
  return result;
}

ExcelValue results_l41() {
  static ExcelValue result;
  if(variable_set[962] == 1) { return result;}
  result = ZERO;
  variable_set[962] = 1;
  return result;
}

ExcelValue results_m41() {
  static ExcelValue result;
  if(variable_set[963] == 1) { return result;}
  result = ZERO;
  variable_set[963] = 1;
  return result;
}

ExcelValue results_n41() {
  static ExcelValue result;
  if(variable_set[964] == 1) { return result;}
  result = ZERO;
  variable_set[964] = 1;
  return result;
}

ExcelValue results_o41() {
  static ExcelValue result;
  if(variable_set[965] == 1) { return result;}
  result = ZERO;
  variable_set[965] = 1;
  return result;
}

ExcelValue results_p41() {
  static ExcelValue result;
  if(variable_set[966] == 1) { return result;}
  result = ZERO;
  variable_set[966] = 1;
  return result;
}

ExcelValue results_q41() {
  static ExcelValue result;
  if(variable_set[967] == 1) { return result;}
  result = ZERO;
  variable_set[967] = 1;
  return result;
}

ExcelValue results_r41() {
  static ExcelValue result;
  if(variable_set[968] == 1) { return result;}
  result = ZERO;
  variable_set[968] = 1;
  return result;
}

ExcelValue results_s41() {
  static ExcelValue result;
  if(variable_set[969] == 1) { return result;}
  result = ZERO;
  variable_set[969] = 1;
  return result;
}

ExcelValue results_t41() {
  static ExcelValue result;
  if(variable_set[970] == 1) { return result;}
  result = ZERO;
  variable_set[970] = 1;
  return result;
}

ExcelValue results_u41() {
  static ExcelValue result;
  if(variable_set[971] == 1) { return result;}
  result = ZERO;
  variable_set[971] = 1;
  return result;
}

ExcelValue results_v41() {
  static ExcelValue result;
  if(variable_set[972] == 1) { return result;}
  result = ZERO;
  variable_set[972] = 1;
  return result;
}

ExcelValue results_w41() {
  static ExcelValue result;
  if(variable_set[973] == 1) { return result;}
  result = ZERO;
  variable_set[973] = 1;
  return result;
}

ExcelValue results_e42() { return constant353; }
ExcelValue results_f42() {
  static ExcelValue result;
  if(variable_set[975] == 1) { return result;}
  result = ZERO;
  variable_set[975] = 1;
  return result;
}

ExcelValue results_g42() {
  static ExcelValue result;
  if(variable_set[976] == 1) { return result;}
  result = ZERO;
  variable_set[976] = 1;
  return result;
}

ExcelValue results_h42() {
  static ExcelValue result;
  if(variable_set[977] == 1) { return result;}
  result = ZERO;
  variable_set[977] = 1;
  return result;
}

ExcelValue results_i42() {
  static ExcelValue result;
  if(variable_set[978] == 1) { return result;}
  result = ZERO;
  variable_set[978] = 1;
  return result;
}

ExcelValue results_j42() {
  static ExcelValue result;
  if(variable_set[979] == 1) { return result;}
  result = ZERO;
  variable_set[979] = 1;
  return result;
}

ExcelValue results_k42() {
  static ExcelValue result;
  if(variable_set[980] == 1) { return result;}
  result = ZERO;
  variable_set[980] = 1;
  return result;
}

ExcelValue results_l42() {
  static ExcelValue result;
  if(variable_set[981] == 1) { return result;}
  result = ZERO;
  variable_set[981] = 1;
  return result;
}

ExcelValue results_m42() {
  static ExcelValue result;
  if(variable_set[982] == 1) { return result;}
  result = ZERO;
  variable_set[982] = 1;
  return result;
}

ExcelValue results_n42() {
  static ExcelValue result;
  if(variable_set[983] == 1) { return result;}
  result = ZERO;
  variable_set[983] = 1;
  return result;
}

ExcelValue results_o42() {
  static ExcelValue result;
  if(variable_set[984] == 1) { return result;}
  result = ZERO;
  variable_set[984] = 1;
  return result;
}

ExcelValue results_p42() {
  static ExcelValue result;
  if(variable_set[985] == 1) { return result;}
  result = ZERO;
  variable_set[985] = 1;
  return result;
}

ExcelValue results_q42() {
  static ExcelValue result;
  if(variable_set[986] == 1) { return result;}
  result = ZERO;
  variable_set[986] = 1;
  return result;
}

ExcelValue results_r42() {
  static ExcelValue result;
  if(variable_set[987] == 1) { return result;}
  result = ZERO;
  variable_set[987] = 1;
  return result;
}

ExcelValue results_s42() {
  static ExcelValue result;
  if(variable_set[988] == 1) { return result;}
  result = ZERO;
  variable_set[988] = 1;
  return result;
}

ExcelValue results_t42() {
  static ExcelValue result;
  if(variable_set[989] == 1) { return result;}
  result = ZERO;
  variable_set[989] = 1;
  return result;
}

ExcelValue results_u42() {
  static ExcelValue result;
  if(variable_set[990] == 1) { return result;}
  result = ZERO;
  variable_set[990] = 1;
  return result;
}

ExcelValue results_v42() {
  static ExcelValue result;
  if(variable_set[991] == 1) { return result;}
  result = ZERO;
  variable_set[991] = 1;
  return result;
}

ExcelValue results_w42() {
  static ExcelValue result;
  if(variable_set[992] == 1) { return result;}
  result = ZERO;
  variable_set[992] = 1;
  return result;
}

ExcelValue results_e43() { return constant354; }
ExcelValue results_f43() {
  static ExcelValue result;
  if(variable_set[994] == 1) { return result;}
  result = ZERO;
  variable_set[994] = 1;
  return result;
}

ExcelValue results_g43() {
  static ExcelValue result;
  if(variable_set[995] == 1) { return result;}
  result = ZERO;
  variable_set[995] = 1;
  return result;
}

ExcelValue results_h43() {
  static ExcelValue result;
  if(variable_set[996] == 1) { return result;}
  result = ZERO;
  variable_set[996] = 1;
  return result;
}

ExcelValue results_i43() {
  static ExcelValue result;
  if(variable_set[997] == 1) { return result;}
  result = ZERO;
  variable_set[997] = 1;
  return result;
}

ExcelValue results_j43() {
  static ExcelValue result;
  if(variable_set[998] == 1) { return result;}
  result = ZERO;
  variable_set[998] = 1;
  return result;
}

ExcelValue results_k43() {
  static ExcelValue result;
  if(variable_set[999] == 1) { return result;}
  result = ZERO;
  variable_set[999] = 1;
  return result;
}

ExcelValue results_l43() {
  static ExcelValue result;
  if(variable_set[1000] == 1) { return result;}
  result = ZERO;
  variable_set[1000] = 1;
  return result;
}

ExcelValue results_m43() {
  static ExcelValue result;
  if(variable_set[1001] == 1) { return result;}
  result = ZERO;
  variable_set[1001] = 1;
  return result;
}

ExcelValue results_n43() {
  static ExcelValue result;
  if(variable_set[1002] == 1) { return result;}
  result = ZERO;
  variable_set[1002] = 1;
  return result;
}

ExcelValue results_o43() {
  static ExcelValue result;
  if(variable_set[1003] == 1) { return result;}
  result = ZERO;
  variable_set[1003] = 1;
  return result;
}

ExcelValue results_p43() {
  static ExcelValue result;
  if(variable_set[1004] == 1) { return result;}
  result = ZERO;
  variable_set[1004] = 1;
  return result;
}

ExcelValue results_q43() {
  static ExcelValue result;
  if(variable_set[1005] == 1) { return result;}
  result = ZERO;
  variable_set[1005] = 1;
  return result;
}

ExcelValue results_r43() {
  static ExcelValue result;
  if(variable_set[1006] == 1) { return result;}
  result = ZERO;
  variable_set[1006] = 1;
  return result;
}

ExcelValue results_s43() {
  static ExcelValue result;
  if(variable_set[1007] == 1) { return result;}
  result = ZERO;
  variable_set[1007] = 1;
  return result;
}

ExcelValue results_t43() {
  static ExcelValue result;
  if(variable_set[1008] == 1) { return result;}
  result = ZERO;
  variable_set[1008] = 1;
  return result;
}

ExcelValue results_u43() {
  static ExcelValue result;
  if(variable_set[1009] == 1) { return result;}
  result = ZERO;
  variable_set[1009] = 1;
  return result;
}

ExcelValue results_v43() {
  static ExcelValue result;
  if(variable_set[1010] == 1) { return result;}
  result = ZERO;
  variable_set[1010] = 1;
  return result;
}

ExcelValue results_w43() {
  static ExcelValue result;
  if(variable_set[1011] == 1) { return result;}
  result = ZERO;
  variable_set[1011] = 1;
  return result;
}

ExcelValue results_e44() { return constant355; }
ExcelValue results_f44() {
  static ExcelValue result;
  if(variable_set[1013] == 1) { return result;}
  result = ZERO;
  variable_set[1013] = 1;
  return result;
}

ExcelValue results_g44() {
  static ExcelValue result;
  if(variable_set[1014] == 1) { return result;}
  result = ZERO;
  variable_set[1014] = 1;
  return result;
}

ExcelValue results_h44() {
  static ExcelValue result;
  if(variable_set[1015] == 1) { return result;}
  result = ZERO;
  variable_set[1015] = 1;
  return result;
}

ExcelValue results_i44() {
  static ExcelValue result;
  if(variable_set[1016] == 1) { return result;}
  result = ZERO;
  variable_set[1016] = 1;
  return result;
}

ExcelValue results_j44() {
  static ExcelValue result;
  if(variable_set[1017] == 1) { return result;}
  result = ZERO;
  variable_set[1017] = 1;
  return result;
}

ExcelValue results_k44() {
  static ExcelValue result;
  if(variable_set[1018] == 1) { return result;}
  result = ZERO;
  variable_set[1018] = 1;
  return result;
}

ExcelValue results_l44() {
  static ExcelValue result;
  if(variable_set[1019] == 1) { return result;}
  result = ZERO;
  variable_set[1019] = 1;
  return result;
}

ExcelValue results_m44() {
  static ExcelValue result;
  if(variable_set[1020] == 1) { return result;}
  result = ZERO;
  variable_set[1020] = 1;
  return result;
}

ExcelValue results_n44() {
  static ExcelValue result;
  if(variable_set[1021] == 1) { return result;}
  result = ZERO;
  variable_set[1021] = 1;
  return result;
}

ExcelValue results_o44() {
  static ExcelValue result;
  if(variable_set[1022] == 1) { return result;}
  result = ZERO;
  variable_set[1022] = 1;
  return result;
}

ExcelValue results_p44() {
  static ExcelValue result;
  if(variable_set[1023] == 1) { return result;}
  result = ZERO;
  variable_set[1023] = 1;
  return result;
}

ExcelValue results_q44() {
  static ExcelValue result;
  if(variable_set[1024] == 1) { return result;}
  result = ZERO;
  variable_set[1024] = 1;
  return result;
}

ExcelValue results_r44() {
  static ExcelValue result;
  if(variable_set[1025] == 1) { return result;}
  result = ZERO;
  variable_set[1025] = 1;
  return result;
}

ExcelValue results_s44() {
  static ExcelValue result;
  if(variable_set[1026] == 1) { return result;}
  result = ZERO;
  variable_set[1026] = 1;
  return result;
}

ExcelValue results_t44() {
  static ExcelValue result;
  if(variable_set[1027] == 1) { return result;}
  result = ZERO;
  variable_set[1027] = 1;
  return result;
}

ExcelValue results_u44() {
  static ExcelValue result;
  if(variable_set[1028] == 1) { return result;}
  result = ZERO;
  variable_set[1028] = 1;
  return result;
}

ExcelValue results_v44() {
  static ExcelValue result;
  if(variable_set[1029] == 1) { return result;}
  result = ZERO;
  variable_set[1029] = 1;
  return result;
}

ExcelValue results_w44() {
  static ExcelValue result;
  if(variable_set[1030] == 1) { return result;}
  result = ZERO;
  variable_set[1030] = 1;
  return result;
}

ExcelValue results_e45() { return constant356; }
ExcelValue results_f45() {
  static ExcelValue result;
  if(variable_set[1032] == 1) { return result;}
  result = ZERO;
  variable_set[1032] = 1;
  return result;
}

ExcelValue results_g45() {
  static ExcelValue result;
  if(variable_set[1033] == 1) { return result;}
  result = ZERO;
  variable_set[1033] = 1;
  return result;
}

ExcelValue results_h45() { return constant357; }
ExcelValue results_i45() { return constant358; }
ExcelValue results_j45() { return constant359; }
ExcelValue results_k45() { return constant360; }
ExcelValue results_l45() { return constant361; }
ExcelValue results_m45() { return constant362; }
ExcelValue results_n45() { return constant363; }
ExcelValue results_o45() { return constant364; }
ExcelValue results_p45() { return constant365; }
ExcelValue results_q45() { return constant366; }
ExcelValue results_r45() { return constant367; }
ExcelValue results_s45() { return constant368; }
ExcelValue results_t45() { return constant369; }
ExcelValue results_u45() { return constant370; }
ExcelValue results_v45() { return constant371; }
ExcelValue results_w45() { return constant372; }
ExcelValue results_e46() { return constant373; }
ExcelValue results_f46() { return constant374; }
ExcelValue results_g46() { return constant375; }
ExcelValue results_h46() { return constant376; }
ExcelValue results_i46() { return constant377; }
ExcelValue results_j46() { return constant378; }
ExcelValue results_k46() { return constant379; }
ExcelValue results_l46() { return constant380; }
ExcelValue results_m46() { return constant381; }
ExcelValue results_n46() { return constant382; }
ExcelValue results_o46() { return constant383; }
ExcelValue results_p46() { return constant384; }
ExcelValue results_q46() { return constant385; }
ExcelValue results_r46() { return constant386; }
ExcelValue results_s46() { return constant387; }
ExcelValue results_t46() { return constant388; }
ExcelValue results_u46() { return constant389; }
ExcelValue results_v46() { return constant390; }
ExcelValue results_w46() { return constant391; }
ExcelValue results_e47() { return constant392; }
ExcelValue results_f47() { return constant393; }
ExcelValue results_g47() { return constant394; }
ExcelValue results_h47() { return constant395; }
ExcelValue results_i47() { return constant396; }
ExcelValue results_j47() { return constant397; }
ExcelValue results_k47() { return constant398; }
ExcelValue results_l47() { return constant399; }
ExcelValue results_m47() { return constant400; }
ExcelValue results_n47() { return constant401; }
ExcelValue results_o47() { return constant402; }
ExcelValue results_p47() { return constant403; }
ExcelValue results_q47() { return constant404; }
ExcelValue results_r47() { return constant405; }
ExcelValue results_s47() { return constant406; }
ExcelValue results_t47() { return constant407; }
ExcelValue results_u47() { return constant408; }
ExcelValue results_v47() { return constant409; }
ExcelValue results_w47() { return constant410; }
ExcelValue results_e48() { return constant411; }
ExcelValue results_f48() { return constant412; }
ExcelValue results_g48() { return constant413; }
ExcelValue results_h48() { return constant414; }
ExcelValue results_i48() { return constant415; }
ExcelValue results_j48() { return constant416; }
ExcelValue results_k48() { return constant417; }
ExcelValue results_l48() { return constant418; }
ExcelValue results_m48() { return constant419; }
ExcelValue results_n48() { return constant420; }
ExcelValue results_o48() { return constant421; }
ExcelValue results_p48() { return constant422; }
ExcelValue results_q48() { return constant423; }
ExcelValue results_r48() { return constant424; }
ExcelValue results_s48() { return constant425; }
ExcelValue results_t48() { return constant426; }
ExcelValue results_u48() { return constant427; }
ExcelValue results_v48() { return constant428; }
ExcelValue results_w48() { return constant429; }
ExcelValue results_e49() { return constant430; }
ExcelValue results_f49() {
  static ExcelValue result;
  if(variable_set[1108] == 1) { return result;}
  result = ZERO;
  variable_set[1108] = 1;
  return result;
}

ExcelValue results_g49() {
  static ExcelValue result;
  if(variable_set[1109] == 1) { return result;}
  result = ZERO;
  variable_set[1109] = 1;
  return result;
}

ExcelValue results_h49() {
  static ExcelValue result;
  if(variable_set[1110] == 1) { return result;}
  result = ZERO;
  variable_set[1110] = 1;
  return result;
}

ExcelValue results_i49() {
  static ExcelValue result;
  if(variable_set[1111] == 1) { return result;}
  result = ZERO;
  variable_set[1111] = 1;
  return result;
}

ExcelValue results_j49() {
  static ExcelValue result;
  if(variable_set[1112] == 1) { return result;}
  result = ZERO;
  variable_set[1112] = 1;
  return result;
}

ExcelValue results_k49() {
  static ExcelValue result;
  if(variable_set[1113] == 1) { return result;}
  result = ZERO;
  variable_set[1113] = 1;
  return result;
}

ExcelValue results_l49() {
  static ExcelValue result;
  if(variable_set[1114] == 1) { return result;}
  result = ZERO;
  variable_set[1114] = 1;
  return result;
}

ExcelValue results_m49() {
  static ExcelValue result;
  if(variable_set[1115] == 1) { return result;}
  result = ZERO;
  variable_set[1115] = 1;
  return result;
}

ExcelValue results_n49() {
  static ExcelValue result;
  if(variable_set[1116] == 1) { return result;}
  result = ZERO;
  variable_set[1116] = 1;
  return result;
}

ExcelValue results_o49() {
  static ExcelValue result;
  if(variable_set[1117] == 1) { return result;}
  result = ZERO;
  variable_set[1117] = 1;
  return result;
}

ExcelValue results_p49() {
  static ExcelValue result;
  if(variable_set[1118] == 1) { return result;}
  result = ZERO;
  variable_set[1118] = 1;
  return result;
}

ExcelValue results_q49() {
  static ExcelValue result;
  if(variable_set[1119] == 1) { return result;}
  result = ZERO;
  variable_set[1119] = 1;
  return result;
}

ExcelValue results_r49() {
  static ExcelValue result;
  if(variable_set[1120] == 1) { return result;}
  result = ZERO;
  variable_set[1120] = 1;
  return result;
}

ExcelValue results_s49() {
  static ExcelValue result;
  if(variable_set[1121] == 1) { return result;}
  result = ZERO;
  variable_set[1121] = 1;
  return result;
}

ExcelValue results_t49() {
  static ExcelValue result;
  if(variable_set[1122] == 1) { return result;}
  result = ZERO;
  variable_set[1122] = 1;
  return result;
}

ExcelValue results_u49() {
  static ExcelValue result;
  if(variable_set[1123] == 1) { return result;}
  result = ZERO;
  variable_set[1123] = 1;
  return result;
}

ExcelValue results_v49() {
  static ExcelValue result;
  if(variable_set[1124] == 1) { return result;}
  result = ZERO;
  variable_set[1124] = 1;
  return result;
}

ExcelValue results_w49() {
  static ExcelValue result;
  if(variable_set[1125] == 1) { return result;}
  result = ZERO;
  variable_set[1125] = 1;
  return result;
}

ExcelValue results_e50() { return constant431; }
ExcelValue results_f50() { return constant432; }
ExcelValue results_g50() { return constant433; }
ExcelValue results_h50() { return constant434; }
ExcelValue results_i50() { return constant435; }
ExcelValue results_j50() { return constant436; }
ExcelValue results_k50() { return constant437; }
ExcelValue results_l50() { return constant438; }
ExcelValue results_m50() { return constant439; }
ExcelValue results_n50() { return constant440; }
ExcelValue results_o50() { return constant441; }
ExcelValue results_p50() { return constant442; }
ExcelValue results_q50() { return constant443; }
ExcelValue results_r50() { return constant444; }
ExcelValue results_s50() { return constant445; }
ExcelValue results_t50() { return constant446; }
ExcelValue results_u50() { return constant447; }
ExcelValue results_v50() { return constant448; }
ExcelValue results_w50() { return constant449; }
ExcelValue results_e51() { return constant450; }
ExcelValue results_f51() {
  static ExcelValue result;
  if(variable_set[1146] == 1) { return result;}
  result = ZERO;
  variable_set[1146] = 1;
  return result;
}

ExcelValue results_g51() {
  static ExcelValue result;
  if(variable_set[1147] == 1) { return result;}
  result = ZERO;
  variable_set[1147] = 1;
  return result;
}

ExcelValue results_h51() {
  static ExcelValue result;
  if(variable_set[1148] == 1) { return result;}
  result = ZERO;
  variable_set[1148] = 1;
  return result;
}

ExcelValue results_i51() {
  static ExcelValue result;
  if(variable_set[1149] == 1) { return result;}
  result = ZERO;
  variable_set[1149] = 1;
  return result;
}

ExcelValue results_j51() {
  static ExcelValue result;
  if(variable_set[1150] == 1) { return result;}
  result = ZERO;
  variable_set[1150] = 1;
  return result;
}

ExcelValue results_k51() {
  static ExcelValue result;
  if(variable_set[1151] == 1) { return result;}
  result = ZERO;
  variable_set[1151] = 1;
  return result;
}

ExcelValue results_l51() {
  static ExcelValue result;
  if(variable_set[1152] == 1) { return result;}
  result = ZERO;
  variable_set[1152] = 1;
  return result;
}

ExcelValue results_m51() {
  static ExcelValue result;
  if(variable_set[1153] == 1) { return result;}
  result = ZERO;
  variable_set[1153] = 1;
  return result;
}

ExcelValue results_n51() {
  static ExcelValue result;
  if(variable_set[1154] == 1) { return result;}
  result = ZERO;
  variable_set[1154] = 1;
  return result;
}

ExcelValue results_o51() {
  static ExcelValue result;
  if(variable_set[1155] == 1) { return result;}
  result = ZERO;
  variable_set[1155] = 1;
  return result;
}

ExcelValue results_p51() {
  static ExcelValue result;
  if(variable_set[1156] == 1) { return result;}
  result = ZERO;
  variable_set[1156] = 1;
  return result;
}

ExcelValue results_q51() {
  static ExcelValue result;
  if(variable_set[1157] == 1) { return result;}
  result = ZERO;
  variable_set[1157] = 1;
  return result;
}

ExcelValue results_r51() {
  static ExcelValue result;
  if(variable_set[1158] == 1) { return result;}
  result = ZERO;
  variable_set[1158] = 1;
  return result;
}

ExcelValue results_s51() {
  static ExcelValue result;
  if(variable_set[1159] == 1) { return result;}
  result = ZERO;
  variable_set[1159] = 1;
  return result;
}

ExcelValue results_t51() {
  static ExcelValue result;
  if(variable_set[1160] == 1) { return result;}
  result = ZERO;
  variable_set[1160] = 1;
  return result;
}

ExcelValue results_u51() {
  static ExcelValue result;
  if(variable_set[1161] == 1) { return result;}
  result = ZERO;
  variable_set[1161] = 1;
  return result;
}

ExcelValue results_v51() {
  static ExcelValue result;
  if(variable_set[1162] == 1) { return result;}
  result = ZERO;
  variable_set[1162] = 1;
  return result;
}

ExcelValue results_w51() {
  static ExcelValue result;
  if(variable_set[1163] == 1) { return result;}
  result = ZERO;
  variable_set[1163] = 1;
  return result;
}

ExcelValue results_e52() { return constant451; }
ExcelValue results_f52() { return constant452; }
ExcelValue results_g52() { return constant453; }
ExcelValue results_h52() { return constant454; }
ExcelValue results_i52() { return constant455; }
ExcelValue results_j52() { return constant456; }
ExcelValue results_k52() { return constant457; }
ExcelValue results_l52() { return constant458; }
ExcelValue results_m52() { return constant459; }
ExcelValue results_n52() { return constant460; }
ExcelValue results_o52() { return constant461; }
ExcelValue results_p52() { return constant462; }
ExcelValue results_q52() { return constant463; }
ExcelValue results_r52() { return constant464; }
ExcelValue results_s52() { return constant465; }
ExcelValue results_t52() { return constant466; }
ExcelValue results_u52() { return constant467; }
ExcelValue results_v52() { return constant468; }
ExcelValue results_w52() { return constant469; }
ExcelValue results_e53() { return constant470; }
ExcelValue results_f53() {
  static ExcelValue result;
  if(variable_set[1184] == 1) { return result;}
  result = ZERO;
  variable_set[1184] = 1;
  return result;
}

ExcelValue results_g53() {
  static ExcelValue result;
  if(variable_set[1185] == 1) { return result;}
  result = ZERO;
  variable_set[1185] = 1;
  return result;
}

ExcelValue results_h53() {
  static ExcelValue result;
  if(variable_set[1186] == 1) { return result;}
  result = ZERO;
  variable_set[1186] = 1;
  return result;
}

ExcelValue results_i53() {
  static ExcelValue result;
  if(variable_set[1187] == 1) { return result;}
  result = ZERO;
  variable_set[1187] = 1;
  return result;
}

ExcelValue results_j53() {
  static ExcelValue result;
  if(variable_set[1188] == 1) { return result;}
  result = ZERO;
  variable_set[1188] = 1;
  return result;
}

ExcelValue results_k53() {
  static ExcelValue result;
  if(variable_set[1189] == 1) { return result;}
  result = ZERO;
  variable_set[1189] = 1;
  return result;
}

ExcelValue results_l53() {
  static ExcelValue result;
  if(variable_set[1190] == 1) { return result;}
  result = ZERO;
  variable_set[1190] = 1;
  return result;
}

ExcelValue results_m53() {
  static ExcelValue result;
  if(variable_set[1191] == 1) { return result;}
  result = ZERO;
  variable_set[1191] = 1;
  return result;
}

ExcelValue results_n53() {
  static ExcelValue result;
  if(variable_set[1192] == 1) { return result;}
  result = ZERO;
  variable_set[1192] = 1;
  return result;
}

ExcelValue results_o53() {
  static ExcelValue result;
  if(variable_set[1193] == 1) { return result;}
  result = ZERO;
  variable_set[1193] = 1;
  return result;
}

ExcelValue results_p53() {
  static ExcelValue result;
  if(variable_set[1194] == 1) { return result;}
  result = ZERO;
  variable_set[1194] = 1;
  return result;
}

ExcelValue results_q53() {
  static ExcelValue result;
  if(variable_set[1195] == 1) { return result;}
  result = ZERO;
  variable_set[1195] = 1;
  return result;
}

ExcelValue results_r53() {
  static ExcelValue result;
  if(variable_set[1196] == 1) { return result;}
  result = ZERO;
  variable_set[1196] = 1;
  return result;
}

ExcelValue results_s53() {
  static ExcelValue result;
  if(variable_set[1197] == 1) { return result;}
  result = ZERO;
  variable_set[1197] = 1;
  return result;
}

ExcelValue results_t53() {
  static ExcelValue result;
  if(variable_set[1198] == 1) { return result;}
  result = ZERO;
  variable_set[1198] = 1;
  return result;
}

ExcelValue results_u53() {
  static ExcelValue result;
  if(variable_set[1199] == 1) { return result;}
  result = ZERO;
  variable_set[1199] = 1;
  return result;
}

ExcelValue results_v53() {
  static ExcelValue result;
  if(variable_set[1200] == 1) { return result;}
  result = ZERO;
  variable_set[1200] = 1;
  return result;
}

ExcelValue results_w53() {
  static ExcelValue result;
  if(variable_set[1201] == 1) { return result;}
  result = ZERO;
  variable_set[1201] = 1;
  return result;
}

ExcelValue results_e54() { return constant471; }
ExcelValue results_f54() {
  static ExcelValue result;
  if(variable_set[1203] == 1) { return result;}
  result = ZERO;
  variable_set[1203] = 1;
  return result;
}

ExcelValue results_g54() {
  static ExcelValue result;
  if(variable_set[1204] == 1) { return result;}
  result = ZERO;
  variable_set[1204] = 1;
  return result;
}

ExcelValue results_h54() {
  static ExcelValue result;
  if(variable_set[1205] == 1) { return result;}
  result = ZERO;
  variable_set[1205] = 1;
  return result;
}

ExcelValue results_i54() {
  static ExcelValue result;
  if(variable_set[1206] == 1) { return result;}
  result = ZERO;
  variable_set[1206] = 1;
  return result;
}

ExcelValue results_j54() {
  static ExcelValue result;
  if(variable_set[1207] == 1) { return result;}
  result = ZERO;
  variable_set[1207] = 1;
  return result;
}

ExcelValue results_k54() {
  static ExcelValue result;
  if(variable_set[1208] == 1) { return result;}
  result = ZERO;
  variable_set[1208] = 1;
  return result;
}

ExcelValue results_l54() {
  static ExcelValue result;
  if(variable_set[1209] == 1) { return result;}
  result = ZERO;
  variable_set[1209] = 1;
  return result;
}

ExcelValue results_m54() {
  static ExcelValue result;
  if(variable_set[1210] == 1) { return result;}
  result = ZERO;
  variable_set[1210] = 1;
  return result;
}

ExcelValue results_n54() {
  static ExcelValue result;
  if(variable_set[1211] == 1) { return result;}
  result = ZERO;
  variable_set[1211] = 1;
  return result;
}

ExcelValue results_o54() {
  static ExcelValue result;
  if(variable_set[1212] == 1) { return result;}
  result = ZERO;
  variable_set[1212] = 1;
  return result;
}

ExcelValue results_p54() {
  static ExcelValue result;
  if(variable_set[1213] == 1) { return result;}
  result = ZERO;
  variable_set[1213] = 1;
  return result;
}

ExcelValue results_q54() {
  static ExcelValue result;
  if(variable_set[1214] == 1) { return result;}
  result = ZERO;
  variable_set[1214] = 1;
  return result;
}

ExcelValue results_r54() {
  static ExcelValue result;
  if(variable_set[1215] == 1) { return result;}
  result = ZERO;
  variable_set[1215] = 1;
  return result;
}

ExcelValue results_s54() {
  static ExcelValue result;
  if(variable_set[1216] == 1) { return result;}
  result = ZERO;
  variable_set[1216] = 1;
  return result;
}

ExcelValue results_t54() {
  static ExcelValue result;
  if(variable_set[1217] == 1) { return result;}
  result = ZERO;
  variable_set[1217] = 1;
  return result;
}

ExcelValue results_u54() {
  static ExcelValue result;
  if(variable_set[1218] == 1) { return result;}
  result = ZERO;
  variable_set[1218] = 1;
  return result;
}

ExcelValue results_v54() {
  static ExcelValue result;
  if(variable_set[1219] == 1) { return result;}
  result = ZERO;
  variable_set[1219] = 1;
  return result;
}

ExcelValue results_w54() {
  static ExcelValue result;
  if(variable_set[1220] == 1) { return result;}
  result = ZERO;
  variable_set[1220] = 1;
  return result;
}

ExcelValue results_e55() { return constant472; }
ExcelValue results_f55() {
  static ExcelValue result;
  if(variable_set[1222] == 1) { return result;}
  result = ZERO;
  variable_set[1222] = 1;
  return result;
}

ExcelValue results_g55() {
  static ExcelValue result;
  if(variable_set[1223] == 1) { return result;}
  result = ZERO;
  variable_set[1223] = 1;
  return result;
}

ExcelValue results_h55() {
  static ExcelValue result;
  if(variable_set[1224] == 1) { return result;}
  result = ZERO;
  variable_set[1224] = 1;
  return result;
}

ExcelValue results_i55() {
  static ExcelValue result;
  if(variable_set[1225] == 1) { return result;}
  result = ZERO;
  variable_set[1225] = 1;
  return result;
}

ExcelValue results_j55() {
  static ExcelValue result;
  if(variable_set[1226] == 1) { return result;}
  result = ZERO;
  variable_set[1226] = 1;
  return result;
}

ExcelValue results_k55() {
  static ExcelValue result;
  if(variable_set[1227] == 1) { return result;}
  result = ZERO;
  variable_set[1227] = 1;
  return result;
}

ExcelValue results_l55() {
  static ExcelValue result;
  if(variable_set[1228] == 1) { return result;}
  result = ZERO;
  variable_set[1228] = 1;
  return result;
}

ExcelValue results_m55() {
  static ExcelValue result;
  if(variable_set[1229] == 1) { return result;}
  result = ZERO;
  variable_set[1229] = 1;
  return result;
}

ExcelValue results_n55() {
  static ExcelValue result;
  if(variable_set[1230] == 1) { return result;}
  result = ZERO;
  variable_set[1230] = 1;
  return result;
}

ExcelValue results_o55() {
  static ExcelValue result;
  if(variable_set[1231] == 1) { return result;}
  result = ZERO;
  variable_set[1231] = 1;
  return result;
}

ExcelValue results_p55() {
  static ExcelValue result;
  if(variable_set[1232] == 1) { return result;}
  result = ZERO;
  variable_set[1232] = 1;
  return result;
}

ExcelValue results_q55() {
  static ExcelValue result;
  if(variable_set[1233] == 1) { return result;}
  result = ZERO;
  variable_set[1233] = 1;
  return result;
}

ExcelValue results_r55() {
  static ExcelValue result;
  if(variable_set[1234] == 1) { return result;}
  result = ZERO;
  variable_set[1234] = 1;
  return result;
}

ExcelValue results_s55() {
  static ExcelValue result;
  if(variable_set[1235] == 1) { return result;}
  result = ZERO;
  variable_set[1235] = 1;
  return result;
}

ExcelValue results_t55() {
  static ExcelValue result;
  if(variable_set[1236] == 1) { return result;}
  result = ZERO;
  variable_set[1236] = 1;
  return result;
}

ExcelValue results_u55() {
  static ExcelValue result;
  if(variable_set[1237] == 1) { return result;}
  result = ZERO;
  variable_set[1237] = 1;
  return result;
}

ExcelValue results_v55() {
  static ExcelValue result;
  if(variable_set[1238] == 1) { return result;}
  result = ZERO;
  variable_set[1238] = 1;
  return result;
}

ExcelValue results_w55() {
  static ExcelValue result;
  if(variable_set[1239] == 1) { return result;}
  result = ZERO;
  variable_set[1239] = 1;
  return result;
}

ExcelValue results_e56() { return constant473; }
ExcelValue results_f56() { return constant474; }
ExcelValue results_g56() { return constant475; }
ExcelValue results_h56() { return constant476; }
ExcelValue results_i56() { return constant477; }
ExcelValue results_j56() { return constant478; }
ExcelValue results_k56() { return constant478; }
ExcelValue results_l56() { return constant478; }
ExcelValue results_m56() { return constant478; }
ExcelValue results_n56() { return constant478; }
ExcelValue results_o56() { return constant478; }
ExcelValue results_p56() { return constant478; }
ExcelValue results_q56() { return constant478; }
ExcelValue results_r56() { return constant478; }
ExcelValue results_s56() { return constant478; }
ExcelValue results_t56() { return constant478; }
ExcelValue results_u56() { return constant478; }
ExcelValue results_v56() { return constant478; }
ExcelValue results_w56() { return constant478; }
ExcelValue results_e57() { return constant479; }
ExcelValue results_f57() { return constant480; }
ExcelValue results_g57() { return constant481; }
ExcelValue results_h57() { return constant482; }
ExcelValue results_i57() { return constant483; }
ExcelValue results_j57() { return constant484; }
ExcelValue results_k57() { return constant484; }
ExcelValue results_l57() { return constant484; }
ExcelValue results_m57() { return constant484; }
ExcelValue results_n57() { return constant484; }
ExcelValue results_o57() { return constant484; }
ExcelValue results_p57() { return constant484; }
ExcelValue results_q57() { return constant484; }
ExcelValue results_r57() { return constant484; }
ExcelValue results_s57() { return constant484; }
ExcelValue results_t57() { return constant484; }
ExcelValue results_u57() { return constant484; }
ExcelValue results_v57() { return constant484; }
ExcelValue results_w57() { return constant484; }
ExcelValue results_e58() { return constant485; }
ExcelValue results_f58() {
  static ExcelValue result;
  if(variable_set[1279] == 1) { return result;}
  result = ZERO;
  variable_set[1279] = 1;
  return result;
}

ExcelValue results_g58() {
  static ExcelValue result;
  if(variable_set[1280] == 1) { return result;}
  result = ZERO;
  variable_set[1280] = 1;
  return result;
}

ExcelValue results_h58() {
  static ExcelValue result;
  if(variable_set[1281] == 1) { return result;}
  result = ZERO;
  variable_set[1281] = 1;
  return result;
}

ExcelValue results_i58() {
  static ExcelValue result;
  if(variable_set[1282] == 1) { return result;}
  result = ZERO;
  variable_set[1282] = 1;
  return result;
}

ExcelValue results_j58() {
  static ExcelValue result;
  if(variable_set[1283] == 1) { return result;}
  result = ZERO;
  variable_set[1283] = 1;
  return result;
}

ExcelValue results_k58() {
  static ExcelValue result;
  if(variable_set[1284] == 1) { return result;}
  result = ZERO;
  variable_set[1284] = 1;
  return result;
}

ExcelValue results_l58() {
  static ExcelValue result;
  if(variable_set[1285] == 1) { return result;}
  result = ZERO;
  variable_set[1285] = 1;
  return result;
}

ExcelValue results_m58() {
  static ExcelValue result;
  if(variable_set[1286] == 1) { return result;}
  result = ZERO;
  variable_set[1286] = 1;
  return result;
}

ExcelValue results_n58() {
  static ExcelValue result;
  if(variable_set[1287] == 1) { return result;}
  result = ZERO;
  variable_set[1287] = 1;
  return result;
}

ExcelValue results_o58() {
  static ExcelValue result;
  if(variable_set[1288] == 1) { return result;}
  result = ZERO;
  variable_set[1288] = 1;
  return result;
}

ExcelValue results_p58() {
  static ExcelValue result;
  if(variable_set[1289] == 1) { return result;}
  result = ZERO;
  variable_set[1289] = 1;
  return result;
}

ExcelValue results_q58() {
  static ExcelValue result;
  if(variable_set[1290] == 1) { return result;}
  result = ZERO;
  variable_set[1290] = 1;
  return result;
}

ExcelValue results_r58() {
  static ExcelValue result;
  if(variable_set[1291] == 1) { return result;}
  result = ZERO;
  variable_set[1291] = 1;
  return result;
}

ExcelValue results_s58() {
  static ExcelValue result;
  if(variable_set[1292] == 1) { return result;}
  result = ZERO;
  variable_set[1292] = 1;
  return result;
}

ExcelValue results_t58() {
  static ExcelValue result;
  if(variable_set[1293] == 1) { return result;}
  result = ZERO;
  variable_set[1293] = 1;
  return result;
}

ExcelValue results_u58() {
  static ExcelValue result;
  if(variable_set[1294] == 1) { return result;}
  result = ZERO;
  variable_set[1294] = 1;
  return result;
}

ExcelValue results_v58() {
  static ExcelValue result;
  if(variable_set[1295] == 1) { return result;}
  result = ZERO;
  variable_set[1295] = 1;
  return result;
}

ExcelValue results_w58() {
  static ExcelValue result;
  if(variable_set[1296] == 1) { return result;}
  result = ZERO;
  variable_set[1296] = 1;
  return result;
}

ExcelValue results_e59() { return constant486; }
ExcelValue results_f59() {
  static ExcelValue result;
  if(variable_set[1298] == 1) { return result;}
  result = ZERO;
  variable_set[1298] = 1;
  return result;
}

ExcelValue results_g59() {
  static ExcelValue result;
  if(variable_set[1299] == 1) { return result;}
  result = ZERO;
  variable_set[1299] = 1;
  return result;
}

ExcelValue results_h59() {
  static ExcelValue result;
  if(variable_set[1300] == 1) { return result;}
  result = ZERO;
  variable_set[1300] = 1;
  return result;
}

ExcelValue results_i59() {
  static ExcelValue result;
  if(variable_set[1301] == 1) { return result;}
  result = ZERO;
  variable_set[1301] = 1;
  return result;
}

ExcelValue results_j59() {
  static ExcelValue result;
  if(variable_set[1302] == 1) { return result;}
  result = ZERO;
  variable_set[1302] = 1;
  return result;
}

ExcelValue results_k59() {
  static ExcelValue result;
  if(variable_set[1303] == 1) { return result;}
  result = ZERO;
  variable_set[1303] = 1;
  return result;
}

ExcelValue results_l59() {
  static ExcelValue result;
  if(variable_set[1304] == 1) { return result;}
  result = ZERO;
  variable_set[1304] = 1;
  return result;
}

ExcelValue results_m59() {
  static ExcelValue result;
  if(variable_set[1305] == 1) { return result;}
  result = ZERO;
  variable_set[1305] = 1;
  return result;
}

ExcelValue results_n59() {
  static ExcelValue result;
  if(variable_set[1306] == 1) { return result;}
  result = ZERO;
  variable_set[1306] = 1;
  return result;
}

ExcelValue results_o59() {
  static ExcelValue result;
  if(variable_set[1307] == 1) { return result;}
  result = ZERO;
  variable_set[1307] = 1;
  return result;
}

ExcelValue results_p59() {
  static ExcelValue result;
  if(variable_set[1308] == 1) { return result;}
  result = ZERO;
  variable_set[1308] = 1;
  return result;
}

ExcelValue results_q59() {
  static ExcelValue result;
  if(variable_set[1309] == 1) { return result;}
  result = ZERO;
  variable_set[1309] = 1;
  return result;
}

ExcelValue results_r59() {
  static ExcelValue result;
  if(variable_set[1310] == 1) { return result;}
  result = ZERO;
  variable_set[1310] = 1;
  return result;
}

ExcelValue results_s59() {
  static ExcelValue result;
  if(variable_set[1311] == 1) { return result;}
  result = ZERO;
  variable_set[1311] = 1;
  return result;
}

ExcelValue results_t59() {
  static ExcelValue result;
  if(variable_set[1312] == 1) { return result;}
  result = ZERO;
  variable_set[1312] = 1;
  return result;
}

ExcelValue results_u59() {
  static ExcelValue result;
  if(variable_set[1313] == 1) { return result;}
  result = ZERO;
  variable_set[1313] = 1;
  return result;
}

ExcelValue results_v59() {
  static ExcelValue result;
  if(variable_set[1314] == 1) { return result;}
  result = ZERO;
  variable_set[1314] = 1;
  return result;
}

ExcelValue results_w59() {
  static ExcelValue result;
  if(variable_set[1315] == 1) { return result;}
  result = ZERO;
  variable_set[1315] = 1;
  return result;
}

ExcelValue results_e60() { return constant487; }
ExcelValue results_f60() {
  static ExcelValue result;
  if(variable_set[1317] == 1) { return result;}
  result = ZERO;
  variable_set[1317] = 1;
  return result;
}

ExcelValue results_g60() {
  static ExcelValue result;
  if(variable_set[1318] == 1) { return result;}
  result = ZERO;
  variable_set[1318] = 1;
  return result;
}

ExcelValue results_h60() {
  static ExcelValue result;
  if(variable_set[1319] == 1) { return result;}
  result = ZERO;
  variable_set[1319] = 1;
  return result;
}

ExcelValue results_i60() {
  static ExcelValue result;
  if(variable_set[1320] == 1) { return result;}
  result = ZERO;
  variable_set[1320] = 1;
  return result;
}

ExcelValue results_j60() {
  static ExcelValue result;
  if(variable_set[1321] == 1) { return result;}
  result = ZERO;
  variable_set[1321] = 1;
  return result;
}

ExcelValue results_k60() {
  static ExcelValue result;
  if(variable_set[1322] == 1) { return result;}
  result = ZERO;
  variable_set[1322] = 1;
  return result;
}

ExcelValue results_l60() {
  static ExcelValue result;
  if(variable_set[1323] == 1) { return result;}
  result = ZERO;
  variable_set[1323] = 1;
  return result;
}

ExcelValue results_m60() {
  static ExcelValue result;
  if(variable_set[1324] == 1) { return result;}
  result = ZERO;
  variable_set[1324] = 1;
  return result;
}

ExcelValue results_n60() {
  static ExcelValue result;
  if(variable_set[1325] == 1) { return result;}
  result = ZERO;
  variable_set[1325] = 1;
  return result;
}

ExcelValue results_o60() {
  static ExcelValue result;
  if(variable_set[1326] == 1) { return result;}
  result = ZERO;
  variable_set[1326] = 1;
  return result;
}

ExcelValue results_p60() {
  static ExcelValue result;
  if(variable_set[1327] == 1) { return result;}
  result = ZERO;
  variable_set[1327] = 1;
  return result;
}

ExcelValue results_q60() {
  static ExcelValue result;
  if(variable_set[1328] == 1) { return result;}
  result = ZERO;
  variable_set[1328] = 1;
  return result;
}

ExcelValue results_r60() {
  static ExcelValue result;
  if(variable_set[1329] == 1) { return result;}
  result = ZERO;
  variable_set[1329] = 1;
  return result;
}

ExcelValue results_s60() {
  static ExcelValue result;
  if(variable_set[1330] == 1) { return result;}
  result = ZERO;
  variable_set[1330] = 1;
  return result;
}

ExcelValue results_t60() {
  static ExcelValue result;
  if(variable_set[1331] == 1) { return result;}
  result = ZERO;
  variable_set[1331] = 1;
  return result;
}

ExcelValue results_u60() {
  static ExcelValue result;
  if(variable_set[1332] == 1) { return result;}
  result = ZERO;
  variable_set[1332] = 1;
  return result;
}

ExcelValue results_v60() {
  static ExcelValue result;
  if(variable_set[1333] == 1) { return result;}
  result = ZERO;
  variable_set[1333] = 1;
  return result;
}

ExcelValue results_w60() {
  static ExcelValue result;
  if(variable_set[1334] == 1) { return result;}
  result = ZERO;
  variable_set[1334] = 1;
  return result;
}

ExcelValue results_e61() { return constant488; }
ExcelValue results_f61() {
  static ExcelValue result;
  if(variable_set[1336] == 1) { return result;}
  result = ZERO;
  variable_set[1336] = 1;
  return result;
}

ExcelValue results_g61() {
  static ExcelValue result;
  if(variable_set[1337] == 1) { return result;}
  result = ZERO;
  variable_set[1337] = 1;
  return result;
}

ExcelValue results_h61() {
  static ExcelValue result;
  if(variable_set[1338] == 1) { return result;}
  result = ZERO;
  variable_set[1338] = 1;
  return result;
}

ExcelValue results_i61() {
  static ExcelValue result;
  if(variable_set[1339] == 1) { return result;}
  result = ZERO;
  variable_set[1339] = 1;
  return result;
}

ExcelValue results_j61() {
  static ExcelValue result;
  if(variable_set[1340] == 1) { return result;}
  result = ZERO;
  variable_set[1340] = 1;
  return result;
}

ExcelValue results_k61() {
  static ExcelValue result;
  if(variable_set[1341] == 1) { return result;}
  result = ZERO;
  variable_set[1341] = 1;
  return result;
}

ExcelValue results_l61() {
  static ExcelValue result;
  if(variable_set[1342] == 1) { return result;}
  result = ZERO;
  variable_set[1342] = 1;
  return result;
}

ExcelValue results_m61() {
  static ExcelValue result;
  if(variable_set[1343] == 1) { return result;}
  result = ZERO;
  variable_set[1343] = 1;
  return result;
}

ExcelValue results_n61() {
  static ExcelValue result;
  if(variable_set[1344] == 1) { return result;}
  result = ZERO;
  variable_set[1344] = 1;
  return result;
}

ExcelValue results_o61() {
  static ExcelValue result;
  if(variable_set[1345] == 1) { return result;}
  result = ZERO;
  variable_set[1345] = 1;
  return result;
}

ExcelValue results_p61() {
  static ExcelValue result;
  if(variable_set[1346] == 1) { return result;}
  result = ZERO;
  variable_set[1346] = 1;
  return result;
}

ExcelValue results_q61() {
  static ExcelValue result;
  if(variable_set[1347] == 1) { return result;}
  result = ZERO;
  variable_set[1347] = 1;
  return result;
}

ExcelValue results_r61() {
  static ExcelValue result;
  if(variable_set[1348] == 1) { return result;}
  result = ZERO;
  variable_set[1348] = 1;
  return result;
}

ExcelValue results_s61() {
  static ExcelValue result;
  if(variable_set[1349] == 1) { return result;}
  result = ZERO;
  variable_set[1349] = 1;
  return result;
}

ExcelValue results_t61() {
  static ExcelValue result;
  if(variable_set[1350] == 1) { return result;}
  result = ZERO;
  variable_set[1350] = 1;
  return result;
}

ExcelValue results_u61() {
  static ExcelValue result;
  if(variable_set[1351] == 1) { return result;}
  result = ZERO;
  variable_set[1351] = 1;
  return result;
}

ExcelValue results_v61() {
  static ExcelValue result;
  if(variable_set[1352] == 1) { return result;}
  result = ZERO;
  variable_set[1352] = 1;
  return result;
}

ExcelValue results_w61() {
  static ExcelValue result;
  if(variable_set[1353] == 1) { return result;}
  result = ZERO;
  variable_set[1353] = 1;
  return result;
}

ExcelValue results_e62() { return constant489; }
ExcelValue results_f62() {
  static ExcelValue result;
  if(variable_set[1355] == 1) { return result;}
  result = ZERO;
  variable_set[1355] = 1;
  return result;
}

ExcelValue results_g62() {
  static ExcelValue result;
  if(variable_set[1356] == 1) { return result;}
  result = ZERO;
  variable_set[1356] = 1;
  return result;
}

ExcelValue results_h62() {
  static ExcelValue result;
  if(variable_set[1357] == 1) { return result;}
  result = ZERO;
  variable_set[1357] = 1;
  return result;
}

ExcelValue results_i62() {
  static ExcelValue result;
  if(variable_set[1358] == 1) { return result;}
  result = ZERO;
  variable_set[1358] = 1;
  return result;
}

ExcelValue results_j62() {
  static ExcelValue result;
  if(variable_set[1359] == 1) { return result;}
  result = ZERO;
  variable_set[1359] = 1;
  return result;
}

ExcelValue results_k62() {
  static ExcelValue result;
  if(variable_set[1360] == 1) { return result;}
  result = ZERO;
  variable_set[1360] = 1;
  return result;
}

ExcelValue results_l62() {
  static ExcelValue result;
  if(variable_set[1361] == 1) { return result;}
  result = ZERO;
  variable_set[1361] = 1;
  return result;
}

ExcelValue results_m62() {
  static ExcelValue result;
  if(variable_set[1362] == 1) { return result;}
  result = ZERO;
  variable_set[1362] = 1;
  return result;
}

ExcelValue results_n62() {
  static ExcelValue result;
  if(variable_set[1363] == 1) { return result;}
  result = ZERO;
  variable_set[1363] = 1;
  return result;
}

ExcelValue results_o62() {
  static ExcelValue result;
  if(variable_set[1364] == 1) { return result;}
  result = ZERO;
  variable_set[1364] = 1;
  return result;
}

ExcelValue results_p62() {
  static ExcelValue result;
  if(variable_set[1365] == 1) { return result;}
  result = ZERO;
  variable_set[1365] = 1;
  return result;
}

ExcelValue results_q62() {
  static ExcelValue result;
  if(variable_set[1366] == 1) { return result;}
  result = ZERO;
  variable_set[1366] = 1;
  return result;
}

ExcelValue results_r62() {
  static ExcelValue result;
  if(variable_set[1367] == 1) { return result;}
  result = ZERO;
  variable_set[1367] = 1;
  return result;
}

ExcelValue results_s62() {
  static ExcelValue result;
  if(variable_set[1368] == 1) { return result;}
  result = ZERO;
  variable_set[1368] = 1;
  return result;
}

ExcelValue results_t62() {
  static ExcelValue result;
  if(variable_set[1369] == 1) { return result;}
  result = ZERO;
  variable_set[1369] = 1;
  return result;
}

ExcelValue results_u62() {
  static ExcelValue result;
  if(variable_set[1370] == 1) { return result;}
  result = ZERO;
  variable_set[1370] = 1;
  return result;
}

ExcelValue results_v62() {
  static ExcelValue result;
  if(variable_set[1371] == 1) { return result;}
  result = ZERO;
  variable_set[1371] = 1;
  return result;
}

ExcelValue results_w62() {
  static ExcelValue result;
  if(variable_set[1372] == 1) { return result;}
  result = ZERO;
  variable_set[1372] = 1;
  return result;
}

ExcelValue results_e63() { return constant490; }
ExcelValue results_f63() { return constant491; }
ExcelValue results_g63() { return constant491; }
ExcelValue results_h63() { return constant492; }
ExcelValue results_i63() { return constant493; }
ExcelValue results_j63() { return constant494; }
ExcelValue results_k63() { return constant495; }
ExcelValue results_l63() {
  static ExcelValue result;
  if(variable_set[1380] == 1) { return result;}
  result = ZERO;
  variable_set[1380] = 1;
  return result;
}

ExcelValue results_m63() {
  static ExcelValue result;
  if(variable_set[1381] == 1) { return result;}
  result = ZERO;
  variable_set[1381] = 1;
  return result;
}

ExcelValue results_n63() { return constant496; }
ExcelValue results_o63() { return constant497; }
ExcelValue results_p63() { return constant496; }
ExcelValue results_q63() { return constant497; }
ExcelValue results_r63() {
  static ExcelValue result;
  if(variable_set[1386] == 1) { return result;}
  result = ZERO;
  variable_set[1386] = 1;
  return result;
}

ExcelValue results_s63() { return constant496; }
ExcelValue results_t63() {
  static ExcelValue result;
  if(variable_set[1388] == 1) { return result;}
  result = ZERO;
  variable_set[1388] = 1;
  return result;
}

ExcelValue results_u63() { return constant496; }
ExcelValue results_v63() {
  static ExcelValue result;
  if(variable_set[1390] == 1) { return result;}
  result = ZERO;
  variable_set[1390] = 1;
  return result;
}

ExcelValue results_w63() {
  static ExcelValue result;
  if(variable_set[1391] == 1) { return result;}
  result = ZERO;
  variable_set[1391] = 1;
  return result;
}

ExcelValue results_e64() { return constant498; }
ExcelValue results_f64() { return constant499; }
ExcelValue results_g64() { return constant500; }
ExcelValue results_h64() { return constant501; }
ExcelValue results_i64() {
  static ExcelValue result;
  if(variable_set[1396] == 1) { return result;}
  result = ZERO;
  variable_set[1396] = 1;
  return result;
}

ExcelValue results_j64() {
  static ExcelValue result;
  if(variable_set[1397] == 1) { return result;}
  result = ZERO;
  variable_set[1397] = 1;
  return result;
}

ExcelValue results_k64() {
  static ExcelValue result;
  if(variable_set[1398] == 1) { return result;}
  result = ZERO;
  variable_set[1398] = 1;
  return result;
}

ExcelValue results_l64() {
  static ExcelValue result;
  if(variable_set[1399] == 1) { return result;}
  result = ZERO;
  variable_set[1399] = 1;
  return result;
}

ExcelValue results_m64() {
  static ExcelValue result;
  if(variable_set[1400] == 1) { return result;}
  result = ZERO;
  variable_set[1400] = 1;
  return result;
}

ExcelValue results_n64() {
  static ExcelValue result;
  if(variable_set[1401] == 1) { return result;}
  result = ZERO;
  variable_set[1401] = 1;
  return result;
}

ExcelValue results_o64() {
  static ExcelValue result;
  if(variable_set[1402] == 1) { return result;}
  result = ZERO;
  variable_set[1402] = 1;
  return result;
}

ExcelValue results_p64() {
  static ExcelValue result;
  if(variable_set[1403] == 1) { return result;}
  result = ZERO;
  variable_set[1403] = 1;
  return result;
}

ExcelValue results_q64() {
  static ExcelValue result;
  if(variable_set[1404] == 1) { return result;}
  result = ZERO;
  variable_set[1404] = 1;
  return result;
}

ExcelValue results_r64() {
  static ExcelValue result;
  if(variable_set[1405] == 1) { return result;}
  result = ZERO;
  variable_set[1405] = 1;
  return result;
}

ExcelValue results_s64() {
  static ExcelValue result;
  if(variable_set[1406] == 1) { return result;}
  result = ZERO;
  variable_set[1406] = 1;
  return result;
}

ExcelValue results_t64() {
  static ExcelValue result;
  if(variable_set[1407] == 1) { return result;}
  result = ZERO;
  variable_set[1407] = 1;
  return result;
}

ExcelValue results_u64() {
  static ExcelValue result;
  if(variable_set[1408] == 1) { return result;}
  result = ZERO;
  variable_set[1408] = 1;
  return result;
}

ExcelValue results_v64() {
  static ExcelValue result;
  if(variable_set[1409] == 1) { return result;}
  result = ZERO;
  variable_set[1409] = 1;
  return result;
}

ExcelValue results_w64() {
  static ExcelValue result;
  if(variable_set[1410] == 1) { return result;}
  result = ZERO;
  variable_set[1410] = 1;
  return result;
}

ExcelValue results_e65() { return constant502; }
ExcelValue results_f65() { return constant503; }
ExcelValue results_g65() { return constant504; }
ExcelValue results_h65() { return constant505; }
ExcelValue results_i65() { return constant506; }
ExcelValue results_j65() { return constant507; }
ExcelValue results_k65() { return constant508; }
ExcelValue results_l65() { return constant509; }
ExcelValue results_m65() { return constant510; }
ExcelValue results_n65() { return constant511; }
ExcelValue results_o65() { return constant512; }
ExcelValue results_p65() { return constant513; }
ExcelValue results_q65() { return constant514; }
ExcelValue results_r65() { return constant515; }
ExcelValue results_s65() { return constant516; }
ExcelValue results_t65() { return constant517; }
ExcelValue results_u65() { return constant518; }
ExcelValue results_v65() { return constant519; }
ExcelValue results_w65() { return constant520; }
ExcelValue results_e66() { return constant521; }
ExcelValue results_f66() { return constant522; }
ExcelValue results_g66() { return constant523; }
ExcelValue results_h66() { return constant524; }
ExcelValue results_i66() { return constant525; }
ExcelValue results_j66() { return constant526; }
ExcelValue results_k66() { return constant527; }
ExcelValue results_l66() { return constant528; }
ExcelValue results_m66() { return constant529; }
ExcelValue results_n66() { return constant530; }
ExcelValue results_o66() { return constant531; }
ExcelValue results_p66() { return constant532; }
ExcelValue results_q66() { return constant533; }
ExcelValue results_r66() { return constant534; }
ExcelValue results_s66() { return constant535; }
ExcelValue results_t66() { return constant536; }
ExcelValue results_u66() { return constant537; }
ExcelValue results_v66() { return constant538; }
ExcelValue results_w66() { return constant539; }
ExcelValue results_e67() { return constant540; }
ExcelValue results_f67() {
  static ExcelValue result;
  if(variable_set[1450] == 1) { return result;}
  result = ZERO;
  variable_set[1450] = 1;
  return result;
}

ExcelValue results_g67() {
  static ExcelValue result;
  if(variable_set[1451] == 1) { return result;}
  result = ZERO;
  variable_set[1451] = 1;
  return result;
}

ExcelValue results_h67() {
  static ExcelValue result;
  if(variable_set[1452] == 1) { return result;}
  result = ZERO;
  variable_set[1452] = 1;
  return result;
}

ExcelValue results_i67() {
  static ExcelValue result;
  if(variable_set[1453] == 1) { return result;}
  result = ZERO;
  variable_set[1453] = 1;
  return result;
}

ExcelValue results_j67() {
  static ExcelValue result;
  if(variable_set[1454] == 1) { return result;}
  result = ZERO;
  variable_set[1454] = 1;
  return result;
}

ExcelValue results_k67() {
  static ExcelValue result;
  if(variable_set[1455] == 1) { return result;}
  result = ZERO;
  variable_set[1455] = 1;
  return result;
}

ExcelValue results_l67() {
  static ExcelValue result;
  if(variable_set[1456] == 1) { return result;}
  result = ZERO;
  variable_set[1456] = 1;
  return result;
}

ExcelValue results_m67() {
  static ExcelValue result;
  if(variable_set[1457] == 1) { return result;}
  result = ZERO;
  variable_set[1457] = 1;
  return result;
}

ExcelValue results_n67() {
  static ExcelValue result;
  if(variable_set[1458] == 1) { return result;}
  result = ZERO;
  variable_set[1458] = 1;
  return result;
}

ExcelValue results_o67() {
  static ExcelValue result;
  if(variable_set[1459] == 1) { return result;}
  result = ZERO;
  variable_set[1459] = 1;
  return result;
}

ExcelValue results_p67() {
  static ExcelValue result;
  if(variable_set[1460] == 1) { return result;}
  result = ZERO;
  variable_set[1460] = 1;
  return result;
}

ExcelValue results_q67() {
  static ExcelValue result;
  if(variable_set[1461] == 1) { return result;}
  result = ZERO;
  variable_set[1461] = 1;
  return result;
}

ExcelValue results_r67() {
  static ExcelValue result;
  if(variable_set[1462] == 1) { return result;}
  result = ZERO;
  variable_set[1462] = 1;
  return result;
}

ExcelValue results_s67() {
  static ExcelValue result;
  if(variable_set[1463] == 1) { return result;}
  result = ZERO;
  variable_set[1463] = 1;
  return result;
}

ExcelValue results_t67() {
  static ExcelValue result;
  if(variable_set[1464] == 1) { return result;}
  result = ZERO;
  variable_set[1464] = 1;
  return result;
}

ExcelValue results_u67() {
  static ExcelValue result;
  if(variable_set[1465] == 1) { return result;}
  result = ZERO;
  variable_set[1465] = 1;
  return result;
}

ExcelValue results_v67() {
  static ExcelValue result;
  if(variable_set[1466] == 1) { return result;}
  result = ZERO;
  variable_set[1466] = 1;
  return result;
}

ExcelValue results_w67() {
  static ExcelValue result;
  if(variable_set[1467] == 1) { return result;}
  result = ZERO;
  variable_set[1467] = 1;
  return result;
}

ExcelValue results_e68() { return constant541; }
ExcelValue results_f68() {
  static ExcelValue result;
  if(variable_set[1469] == 1) { return result;}
  result = ZERO;
  variable_set[1469] = 1;
  return result;
}

ExcelValue results_g68() {
  static ExcelValue result;
  if(variable_set[1470] == 1) { return result;}
  result = ZERO;
  variable_set[1470] = 1;
  return result;
}

ExcelValue results_h68() {
  static ExcelValue result;
  if(variable_set[1471] == 1) { return result;}
  result = ZERO;
  variable_set[1471] = 1;
  return result;
}

ExcelValue results_i68() {
  static ExcelValue result;
  if(variable_set[1472] == 1) { return result;}
  result = ZERO;
  variable_set[1472] = 1;
  return result;
}

ExcelValue results_j68() {
  static ExcelValue result;
  if(variable_set[1473] == 1) { return result;}
  result = ZERO;
  variable_set[1473] = 1;
  return result;
}

ExcelValue results_k68() {
  static ExcelValue result;
  if(variable_set[1474] == 1) { return result;}
  result = ZERO;
  variable_set[1474] = 1;
  return result;
}

ExcelValue results_l68() {
  static ExcelValue result;
  if(variable_set[1475] == 1) { return result;}
  result = ZERO;
  variable_set[1475] = 1;
  return result;
}

ExcelValue results_m68() {
  static ExcelValue result;
  if(variable_set[1476] == 1) { return result;}
  result = ZERO;
  variable_set[1476] = 1;
  return result;
}

ExcelValue results_n68() {
  static ExcelValue result;
  if(variable_set[1477] == 1) { return result;}
  result = ZERO;
  variable_set[1477] = 1;
  return result;
}

ExcelValue results_o68() {
  static ExcelValue result;
  if(variable_set[1478] == 1) { return result;}
  result = ZERO;
  variable_set[1478] = 1;
  return result;
}

ExcelValue results_p68() {
  static ExcelValue result;
  if(variable_set[1479] == 1) { return result;}
  result = ZERO;
  variable_set[1479] = 1;
  return result;
}

ExcelValue results_q68() {
  static ExcelValue result;
  if(variable_set[1480] == 1) { return result;}
  result = ZERO;
  variable_set[1480] = 1;
  return result;
}

ExcelValue results_r68() {
  static ExcelValue result;
  if(variable_set[1481] == 1) { return result;}
  result = ZERO;
  variable_set[1481] = 1;
  return result;
}

ExcelValue results_s68() {
  static ExcelValue result;
  if(variable_set[1482] == 1) { return result;}
  result = ZERO;
  variable_set[1482] = 1;
  return result;
}

ExcelValue results_t68() {
  static ExcelValue result;
  if(variable_set[1483] == 1) { return result;}
  result = ZERO;
  variable_set[1483] = 1;
  return result;
}

ExcelValue results_u68() {
  static ExcelValue result;
  if(variable_set[1484] == 1) { return result;}
  result = ZERO;
  variable_set[1484] = 1;
  return result;
}

ExcelValue results_v68() {
  static ExcelValue result;
  if(variable_set[1485] == 1) { return result;}
  result = ZERO;
  variable_set[1485] = 1;
  return result;
}

ExcelValue results_w68() {
  static ExcelValue result;
  if(variable_set[1486] == 1) { return result;}
  result = ZERO;
  variable_set[1486] = 1;
  return result;
}

ExcelValue results_e69() { return constant542; }
ExcelValue results_f69() { return constant543; }
ExcelValue results_g69() { return constant544; }
ExcelValue results_h69() { return constant545; }
ExcelValue results_i69() { return constant546; }
ExcelValue results_j69() { return constant547; }
ExcelValue results_k69() { return constant548; }
ExcelValue results_l69() { return constant549; }
ExcelValue results_m69() {
  static ExcelValue result;
  if(variable_set[1495] == 1) { return result;}
  result = ZERO;
  variable_set[1495] = 1;
  return result;
}

ExcelValue results_n69() {
  static ExcelValue result;
  if(variable_set[1496] == 1) { return result;}
  result = ZERO;
  variable_set[1496] = 1;
  return result;
}

ExcelValue results_o69() {
  static ExcelValue result;
  if(variable_set[1497] == 1) { return result;}
  result = ZERO;
  variable_set[1497] = 1;
  return result;
}

ExcelValue results_p69() {
  static ExcelValue result;
  if(variable_set[1498] == 1) { return result;}
  result = ZERO;
  variable_set[1498] = 1;
  return result;
}

ExcelValue results_q69() {
  static ExcelValue result;
  if(variable_set[1499] == 1) { return result;}
  result = ZERO;
  variable_set[1499] = 1;
  return result;
}

ExcelValue results_r69() {
  static ExcelValue result;
  if(variable_set[1500] == 1) { return result;}
  result = ZERO;
  variable_set[1500] = 1;
  return result;
}

ExcelValue results_s69() {
  static ExcelValue result;
  if(variable_set[1501] == 1) { return result;}
  result = ZERO;
  variable_set[1501] = 1;
  return result;
}

ExcelValue results_t69() {
  static ExcelValue result;
  if(variable_set[1502] == 1) { return result;}
  result = ZERO;
  variable_set[1502] = 1;
  return result;
}

ExcelValue results_u69() {
  static ExcelValue result;
  if(variable_set[1503] == 1) { return result;}
  result = ZERO;
  variable_set[1503] = 1;
  return result;
}

ExcelValue results_v69() {
  static ExcelValue result;
  if(variable_set[1504] == 1) { return result;}
  result = ZERO;
  variable_set[1504] = 1;
  return result;
}

ExcelValue results_w69() {
  static ExcelValue result;
  if(variable_set[1505] == 1) { return result;}
  result = ZERO;
  variable_set[1505] = 1;
  return result;
}

ExcelValue results_e70() { return constant550; }
ExcelValue results_f70() { return constant551; }
ExcelValue results_g70() { return constant552; }
ExcelValue results_h70() { return constant553; }
ExcelValue results_i70() { return constant554; }
ExcelValue results_j70() {
  static ExcelValue result;
  if(variable_set[1511] == 1) { return result;}
  result = ZERO;
  variable_set[1511] = 1;
  return result;
}

ExcelValue results_k70() {
  static ExcelValue result;
  if(variable_set[1512] == 1) { return result;}
  result = ZERO;
  variable_set[1512] = 1;
  return result;
}

ExcelValue results_l70() {
  static ExcelValue result;
  if(variable_set[1513] == 1) { return result;}
  result = ZERO;
  variable_set[1513] = 1;
  return result;
}

ExcelValue results_m70() {
  static ExcelValue result;
  if(variable_set[1514] == 1) { return result;}
  result = ZERO;
  variable_set[1514] = 1;
  return result;
}

ExcelValue results_n70() {
  static ExcelValue result;
  if(variable_set[1515] == 1) { return result;}
  result = ZERO;
  variable_set[1515] = 1;
  return result;
}

ExcelValue results_o70() {
  static ExcelValue result;
  if(variable_set[1516] == 1) { return result;}
  result = ZERO;
  variable_set[1516] = 1;
  return result;
}

ExcelValue results_p70() {
  static ExcelValue result;
  if(variable_set[1517] == 1) { return result;}
  result = ZERO;
  variable_set[1517] = 1;
  return result;
}

ExcelValue results_q70() {
  static ExcelValue result;
  if(variable_set[1518] == 1) { return result;}
  result = ZERO;
  variable_set[1518] = 1;
  return result;
}

ExcelValue results_r70() {
  static ExcelValue result;
  if(variable_set[1519] == 1) { return result;}
  result = ZERO;
  variable_set[1519] = 1;
  return result;
}

ExcelValue results_s70() {
  static ExcelValue result;
  if(variable_set[1520] == 1) { return result;}
  result = ZERO;
  variable_set[1520] = 1;
  return result;
}

ExcelValue results_t70() {
  static ExcelValue result;
  if(variable_set[1521] == 1) { return result;}
  result = ZERO;
  variable_set[1521] = 1;
  return result;
}

ExcelValue results_u70() {
  static ExcelValue result;
  if(variable_set[1522] == 1) { return result;}
  result = ZERO;
  variable_set[1522] = 1;
  return result;
}

ExcelValue results_v70() {
  static ExcelValue result;
  if(variable_set[1523] == 1) { return result;}
  result = ZERO;
  variable_set[1523] = 1;
  return result;
}

ExcelValue results_w70() {
  static ExcelValue result;
  if(variable_set[1524] == 1) { return result;}
  result = ZERO;
  variable_set[1524] = 1;
  return result;
}

ExcelValue results_e71() { return constant555; }
ExcelValue results_f71() {
  static ExcelValue result;
  if(variable_set[1526] == 1) { return result;}
  result = ZERO;
  variable_set[1526] = 1;
  return result;
}

ExcelValue results_g71() {
  static ExcelValue result;
  if(variable_set[1527] == 1) { return result;}
  result = ZERO;
  variable_set[1527] = 1;
  return result;
}

ExcelValue results_h71() {
  static ExcelValue result;
  if(variable_set[1528] == 1) { return result;}
  result = ZERO;
  variable_set[1528] = 1;
  return result;
}

ExcelValue results_i71() {
  static ExcelValue result;
  if(variable_set[1529] == 1) { return result;}
  result = ZERO;
  variable_set[1529] = 1;
  return result;
}

ExcelValue results_j71() {
  static ExcelValue result;
  if(variable_set[1530] == 1) { return result;}
  result = ZERO;
  variable_set[1530] = 1;
  return result;
}

ExcelValue results_k71() {
  static ExcelValue result;
  if(variable_set[1531] == 1) { return result;}
  result = ZERO;
  variable_set[1531] = 1;
  return result;
}

ExcelValue results_l71() {
  static ExcelValue result;
  if(variable_set[1532] == 1) { return result;}
  result = ZERO;
  variable_set[1532] = 1;
  return result;
}

ExcelValue results_m71() {
  static ExcelValue result;
  if(variable_set[1533] == 1) { return result;}
  result = ZERO;
  variable_set[1533] = 1;
  return result;
}

ExcelValue results_n71() {
  static ExcelValue result;
  if(variable_set[1534] == 1) { return result;}
  result = ZERO;
  variable_set[1534] = 1;
  return result;
}

ExcelValue results_o71() {
  static ExcelValue result;
  if(variable_set[1535] == 1) { return result;}
  result = ZERO;
  variable_set[1535] = 1;
  return result;
}

ExcelValue results_p71() {
  static ExcelValue result;
  if(variable_set[1536] == 1) { return result;}
  result = ZERO;
  variable_set[1536] = 1;
  return result;
}

ExcelValue results_q71() {
  static ExcelValue result;
  if(variable_set[1537] == 1) { return result;}
  result = ZERO;
  variable_set[1537] = 1;
  return result;
}

ExcelValue results_r71() {
  static ExcelValue result;
  if(variable_set[1538] == 1) { return result;}
  result = ZERO;
  variable_set[1538] = 1;
  return result;
}

ExcelValue results_s71() {
  static ExcelValue result;
  if(variable_set[1539] == 1) { return result;}
  result = ZERO;
  variable_set[1539] = 1;
  return result;
}

ExcelValue results_t71() {
  static ExcelValue result;
  if(variable_set[1540] == 1) { return result;}
  result = ZERO;
  variable_set[1540] = 1;
  return result;
}

ExcelValue results_u71() {
  static ExcelValue result;
  if(variable_set[1541] == 1) { return result;}
  result = ZERO;
  variable_set[1541] = 1;
  return result;
}

ExcelValue results_v71() {
  static ExcelValue result;
  if(variable_set[1542] == 1) { return result;}
  result = ZERO;
  variable_set[1542] = 1;
  return result;
}

ExcelValue results_w71() {
  static ExcelValue result;
  if(variable_set[1543] == 1) { return result;}
  result = ZERO;
  variable_set[1543] = 1;
  return result;
}

ExcelValue results_e72() { return constant556; }
ExcelValue results_f72() {
  static ExcelValue result;
  if(variable_set[1545] == 1) { return result;}
  result = ZERO;
  variable_set[1545] = 1;
  return result;
}

ExcelValue results_g72() {
  static ExcelValue result;
  if(variable_set[1546] == 1) { return result;}
  result = ZERO;
  variable_set[1546] = 1;
  return result;
}

ExcelValue results_h72() {
  static ExcelValue result;
  if(variable_set[1547] == 1) { return result;}
  result = ZERO;
  variable_set[1547] = 1;
  return result;
}

ExcelValue results_i72() {
  static ExcelValue result;
  if(variable_set[1548] == 1) { return result;}
  result = ZERO;
  variable_set[1548] = 1;
  return result;
}

ExcelValue results_j72() {
  static ExcelValue result;
  if(variable_set[1549] == 1) { return result;}
  result = ZERO;
  variable_set[1549] = 1;
  return result;
}

ExcelValue results_k72() {
  static ExcelValue result;
  if(variable_set[1550] == 1) { return result;}
  result = ZERO;
  variable_set[1550] = 1;
  return result;
}

ExcelValue results_l72() {
  static ExcelValue result;
  if(variable_set[1551] == 1) { return result;}
  result = ZERO;
  variable_set[1551] = 1;
  return result;
}

ExcelValue results_m72() {
  static ExcelValue result;
  if(variable_set[1552] == 1) { return result;}
  result = ZERO;
  variable_set[1552] = 1;
  return result;
}

ExcelValue results_n72() {
  static ExcelValue result;
  if(variable_set[1553] == 1) { return result;}
  result = ZERO;
  variable_set[1553] = 1;
  return result;
}

ExcelValue results_o72() {
  static ExcelValue result;
  if(variable_set[1554] == 1) { return result;}
  result = ZERO;
  variable_set[1554] = 1;
  return result;
}

ExcelValue results_p72() {
  static ExcelValue result;
  if(variable_set[1555] == 1) { return result;}
  result = ZERO;
  variable_set[1555] = 1;
  return result;
}

ExcelValue results_q72() {
  static ExcelValue result;
  if(variable_set[1556] == 1) { return result;}
  result = ZERO;
  variable_set[1556] = 1;
  return result;
}

ExcelValue results_r72() {
  static ExcelValue result;
  if(variable_set[1557] == 1) { return result;}
  result = ZERO;
  variable_set[1557] = 1;
  return result;
}

ExcelValue results_s72() {
  static ExcelValue result;
  if(variable_set[1558] == 1) { return result;}
  result = ZERO;
  variable_set[1558] = 1;
  return result;
}

ExcelValue results_t72() {
  static ExcelValue result;
  if(variable_set[1559] == 1) { return result;}
  result = ZERO;
  variable_set[1559] = 1;
  return result;
}

ExcelValue results_u72() {
  static ExcelValue result;
  if(variable_set[1560] == 1) { return result;}
  result = ZERO;
  variable_set[1560] = 1;
  return result;
}

ExcelValue results_v72() {
  static ExcelValue result;
  if(variable_set[1561] == 1) { return result;}
  result = ZERO;
  variable_set[1561] = 1;
  return result;
}

ExcelValue results_w72() {
  static ExcelValue result;
  if(variable_set[1562] == 1) { return result;}
  result = ZERO;
  variable_set[1562] = 1;
  return result;
}

ExcelValue results_e73() { return constant557; }
ExcelValue results_f73() { return constant558; }
ExcelValue results_g73() { return constant559; }
ExcelValue results_h73() { return constant560; }
ExcelValue results_i73() { return constant561; }
ExcelValue results_j73() { return constant562; }
ExcelValue results_k73() { return constant563; }
ExcelValue results_l73() { return constant564; }
ExcelValue results_m73() {
  static ExcelValue result;
  if(variable_set[1571] == 1) { return result;}
  result = ZERO;
  variable_set[1571] = 1;
  return result;
}

ExcelValue results_n73() {
  static ExcelValue result;
  if(variable_set[1572] == 1) { return result;}
  result = ZERO;
  variable_set[1572] = 1;
  return result;
}

ExcelValue results_o73() {
  static ExcelValue result;
  if(variable_set[1573] == 1) { return result;}
  result = ZERO;
  variable_set[1573] = 1;
  return result;
}

ExcelValue results_p73() {
  static ExcelValue result;
  if(variable_set[1574] == 1) { return result;}
  result = ZERO;
  variable_set[1574] = 1;
  return result;
}

ExcelValue results_q73() {
  static ExcelValue result;
  if(variable_set[1575] == 1) { return result;}
  result = ZERO;
  variable_set[1575] = 1;
  return result;
}

ExcelValue results_r73() {
  static ExcelValue result;
  if(variable_set[1576] == 1) { return result;}
  result = ZERO;
  variable_set[1576] = 1;
  return result;
}

ExcelValue results_s73() {
  static ExcelValue result;
  if(variable_set[1577] == 1) { return result;}
  result = ZERO;
  variable_set[1577] = 1;
  return result;
}

ExcelValue results_t73() {
  static ExcelValue result;
  if(variable_set[1578] == 1) { return result;}
  result = ZERO;
  variable_set[1578] = 1;
  return result;
}

ExcelValue results_u73() {
  static ExcelValue result;
  if(variable_set[1579] == 1) { return result;}
  result = ZERO;
  variable_set[1579] = 1;
  return result;
}

ExcelValue results_v73() {
  static ExcelValue result;
  if(variable_set[1580] == 1) { return result;}
  result = ZERO;
  variable_set[1580] = 1;
  return result;
}

ExcelValue results_w73() {
  static ExcelValue result;
  if(variable_set[1581] == 1) { return result;}
  result = ZERO;
  variable_set[1581] = 1;
  return result;
}

ExcelValue results_e74() { return constant565; }
ExcelValue results_f74() { return constant566; }
ExcelValue results_g74() { return constant567; }
ExcelValue results_h74() { return constant568; }
ExcelValue results_i74() { return constant569; }
ExcelValue results_j74() {
  static ExcelValue result;
  if(variable_set[1587] == 1) { return result;}
  result = ZERO;
  variable_set[1587] = 1;
  return result;
}

ExcelValue results_k74() {
  static ExcelValue result;
  if(variable_set[1588] == 1) { return result;}
  result = ZERO;
  variable_set[1588] = 1;
  return result;
}

ExcelValue results_l74() {
  static ExcelValue result;
  if(variable_set[1589] == 1) { return result;}
  result = ZERO;
  variable_set[1589] = 1;
  return result;
}

ExcelValue results_m74() {
  static ExcelValue result;
  if(variable_set[1590] == 1) { return result;}
  result = ZERO;
  variable_set[1590] = 1;
  return result;
}

ExcelValue results_n74() {
  static ExcelValue result;
  if(variable_set[1591] == 1) { return result;}
  result = ZERO;
  variable_set[1591] = 1;
  return result;
}

ExcelValue results_o74() {
  static ExcelValue result;
  if(variable_set[1592] == 1) { return result;}
  result = ZERO;
  variable_set[1592] = 1;
  return result;
}

ExcelValue results_p74() {
  static ExcelValue result;
  if(variable_set[1593] == 1) { return result;}
  result = ZERO;
  variable_set[1593] = 1;
  return result;
}

ExcelValue results_q74() {
  static ExcelValue result;
  if(variable_set[1594] == 1) { return result;}
  result = ZERO;
  variable_set[1594] = 1;
  return result;
}

ExcelValue results_r74() {
  static ExcelValue result;
  if(variable_set[1595] == 1) { return result;}
  result = ZERO;
  variable_set[1595] = 1;
  return result;
}

ExcelValue results_s74() {
  static ExcelValue result;
  if(variable_set[1596] == 1) { return result;}
  result = ZERO;
  variable_set[1596] = 1;
  return result;
}

ExcelValue results_t74() {
  static ExcelValue result;
  if(variable_set[1597] == 1) { return result;}
  result = ZERO;
  variable_set[1597] = 1;
  return result;
}

ExcelValue results_u74() {
  static ExcelValue result;
  if(variable_set[1598] == 1) { return result;}
  result = ZERO;
  variable_set[1598] = 1;
  return result;
}

ExcelValue results_v74() {
  static ExcelValue result;
  if(variable_set[1599] == 1) { return result;}
  result = ZERO;
  variable_set[1599] = 1;
  return result;
}

ExcelValue results_w74() {
  static ExcelValue result;
  if(variable_set[1600] == 1) { return result;}
  result = ZERO;
  variable_set[1600] = 1;
  return result;
}

ExcelValue results_e75() { return constant570; }
ExcelValue results_f75() {
  static ExcelValue result;
  if(variable_set[1602] == 1) { return result;}
  result = ZERO;
  variable_set[1602] = 1;
  return result;
}

ExcelValue results_g75() {
  static ExcelValue result;
  if(variable_set[1603] == 1) { return result;}
  result = ZERO;
  variable_set[1603] = 1;
  return result;
}

ExcelValue results_h75() {
  static ExcelValue result;
  if(variable_set[1604] == 1) { return result;}
  result = ZERO;
  variable_set[1604] = 1;
  return result;
}

ExcelValue results_i75() {
  static ExcelValue result;
  if(variable_set[1605] == 1) { return result;}
  result = ZERO;
  variable_set[1605] = 1;
  return result;
}

ExcelValue results_j75() {
  static ExcelValue result;
  if(variable_set[1606] == 1) { return result;}
  result = ZERO;
  variable_set[1606] = 1;
  return result;
}

ExcelValue results_k75() {
  static ExcelValue result;
  if(variable_set[1607] == 1) { return result;}
  result = ZERO;
  variable_set[1607] = 1;
  return result;
}

ExcelValue results_l75() {
  static ExcelValue result;
  if(variable_set[1608] == 1) { return result;}
  result = ZERO;
  variable_set[1608] = 1;
  return result;
}

ExcelValue results_m75() {
  static ExcelValue result;
  if(variable_set[1609] == 1) { return result;}
  result = ZERO;
  variable_set[1609] = 1;
  return result;
}

ExcelValue results_n75() {
  static ExcelValue result;
  if(variable_set[1610] == 1) { return result;}
  result = ZERO;
  variable_set[1610] = 1;
  return result;
}

ExcelValue results_o75() {
  static ExcelValue result;
  if(variable_set[1611] == 1) { return result;}
  result = ZERO;
  variable_set[1611] = 1;
  return result;
}

ExcelValue results_p75() {
  static ExcelValue result;
  if(variable_set[1612] == 1) { return result;}
  result = ZERO;
  variable_set[1612] = 1;
  return result;
}

ExcelValue results_q75() {
  static ExcelValue result;
  if(variable_set[1613] == 1) { return result;}
  result = ZERO;
  variable_set[1613] = 1;
  return result;
}

ExcelValue results_r75() {
  static ExcelValue result;
  if(variable_set[1614] == 1) { return result;}
  result = ZERO;
  variable_set[1614] = 1;
  return result;
}

ExcelValue results_s75() {
  static ExcelValue result;
  if(variable_set[1615] == 1) { return result;}
  result = ZERO;
  variable_set[1615] = 1;
  return result;
}

ExcelValue results_t75() {
  static ExcelValue result;
  if(variable_set[1616] == 1) { return result;}
  result = ZERO;
  variable_set[1616] = 1;
  return result;
}

ExcelValue results_u75() {
  static ExcelValue result;
  if(variable_set[1617] == 1) { return result;}
  result = ZERO;
  variable_set[1617] = 1;
  return result;
}

ExcelValue results_v75() {
  static ExcelValue result;
  if(variable_set[1618] == 1) { return result;}
  result = ZERO;
  variable_set[1618] = 1;
  return result;
}

ExcelValue results_w75() {
  static ExcelValue result;
  if(variable_set[1619] == 1) { return result;}
  result = ZERO;
  variable_set[1619] = 1;
  return result;
}

ExcelValue results_e76() { return constant571; }
ExcelValue results_f76() {
  static ExcelValue result;
  if(variable_set[1621] == 1) { return result;}
  result = ZERO;
  variable_set[1621] = 1;
  return result;
}

ExcelValue results_g76() {
  static ExcelValue result;
  if(variable_set[1622] == 1) { return result;}
  result = ZERO;
  variable_set[1622] = 1;
  return result;
}

ExcelValue results_h76() {
  static ExcelValue result;
  if(variable_set[1623] == 1) { return result;}
  result = ZERO;
  variable_set[1623] = 1;
  return result;
}

ExcelValue results_i76() {
  static ExcelValue result;
  if(variable_set[1624] == 1) { return result;}
  result = ZERO;
  variable_set[1624] = 1;
  return result;
}

ExcelValue results_j76() {
  static ExcelValue result;
  if(variable_set[1625] == 1) { return result;}
  result = ZERO;
  variable_set[1625] = 1;
  return result;
}

ExcelValue results_k76() {
  static ExcelValue result;
  if(variable_set[1626] == 1) { return result;}
  result = ZERO;
  variable_set[1626] = 1;
  return result;
}

ExcelValue results_l76() {
  static ExcelValue result;
  if(variable_set[1627] == 1) { return result;}
  result = ZERO;
  variable_set[1627] = 1;
  return result;
}

ExcelValue results_m76() {
  static ExcelValue result;
  if(variable_set[1628] == 1) { return result;}
  result = ZERO;
  variable_set[1628] = 1;
  return result;
}

ExcelValue results_n76() {
  static ExcelValue result;
  if(variable_set[1629] == 1) { return result;}
  result = ZERO;
  variable_set[1629] = 1;
  return result;
}

ExcelValue results_o76() {
  static ExcelValue result;
  if(variable_set[1630] == 1) { return result;}
  result = ZERO;
  variable_set[1630] = 1;
  return result;
}

ExcelValue results_p76() {
  static ExcelValue result;
  if(variable_set[1631] == 1) { return result;}
  result = ZERO;
  variable_set[1631] = 1;
  return result;
}

ExcelValue results_q76() {
  static ExcelValue result;
  if(variable_set[1632] == 1) { return result;}
  result = ZERO;
  variable_set[1632] = 1;
  return result;
}

ExcelValue results_r76() {
  static ExcelValue result;
  if(variable_set[1633] == 1) { return result;}
  result = ZERO;
  variable_set[1633] = 1;
  return result;
}

ExcelValue results_s76() {
  static ExcelValue result;
  if(variable_set[1634] == 1) { return result;}
  result = ZERO;
  variable_set[1634] = 1;
  return result;
}

ExcelValue results_t76() {
  static ExcelValue result;
  if(variable_set[1635] == 1) { return result;}
  result = ZERO;
  variable_set[1635] = 1;
  return result;
}

ExcelValue results_u76() {
  static ExcelValue result;
  if(variable_set[1636] == 1) { return result;}
  result = ZERO;
  variable_set[1636] = 1;
  return result;
}

ExcelValue results_v76() {
  static ExcelValue result;
  if(variable_set[1637] == 1) { return result;}
  result = ZERO;
  variable_set[1637] = 1;
  return result;
}

ExcelValue results_w76() {
  static ExcelValue result;
  if(variable_set[1638] == 1) { return result;}
  result = ZERO;
  variable_set[1638] = 1;
  return result;
}

ExcelValue results_e77() { return constant572; }
ExcelValue results_f77() { return constant573; }
ExcelValue results_g77() { return constant574; }
ExcelValue results_h77() { return constant575; }
ExcelValue results_i77() { return constant576; }
ExcelValue results_j77() { return constant577; }
ExcelValue results_k77() { return constant578; }
ExcelValue results_l77() { return constant579; }
ExcelValue results_m77() {
  static ExcelValue result;
  if(variable_set[1647] == 1) { return result;}
  result = ZERO;
  variable_set[1647] = 1;
  return result;
}

ExcelValue results_n77() {
  static ExcelValue result;
  if(variable_set[1648] == 1) { return result;}
  result = ZERO;
  variable_set[1648] = 1;
  return result;
}

ExcelValue results_o77() {
  static ExcelValue result;
  if(variable_set[1649] == 1) { return result;}
  result = ZERO;
  variable_set[1649] = 1;
  return result;
}

ExcelValue results_p77() {
  static ExcelValue result;
  if(variable_set[1650] == 1) { return result;}
  result = ZERO;
  variable_set[1650] = 1;
  return result;
}

ExcelValue results_q77() {
  static ExcelValue result;
  if(variable_set[1651] == 1) { return result;}
  result = ZERO;
  variable_set[1651] = 1;
  return result;
}

ExcelValue results_r77() {
  static ExcelValue result;
  if(variable_set[1652] == 1) { return result;}
  result = ZERO;
  variable_set[1652] = 1;
  return result;
}

ExcelValue results_s77() {
  static ExcelValue result;
  if(variable_set[1653] == 1) { return result;}
  result = ZERO;
  variable_set[1653] = 1;
  return result;
}

ExcelValue results_t77() {
  static ExcelValue result;
  if(variable_set[1654] == 1) { return result;}
  result = ZERO;
  variable_set[1654] = 1;
  return result;
}

ExcelValue results_u77() {
  static ExcelValue result;
  if(variable_set[1655] == 1) { return result;}
  result = ZERO;
  variable_set[1655] = 1;
  return result;
}

ExcelValue results_v77() {
  static ExcelValue result;
  if(variable_set[1656] == 1) { return result;}
  result = ZERO;
  variable_set[1656] = 1;
  return result;
}

ExcelValue results_w77() {
  static ExcelValue result;
  if(variable_set[1657] == 1) { return result;}
  result = ZERO;
  variable_set[1657] = 1;
  return result;
}

ExcelValue results_e78() { return constant580; }
ExcelValue results_f78() { return constant581; }
ExcelValue results_g78() { return constant582; }
ExcelValue results_h78() { return constant583; }
ExcelValue results_i78() { return constant584; }
ExcelValue results_j78() {
  static ExcelValue result;
  if(variable_set[1663] == 1) { return result;}
  result = ZERO;
  variable_set[1663] = 1;
  return result;
}

ExcelValue results_k78() {
  static ExcelValue result;
  if(variable_set[1664] == 1) { return result;}
  result = ZERO;
  variable_set[1664] = 1;
  return result;
}

ExcelValue results_l78() {
  static ExcelValue result;
  if(variable_set[1665] == 1) { return result;}
  result = ZERO;
  variable_set[1665] = 1;
  return result;
}

ExcelValue results_m78() {
  static ExcelValue result;
  if(variable_set[1666] == 1) { return result;}
  result = ZERO;
  variable_set[1666] = 1;
  return result;
}

ExcelValue results_n78() {
  static ExcelValue result;
  if(variable_set[1667] == 1) { return result;}
  result = ZERO;
  variable_set[1667] = 1;
  return result;
}

ExcelValue results_o78() {
  static ExcelValue result;
  if(variable_set[1668] == 1) { return result;}
  result = ZERO;
  variable_set[1668] = 1;
  return result;
}

ExcelValue results_p78() {
  static ExcelValue result;
  if(variable_set[1669] == 1) { return result;}
  result = ZERO;
  variable_set[1669] = 1;
  return result;
}

ExcelValue results_q78() {
  static ExcelValue result;
  if(variable_set[1670] == 1) { return result;}
  result = ZERO;
  variable_set[1670] = 1;
  return result;
}

ExcelValue results_r78() {
  static ExcelValue result;
  if(variable_set[1671] == 1) { return result;}
  result = ZERO;
  variable_set[1671] = 1;
  return result;
}

ExcelValue results_s78() {
  static ExcelValue result;
  if(variable_set[1672] == 1) { return result;}
  result = ZERO;
  variable_set[1672] = 1;
  return result;
}

ExcelValue results_t78() {
  static ExcelValue result;
  if(variable_set[1673] == 1) { return result;}
  result = ZERO;
  variable_set[1673] = 1;
  return result;
}

ExcelValue results_u78() {
  static ExcelValue result;
  if(variable_set[1674] == 1) { return result;}
  result = ZERO;
  variable_set[1674] = 1;
  return result;
}

ExcelValue results_v78() {
  static ExcelValue result;
  if(variable_set[1675] == 1) { return result;}
  result = ZERO;
  variable_set[1675] = 1;
  return result;
}

ExcelValue results_w78() {
  static ExcelValue result;
  if(variable_set[1676] == 1) { return result;}
  result = ZERO;
  variable_set[1676] = 1;
  return result;
}

ExcelValue results_e79() { return constant585; }
ExcelValue results_f79() {
  static ExcelValue result;
  if(variable_set[1678] == 1) { return result;}
  result = ZERO;
  variable_set[1678] = 1;
  return result;
}

ExcelValue results_g79() {
  static ExcelValue result;
  if(variable_set[1679] == 1) { return result;}
  result = ZERO;
  variable_set[1679] = 1;
  return result;
}

ExcelValue results_h79() {
  static ExcelValue result;
  if(variable_set[1680] == 1) { return result;}
  result = ZERO;
  variable_set[1680] = 1;
  return result;
}

ExcelValue results_i79() {
  static ExcelValue result;
  if(variable_set[1681] == 1) { return result;}
  result = ZERO;
  variable_set[1681] = 1;
  return result;
}

ExcelValue results_j79() {
  static ExcelValue result;
  if(variable_set[1682] == 1) { return result;}
  result = ZERO;
  variable_set[1682] = 1;
  return result;
}

ExcelValue results_k79() {
  static ExcelValue result;
  if(variable_set[1683] == 1) { return result;}
  result = ZERO;
  variable_set[1683] = 1;
  return result;
}

ExcelValue results_l79() {
  static ExcelValue result;
  if(variable_set[1684] == 1) { return result;}
  result = ZERO;
  variable_set[1684] = 1;
  return result;
}

ExcelValue results_m79() {
  static ExcelValue result;
  if(variable_set[1685] == 1) { return result;}
  result = ZERO;
  variable_set[1685] = 1;
  return result;
}

ExcelValue results_n79() {
  static ExcelValue result;
  if(variable_set[1686] == 1) { return result;}
  result = ZERO;
  variable_set[1686] = 1;
  return result;
}

ExcelValue results_o79() {
  static ExcelValue result;
  if(variable_set[1687] == 1) { return result;}
  result = ZERO;
  variable_set[1687] = 1;
  return result;
}

ExcelValue results_p79() {
  static ExcelValue result;
  if(variable_set[1688] == 1) { return result;}
  result = ZERO;
  variable_set[1688] = 1;
  return result;
}

ExcelValue results_q79() {
  static ExcelValue result;
  if(variable_set[1689] == 1) { return result;}
  result = ZERO;
  variable_set[1689] = 1;
  return result;
}

ExcelValue results_r79() {
  static ExcelValue result;
  if(variable_set[1690] == 1) { return result;}
  result = ZERO;
  variable_set[1690] = 1;
  return result;
}

ExcelValue results_s79() {
  static ExcelValue result;
  if(variable_set[1691] == 1) { return result;}
  result = ZERO;
  variable_set[1691] = 1;
  return result;
}

ExcelValue results_t79() {
  static ExcelValue result;
  if(variable_set[1692] == 1) { return result;}
  result = ZERO;
  variable_set[1692] = 1;
  return result;
}

ExcelValue results_u79() {
  static ExcelValue result;
  if(variable_set[1693] == 1) { return result;}
  result = ZERO;
  variable_set[1693] = 1;
  return result;
}

ExcelValue results_v79() {
  static ExcelValue result;
  if(variable_set[1694] == 1) { return result;}
  result = ZERO;
  variable_set[1694] = 1;
  return result;
}

ExcelValue results_w79() {
  static ExcelValue result;
  if(variable_set[1695] == 1) { return result;}
  result = ZERO;
  variable_set[1695] = 1;
  return result;
}

ExcelValue results_e80() { return constant586; }
ExcelValue results_f80() {
  static ExcelValue result;
  if(variable_set[1697] == 1) { return result;}
  result = ZERO;
  variable_set[1697] = 1;
  return result;
}

ExcelValue results_g80() {
  static ExcelValue result;
  if(variable_set[1698] == 1) { return result;}
  result = ZERO;
  variable_set[1698] = 1;
  return result;
}

ExcelValue results_h80() {
  static ExcelValue result;
  if(variable_set[1699] == 1) { return result;}
  result = ZERO;
  variable_set[1699] = 1;
  return result;
}

ExcelValue results_i80() {
  static ExcelValue result;
  if(variable_set[1700] == 1) { return result;}
  result = ZERO;
  variable_set[1700] = 1;
  return result;
}

ExcelValue results_j80() {
  static ExcelValue result;
  if(variable_set[1701] == 1) { return result;}
  result = ZERO;
  variable_set[1701] = 1;
  return result;
}

ExcelValue results_k80() {
  static ExcelValue result;
  if(variable_set[1702] == 1) { return result;}
  result = ZERO;
  variable_set[1702] = 1;
  return result;
}

ExcelValue results_l80() {
  static ExcelValue result;
  if(variable_set[1703] == 1) { return result;}
  result = ZERO;
  variable_set[1703] = 1;
  return result;
}

ExcelValue results_m80() {
  static ExcelValue result;
  if(variable_set[1704] == 1) { return result;}
  result = ZERO;
  variable_set[1704] = 1;
  return result;
}

ExcelValue results_n80() {
  static ExcelValue result;
  if(variable_set[1705] == 1) { return result;}
  result = ZERO;
  variable_set[1705] = 1;
  return result;
}

ExcelValue results_o80() {
  static ExcelValue result;
  if(variable_set[1706] == 1) { return result;}
  result = ZERO;
  variable_set[1706] = 1;
  return result;
}

ExcelValue results_p80() {
  static ExcelValue result;
  if(variable_set[1707] == 1) { return result;}
  result = ZERO;
  variable_set[1707] = 1;
  return result;
}

ExcelValue results_q80() {
  static ExcelValue result;
  if(variable_set[1708] == 1) { return result;}
  result = ZERO;
  variable_set[1708] = 1;
  return result;
}

ExcelValue results_r80() {
  static ExcelValue result;
  if(variable_set[1709] == 1) { return result;}
  result = ZERO;
  variable_set[1709] = 1;
  return result;
}

ExcelValue results_s80() {
  static ExcelValue result;
  if(variable_set[1710] == 1) { return result;}
  result = ZERO;
  variable_set[1710] = 1;
  return result;
}

ExcelValue results_t80() {
  static ExcelValue result;
  if(variable_set[1711] == 1) { return result;}
  result = ZERO;
  variable_set[1711] = 1;
  return result;
}

ExcelValue results_u80() {
  static ExcelValue result;
  if(variable_set[1712] == 1) { return result;}
  result = ZERO;
  variable_set[1712] = 1;
  return result;
}

ExcelValue results_v80() {
  static ExcelValue result;
  if(variable_set[1713] == 1) { return result;}
  result = ZERO;
  variable_set[1713] = 1;
  return result;
}

ExcelValue results_w80() {
  static ExcelValue result;
  if(variable_set[1714] == 1) { return result;}
  result = ZERO;
  variable_set[1714] = 1;
  return result;
}

ExcelValue results_e81() { return constant587; }
ExcelValue results_f81() { return constant588; }
ExcelValue results_g81() { return constant589; }
ExcelValue results_h81() { return constant590; }
ExcelValue results_i81() { return constant591; }
ExcelValue results_j81() { return constant592; }
ExcelValue results_k81() { return constant593; }
ExcelValue results_l81() { return constant594; }
ExcelValue results_m81() {
  static ExcelValue result;
  if(variable_set[1723] == 1) { return result;}
  result = ZERO;
  variable_set[1723] = 1;
  return result;
}

ExcelValue results_n81() {
  static ExcelValue result;
  if(variable_set[1724] == 1) { return result;}
  result = ZERO;
  variable_set[1724] = 1;
  return result;
}

ExcelValue results_o81() {
  static ExcelValue result;
  if(variable_set[1725] == 1) { return result;}
  result = ZERO;
  variable_set[1725] = 1;
  return result;
}

ExcelValue results_p81() {
  static ExcelValue result;
  if(variable_set[1726] == 1) { return result;}
  result = ZERO;
  variable_set[1726] = 1;
  return result;
}

ExcelValue results_q81() {
  static ExcelValue result;
  if(variable_set[1727] == 1) { return result;}
  result = ZERO;
  variable_set[1727] = 1;
  return result;
}

ExcelValue results_r81() {
  static ExcelValue result;
  if(variable_set[1728] == 1) { return result;}
  result = ZERO;
  variable_set[1728] = 1;
  return result;
}

ExcelValue results_s81() {
  static ExcelValue result;
  if(variable_set[1729] == 1) { return result;}
  result = ZERO;
  variable_set[1729] = 1;
  return result;
}

ExcelValue results_t81() {
  static ExcelValue result;
  if(variable_set[1730] == 1) { return result;}
  result = ZERO;
  variable_set[1730] = 1;
  return result;
}

ExcelValue results_u81() {
  static ExcelValue result;
  if(variable_set[1731] == 1) { return result;}
  result = ZERO;
  variable_set[1731] = 1;
  return result;
}

ExcelValue results_v81() {
  static ExcelValue result;
  if(variable_set[1732] == 1) { return result;}
  result = ZERO;
  variable_set[1732] = 1;
  return result;
}

ExcelValue results_w81() {
  static ExcelValue result;
  if(variable_set[1733] == 1) { return result;}
  result = ZERO;
  variable_set[1733] = 1;
  return result;
}

ExcelValue results_e82() { return constant595; }
ExcelValue results_f82() { return constant596; }
ExcelValue results_g82() { return constant597; }
ExcelValue results_h82() { return constant598; }
ExcelValue results_i82() { return constant599; }
ExcelValue results_j82() { return constant600; }
ExcelValue results_k82() {
  static ExcelValue result;
  if(variable_set[1740] == 1) { return result;}
  result = ZERO;
  variable_set[1740] = 1;
  return result;
}

ExcelValue results_l82() {
  static ExcelValue result;
  if(variable_set[1741] == 1) { return result;}
  result = ZERO;
  variable_set[1741] = 1;
  return result;
}

ExcelValue results_m82() {
  static ExcelValue result;
  if(variable_set[1742] == 1) { return result;}
  result = ZERO;
  variable_set[1742] = 1;
  return result;
}

ExcelValue results_n82() {
  static ExcelValue result;
  if(variable_set[1743] == 1) { return result;}
  result = ZERO;
  variable_set[1743] = 1;
  return result;
}

ExcelValue results_o82() {
  static ExcelValue result;
  if(variable_set[1744] == 1) { return result;}
  result = ZERO;
  variable_set[1744] = 1;
  return result;
}

ExcelValue results_p82() {
  static ExcelValue result;
  if(variable_set[1745] == 1) { return result;}
  result = ZERO;
  variable_set[1745] = 1;
  return result;
}

ExcelValue results_q82() {
  static ExcelValue result;
  if(variable_set[1746] == 1) { return result;}
  result = ZERO;
  variable_set[1746] = 1;
  return result;
}

ExcelValue results_r82() {
  static ExcelValue result;
  if(variable_set[1747] == 1) { return result;}
  result = ZERO;
  variable_set[1747] = 1;
  return result;
}

ExcelValue results_s82() {
  static ExcelValue result;
  if(variable_set[1748] == 1) { return result;}
  result = ZERO;
  variable_set[1748] = 1;
  return result;
}

ExcelValue results_t82() {
  static ExcelValue result;
  if(variable_set[1749] == 1) { return result;}
  result = ZERO;
  variable_set[1749] = 1;
  return result;
}

ExcelValue results_u82() {
  static ExcelValue result;
  if(variable_set[1750] == 1) { return result;}
  result = ZERO;
  variable_set[1750] = 1;
  return result;
}

ExcelValue results_v82() {
  static ExcelValue result;
  if(variable_set[1751] == 1) { return result;}
  result = ZERO;
  variable_set[1751] = 1;
  return result;
}

ExcelValue results_w82() {
  static ExcelValue result;
  if(variable_set[1752] == 1) { return result;}
  result = ZERO;
  variable_set[1752] = 1;
  return result;
}

ExcelValue results_e83() { return constant601; }
ExcelValue results_f83() {
  static ExcelValue result;
  if(variable_set[1754] == 1) { return result;}
  result = ZERO;
  variable_set[1754] = 1;
  return result;
}

ExcelValue results_g83() {
  static ExcelValue result;
  if(variable_set[1755] == 1) { return result;}
  result = ZERO;
  variable_set[1755] = 1;
  return result;
}

ExcelValue results_h83() {
  static ExcelValue result;
  if(variable_set[1756] == 1) { return result;}
  result = ZERO;
  variable_set[1756] = 1;
  return result;
}

ExcelValue results_i83() {
  static ExcelValue result;
  if(variable_set[1757] == 1) { return result;}
  result = ZERO;
  variable_set[1757] = 1;
  return result;
}

ExcelValue results_j83() {
  static ExcelValue result;
  if(variable_set[1758] == 1) { return result;}
  result = ZERO;
  variable_set[1758] = 1;
  return result;
}

ExcelValue results_k83() {
  static ExcelValue result;
  if(variable_set[1759] == 1) { return result;}
  result = ZERO;
  variable_set[1759] = 1;
  return result;
}

ExcelValue results_l83() {
  static ExcelValue result;
  if(variable_set[1760] == 1) { return result;}
  result = ZERO;
  variable_set[1760] = 1;
  return result;
}

ExcelValue results_m83() {
  static ExcelValue result;
  if(variable_set[1761] == 1) { return result;}
  result = ZERO;
  variable_set[1761] = 1;
  return result;
}

ExcelValue results_n83() {
  static ExcelValue result;
  if(variable_set[1762] == 1) { return result;}
  result = ZERO;
  variable_set[1762] = 1;
  return result;
}

ExcelValue results_o83() {
  static ExcelValue result;
  if(variable_set[1763] == 1) { return result;}
  result = ZERO;
  variable_set[1763] = 1;
  return result;
}

ExcelValue results_p83() {
  static ExcelValue result;
  if(variable_set[1764] == 1) { return result;}
  result = ZERO;
  variable_set[1764] = 1;
  return result;
}

ExcelValue results_q83() {
  static ExcelValue result;
  if(variable_set[1765] == 1) { return result;}
  result = ZERO;
  variable_set[1765] = 1;
  return result;
}

ExcelValue results_r83() {
  static ExcelValue result;
  if(variable_set[1766] == 1) { return result;}
  result = ZERO;
  variable_set[1766] = 1;
  return result;
}

ExcelValue results_s83() {
  static ExcelValue result;
  if(variable_set[1767] == 1) { return result;}
  result = ZERO;
  variable_set[1767] = 1;
  return result;
}

ExcelValue results_t83() {
  static ExcelValue result;
  if(variable_set[1768] == 1) { return result;}
  result = ZERO;
  variable_set[1768] = 1;
  return result;
}

ExcelValue results_u83() {
  static ExcelValue result;
  if(variable_set[1769] == 1) { return result;}
  result = ZERO;
  variable_set[1769] = 1;
  return result;
}

ExcelValue results_v83() {
  static ExcelValue result;
  if(variable_set[1770] == 1) { return result;}
  result = ZERO;
  variable_set[1770] = 1;
  return result;
}

ExcelValue results_w83() {
  static ExcelValue result;
  if(variable_set[1771] == 1) { return result;}
  result = ZERO;
  variable_set[1771] = 1;
  return result;
}

ExcelValue results_e84() { return constant602; }
ExcelValue results_f84() {
  static ExcelValue result;
  if(variable_set[1773] == 1) { return result;}
  result = ZERO;
  variable_set[1773] = 1;
  return result;
}

ExcelValue results_g84() {
  static ExcelValue result;
  if(variable_set[1774] == 1) { return result;}
  result = ZERO;
  variable_set[1774] = 1;
  return result;
}

ExcelValue results_h84() {
  static ExcelValue result;
  if(variable_set[1775] == 1) { return result;}
  result = ZERO;
  variable_set[1775] = 1;
  return result;
}

ExcelValue results_i84() {
  static ExcelValue result;
  if(variable_set[1776] == 1) { return result;}
  result = ZERO;
  variable_set[1776] = 1;
  return result;
}

ExcelValue results_j84() {
  static ExcelValue result;
  if(variable_set[1777] == 1) { return result;}
  result = ZERO;
  variable_set[1777] = 1;
  return result;
}

ExcelValue results_k84() {
  static ExcelValue result;
  if(variable_set[1778] == 1) { return result;}
  result = ZERO;
  variable_set[1778] = 1;
  return result;
}

ExcelValue results_l84() {
  static ExcelValue result;
  if(variable_set[1779] == 1) { return result;}
  result = ZERO;
  variable_set[1779] = 1;
  return result;
}

ExcelValue results_m84() {
  static ExcelValue result;
  if(variable_set[1780] == 1) { return result;}
  result = ZERO;
  variable_set[1780] = 1;
  return result;
}

ExcelValue results_n84() {
  static ExcelValue result;
  if(variable_set[1781] == 1) { return result;}
  result = ZERO;
  variable_set[1781] = 1;
  return result;
}

ExcelValue results_o84() {
  static ExcelValue result;
  if(variable_set[1782] == 1) { return result;}
  result = ZERO;
  variable_set[1782] = 1;
  return result;
}

ExcelValue results_p84() {
  static ExcelValue result;
  if(variable_set[1783] == 1) { return result;}
  result = ZERO;
  variable_set[1783] = 1;
  return result;
}

ExcelValue results_q84() {
  static ExcelValue result;
  if(variable_set[1784] == 1) { return result;}
  result = ZERO;
  variable_set[1784] = 1;
  return result;
}

ExcelValue results_r84() {
  static ExcelValue result;
  if(variable_set[1785] == 1) { return result;}
  result = ZERO;
  variable_set[1785] = 1;
  return result;
}

ExcelValue results_s84() {
  static ExcelValue result;
  if(variable_set[1786] == 1) { return result;}
  result = ZERO;
  variable_set[1786] = 1;
  return result;
}

ExcelValue results_t84() {
  static ExcelValue result;
  if(variable_set[1787] == 1) { return result;}
  result = ZERO;
  variable_set[1787] = 1;
  return result;
}

ExcelValue results_u84() {
  static ExcelValue result;
  if(variable_set[1788] == 1) { return result;}
  result = ZERO;
  variable_set[1788] = 1;
  return result;
}

ExcelValue results_v84() {
  static ExcelValue result;
  if(variable_set[1789] == 1) { return result;}
  result = ZERO;
  variable_set[1789] = 1;
  return result;
}

ExcelValue results_w84() {
  static ExcelValue result;
  if(variable_set[1790] == 1) { return result;}
  result = ZERO;
  variable_set[1790] = 1;
  return result;
}

ExcelValue results_e85() { return constant603; }
ExcelValue results_f85() { return constant604; }
ExcelValue results_g85() { return constant605; }
ExcelValue results_h85() { return constant606; }
ExcelValue results_i85() { return constant607; }
ExcelValue results_j85() { return constant608; }
ExcelValue results_k85() { return constant609; }
ExcelValue results_l85() { return constant610; }
ExcelValue results_m85() {
  static ExcelValue result;
  if(variable_set[1799] == 1) { return result;}
  result = ZERO;
  variable_set[1799] = 1;
  return result;
}

ExcelValue results_n85() {
  static ExcelValue result;
  if(variable_set[1800] == 1) { return result;}
  result = ZERO;
  variable_set[1800] = 1;
  return result;
}

ExcelValue results_o85() {
  static ExcelValue result;
  if(variable_set[1801] == 1) { return result;}
  result = ZERO;
  variable_set[1801] = 1;
  return result;
}

ExcelValue results_p85() {
  static ExcelValue result;
  if(variable_set[1802] == 1) { return result;}
  result = ZERO;
  variable_set[1802] = 1;
  return result;
}

ExcelValue results_q85() {
  static ExcelValue result;
  if(variable_set[1803] == 1) { return result;}
  result = ZERO;
  variable_set[1803] = 1;
  return result;
}

ExcelValue results_r85() {
  static ExcelValue result;
  if(variable_set[1804] == 1) { return result;}
  result = ZERO;
  variable_set[1804] = 1;
  return result;
}

ExcelValue results_s85() {
  static ExcelValue result;
  if(variable_set[1805] == 1) { return result;}
  result = ZERO;
  variable_set[1805] = 1;
  return result;
}

ExcelValue results_t85() {
  static ExcelValue result;
  if(variable_set[1806] == 1) { return result;}
  result = ZERO;
  variable_set[1806] = 1;
  return result;
}

ExcelValue results_u85() {
  static ExcelValue result;
  if(variable_set[1807] == 1) { return result;}
  result = ZERO;
  variable_set[1807] = 1;
  return result;
}

ExcelValue results_v85() {
  static ExcelValue result;
  if(variable_set[1808] == 1) { return result;}
  result = ZERO;
  variable_set[1808] = 1;
  return result;
}

ExcelValue results_w85() {
  static ExcelValue result;
  if(variable_set[1809] == 1) { return result;}
  result = ZERO;
  variable_set[1809] = 1;
  return result;
}

ExcelValue results_e86() { return constant611; }
ExcelValue results_f86() { return constant612; }
ExcelValue results_g86() { return constant613; }
ExcelValue results_h86() { return constant614; }
ExcelValue results_i86() { return constant615; }
ExcelValue results_j86() { return constant616; }
ExcelValue results_k86() {
  static ExcelValue result;
  if(variable_set[1816] == 1) { return result;}
  result = ZERO;
  variable_set[1816] = 1;
  return result;
}

ExcelValue results_l86() {
  static ExcelValue result;
  if(variable_set[1817] == 1) { return result;}
  result = ZERO;
  variable_set[1817] = 1;
  return result;
}

ExcelValue results_m86() {
  static ExcelValue result;
  if(variable_set[1818] == 1) { return result;}
  result = ZERO;
  variable_set[1818] = 1;
  return result;
}

ExcelValue results_n86() {
  static ExcelValue result;
  if(variable_set[1819] == 1) { return result;}
  result = ZERO;
  variable_set[1819] = 1;
  return result;
}

ExcelValue results_o86() {
  static ExcelValue result;
  if(variable_set[1820] == 1) { return result;}
  result = ZERO;
  variable_set[1820] = 1;
  return result;
}

ExcelValue results_p86() {
  static ExcelValue result;
  if(variable_set[1821] == 1) { return result;}
  result = ZERO;
  variable_set[1821] = 1;
  return result;
}

ExcelValue results_q86() {
  static ExcelValue result;
  if(variable_set[1822] == 1) { return result;}
  result = ZERO;
  variable_set[1822] = 1;
  return result;
}

ExcelValue results_r86() {
  static ExcelValue result;
  if(variable_set[1823] == 1) { return result;}
  result = ZERO;
  variable_set[1823] = 1;
  return result;
}

ExcelValue results_s86() {
  static ExcelValue result;
  if(variable_set[1824] == 1) { return result;}
  result = ZERO;
  variable_set[1824] = 1;
  return result;
}

ExcelValue results_t86() {
  static ExcelValue result;
  if(variable_set[1825] == 1) { return result;}
  result = ZERO;
  variable_set[1825] = 1;
  return result;
}

ExcelValue results_u86() {
  static ExcelValue result;
  if(variable_set[1826] == 1) { return result;}
  result = ZERO;
  variable_set[1826] = 1;
  return result;
}

ExcelValue results_v86() {
  static ExcelValue result;
  if(variable_set[1827] == 1) { return result;}
  result = ZERO;
  variable_set[1827] = 1;
  return result;
}

ExcelValue results_w86() {
  static ExcelValue result;
  if(variable_set[1828] == 1) { return result;}
  result = ZERO;
  variable_set[1828] = 1;
  return result;
}

ExcelValue results_e87() { return constant617; }
ExcelValue results_f87() {
  static ExcelValue result;
  if(variable_set[1830] == 1) { return result;}
  result = ZERO;
  variable_set[1830] = 1;
  return result;
}

ExcelValue results_g87() {
  static ExcelValue result;
  if(variable_set[1831] == 1) { return result;}
  result = ZERO;
  variable_set[1831] = 1;
  return result;
}

ExcelValue results_h87() {
  static ExcelValue result;
  if(variable_set[1832] == 1) { return result;}
  result = ZERO;
  variable_set[1832] = 1;
  return result;
}

ExcelValue results_i87() {
  static ExcelValue result;
  if(variable_set[1833] == 1) { return result;}
  result = ZERO;
  variable_set[1833] = 1;
  return result;
}

ExcelValue results_j87() {
  static ExcelValue result;
  if(variable_set[1834] == 1) { return result;}
  result = ZERO;
  variable_set[1834] = 1;
  return result;
}

ExcelValue results_k87() {
  static ExcelValue result;
  if(variable_set[1835] == 1) { return result;}
  result = ZERO;
  variable_set[1835] = 1;
  return result;
}

ExcelValue results_l87() {
  static ExcelValue result;
  if(variable_set[1836] == 1) { return result;}
  result = ZERO;
  variable_set[1836] = 1;
  return result;
}

ExcelValue results_m87() {
  static ExcelValue result;
  if(variable_set[1837] == 1) { return result;}
  result = ZERO;
  variable_set[1837] = 1;
  return result;
}

ExcelValue results_n87() {
  static ExcelValue result;
  if(variable_set[1838] == 1) { return result;}
  result = ZERO;
  variable_set[1838] = 1;
  return result;
}

ExcelValue results_o87() {
  static ExcelValue result;
  if(variable_set[1839] == 1) { return result;}
  result = ZERO;
  variable_set[1839] = 1;
  return result;
}

ExcelValue results_p87() {
  static ExcelValue result;
  if(variable_set[1840] == 1) { return result;}
  result = ZERO;
  variable_set[1840] = 1;
  return result;
}

ExcelValue results_q87() {
  static ExcelValue result;
  if(variable_set[1841] == 1) { return result;}
  result = ZERO;
  variable_set[1841] = 1;
  return result;
}

ExcelValue results_r87() {
  static ExcelValue result;
  if(variable_set[1842] == 1) { return result;}
  result = ZERO;
  variable_set[1842] = 1;
  return result;
}

ExcelValue results_s87() {
  static ExcelValue result;
  if(variable_set[1843] == 1) { return result;}
  result = ZERO;
  variable_set[1843] = 1;
  return result;
}

ExcelValue results_t87() {
  static ExcelValue result;
  if(variable_set[1844] == 1) { return result;}
  result = ZERO;
  variable_set[1844] = 1;
  return result;
}

ExcelValue results_u87() {
  static ExcelValue result;
  if(variable_set[1845] == 1) { return result;}
  result = ZERO;
  variable_set[1845] = 1;
  return result;
}

ExcelValue results_v87() {
  static ExcelValue result;
  if(variable_set[1846] == 1) { return result;}
  result = ZERO;
  variable_set[1846] = 1;
  return result;
}

ExcelValue results_w87() {
  static ExcelValue result;
  if(variable_set[1847] == 1) { return result;}
  result = ZERO;
  variable_set[1847] = 1;
  return result;
}

ExcelValue results_e88() { return constant618; }
ExcelValue results_f88() { return constant619; }
ExcelValue results_g88() { return constant620; }
ExcelValue results_h88() { return constant621; }
ExcelValue results_i88() { return constant622; }
ExcelValue results_j88() { return constant623; }
ExcelValue results_k88() { return constant624; }
ExcelValue results_l88() { return constant625; }
ExcelValue results_m88() { return constant626; }
ExcelValue results_n88() { return constant627; }
ExcelValue results_o88() { return constant628; }
ExcelValue results_p88() { return constant629; }
ExcelValue results_q88() { return constant630; }
ExcelValue results_r88() { return constant631; }
ExcelValue results_s88() { return constant632; }
ExcelValue results_t88() { return constant633; }
ExcelValue results_u88() { return constant634; }
ExcelValue results_v88() { return constant635; }
ExcelValue results_w88() { return constant636; }
ExcelValue results_e89() { return constant637; }
ExcelValue results_f89() {
  static ExcelValue result;
  if(variable_set[1868] == 1) { return result;}
  result = ZERO;
  variable_set[1868] = 1;
  return result;
}

ExcelValue results_g89() {
  static ExcelValue result;
  if(variable_set[1869] == 1) { return result;}
  result = ZERO;
  variable_set[1869] = 1;
  return result;
}

ExcelValue results_h89() {
  static ExcelValue result;
  if(variable_set[1870] == 1) { return result;}
  result = ZERO;
  variable_set[1870] = 1;
  return result;
}

ExcelValue results_i89() {
  static ExcelValue result;
  if(variable_set[1871] == 1) { return result;}
  result = ZERO;
  variable_set[1871] = 1;
  return result;
}

ExcelValue results_j89() {
  static ExcelValue result;
  if(variable_set[1872] == 1) { return result;}
  result = ZERO;
  variable_set[1872] = 1;
  return result;
}

ExcelValue results_k89() {
  static ExcelValue result;
  if(variable_set[1873] == 1) { return result;}
  result = ZERO;
  variable_set[1873] = 1;
  return result;
}

ExcelValue results_l89() {
  static ExcelValue result;
  if(variable_set[1874] == 1) { return result;}
  result = ZERO;
  variable_set[1874] = 1;
  return result;
}

ExcelValue results_m89() {
  static ExcelValue result;
  if(variable_set[1875] == 1) { return result;}
  result = ZERO;
  variable_set[1875] = 1;
  return result;
}

ExcelValue results_n89() {
  static ExcelValue result;
  if(variable_set[1876] == 1) { return result;}
  result = ZERO;
  variable_set[1876] = 1;
  return result;
}

ExcelValue results_o89() {
  static ExcelValue result;
  if(variable_set[1877] == 1) { return result;}
  result = ZERO;
  variable_set[1877] = 1;
  return result;
}

ExcelValue results_p89() {
  static ExcelValue result;
  if(variable_set[1878] == 1) { return result;}
  result = ZERO;
  variable_set[1878] = 1;
  return result;
}

ExcelValue results_q89() {
  static ExcelValue result;
  if(variable_set[1879] == 1) { return result;}
  result = ZERO;
  variable_set[1879] = 1;
  return result;
}

ExcelValue results_r89() {
  static ExcelValue result;
  if(variable_set[1880] == 1) { return result;}
  result = ZERO;
  variable_set[1880] = 1;
  return result;
}

ExcelValue results_s89() {
  static ExcelValue result;
  if(variable_set[1881] == 1) { return result;}
  result = ZERO;
  variable_set[1881] = 1;
  return result;
}

ExcelValue results_t89() {
  static ExcelValue result;
  if(variable_set[1882] == 1) { return result;}
  result = ZERO;
  variable_set[1882] = 1;
  return result;
}

ExcelValue results_u89() {
  static ExcelValue result;
  if(variable_set[1883] == 1) { return result;}
  result = ZERO;
  variable_set[1883] = 1;
  return result;
}

ExcelValue results_v89() {
  static ExcelValue result;
  if(variable_set[1884] == 1) { return result;}
  result = ZERO;
  variable_set[1884] = 1;
  return result;
}

ExcelValue results_w89() {
  static ExcelValue result;
  if(variable_set[1885] == 1) { return result;}
  result = ZERO;
  variable_set[1885] = 1;
  return result;
}

ExcelValue results_e90() { return constant638; }
ExcelValue results_f90() { return constant639; }
ExcelValue results_g90() { return constant640; }
ExcelValue results_h90() { return constant641; }
ExcelValue results_i90() { return constant642; }
ExcelValue results_j90() { return constant643; }
ExcelValue results_k90() { return constant644; }
ExcelValue results_l90() { return constant645; }
ExcelValue results_m90() { return constant646; }
ExcelValue results_n90() { return constant646; }
ExcelValue results_o90() { return constant646; }
ExcelValue results_p90() { return constant646; }
ExcelValue results_q90() { return constant646; }
ExcelValue results_r90() { return constant646; }
ExcelValue results_s90() { return constant646; }
ExcelValue results_t90() { return constant646; }
ExcelValue results_u90() { return constant646; }
ExcelValue results_v90() { return constant646; }
ExcelValue results_w90() { return constant646; }
ExcelValue results_e91() { return constant647; }
ExcelValue results_f91() { return constant648; }
ExcelValue results_g91() { return constant648; }
ExcelValue results_h91() { return constant649; }
ExcelValue results_i91() { return constant650; }
ExcelValue results_j91() { return constant651; }
ExcelValue results_k91() { return constant652; }
ExcelValue results_l91() { return constant653; }
ExcelValue results_m91() { return constant654; }
ExcelValue results_n91() { return constant654; }
ExcelValue results_o91() { return constant654; }
ExcelValue results_p91() { return constant654; }
ExcelValue results_q91() { return constant654; }
ExcelValue results_r91() { return constant654; }
ExcelValue results_s91() { return constant654; }
ExcelValue results_t91() { return constant654; }
ExcelValue results_u91() { return constant654; }
ExcelValue results_v91() { return constant654; }
ExcelValue results_w91() { return constant654; }
ExcelValue results_e92() { return constant655; }
ExcelValue results_f92() { return constant656; }
ExcelValue results_g92() { return constant657; }
ExcelValue results_h92() { return constant658; }
ExcelValue results_i92() { return constant659; }
ExcelValue results_j92() { return constant660; }
ExcelValue results_k92() { return constant661; }
ExcelValue results_l92() { return constant662; }
ExcelValue results_m92() { return constant663; }
ExcelValue results_n92() { return constant663; }
ExcelValue results_o92() { return constant663; }
ExcelValue results_p92() { return constant663; }
ExcelValue results_q92() { return constant663; }
ExcelValue results_r92() { return constant663; }
ExcelValue results_s92() { return constant663; }
ExcelValue results_t92() { return constant663; }
ExcelValue results_u92() { return constant663; }
ExcelValue results_v92() { return constant663; }
ExcelValue results_w92() { return constant663; }
ExcelValue results_e93() { return constant664; }
ExcelValue results_f93() { return constant665; }
ExcelValue results_g93() { return constant666; }
ExcelValue results_h93() { return constant667; }
ExcelValue results_i93() { return constant668; }
ExcelValue results_j93() { return constant669; }
ExcelValue results_k93() { return constant670; }
ExcelValue results_l93() { return constant671; }
ExcelValue results_m93() { return constant672; }
ExcelValue results_n93() { return constant672; }
ExcelValue results_o93() { return constant672; }
ExcelValue results_p93() { return constant672; }
ExcelValue results_q93() { return constant672; }
ExcelValue results_r93() { return constant672; }
ExcelValue results_s93() { return constant672; }
ExcelValue results_t93() { return constant672; }
ExcelValue results_u93() { return constant672; }
ExcelValue results_v93() { return constant672; }
ExcelValue results_w93() { return constant672; }
ExcelValue results_e94() { return constant673; }
ExcelValue results_f94() {
  static ExcelValue result;
  if(variable_set[1963] == 1) { return result;}
  result = ZERO;
  variable_set[1963] = 1;
  return result;
}

ExcelValue results_g94() {
  static ExcelValue result;
  if(variable_set[1964] == 1) { return result;}
  result = ZERO;
  variable_set[1964] = 1;
  return result;
}

ExcelValue results_h94() {
  static ExcelValue result;
  if(variable_set[1965] == 1) { return result;}
  result = ZERO;
  variable_set[1965] = 1;
  return result;
}

ExcelValue results_i94() {
  static ExcelValue result;
  if(variable_set[1966] == 1) { return result;}
  result = ZERO;
  variable_set[1966] = 1;
  return result;
}

ExcelValue results_j94() {
  static ExcelValue result;
  if(variable_set[1967] == 1) { return result;}
  result = ZERO;
  variable_set[1967] = 1;
  return result;
}

ExcelValue results_k94() {
  static ExcelValue result;
  if(variable_set[1968] == 1) { return result;}
  result = ZERO;
  variable_set[1968] = 1;
  return result;
}

ExcelValue results_l94() {
  static ExcelValue result;
  if(variable_set[1969] == 1) { return result;}
  result = ZERO;
  variable_set[1969] = 1;
  return result;
}

ExcelValue results_m94() {
  static ExcelValue result;
  if(variable_set[1970] == 1) { return result;}
  result = ZERO;
  variable_set[1970] = 1;
  return result;
}

ExcelValue results_n94() {
  static ExcelValue result;
  if(variable_set[1971] == 1) { return result;}
  result = ZERO;
  variable_set[1971] = 1;
  return result;
}

ExcelValue results_o94() {
  static ExcelValue result;
  if(variable_set[1972] == 1) { return result;}
  result = ZERO;
  variable_set[1972] = 1;
  return result;
}

ExcelValue results_p94() {
  static ExcelValue result;
  if(variable_set[1973] == 1) { return result;}
  result = ZERO;
  variable_set[1973] = 1;
  return result;
}

ExcelValue results_q94() {
  static ExcelValue result;
  if(variable_set[1974] == 1) { return result;}
  result = ZERO;
  variable_set[1974] = 1;
  return result;
}

ExcelValue results_r94() {
  static ExcelValue result;
  if(variable_set[1975] == 1) { return result;}
  result = ZERO;
  variable_set[1975] = 1;
  return result;
}

ExcelValue results_s94() {
  static ExcelValue result;
  if(variable_set[1976] == 1) { return result;}
  result = ZERO;
  variable_set[1976] = 1;
  return result;
}

ExcelValue results_t94() {
  static ExcelValue result;
  if(variable_set[1977] == 1) { return result;}
  result = ZERO;
  variable_set[1977] = 1;
  return result;
}

ExcelValue results_u94() {
  static ExcelValue result;
  if(variable_set[1978] == 1) { return result;}
  result = ZERO;
  variable_set[1978] = 1;
  return result;
}

ExcelValue results_v94() {
  static ExcelValue result;
  if(variable_set[1979] == 1) { return result;}
  result = ZERO;
  variable_set[1979] = 1;
  return result;
}

ExcelValue results_w94() {
  static ExcelValue result;
  if(variable_set[1980] == 1) { return result;}
  result = ZERO;
  variable_set[1980] = 1;
  return result;
}

ExcelValue results_e95() { return constant674; }
ExcelValue results_f95() {
  static ExcelValue result;
  if(variable_set[1982] == 1) { return result;}
  result = ZERO;
  variable_set[1982] = 1;
  return result;
}

ExcelValue results_g95() {
  static ExcelValue result;
  if(variable_set[1983] == 1) { return result;}
  result = ZERO;
  variable_set[1983] = 1;
  return result;
}

ExcelValue results_h95() {
  static ExcelValue result;
  if(variable_set[1984] == 1) { return result;}
  result = ZERO;
  variable_set[1984] = 1;
  return result;
}

ExcelValue results_i95() {
  static ExcelValue result;
  if(variable_set[1985] == 1) { return result;}
  result = ZERO;
  variable_set[1985] = 1;
  return result;
}

ExcelValue results_j95() {
  static ExcelValue result;
  if(variable_set[1986] == 1) { return result;}
  result = ZERO;
  variable_set[1986] = 1;
  return result;
}

ExcelValue results_k95() {
  static ExcelValue result;
  if(variable_set[1987] == 1) { return result;}
  result = ZERO;
  variable_set[1987] = 1;
  return result;
}

ExcelValue results_l95() {
  static ExcelValue result;
  if(variable_set[1988] == 1) { return result;}
  result = ZERO;
  variable_set[1988] = 1;
  return result;
}

ExcelValue results_m95() {
  static ExcelValue result;
  if(variable_set[1989] == 1) { return result;}
  result = ZERO;
  variable_set[1989] = 1;
  return result;
}

ExcelValue results_n95() {
  static ExcelValue result;
  if(variable_set[1990] == 1) { return result;}
  result = ZERO;
  variable_set[1990] = 1;
  return result;
}

ExcelValue results_o95() {
  static ExcelValue result;
  if(variable_set[1991] == 1) { return result;}
  result = ZERO;
  variable_set[1991] = 1;
  return result;
}

ExcelValue results_p95() {
  static ExcelValue result;
  if(variable_set[1992] == 1) { return result;}
  result = ZERO;
  variable_set[1992] = 1;
  return result;
}

ExcelValue results_q95() {
  static ExcelValue result;
  if(variable_set[1993] == 1) { return result;}
  result = ZERO;
  variable_set[1993] = 1;
  return result;
}

ExcelValue results_r95() {
  static ExcelValue result;
  if(variable_set[1994] == 1) { return result;}
  result = ZERO;
  variable_set[1994] = 1;
  return result;
}

ExcelValue results_s95() {
  static ExcelValue result;
  if(variable_set[1995] == 1) { return result;}
  result = ZERO;
  variable_set[1995] = 1;
  return result;
}

ExcelValue results_t95() {
  static ExcelValue result;
  if(variable_set[1996] == 1) { return result;}
  result = ZERO;
  variable_set[1996] = 1;
  return result;
}

ExcelValue results_u95() {
  static ExcelValue result;
  if(variable_set[1997] == 1) { return result;}
  result = ZERO;
  variable_set[1997] = 1;
  return result;
}

ExcelValue results_v95() {
  static ExcelValue result;
  if(variable_set[1998] == 1) { return result;}
  result = ZERO;
  variable_set[1998] = 1;
  return result;
}

ExcelValue results_w95() {
  static ExcelValue result;
  if(variable_set[1999] == 1) { return result;}
  result = ZERO;
  variable_set[1999] = 1;
  return result;
}

ExcelValue results_e96() { return constant675; }
ExcelValue results_f96() {
  static ExcelValue result;
  if(variable_set[2001] == 1) { return result;}
  result = ZERO;
  variable_set[2001] = 1;
  return result;
}

ExcelValue results_g96() {
  static ExcelValue result;
  if(variable_set[2002] == 1) { return result;}
  result = ZERO;
  variable_set[2002] = 1;
  return result;
}

ExcelValue results_h96() {
  static ExcelValue result;
  if(variable_set[2003] == 1) { return result;}
  result = ZERO;
  variable_set[2003] = 1;
  return result;
}

ExcelValue results_i96() {
  static ExcelValue result;
  if(variable_set[2004] == 1) { return result;}
  result = ZERO;
  variable_set[2004] = 1;
  return result;
}

ExcelValue results_j96() {
  static ExcelValue result;
  if(variable_set[2005] == 1) { return result;}
  result = ZERO;
  variable_set[2005] = 1;
  return result;
}

ExcelValue results_k96() {
  static ExcelValue result;
  if(variable_set[2006] == 1) { return result;}
  result = ZERO;
  variable_set[2006] = 1;
  return result;
}

ExcelValue results_l96() {
  static ExcelValue result;
  if(variable_set[2007] == 1) { return result;}
  result = ZERO;
  variable_set[2007] = 1;
  return result;
}

ExcelValue results_m96() {
  static ExcelValue result;
  if(variable_set[2008] == 1) { return result;}
  result = ZERO;
  variable_set[2008] = 1;
  return result;
}

ExcelValue results_n96() {
  static ExcelValue result;
  if(variable_set[2009] == 1) { return result;}
  result = ZERO;
  variable_set[2009] = 1;
  return result;
}

ExcelValue results_o96() {
  static ExcelValue result;
  if(variable_set[2010] == 1) { return result;}
  result = ZERO;
  variable_set[2010] = 1;
  return result;
}

ExcelValue results_p96() {
  static ExcelValue result;
  if(variable_set[2011] == 1) { return result;}
  result = ZERO;
  variable_set[2011] = 1;
  return result;
}

ExcelValue results_q96() {
  static ExcelValue result;
  if(variable_set[2012] == 1) { return result;}
  result = ZERO;
  variable_set[2012] = 1;
  return result;
}

ExcelValue results_r96() {
  static ExcelValue result;
  if(variable_set[2013] == 1) { return result;}
  result = ZERO;
  variable_set[2013] = 1;
  return result;
}

ExcelValue results_s96() {
  static ExcelValue result;
  if(variable_set[2014] == 1) { return result;}
  result = ZERO;
  variable_set[2014] = 1;
  return result;
}

ExcelValue results_t96() {
  static ExcelValue result;
  if(variable_set[2015] == 1) { return result;}
  result = ZERO;
  variable_set[2015] = 1;
  return result;
}

ExcelValue results_u96() {
  static ExcelValue result;
  if(variable_set[2016] == 1) { return result;}
  result = ZERO;
  variable_set[2016] = 1;
  return result;
}

ExcelValue results_v96() {
  static ExcelValue result;
  if(variable_set[2017] == 1) { return result;}
  result = ZERO;
  variable_set[2017] = 1;
  return result;
}

ExcelValue results_w96() {
  static ExcelValue result;
  if(variable_set[2018] == 1) { return result;}
  result = ZERO;
  variable_set[2018] = 1;
  return result;
}

ExcelValue results_e97() { return constant676; }
ExcelValue results_f97() {
  static ExcelValue result;
  if(variable_set[2020] == 1) { return result;}
  result = ZERO;
  variable_set[2020] = 1;
  return result;
}

ExcelValue results_g97() {
  static ExcelValue result;
  if(variable_set[2021] == 1) { return result;}
  result = ZERO;
  variable_set[2021] = 1;
  return result;
}

ExcelValue results_h97() {
  static ExcelValue result;
  if(variable_set[2022] == 1) { return result;}
  result = ZERO;
  variable_set[2022] = 1;
  return result;
}

ExcelValue results_i97() {
  static ExcelValue result;
  if(variable_set[2023] == 1) { return result;}
  result = ZERO;
  variable_set[2023] = 1;
  return result;
}

ExcelValue results_j97() {
  static ExcelValue result;
  if(variable_set[2024] == 1) { return result;}
  result = ZERO;
  variable_set[2024] = 1;
  return result;
}

ExcelValue results_k97() {
  static ExcelValue result;
  if(variable_set[2025] == 1) { return result;}
  result = ZERO;
  variable_set[2025] = 1;
  return result;
}

ExcelValue results_l97() {
  static ExcelValue result;
  if(variable_set[2026] == 1) { return result;}
  result = ZERO;
  variable_set[2026] = 1;
  return result;
}

ExcelValue results_m97() {
  static ExcelValue result;
  if(variable_set[2027] == 1) { return result;}
  result = ZERO;
  variable_set[2027] = 1;
  return result;
}

ExcelValue results_n97() {
  static ExcelValue result;
  if(variable_set[2028] == 1) { return result;}
  result = ZERO;
  variable_set[2028] = 1;
  return result;
}

ExcelValue results_o97() {
  static ExcelValue result;
  if(variable_set[2029] == 1) { return result;}
  result = ZERO;
  variable_set[2029] = 1;
  return result;
}

ExcelValue results_p97() {
  static ExcelValue result;
  if(variable_set[2030] == 1) { return result;}
  result = ZERO;
  variable_set[2030] = 1;
  return result;
}

ExcelValue results_q97() {
  static ExcelValue result;
  if(variable_set[2031] == 1) { return result;}
  result = ZERO;
  variable_set[2031] = 1;
  return result;
}

ExcelValue results_r97() {
  static ExcelValue result;
  if(variable_set[2032] == 1) { return result;}
  result = ZERO;
  variable_set[2032] = 1;
  return result;
}

ExcelValue results_s97() {
  static ExcelValue result;
  if(variable_set[2033] == 1) { return result;}
  result = ZERO;
  variable_set[2033] = 1;
  return result;
}

ExcelValue results_t97() {
  static ExcelValue result;
  if(variable_set[2034] == 1) { return result;}
  result = ZERO;
  variable_set[2034] = 1;
  return result;
}

ExcelValue results_u97() {
  static ExcelValue result;
  if(variable_set[2035] == 1) { return result;}
  result = ZERO;
  variable_set[2035] = 1;
  return result;
}

ExcelValue results_v97() {
  static ExcelValue result;
  if(variable_set[2036] == 1) { return result;}
  result = ZERO;
  variable_set[2036] = 1;
  return result;
}

ExcelValue results_w97() {
  static ExcelValue result;
  if(variable_set[2037] == 1) { return result;}
  result = ZERO;
  variable_set[2037] = 1;
  return result;
}

ExcelValue results_e98() { return constant677; }
ExcelValue results_f98() {
  static ExcelValue result;
  if(variable_set[2039] == 1) { return result;}
  result = ZERO;
  variable_set[2039] = 1;
  return result;
}

ExcelValue results_g98() {
  static ExcelValue result;
  if(variable_set[2040] == 1) { return result;}
  result = ZERO;
  variable_set[2040] = 1;
  return result;
}

ExcelValue results_h98() {
  static ExcelValue result;
  if(variable_set[2041] == 1) { return result;}
  result = ZERO;
  variable_set[2041] = 1;
  return result;
}

ExcelValue results_i98() {
  static ExcelValue result;
  if(variable_set[2042] == 1) { return result;}
  result = ZERO;
  variable_set[2042] = 1;
  return result;
}

ExcelValue results_j98() {
  static ExcelValue result;
  if(variable_set[2043] == 1) { return result;}
  result = ZERO;
  variable_set[2043] = 1;
  return result;
}

ExcelValue results_k98() {
  static ExcelValue result;
  if(variable_set[2044] == 1) { return result;}
  result = ZERO;
  variable_set[2044] = 1;
  return result;
}

ExcelValue results_l98() {
  static ExcelValue result;
  if(variable_set[2045] == 1) { return result;}
  result = ZERO;
  variable_set[2045] = 1;
  return result;
}

ExcelValue results_m98() {
  static ExcelValue result;
  if(variable_set[2046] == 1) { return result;}
  result = ZERO;
  variable_set[2046] = 1;
  return result;
}

ExcelValue results_n98() {
  static ExcelValue result;
  if(variable_set[2047] == 1) { return result;}
  result = ZERO;
  variable_set[2047] = 1;
  return result;
}

ExcelValue results_o98() {
  static ExcelValue result;
  if(variable_set[2048] == 1) { return result;}
  result = ZERO;
  variable_set[2048] = 1;
  return result;
}

ExcelValue results_p98() {
  static ExcelValue result;
  if(variable_set[2049] == 1) { return result;}
  result = ZERO;
  variable_set[2049] = 1;
  return result;
}

ExcelValue results_q98() {
  static ExcelValue result;
  if(variable_set[2050] == 1) { return result;}
  result = ZERO;
  variable_set[2050] = 1;
  return result;
}

ExcelValue results_r98() {
  static ExcelValue result;
  if(variable_set[2051] == 1) { return result;}
  result = ZERO;
  variable_set[2051] = 1;
  return result;
}

ExcelValue results_s98() {
  static ExcelValue result;
  if(variable_set[2052] == 1) { return result;}
  result = ZERO;
  variable_set[2052] = 1;
  return result;
}

ExcelValue results_t98() {
  static ExcelValue result;
  if(variable_set[2053] == 1) { return result;}
  result = ZERO;
  variable_set[2053] = 1;
  return result;
}

ExcelValue results_u98() {
  static ExcelValue result;
  if(variable_set[2054] == 1) { return result;}
  result = ZERO;
  variable_set[2054] = 1;
  return result;
}

ExcelValue results_v98() {
  static ExcelValue result;
  if(variable_set[2055] == 1) { return result;}
  result = ZERO;
  variable_set[2055] = 1;
  return result;
}

ExcelValue results_w98() {
  static ExcelValue result;
  if(variable_set[2056] == 1) { return result;}
  result = ZERO;
  variable_set[2056] = 1;
  return result;
}

ExcelValue results_e99() { return constant678; }
ExcelValue results_f99() {
  static ExcelValue result;
  if(variable_set[2058] == 1) { return result;}
  result = ZERO;
  variable_set[2058] = 1;
  return result;
}

ExcelValue results_g99() {
  static ExcelValue result;
  if(variable_set[2059] == 1) { return result;}
  result = ZERO;
  variable_set[2059] = 1;
  return result;
}

ExcelValue results_h99() {
  static ExcelValue result;
  if(variable_set[2060] == 1) { return result;}
  result = ZERO;
  variable_set[2060] = 1;
  return result;
}

ExcelValue results_i99() {
  static ExcelValue result;
  if(variable_set[2061] == 1) { return result;}
  result = ZERO;
  variable_set[2061] = 1;
  return result;
}

ExcelValue results_j99() {
  static ExcelValue result;
  if(variable_set[2062] == 1) { return result;}
  result = ZERO;
  variable_set[2062] = 1;
  return result;
}

ExcelValue results_k99() {
  static ExcelValue result;
  if(variable_set[2063] == 1) { return result;}
  result = ZERO;
  variable_set[2063] = 1;
  return result;
}

ExcelValue results_l99() {
  static ExcelValue result;
  if(variable_set[2064] == 1) { return result;}
  result = ZERO;
  variable_set[2064] = 1;
  return result;
}

ExcelValue results_m99() {
  static ExcelValue result;
  if(variable_set[2065] == 1) { return result;}
  result = ZERO;
  variable_set[2065] = 1;
  return result;
}

ExcelValue results_n99() {
  static ExcelValue result;
  if(variable_set[2066] == 1) { return result;}
  result = ZERO;
  variable_set[2066] = 1;
  return result;
}

ExcelValue results_o99() {
  static ExcelValue result;
  if(variable_set[2067] == 1) { return result;}
  result = ZERO;
  variable_set[2067] = 1;
  return result;
}

ExcelValue results_p99() {
  static ExcelValue result;
  if(variable_set[2068] == 1) { return result;}
  result = ZERO;
  variable_set[2068] = 1;
  return result;
}

ExcelValue results_q99() {
  static ExcelValue result;
  if(variable_set[2069] == 1) { return result;}
  result = ZERO;
  variable_set[2069] = 1;
  return result;
}

ExcelValue results_r99() {
  static ExcelValue result;
  if(variable_set[2070] == 1) { return result;}
  result = ZERO;
  variable_set[2070] = 1;
  return result;
}

ExcelValue results_s99() {
  static ExcelValue result;
  if(variable_set[2071] == 1) { return result;}
  result = ZERO;
  variable_set[2071] = 1;
  return result;
}

ExcelValue results_t99() {
  static ExcelValue result;
  if(variable_set[2072] == 1) { return result;}
  result = ZERO;
  variable_set[2072] = 1;
  return result;
}

ExcelValue results_u99() {
  static ExcelValue result;
  if(variable_set[2073] == 1) { return result;}
  result = ZERO;
  variable_set[2073] = 1;
  return result;
}

ExcelValue results_v99() {
  static ExcelValue result;
  if(variable_set[2074] == 1) { return result;}
  result = ZERO;
  variable_set[2074] = 1;
  return result;
}

ExcelValue results_w99() {
  static ExcelValue result;
  if(variable_set[2075] == 1) { return result;}
  result = ZERO;
  variable_set[2075] = 1;
  return result;
}

ExcelValue results_e100() { return constant679; }
ExcelValue results_f100() {
  static ExcelValue result;
  if(variable_set[2077] == 1) { return result;}
  result = ZERO;
  variable_set[2077] = 1;
  return result;
}

ExcelValue results_g100() {
  static ExcelValue result;
  if(variable_set[2078] == 1) { return result;}
  result = ZERO;
  variable_set[2078] = 1;
  return result;
}

ExcelValue results_h100() {
  static ExcelValue result;
  if(variable_set[2079] == 1) { return result;}
  result = ZERO;
  variable_set[2079] = 1;
  return result;
}

ExcelValue results_i100() {
  static ExcelValue result;
  if(variable_set[2080] == 1) { return result;}
  result = ZERO;
  variable_set[2080] = 1;
  return result;
}

ExcelValue results_j100() {
  static ExcelValue result;
  if(variable_set[2081] == 1) { return result;}
  result = ZERO;
  variable_set[2081] = 1;
  return result;
}

ExcelValue results_k100() {
  static ExcelValue result;
  if(variable_set[2082] == 1) { return result;}
  result = ZERO;
  variable_set[2082] = 1;
  return result;
}

ExcelValue results_l100() {
  static ExcelValue result;
  if(variable_set[2083] == 1) { return result;}
  result = ZERO;
  variable_set[2083] = 1;
  return result;
}

ExcelValue results_m100() {
  static ExcelValue result;
  if(variable_set[2084] == 1) { return result;}
  result = ZERO;
  variable_set[2084] = 1;
  return result;
}

ExcelValue results_n100() {
  static ExcelValue result;
  if(variable_set[2085] == 1) { return result;}
  result = ZERO;
  variable_set[2085] = 1;
  return result;
}

ExcelValue results_o100() {
  static ExcelValue result;
  if(variable_set[2086] == 1) { return result;}
  result = ZERO;
  variable_set[2086] = 1;
  return result;
}

ExcelValue results_p100() {
  static ExcelValue result;
  if(variable_set[2087] == 1) { return result;}
  result = ZERO;
  variable_set[2087] = 1;
  return result;
}

ExcelValue results_q100() {
  static ExcelValue result;
  if(variable_set[2088] == 1) { return result;}
  result = ZERO;
  variable_set[2088] = 1;
  return result;
}

ExcelValue results_r100() {
  static ExcelValue result;
  if(variable_set[2089] == 1) { return result;}
  result = ZERO;
  variable_set[2089] = 1;
  return result;
}

ExcelValue results_s100() {
  static ExcelValue result;
  if(variable_set[2090] == 1) { return result;}
  result = ZERO;
  variable_set[2090] = 1;
  return result;
}

ExcelValue results_t100() {
  static ExcelValue result;
  if(variable_set[2091] == 1) { return result;}
  result = ZERO;
  variable_set[2091] = 1;
  return result;
}

ExcelValue results_u100() {
  static ExcelValue result;
  if(variable_set[2092] == 1) { return result;}
  result = ZERO;
  variable_set[2092] = 1;
  return result;
}

ExcelValue results_v100() {
  static ExcelValue result;
  if(variable_set[2093] == 1) { return result;}
  result = ZERO;
  variable_set[2093] = 1;
  return result;
}

ExcelValue results_w100() {
  static ExcelValue result;
  if(variable_set[2094] == 1) { return result;}
  result = ZERO;
  variable_set[2094] = 1;
  return result;
}

ExcelValue results_e101() { return constant680; }
ExcelValue results_f101() {
  static ExcelValue result;
  if(variable_set[2096] == 1) { return result;}
  result = ZERO;
  variable_set[2096] = 1;
  return result;
}

ExcelValue results_g101() {
  static ExcelValue result;
  if(variable_set[2097] == 1) { return result;}
  result = ZERO;
  variable_set[2097] = 1;
  return result;
}

ExcelValue results_h101() {
  static ExcelValue result;
  if(variable_set[2098] == 1) { return result;}
  result = ZERO;
  variable_set[2098] = 1;
  return result;
}

ExcelValue results_i101() {
  static ExcelValue result;
  if(variable_set[2099] == 1) { return result;}
  result = ZERO;
  variable_set[2099] = 1;
  return result;
}

ExcelValue results_j101() {
  static ExcelValue result;
  if(variable_set[2100] == 1) { return result;}
  result = ZERO;
  variable_set[2100] = 1;
  return result;
}

ExcelValue results_k101() {
  static ExcelValue result;
  if(variable_set[2101] == 1) { return result;}
  result = ZERO;
  variable_set[2101] = 1;
  return result;
}

ExcelValue results_l101() {
  static ExcelValue result;
  if(variable_set[2102] == 1) { return result;}
  result = ZERO;
  variable_set[2102] = 1;
  return result;
}

ExcelValue results_m101() {
  static ExcelValue result;
  if(variable_set[2103] == 1) { return result;}
  result = ZERO;
  variable_set[2103] = 1;
  return result;
}

ExcelValue results_n101() {
  static ExcelValue result;
  if(variable_set[2104] == 1) { return result;}
  result = ZERO;
  variable_set[2104] = 1;
  return result;
}

ExcelValue results_o101() {
  static ExcelValue result;
  if(variable_set[2105] == 1) { return result;}
  result = ZERO;
  variable_set[2105] = 1;
  return result;
}

ExcelValue results_p101() {
  static ExcelValue result;
  if(variable_set[2106] == 1) { return result;}
  result = ZERO;
  variable_set[2106] = 1;
  return result;
}

ExcelValue results_q101() {
  static ExcelValue result;
  if(variable_set[2107] == 1) { return result;}
  result = ZERO;
  variable_set[2107] = 1;
  return result;
}

ExcelValue results_r101() {
  static ExcelValue result;
  if(variable_set[2108] == 1) { return result;}
  result = ZERO;
  variable_set[2108] = 1;
  return result;
}

ExcelValue results_s101() {
  static ExcelValue result;
  if(variable_set[2109] == 1) { return result;}
  result = ZERO;
  variable_set[2109] = 1;
  return result;
}

ExcelValue results_t101() {
  static ExcelValue result;
  if(variable_set[2110] == 1) { return result;}
  result = ZERO;
  variable_set[2110] = 1;
  return result;
}

ExcelValue results_u101() {
  static ExcelValue result;
  if(variable_set[2111] == 1) { return result;}
  result = ZERO;
  variable_set[2111] = 1;
  return result;
}

ExcelValue results_v101() {
  static ExcelValue result;
  if(variable_set[2112] == 1) { return result;}
  result = ZERO;
  variable_set[2112] = 1;
  return result;
}

ExcelValue results_w101() {
  static ExcelValue result;
  if(variable_set[2113] == 1) { return result;}
  result = ZERO;
  variable_set[2113] = 1;
  return result;
}

ExcelValue results_e102() { return constant681; }
ExcelValue results_f102() {
  static ExcelValue result;
  if(variable_set[2115] == 1) { return result;}
  result = ZERO;
  variable_set[2115] = 1;
  return result;
}

ExcelValue results_g102() {
  static ExcelValue result;
  if(variable_set[2116] == 1) { return result;}
  result = ZERO;
  variable_set[2116] = 1;
  return result;
}

ExcelValue results_h102() {
  static ExcelValue result;
  if(variable_set[2117] == 1) { return result;}
  result = ZERO;
  variable_set[2117] = 1;
  return result;
}

ExcelValue results_i102() {
  static ExcelValue result;
  if(variable_set[2118] == 1) { return result;}
  result = ZERO;
  variable_set[2118] = 1;
  return result;
}

ExcelValue results_j102() {
  static ExcelValue result;
  if(variable_set[2119] == 1) { return result;}
  result = ZERO;
  variable_set[2119] = 1;
  return result;
}

ExcelValue results_k102() {
  static ExcelValue result;
  if(variable_set[2120] == 1) { return result;}
  result = ZERO;
  variable_set[2120] = 1;
  return result;
}

ExcelValue results_l102() {
  static ExcelValue result;
  if(variable_set[2121] == 1) { return result;}
  result = ZERO;
  variable_set[2121] = 1;
  return result;
}

ExcelValue results_m102() {
  static ExcelValue result;
  if(variable_set[2122] == 1) { return result;}
  result = ZERO;
  variable_set[2122] = 1;
  return result;
}

ExcelValue results_n102() {
  static ExcelValue result;
  if(variable_set[2123] == 1) { return result;}
  result = ZERO;
  variable_set[2123] = 1;
  return result;
}

ExcelValue results_o102() {
  static ExcelValue result;
  if(variable_set[2124] == 1) { return result;}
  result = ZERO;
  variable_set[2124] = 1;
  return result;
}

ExcelValue results_p102() {
  static ExcelValue result;
  if(variable_set[2125] == 1) { return result;}
  result = ZERO;
  variable_set[2125] = 1;
  return result;
}

ExcelValue results_q102() {
  static ExcelValue result;
  if(variable_set[2126] == 1) { return result;}
  result = ZERO;
  variable_set[2126] = 1;
  return result;
}

ExcelValue results_r102() {
  static ExcelValue result;
  if(variable_set[2127] == 1) { return result;}
  result = ZERO;
  variable_set[2127] = 1;
  return result;
}

ExcelValue results_s102() {
  static ExcelValue result;
  if(variable_set[2128] == 1) { return result;}
  result = ZERO;
  variable_set[2128] = 1;
  return result;
}

ExcelValue results_t102() {
  static ExcelValue result;
  if(variable_set[2129] == 1) { return result;}
  result = ZERO;
  variable_set[2129] = 1;
  return result;
}

ExcelValue results_u102() {
  static ExcelValue result;
  if(variable_set[2130] == 1) { return result;}
  result = ZERO;
  variable_set[2130] = 1;
  return result;
}

ExcelValue results_v102() {
  static ExcelValue result;
  if(variable_set[2131] == 1) { return result;}
  result = ZERO;
  variable_set[2131] = 1;
  return result;
}

ExcelValue results_w102() {
  static ExcelValue result;
  if(variable_set[2132] == 1) { return result;}
  result = ZERO;
  variable_set[2132] = 1;
  return result;
}

ExcelValue results_e103() { return constant682; }
ExcelValue results_f103() {
  static ExcelValue result;
  if(variable_set[2134] == 1) { return result;}
  result = ZERO;
  variable_set[2134] = 1;
  return result;
}

ExcelValue results_g103() {
  static ExcelValue result;
  if(variable_set[2135] == 1) { return result;}
  result = ZERO;
  variable_set[2135] = 1;
  return result;
}

ExcelValue results_h103() {
  static ExcelValue result;
  if(variable_set[2136] == 1) { return result;}
  result = ZERO;
  variable_set[2136] = 1;
  return result;
}

ExcelValue results_i103() {
  static ExcelValue result;
  if(variable_set[2137] == 1) { return result;}
  result = ZERO;
  variable_set[2137] = 1;
  return result;
}

ExcelValue results_j103() {
  static ExcelValue result;
  if(variable_set[2138] == 1) { return result;}
  result = ZERO;
  variable_set[2138] = 1;
  return result;
}

ExcelValue results_k103() {
  static ExcelValue result;
  if(variable_set[2139] == 1) { return result;}
  result = ZERO;
  variable_set[2139] = 1;
  return result;
}

ExcelValue results_l103() {
  static ExcelValue result;
  if(variable_set[2140] == 1) { return result;}
  result = ZERO;
  variable_set[2140] = 1;
  return result;
}

ExcelValue results_m103() {
  static ExcelValue result;
  if(variable_set[2141] == 1) { return result;}
  result = ZERO;
  variable_set[2141] = 1;
  return result;
}

ExcelValue results_n103() {
  static ExcelValue result;
  if(variable_set[2142] == 1) { return result;}
  result = ZERO;
  variable_set[2142] = 1;
  return result;
}

ExcelValue results_o103() {
  static ExcelValue result;
  if(variable_set[2143] == 1) { return result;}
  result = ZERO;
  variable_set[2143] = 1;
  return result;
}

ExcelValue results_p103() {
  static ExcelValue result;
  if(variable_set[2144] == 1) { return result;}
  result = ZERO;
  variable_set[2144] = 1;
  return result;
}

ExcelValue results_q103() {
  static ExcelValue result;
  if(variable_set[2145] == 1) { return result;}
  result = ZERO;
  variable_set[2145] = 1;
  return result;
}

ExcelValue results_r103() {
  static ExcelValue result;
  if(variable_set[2146] == 1) { return result;}
  result = ZERO;
  variable_set[2146] = 1;
  return result;
}

ExcelValue results_s103() {
  static ExcelValue result;
  if(variable_set[2147] == 1) { return result;}
  result = ZERO;
  variable_set[2147] = 1;
  return result;
}

ExcelValue results_t103() {
  static ExcelValue result;
  if(variable_set[2148] == 1) { return result;}
  result = ZERO;
  variable_set[2148] = 1;
  return result;
}

ExcelValue results_u103() {
  static ExcelValue result;
  if(variable_set[2149] == 1) { return result;}
  result = ZERO;
  variable_set[2149] = 1;
  return result;
}

ExcelValue results_v103() {
  static ExcelValue result;
  if(variable_set[2150] == 1) { return result;}
  result = ZERO;
  variable_set[2150] = 1;
  return result;
}

ExcelValue results_w103() {
  static ExcelValue result;
  if(variable_set[2151] == 1) { return result;}
  result = ZERO;
  variable_set[2151] = 1;
  return result;
}

ExcelValue results_e104() { return constant683; }
ExcelValue results_f104() { return constant684; }
ExcelValue results_g104() { return constant685; }
ExcelValue results_h104() { return constant686; }
ExcelValue results_i104() { return constant687; }
ExcelValue results_j104() { return constant688; }
ExcelValue results_k104() { return constant689; }
ExcelValue results_l104() { return constant690; }
ExcelValue results_m104() { return constant691; }
ExcelValue results_n104() { return constant692; }
ExcelValue results_o104() { return constant693; }
ExcelValue results_p104() { return constant694; }
ExcelValue results_q104() { return constant695; }
ExcelValue results_r104() { return constant696; }
ExcelValue results_s104() { return constant697; }
ExcelValue results_t104() { return constant698; }
ExcelValue results_u104() { return constant699; }
ExcelValue results_v104() { return constant700; }
ExcelValue results_w104() { return constant701; }
ExcelValue results_e105() { return constant702; }
ExcelValue results_f105() {
  static ExcelValue result;
  if(variable_set[2172] == 1) { return result;}
  result = ZERO;
  variable_set[2172] = 1;
  return result;
}

ExcelValue results_g105() {
  static ExcelValue result;
  if(variable_set[2173] == 1) { return result;}
  result = ZERO;
  variable_set[2173] = 1;
  return result;
}

ExcelValue results_h105() {
  static ExcelValue result;
  if(variable_set[2174] == 1) { return result;}
  result = ZERO;
  variable_set[2174] = 1;
  return result;
}

ExcelValue results_i105() {
  static ExcelValue result;
  if(variable_set[2175] == 1) { return result;}
  result = ZERO;
  variable_set[2175] = 1;
  return result;
}

ExcelValue results_j105() {
  static ExcelValue result;
  if(variable_set[2176] == 1) { return result;}
  result = ZERO;
  variable_set[2176] = 1;
  return result;
}

ExcelValue results_k105() {
  static ExcelValue result;
  if(variable_set[2177] == 1) { return result;}
  result = ZERO;
  variable_set[2177] = 1;
  return result;
}

ExcelValue results_l105() {
  static ExcelValue result;
  if(variable_set[2178] == 1) { return result;}
  result = ZERO;
  variable_set[2178] = 1;
  return result;
}

ExcelValue results_m105() {
  static ExcelValue result;
  if(variable_set[2179] == 1) { return result;}
  result = ZERO;
  variable_set[2179] = 1;
  return result;
}

ExcelValue results_n105() {
  static ExcelValue result;
  if(variable_set[2180] == 1) { return result;}
  result = ZERO;
  variable_set[2180] = 1;
  return result;
}

ExcelValue results_o105() {
  static ExcelValue result;
  if(variable_set[2181] == 1) { return result;}
  result = ZERO;
  variable_set[2181] = 1;
  return result;
}

ExcelValue results_p105() {
  static ExcelValue result;
  if(variable_set[2182] == 1) { return result;}
  result = ZERO;
  variable_set[2182] = 1;
  return result;
}

ExcelValue results_q105() {
  static ExcelValue result;
  if(variable_set[2183] == 1) { return result;}
  result = ZERO;
  variable_set[2183] = 1;
  return result;
}

ExcelValue results_r105() {
  static ExcelValue result;
  if(variable_set[2184] == 1) { return result;}
  result = ZERO;
  variable_set[2184] = 1;
  return result;
}

ExcelValue results_s105() {
  static ExcelValue result;
  if(variable_set[2185] == 1) { return result;}
  result = ZERO;
  variable_set[2185] = 1;
  return result;
}

ExcelValue results_t105() {
  static ExcelValue result;
  if(variable_set[2186] == 1) { return result;}
  result = ZERO;
  variable_set[2186] = 1;
  return result;
}

ExcelValue results_u105() {
  static ExcelValue result;
  if(variable_set[2187] == 1) { return result;}
  result = ZERO;
  variable_set[2187] = 1;
  return result;
}

ExcelValue results_v105() {
  static ExcelValue result;
  if(variable_set[2188] == 1) { return result;}
  result = ZERO;
  variable_set[2188] = 1;
  return result;
}

ExcelValue results_w105() {
  static ExcelValue result;
  if(variable_set[2189] == 1) { return result;}
  result = ZERO;
  variable_set[2189] = 1;
  return result;
}

ExcelValue results_e106() { return constant703; }
ExcelValue results_f106() { return constant704; }
ExcelValue results_g106() { return constant705; }
ExcelValue results_h106() { return constant706; }
ExcelValue results_i106() { return constant707; }
ExcelValue results_j106() { return constant708; }
ExcelValue results_k106() { return constant709; }
ExcelValue results_l106() { return constant710; }
ExcelValue results_m106() { return constant711; }
ExcelValue results_n106() { return constant712; }
ExcelValue results_o106() { return constant713; }
ExcelValue results_p106() { return constant714; }
ExcelValue results_q106() { return constant715; }
ExcelValue results_r106() { return constant716; }
ExcelValue results_s106() { return constant717; }
ExcelValue results_t106() { return constant718; }
ExcelValue results_u106() { return constant719; }
ExcelValue results_v106() { return constant720; }
ExcelValue results_w106() { return constant721; }
ExcelValue results_e107() { return constant722; }
ExcelValue results_f107() { return constant723; }
ExcelValue results_g107() { return constant724; }
ExcelValue results_h107() { return constant725; }
ExcelValue results_i107() { return constant726; }
ExcelValue results_j107() { return constant727; }
ExcelValue results_k107() { return constant728; }
ExcelValue results_l107() { return constant729; }
ExcelValue results_m107() { return constant730; }
ExcelValue results_n107() { return constant731; }
ExcelValue results_o107() { return constant732; }
ExcelValue results_p107() { return constant733; }
ExcelValue results_q107() { return constant734; }
ExcelValue results_r107() { return constant734; }
ExcelValue results_s107() { return constant734; }
ExcelValue results_t107() { return constant734; }
ExcelValue results_u107() { return constant734; }
ExcelValue results_v107() { return constant734; }
ExcelValue results_w107() { return constant734; }
ExcelValue results_e108() { return constant735; }
ExcelValue results_f108() { return constant736; }
ExcelValue results_g108() { return constant736; }
ExcelValue results_h108() { return constant737; }
ExcelValue results_i108() { return constant738; }
ExcelValue results_j108() { return constant739; }
ExcelValue results_k108() { return constant740; }
ExcelValue results_l108() { return constant741; }
ExcelValue results_m108() { return constant742; }
ExcelValue results_n108() { return constant742; }
ExcelValue results_o108() { return constant742; }
ExcelValue results_p108() { return constant742; }
ExcelValue results_q108() { return constant742; }
ExcelValue results_r108() { return constant742; }
ExcelValue results_s108() { return constant742; }
ExcelValue results_t108() { return constant742; }
ExcelValue results_u108() { return constant742; }
ExcelValue results_v108() { return constant742; }
ExcelValue results_w108() { return constant742; }
ExcelValue results_e109() { return constant743; }
ExcelValue results_f109() { return constant744; }
ExcelValue results_g109() { return constant745; }
ExcelValue results_h109() { return constant746; }
ExcelValue results_i109() { return constant747; }
ExcelValue results_j109() { return constant748; }
ExcelValue results_k109() { return constant749; }
ExcelValue results_l109() { return constant750; }
ExcelValue results_m109() { return constant751; }
ExcelValue results_n109() { return constant751; }
ExcelValue results_o109() { return constant751; }
ExcelValue results_p109() { return constant751; }
ExcelValue results_q109() { return constant751; }
ExcelValue results_r109() { return constant751; }
ExcelValue results_s109() { return constant751; }
ExcelValue results_t109() { return constant751; }
ExcelValue results_u109() { return constant751; }
ExcelValue results_v109() { return constant751; }
ExcelValue results_w109() { return constant751; }
ExcelValue results_e110() { return constant752; }
ExcelValue results_f110() { return constant753; }
ExcelValue results_g110() { return constant753; }
ExcelValue results_h110() { return constant753; }
ExcelValue results_i110() { return constant753; }
ExcelValue results_j110() { return constant753; }
ExcelValue results_k110() { return constant753; }
ExcelValue results_l110() { return constant753; }
ExcelValue results_m110() { return constant753; }
ExcelValue results_n110() { return constant753; }
ExcelValue results_o110() { return constant753; }
ExcelValue results_p110() { return constant753; }
ExcelValue results_q110() { return constant753; }
ExcelValue results_r110() { return constant753; }
ExcelValue results_s110() { return constant753; }
ExcelValue results_t110() { return constant753; }
ExcelValue results_u110() { return constant753; }
ExcelValue results_v110() { return constant753; }
ExcelValue results_w110() { return constant753; }
ExcelValue results_e111() { return constant754; }
ExcelValue results_f111() { return constant755; }
ExcelValue results_g111() { return constant755; }
ExcelValue results_h111() { return constant755; }
ExcelValue results_i111() { return constant755; }
ExcelValue results_j111() { return constant755; }
ExcelValue results_k111() { return constant755; }
ExcelValue results_l111() { return constant755; }
ExcelValue results_m111() { return constant755; }
ExcelValue results_n111() { return constant755; }
ExcelValue results_o111() { return constant755; }
ExcelValue results_p111() { return constant755; }
ExcelValue results_q111() { return constant755; }
ExcelValue results_r111() { return constant755; }
ExcelValue results_s111() { return constant755; }
ExcelValue results_t111() { return constant755; }
ExcelValue results_u111() { return constant755; }
ExcelValue results_v111() { return constant755; }
ExcelValue results_w111() { return constant755; }
ExcelValue results_e112() { return constant756; }
ExcelValue results_f112() { return constant757; }
ExcelValue results_g112() { return constant758; }
ExcelValue results_h112() { return constant759; }
ExcelValue results_i112() { return constant760; }
ExcelValue results_j112() { return constant761; }
ExcelValue results_k112() { return constant762; }
ExcelValue results_l112() { return constant763; }
ExcelValue results_m112() { return constant764; }
ExcelValue results_n112() { return constant764; }
ExcelValue results_o112() { return constant764; }
ExcelValue results_p112() { return constant764; }
ExcelValue results_q112() { return constant764; }
ExcelValue results_r112() { return constant764; }
ExcelValue results_s112() { return constant764; }
ExcelValue results_t112() { return constant764; }
ExcelValue results_u112() { return constant764; }
ExcelValue results_v112() { return constant764; }
ExcelValue results_w112() { return constant764; }
ExcelValue results_e113() { return constant765; }
ExcelValue results_f113() { return constant766; }
ExcelValue results_g113() { return constant767; }
ExcelValue results_h113() { return constant768; }
ExcelValue results_i113() { return constant769; }
ExcelValue results_j113() { return constant770; }
ExcelValue results_k113() { return constant771; }
ExcelValue results_l113() { return constant772; }
ExcelValue results_m113() { return constant773; }
ExcelValue results_n113() { return constant773; }
ExcelValue results_o113() { return constant773; }
ExcelValue results_p113() { return constant773; }
ExcelValue results_q113() { return constant773; }
ExcelValue results_r113() { return constant773; }
ExcelValue results_s113() { return constant773; }
ExcelValue results_t113() { return constant773; }
ExcelValue results_u113() { return constant773; }
ExcelValue results_v113() { return constant773; }
ExcelValue results_w113() { return constant773; }
ExcelValue results_e114() { return constant774; }
ExcelValue results_f114() { return constant775; }
ExcelValue results_g114() { return constant776; }
ExcelValue results_h114() { return constant777; }
ExcelValue results_i114() { return constant778; }
ExcelValue results_j114() { return constant779; }
ExcelValue results_k114() { return constant780; }
ExcelValue results_l114() { return constant781; }
ExcelValue results_m114() { return constant782; }
ExcelValue results_n114() { return constant782; }
ExcelValue results_o114() { return constant782; }
ExcelValue results_p114() { return constant782; }
ExcelValue results_q114() { return constant782; }
ExcelValue results_r114() { return constant782; }
ExcelValue results_s114() { return constant782; }
ExcelValue results_t114() { return constant782; }
ExcelValue results_u114() { return constant782; }
ExcelValue results_v114() { return constant782; }
ExcelValue results_w114() { return constant782; }
ExcelValue results_e115() { return constant783; }
ExcelValue results_f115() { return constant784; }
ExcelValue results_g115() { return constant785; }
ExcelValue results_h115() { return constant786; }
ExcelValue results_i115() { return constant787; }
ExcelValue results_j115() { return constant788; }
ExcelValue results_k115() { return constant789; }
ExcelValue results_l115() { return constant790; }
ExcelValue results_m115() { return constant791; }
ExcelValue results_n115() { return constant792; }
ExcelValue results_o115() { return constant793; }
ExcelValue results_p115() { return constant793; }
ExcelValue results_q115() { return constant793; }
ExcelValue results_r115() { return constant793; }
ExcelValue results_s115() { return constant793; }
ExcelValue results_t115() { return constant793; }
ExcelValue results_u115() { return constant793; }
ExcelValue results_v115() { return constant793; }
ExcelValue results_w115() { return constant793; }
ExcelValue results_e116() { return constant794; }
ExcelValue results_f116() { return constant795; }
ExcelValue results_g116() { return constant796; }
ExcelValue results_h116() { return constant797; }
ExcelValue results_i116() { return constant798; }
ExcelValue results_j116() { return constant799; }
ExcelValue results_k116() { return constant800; }
ExcelValue results_l116() { return constant801; }
ExcelValue results_m116() { return constant802; }
ExcelValue results_n116() { return constant803; }
ExcelValue results_o116() { return constant804; }
ExcelValue results_p116() { return constant805; }
ExcelValue results_q116() { return constant806; }
ExcelValue results_r116() { return constant806; }
ExcelValue results_s116() { return constant806; }
ExcelValue results_t116() { return constant806; }
ExcelValue results_u116() { return constant806; }
ExcelValue results_v116() { return constant806; }
ExcelValue results_w116() { return constant806; }
ExcelValue results_e117() { return constant807; }
ExcelValue results_f117() { return constant808; }
ExcelValue results_g117() { return constant808; }
ExcelValue results_h117() { return constant808; }
ExcelValue results_i117() { return constant808; }
ExcelValue results_j117() { return constant808; }
ExcelValue results_k117() { return constant808; }
ExcelValue results_l117() { return constant808; }
ExcelValue results_m117() { return constant808; }
ExcelValue results_n117() { return constant808; }
ExcelValue results_o117() { return constant808; }
ExcelValue results_p117() { return constant808; }
ExcelValue results_q117() { return constant808; }
ExcelValue results_r117() { return constant808; }
ExcelValue results_s117() { return constant808; }
ExcelValue results_t117() { return constant808; }
ExcelValue results_u117() { return constant808; }
ExcelValue results_v117() { return constant808; }
ExcelValue results_w117() { return constant808; }
ExcelValue results_e118() { return constant809; }
ExcelValue results_f118() { return constant810; }
ExcelValue results_g118() { return constant811; }
ExcelValue results_h118() { return constant812; }
ExcelValue results_i118() { return constant813; }
ExcelValue results_j118() { return constant814; }
ExcelValue results_k118() { return constant815; }
ExcelValue results_l118() { return constant816; }
ExcelValue results_m118() { return constant817; }
ExcelValue results_n118() { return constant817; }
ExcelValue results_o118() { return constant817; }
ExcelValue results_p118() { return constant817; }
ExcelValue results_q118() { return constant817; }
ExcelValue results_r118() { return constant817; }
ExcelValue results_s118() { return constant817; }
ExcelValue results_t118() { return constant817; }
ExcelValue results_u118() { return constant817; }
ExcelValue results_v118() { return constant817; }
ExcelValue results_w118() { return constant817; }
ExcelValue results_e119() { return constant818; }
ExcelValue results_f119() { return constant819; }
ExcelValue results_g119() { return constant820; }
ExcelValue results_h119() { return constant821; }
ExcelValue results_i119() { return constant822; }
ExcelValue results_j119() { return constant823; }
ExcelValue results_k119() { return constant824; }
ExcelValue results_l119() { return constant825; }
ExcelValue results_m119() { return constant826; }
ExcelValue results_n119() { return constant826; }
ExcelValue results_o119() { return constant826; }
ExcelValue results_p119() { return constant826; }
ExcelValue results_q119() { return constant826; }
ExcelValue results_r119() { return constant826; }
ExcelValue results_s119() { return constant826; }
ExcelValue results_t119() { return constant826; }
ExcelValue results_u119() { return constant826; }
ExcelValue results_v119() { return constant826; }
ExcelValue results_w119() { return constant826; }
ExcelValue results_e120() { return constant827; }
ExcelValue results_f120() {
  static ExcelValue result;
  if(variable_set[2457] == 1) { return result;}
  result = ZERO;
  variable_set[2457] = 1;
  return result;
}

ExcelValue results_g120() {
  static ExcelValue result;
  if(variable_set[2458] == 1) { return result;}
  result = ZERO;
  variable_set[2458] = 1;
  return result;
}

ExcelValue results_h120() {
  static ExcelValue result;
  if(variable_set[2459] == 1) { return result;}
  result = ZERO;
  variable_set[2459] = 1;
  return result;
}

ExcelValue results_i120() {
  static ExcelValue result;
  if(variable_set[2460] == 1) { return result;}
  result = ZERO;
  variable_set[2460] = 1;
  return result;
}

ExcelValue results_j120() {
  static ExcelValue result;
  if(variable_set[2461] == 1) { return result;}
  result = ZERO;
  variable_set[2461] = 1;
  return result;
}

ExcelValue results_k120() {
  static ExcelValue result;
  if(variable_set[2462] == 1) { return result;}
  result = ZERO;
  variable_set[2462] = 1;
  return result;
}

ExcelValue results_l120() {
  static ExcelValue result;
  if(variable_set[2463] == 1) { return result;}
  result = ZERO;
  variable_set[2463] = 1;
  return result;
}

ExcelValue results_m120() {
  static ExcelValue result;
  if(variable_set[2464] == 1) { return result;}
  result = ZERO;
  variable_set[2464] = 1;
  return result;
}

ExcelValue results_n120() {
  static ExcelValue result;
  if(variable_set[2465] == 1) { return result;}
  result = ZERO;
  variable_set[2465] = 1;
  return result;
}

ExcelValue results_o120() {
  static ExcelValue result;
  if(variable_set[2466] == 1) { return result;}
  result = ZERO;
  variable_set[2466] = 1;
  return result;
}

ExcelValue results_p120() {
  static ExcelValue result;
  if(variable_set[2467] == 1) { return result;}
  result = ZERO;
  variable_set[2467] = 1;
  return result;
}

ExcelValue results_q120() {
  static ExcelValue result;
  if(variable_set[2468] == 1) { return result;}
  result = ZERO;
  variable_set[2468] = 1;
  return result;
}

ExcelValue results_r120() {
  static ExcelValue result;
  if(variable_set[2469] == 1) { return result;}
  result = ZERO;
  variable_set[2469] = 1;
  return result;
}

ExcelValue results_s120() {
  static ExcelValue result;
  if(variable_set[2470] == 1) { return result;}
  result = ZERO;
  variable_set[2470] = 1;
  return result;
}

ExcelValue results_t120() {
  static ExcelValue result;
  if(variable_set[2471] == 1) { return result;}
  result = ZERO;
  variable_set[2471] = 1;
  return result;
}

ExcelValue results_u120() {
  static ExcelValue result;
  if(variable_set[2472] == 1) { return result;}
  result = ZERO;
  variable_set[2472] = 1;
  return result;
}

ExcelValue results_v120() {
  static ExcelValue result;
  if(variable_set[2473] == 1) { return result;}
  result = ZERO;
  variable_set[2473] = 1;
  return result;
}

ExcelValue results_w120() {
  static ExcelValue result;
  if(variable_set[2474] == 1) { return result;}
  result = ZERO;
  variable_set[2474] = 1;
  return result;
}

ExcelValue results_e121() { return constant828; }
ExcelValue results_f121() {
  static ExcelValue result;
  if(variable_set[2476] == 1) { return result;}
  result = ZERO;
  variable_set[2476] = 1;
  return result;
}

ExcelValue results_g121() {
  static ExcelValue result;
  if(variable_set[2477] == 1) { return result;}
  result = ZERO;
  variable_set[2477] = 1;
  return result;
}

ExcelValue results_h121() {
  static ExcelValue result;
  if(variable_set[2478] == 1) { return result;}
  result = ZERO;
  variable_set[2478] = 1;
  return result;
}

ExcelValue results_i121() {
  static ExcelValue result;
  if(variable_set[2479] == 1) { return result;}
  result = ZERO;
  variable_set[2479] = 1;
  return result;
}

ExcelValue results_j121() {
  static ExcelValue result;
  if(variable_set[2480] == 1) { return result;}
  result = ZERO;
  variable_set[2480] = 1;
  return result;
}

ExcelValue results_k121() {
  static ExcelValue result;
  if(variable_set[2481] == 1) { return result;}
  result = ZERO;
  variable_set[2481] = 1;
  return result;
}

ExcelValue results_l121() {
  static ExcelValue result;
  if(variable_set[2482] == 1) { return result;}
  result = ZERO;
  variable_set[2482] = 1;
  return result;
}

ExcelValue results_m121() {
  static ExcelValue result;
  if(variable_set[2483] == 1) { return result;}
  result = ZERO;
  variable_set[2483] = 1;
  return result;
}

ExcelValue results_n121() {
  static ExcelValue result;
  if(variable_set[2484] == 1) { return result;}
  result = ZERO;
  variable_set[2484] = 1;
  return result;
}

ExcelValue results_o121() {
  static ExcelValue result;
  if(variable_set[2485] == 1) { return result;}
  result = ZERO;
  variable_set[2485] = 1;
  return result;
}

ExcelValue results_p121() {
  static ExcelValue result;
  if(variable_set[2486] == 1) { return result;}
  result = ZERO;
  variable_set[2486] = 1;
  return result;
}

ExcelValue results_q121() {
  static ExcelValue result;
  if(variable_set[2487] == 1) { return result;}
  result = ZERO;
  variable_set[2487] = 1;
  return result;
}

ExcelValue results_r121() {
  static ExcelValue result;
  if(variable_set[2488] == 1) { return result;}
  result = ZERO;
  variable_set[2488] = 1;
  return result;
}

ExcelValue results_s121() {
  static ExcelValue result;
  if(variable_set[2489] == 1) { return result;}
  result = ZERO;
  variable_set[2489] = 1;
  return result;
}

ExcelValue results_t121() {
  static ExcelValue result;
  if(variable_set[2490] == 1) { return result;}
  result = ZERO;
  variable_set[2490] = 1;
  return result;
}

ExcelValue results_u121() {
  static ExcelValue result;
  if(variable_set[2491] == 1) { return result;}
  result = ZERO;
  variable_set[2491] = 1;
  return result;
}

ExcelValue results_v121() {
  static ExcelValue result;
  if(variable_set[2492] == 1) { return result;}
  result = ZERO;
  variable_set[2492] = 1;
  return result;
}

ExcelValue results_w121() {
  static ExcelValue result;
  if(variable_set[2493] == 1) { return result;}
  result = ZERO;
  variable_set[2493] = 1;
  return result;
}

ExcelValue results_e122() { return constant829; }
ExcelValue results_f122() {
  static ExcelValue result;
  if(variable_set[2495] == 1) { return result;}
  result = ZERO;
  variable_set[2495] = 1;
  return result;
}

ExcelValue results_g122() {
  static ExcelValue result;
  if(variable_set[2496] == 1) { return result;}
  result = ZERO;
  variable_set[2496] = 1;
  return result;
}

ExcelValue results_h122() { return constant830; }
ExcelValue results_i122() { return constant831; }
ExcelValue results_j122() { return constant832; }
ExcelValue results_k122() { return constant833; }
ExcelValue results_l122() { return constant834; }
ExcelValue results_m122() { return constant835; }
ExcelValue results_n122() { return constant836; }
ExcelValue results_o122() { return constant837; }
ExcelValue results_p122() { return constant838; }
ExcelValue results_q122() { return constant839; }
ExcelValue results_r122() { return constant840; }
ExcelValue results_s122() { return constant841; }
ExcelValue results_t122() { return constant842; }
ExcelValue results_u122() { return constant843; }
ExcelValue results_v122() { return constant844; }
ExcelValue results_w122() { return constant845; }
ExcelValue results_e123() { return constant846; }
ExcelValue results_f123() {
  static ExcelValue result;
  if(variable_set[2514] == 1) { return result;}
  result = ZERO;
  variable_set[2514] = 1;
  return result;
}

ExcelValue results_g123() {
  static ExcelValue result;
  if(variable_set[2515] == 1) { return result;}
  result = ZERO;
  variable_set[2515] = 1;
  return result;
}

ExcelValue results_h123() {
  static ExcelValue result;
  if(variable_set[2516] == 1) { return result;}
  result = ZERO;
  variable_set[2516] = 1;
  return result;
}

ExcelValue results_i123() {
  static ExcelValue result;
  if(variable_set[2517] == 1) { return result;}
  result = ZERO;
  variable_set[2517] = 1;
  return result;
}

ExcelValue results_j123() {
  static ExcelValue result;
  if(variable_set[2518] == 1) { return result;}
  result = ZERO;
  variable_set[2518] = 1;
  return result;
}

ExcelValue results_k123() {
  static ExcelValue result;
  if(variable_set[2519] == 1) { return result;}
  result = ZERO;
  variable_set[2519] = 1;
  return result;
}

ExcelValue results_l123() {
  static ExcelValue result;
  if(variable_set[2520] == 1) { return result;}
  result = ZERO;
  variable_set[2520] = 1;
  return result;
}

ExcelValue results_m123() {
  static ExcelValue result;
  if(variable_set[2521] == 1) { return result;}
  result = ZERO;
  variable_set[2521] = 1;
  return result;
}

ExcelValue results_n123() {
  static ExcelValue result;
  if(variable_set[2522] == 1) { return result;}
  result = ZERO;
  variable_set[2522] = 1;
  return result;
}

ExcelValue results_o123() {
  static ExcelValue result;
  if(variable_set[2523] == 1) { return result;}
  result = ZERO;
  variable_set[2523] = 1;
  return result;
}

ExcelValue results_p123() {
  static ExcelValue result;
  if(variable_set[2524] == 1) { return result;}
  result = ZERO;
  variable_set[2524] = 1;
  return result;
}

ExcelValue results_q123() {
  static ExcelValue result;
  if(variable_set[2525] == 1) { return result;}
  result = ZERO;
  variable_set[2525] = 1;
  return result;
}

ExcelValue results_r123() {
  static ExcelValue result;
  if(variable_set[2526] == 1) { return result;}
  result = ZERO;
  variable_set[2526] = 1;
  return result;
}

ExcelValue results_s123() {
  static ExcelValue result;
  if(variable_set[2527] == 1) { return result;}
  result = ZERO;
  variable_set[2527] = 1;
  return result;
}

ExcelValue results_t123() {
  static ExcelValue result;
  if(variable_set[2528] == 1) { return result;}
  result = ZERO;
  variable_set[2528] = 1;
  return result;
}

ExcelValue results_u123() {
  static ExcelValue result;
  if(variable_set[2529] == 1) { return result;}
  result = ZERO;
  variable_set[2529] = 1;
  return result;
}

ExcelValue results_v123() {
  static ExcelValue result;
  if(variable_set[2530] == 1) { return result;}
  result = ZERO;
  variable_set[2530] = 1;
  return result;
}

ExcelValue results_w123() {
  static ExcelValue result;
  if(variable_set[2531] == 1) { return result;}
  result = ZERO;
  variable_set[2531] = 1;
  return result;
}

ExcelValue results_e124() { return constant847; }
ExcelValue results_f124() { return constant848; }
ExcelValue results_g124() { return constant849; }
ExcelValue results_h124() { return constant850; }
ExcelValue results_i124() { return constant851; }
ExcelValue results_j124() { return constant852; }
ExcelValue results_k124() { return constant853; }
ExcelValue results_l124() { return constant854; }
ExcelValue results_m124() { return constant855; }
ExcelValue results_n124() { return constant856; }
ExcelValue results_o124() { return constant857; }
ExcelValue results_p124() { return constant858; }
ExcelValue results_q124() { return constant859; }
ExcelValue results_r124() { return constant860; }
ExcelValue results_s124() { return constant861; }
ExcelValue results_t124() { return constant862; }
ExcelValue results_u124() { return constant863; }
ExcelValue results_v124() { return constant864; }
ExcelValue results_w124() { return constant865; }
ExcelValue results_e125() { return constant866; }
ExcelValue results_f125() { return constant867; }
ExcelValue results_g125() { return constant868; }
ExcelValue results_h125() { return constant869; }
ExcelValue results_i125() { return constant870; }
ExcelValue results_j125() { return constant871; }
ExcelValue results_k125() { return constant872; }
ExcelValue results_l125() { return constant873; }
ExcelValue results_m125() { return constant874; }
ExcelValue results_n125() { return constant875; }
ExcelValue results_o125() { return constant876; }
ExcelValue results_p125() { return constant877; }
ExcelValue results_q125() { return constant878; }
ExcelValue results_r125() { return constant879; }
ExcelValue results_s125() { return constant880; }
ExcelValue results_t125() { return constant881; }
ExcelValue results_u125() { return constant882; }
ExcelValue results_v125() { return constant883; }
ExcelValue results_w125() { return constant883; }
ExcelValue results_e126() { return constant884; }
ExcelValue results_f126() {
  static ExcelValue result;
  if(variable_set[2571] == 1) { return result;}
  result = ZERO;
  variable_set[2571] = 1;
  return result;
}

ExcelValue results_g126() {
  static ExcelValue result;
  if(variable_set[2572] == 1) { return result;}
  result = ZERO;
  variable_set[2572] = 1;
  return result;
}

ExcelValue results_h126() {
  static ExcelValue result;
  if(variable_set[2573] == 1) { return result;}
  result = ZERO;
  variable_set[2573] = 1;
  return result;
}

ExcelValue results_i126() {
  static ExcelValue result;
  if(variable_set[2574] == 1) { return result;}
  result = ZERO;
  variable_set[2574] = 1;
  return result;
}

ExcelValue results_j126() {
  static ExcelValue result;
  if(variable_set[2575] == 1) { return result;}
  result = ZERO;
  variable_set[2575] = 1;
  return result;
}

ExcelValue results_k126() {
  static ExcelValue result;
  if(variable_set[2576] == 1) { return result;}
  result = ZERO;
  variable_set[2576] = 1;
  return result;
}

ExcelValue results_l126() {
  static ExcelValue result;
  if(variable_set[2577] == 1) { return result;}
  result = ZERO;
  variable_set[2577] = 1;
  return result;
}

ExcelValue results_m126() {
  static ExcelValue result;
  if(variable_set[2578] == 1) { return result;}
  result = ZERO;
  variable_set[2578] = 1;
  return result;
}

ExcelValue results_n126() {
  static ExcelValue result;
  if(variable_set[2579] == 1) { return result;}
  result = ZERO;
  variable_set[2579] = 1;
  return result;
}

ExcelValue results_o126() {
  static ExcelValue result;
  if(variable_set[2580] == 1) { return result;}
  result = ZERO;
  variable_set[2580] = 1;
  return result;
}

ExcelValue results_p126() {
  static ExcelValue result;
  if(variable_set[2581] == 1) { return result;}
  result = ZERO;
  variable_set[2581] = 1;
  return result;
}

ExcelValue results_q126() {
  static ExcelValue result;
  if(variable_set[2582] == 1) { return result;}
  result = ZERO;
  variable_set[2582] = 1;
  return result;
}

ExcelValue results_r126() {
  static ExcelValue result;
  if(variable_set[2583] == 1) { return result;}
  result = ZERO;
  variable_set[2583] = 1;
  return result;
}

ExcelValue results_s126() {
  static ExcelValue result;
  if(variable_set[2584] == 1) { return result;}
  result = ZERO;
  variable_set[2584] = 1;
  return result;
}

ExcelValue results_t126() {
  static ExcelValue result;
  if(variable_set[2585] == 1) { return result;}
  result = ZERO;
  variable_set[2585] = 1;
  return result;
}

ExcelValue results_u126() {
  static ExcelValue result;
  if(variable_set[2586] == 1) { return result;}
  result = ZERO;
  variable_set[2586] = 1;
  return result;
}

ExcelValue results_v126() {
  static ExcelValue result;
  if(variable_set[2587] == 1) { return result;}
  result = ZERO;
  variable_set[2587] = 1;
  return result;
}

ExcelValue results_w126() {
  static ExcelValue result;
  if(variable_set[2588] == 1) { return result;}
  result = ZERO;
  variable_set[2588] = 1;
  return result;
}

ExcelValue results_e127() { return constant885; }
ExcelValue results_f127() {
  static ExcelValue result;
  if(variable_set[2590] == 1) { return result;}
  result = ZERO;
  variable_set[2590] = 1;
  return result;
}

ExcelValue results_g127() {
  static ExcelValue result;
  if(variable_set[2591] == 1) { return result;}
  result = ZERO;
  variable_set[2591] = 1;
  return result;
}

ExcelValue results_h127() {
  static ExcelValue result;
  if(variable_set[2592] == 1) { return result;}
  result = ZERO;
  variable_set[2592] = 1;
  return result;
}

ExcelValue results_i127() {
  static ExcelValue result;
  if(variable_set[2593] == 1) { return result;}
  result = ZERO;
  variable_set[2593] = 1;
  return result;
}

ExcelValue results_j127() { return constant886; }
ExcelValue results_k127() { return constant887; }
ExcelValue results_l127() { return constant888; }
ExcelValue results_m127() { return constant889; }
ExcelValue results_n127() { return constant890; }
ExcelValue results_o127() { return constant891; }
ExcelValue results_p127() { return constant892; }
ExcelValue results_q127() { return constant893; }
ExcelValue results_r127() { return constant894; }
ExcelValue results_s127() { return constant895; }
ExcelValue results_t127() { return constant896; }
ExcelValue results_u127() { return constant897; }
ExcelValue results_v127() { return constant898; }
ExcelValue results_w127() { return constant899; }
ExcelValue results_e128() { return constant900; }
ExcelValue results_f128() {
  static ExcelValue result;
  if(variable_set[2609] == 1) { return result;}
  result = ZERO;
  variable_set[2609] = 1;
  return result;
}

ExcelValue results_g128() {
  static ExcelValue result;
  if(variable_set[2610] == 1) { return result;}
  result = ZERO;
  variable_set[2610] = 1;
  return result;
}

ExcelValue results_h128() {
  static ExcelValue result;
  if(variable_set[2611] == 1) { return result;}
  result = ZERO;
  variable_set[2611] = 1;
  return result;
}

ExcelValue results_i128() {
  static ExcelValue result;
  if(variable_set[2612] == 1) { return result;}
  result = ZERO;
  variable_set[2612] = 1;
  return result;
}

ExcelValue results_j128() { return constant901; }
ExcelValue results_k128() { return constant902; }
ExcelValue results_l128() { return constant903; }
ExcelValue results_m128() { return constant904; }
ExcelValue results_n128() { return constant905; }
ExcelValue results_o128() { return constant906; }
ExcelValue results_p128() { return constant907; }
ExcelValue results_q128() { return constant908; }
ExcelValue results_r128() { return constant909; }
ExcelValue results_s128() { return constant910; }
ExcelValue results_t128() { return constant911; }
ExcelValue results_u128() { return constant912; }
ExcelValue results_v128() { return constant913; }
ExcelValue results_w128() { return constant913; }
ExcelValue results_e129() { return constant914; }
ExcelValue results_f129() { return constant915; }
ExcelValue results_g129() { return constant916; }
ExcelValue results_h129() { return constant917; }
ExcelValue results_i129() { return constant918; }
ExcelValue results_j129() { return constant919; }
ExcelValue results_k129() { return constant920; }
ExcelValue results_l129() { return constant921; }
ExcelValue results_m129() { return constant922; }
ExcelValue results_n129() { return constant923; }
ExcelValue results_o129() { return constant924; }
ExcelValue results_p129() { return constant925; }
ExcelValue results_q129() { return constant926; }
ExcelValue results_r129() { return constant927; }
ExcelValue results_s129() { return constant928; }
ExcelValue results_t129() { return constant929; }
ExcelValue results_u129() { return constant930; }
ExcelValue results_v129() { return constant931; }
ExcelValue results_w129() { return constant932; }
ExcelValue results_e130() { return constant933; }
ExcelValue results_f130() {
  static ExcelValue result;
  if(variable_set[2647] == 1) { return result;}
  result = ZERO;
  variable_set[2647] = 1;
  return result;
}

ExcelValue results_g130() {
  static ExcelValue result;
  if(variable_set[2648] == 1) { return result;}
  result = ZERO;
  variable_set[2648] = 1;
  return result;
}

ExcelValue results_h130() {
  static ExcelValue result;
  if(variable_set[2649] == 1) { return result;}
  result = ZERO;
  variable_set[2649] = 1;
  return result;
}

ExcelValue results_i130() {
  static ExcelValue result;
  if(variable_set[2650] == 1) { return result;}
  result = ZERO;
  variable_set[2650] = 1;
  return result;
}

ExcelValue results_j130() {
  static ExcelValue result;
  if(variable_set[2651] == 1) { return result;}
  result = ZERO;
  variable_set[2651] = 1;
  return result;
}

ExcelValue results_k130() {
  static ExcelValue result;
  if(variable_set[2652] == 1) { return result;}
  result = ZERO;
  variable_set[2652] = 1;
  return result;
}

ExcelValue results_l130() { return constant934; }
ExcelValue results_m130() { return constant935; }
ExcelValue results_n130() { return constant936; }
ExcelValue results_o130() { return constant937; }
ExcelValue results_p130() { return constant938; }
ExcelValue results_q130() { return constant939; }
ExcelValue results_r130() { return constant939; }
ExcelValue results_s130() { return constant939; }
ExcelValue results_t130() { return constant939; }
ExcelValue results_u130() { return constant939; }
ExcelValue results_v130() { return constant939; }
ExcelValue results_w130() { return constant939; }
ExcelValue results_e131() { return constant940; }
ExcelValue results_f131() {
  static ExcelValue result;
  if(variable_set[2666] == 1) { return result;}
  result = ZERO;
  variable_set[2666] = 1;
  return result;
}

ExcelValue results_g131() {
  static ExcelValue result;
  if(variable_set[2667] == 1) { return result;}
  result = ZERO;
  variable_set[2667] = 1;
  return result;
}

ExcelValue results_h131() {
  static ExcelValue result;
  if(variable_set[2668] == 1) { return result;}
  result = ZERO;
  variable_set[2668] = 1;
  return result;
}

ExcelValue results_i131() {
  static ExcelValue result;
  if(variable_set[2669] == 1) { return result;}
  result = ZERO;
  variable_set[2669] = 1;
  return result;
}

ExcelValue results_j131() {
  static ExcelValue result;
  if(variable_set[2670] == 1) { return result;}
  result = ZERO;
  variable_set[2670] = 1;
  return result;
}

ExcelValue results_k131() {
  static ExcelValue result;
  if(variable_set[2671] == 1) { return result;}
  result = ZERO;
  variable_set[2671] = 1;
  return result;
}

ExcelValue results_l131() { return constant941; }
ExcelValue results_m131() { return constant942; }
ExcelValue results_n131() { return constant943; }
ExcelValue results_o131() { return constant944; }
ExcelValue results_p131() { return constant945; }
ExcelValue results_q131() { return constant946; }
ExcelValue results_r131() { return constant946; }
ExcelValue results_s131() { return constant946; }
ExcelValue results_t131() { return constant946; }
ExcelValue results_u131() { return constant946; }
ExcelValue results_v131() { return constant946; }
ExcelValue results_w131() { return constant946; }
ExcelValue results_e132() { return constant947; }
ExcelValue results_f132() {
  static ExcelValue result;
  if(variable_set[2685] == 1) { return result;}
  result = ZERO;
  variable_set[2685] = 1;
  return result;
}

ExcelValue results_g132() {
  static ExcelValue result;
  if(variable_set[2686] == 1) { return result;}
  result = ZERO;
  variable_set[2686] = 1;
  return result;
}

ExcelValue results_h132() { return constant948; }
ExcelValue results_i132() { return constant949; }
ExcelValue results_j132() { return constant950; }
ExcelValue results_k132() { return constant951; }
ExcelValue results_l132() { return constant952; }
ExcelValue results_m132() { return constant953; }
ExcelValue results_n132() { return constant954; }
ExcelValue results_o132() { return constant955; }
ExcelValue results_p132() { return constant956; }
ExcelValue results_q132() { return constant956; }
ExcelValue results_r132() { return constant956; }
ExcelValue results_s132() { return constant956; }
ExcelValue results_t132() { return constant956; }
ExcelValue results_u132() { return constant956; }
ExcelValue results_v132() { return constant956; }
ExcelValue results_w132() { return constant956; }
ExcelValue results_e133() { return constant957; }
ExcelValue results_f133() {
  static ExcelValue result;
  if(variable_set[2704] == 1) { return result;}
  result = ZERO;
  variable_set[2704] = 1;
  return result;
}

ExcelValue results_g133() {
  static ExcelValue result;
  if(variable_set[2705] == 1) { return result;}
  result = ZERO;
  variable_set[2705] = 1;
  return result;
}

ExcelValue results_h133() { return constant958; }
ExcelValue results_i133() { return constant959; }
ExcelValue results_j133() { return constant960; }
ExcelValue results_k133() { return constant961; }
ExcelValue results_l133() { return constant962; }
ExcelValue results_m133() { return constant963; }
ExcelValue results_n133() { return constant964; }
ExcelValue results_o133() { return constant965; }
ExcelValue results_p133() { return constant966; }
ExcelValue results_q133() { return constant966; }
ExcelValue results_r133() { return constant966; }
ExcelValue results_s133() { return constant966; }
ExcelValue results_t133() { return constant966; }
ExcelValue results_u133() { return constant966; }
ExcelValue results_v133() { return constant966; }
ExcelValue results_w133() { return constant966; }
ExcelValue results_e134() { return constant967; }
ExcelValue results_f134() {
  static ExcelValue result;
  if(variable_set[2723] == 1) { return result;}
  result = ZERO;
  variable_set[2723] = 1;
  return result;
}

ExcelValue results_g134() {
  static ExcelValue result;
  if(variable_set[2724] == 1) { return result;}
  result = ZERO;
  variable_set[2724] = 1;
  return result;
}

ExcelValue results_h134() {
  static ExcelValue result;
  if(variable_set[2725] == 1) { return result;}
  result = ZERO;
  variable_set[2725] = 1;
  return result;
}

ExcelValue results_i134() {
  static ExcelValue result;
  if(variable_set[2726] == 1) { return result;}
  result = ZERO;
  variable_set[2726] = 1;
  return result;
}

ExcelValue results_j134() {
  static ExcelValue result;
  if(variable_set[2727] == 1) { return result;}
  result = ZERO;
  variable_set[2727] = 1;
  return result;
}

ExcelValue results_k134() {
  static ExcelValue result;
  if(variable_set[2728] == 1) { return result;}
  result = ZERO;
  variable_set[2728] = 1;
  return result;
}

ExcelValue results_l134() {
  static ExcelValue result;
  if(variable_set[2729] == 1) { return result;}
  result = ZERO;
  variable_set[2729] = 1;
  return result;
}

ExcelValue results_m134() {
  static ExcelValue result;
  if(variable_set[2730] == 1) { return result;}
  result = ZERO;
  variable_set[2730] = 1;
  return result;
}

ExcelValue results_n134() {
  static ExcelValue result;
  if(variable_set[2731] == 1) { return result;}
  result = ZERO;
  variable_set[2731] = 1;
  return result;
}

ExcelValue results_o134() {
  static ExcelValue result;
  if(variable_set[2732] == 1) { return result;}
  result = ZERO;
  variable_set[2732] = 1;
  return result;
}

ExcelValue results_p134() {
  static ExcelValue result;
  if(variable_set[2733] == 1) { return result;}
  result = ZERO;
  variable_set[2733] = 1;
  return result;
}

ExcelValue results_q134() {
  static ExcelValue result;
  if(variable_set[2734] == 1) { return result;}
  result = ZERO;
  variable_set[2734] = 1;
  return result;
}

ExcelValue results_r134() {
  static ExcelValue result;
  if(variable_set[2735] == 1) { return result;}
  result = ZERO;
  variable_set[2735] = 1;
  return result;
}

ExcelValue results_s134() {
  static ExcelValue result;
  if(variable_set[2736] == 1) { return result;}
  result = ZERO;
  variable_set[2736] = 1;
  return result;
}

ExcelValue results_t134() {
  static ExcelValue result;
  if(variable_set[2737] == 1) { return result;}
  result = ZERO;
  variable_set[2737] = 1;
  return result;
}

ExcelValue results_u134() {
  static ExcelValue result;
  if(variable_set[2738] == 1) { return result;}
  result = ZERO;
  variable_set[2738] = 1;
  return result;
}

ExcelValue results_v134() {
  static ExcelValue result;
  if(variable_set[2739] == 1) { return result;}
  result = ZERO;
  variable_set[2739] = 1;
  return result;
}

ExcelValue results_w134() {
  static ExcelValue result;
  if(variable_set[2740] == 1) { return result;}
  result = ZERO;
  variable_set[2740] = 1;
  return result;
}

ExcelValue results_e135() { return constant968; }
ExcelValue results_f135() {
  static ExcelValue result;
  if(variable_set[2742] == 1) { return result;}
  result = ZERO;
  variable_set[2742] = 1;
  return result;
}

ExcelValue results_g135() {
  static ExcelValue result;
  if(variable_set[2743] == 1) { return result;}
  result = ZERO;
  variable_set[2743] = 1;
  return result;
}

ExcelValue results_h135() {
  static ExcelValue result;
  if(variable_set[2744] == 1) { return result;}
  result = ZERO;
  variable_set[2744] = 1;
  return result;
}

ExcelValue results_i135() {
  static ExcelValue result;
  if(variable_set[2745] == 1) { return result;}
  result = ZERO;
  variable_set[2745] = 1;
  return result;
}

ExcelValue results_j135() {
  static ExcelValue result;
  if(variable_set[2746] == 1) { return result;}
  result = ZERO;
  variable_set[2746] = 1;
  return result;
}

ExcelValue results_k135() {
  static ExcelValue result;
  if(variable_set[2747] == 1) { return result;}
  result = ZERO;
  variable_set[2747] = 1;
  return result;
}

ExcelValue results_l135() {
  static ExcelValue result;
  if(variable_set[2748] == 1) { return result;}
  result = ZERO;
  variable_set[2748] = 1;
  return result;
}

ExcelValue results_m135() {
  static ExcelValue result;
  if(variable_set[2749] == 1) { return result;}
  result = ZERO;
  variable_set[2749] = 1;
  return result;
}

ExcelValue results_n135() {
  static ExcelValue result;
  if(variable_set[2750] == 1) { return result;}
  result = ZERO;
  variable_set[2750] = 1;
  return result;
}

ExcelValue results_o135() {
  static ExcelValue result;
  if(variable_set[2751] == 1) { return result;}
  result = ZERO;
  variable_set[2751] = 1;
  return result;
}

ExcelValue results_p135() {
  static ExcelValue result;
  if(variable_set[2752] == 1) { return result;}
  result = ZERO;
  variable_set[2752] = 1;
  return result;
}

ExcelValue results_q135() {
  static ExcelValue result;
  if(variable_set[2753] == 1) { return result;}
  result = ZERO;
  variable_set[2753] = 1;
  return result;
}

ExcelValue results_r135() {
  static ExcelValue result;
  if(variable_set[2754] == 1) { return result;}
  result = ZERO;
  variable_set[2754] = 1;
  return result;
}

ExcelValue results_s135() {
  static ExcelValue result;
  if(variable_set[2755] == 1) { return result;}
  result = ZERO;
  variable_set[2755] = 1;
  return result;
}

ExcelValue results_t135() {
  static ExcelValue result;
  if(variable_set[2756] == 1) { return result;}
  result = ZERO;
  variable_set[2756] = 1;
  return result;
}

ExcelValue results_u135() {
  static ExcelValue result;
  if(variable_set[2757] == 1) { return result;}
  result = ZERO;
  variable_set[2757] = 1;
  return result;
}

ExcelValue results_v135() {
  static ExcelValue result;
  if(variable_set[2758] == 1) { return result;}
  result = ZERO;
  variable_set[2758] = 1;
  return result;
}

ExcelValue results_w135() {
  static ExcelValue result;
  if(variable_set[2759] == 1) { return result;}
  result = ZERO;
  variable_set[2759] = 1;
  return result;
}

ExcelValue results_e136() { return constant969; }
ExcelValue results_f136() {
  static ExcelValue result;
  if(variable_set[2761] == 1) { return result;}
  result = ZERO;
  variable_set[2761] = 1;
  return result;
}

ExcelValue results_g136() {
  static ExcelValue result;
  if(variable_set[2762] == 1) { return result;}
  result = ZERO;
  variable_set[2762] = 1;
  return result;
}

ExcelValue results_h136() {
  static ExcelValue result;
  if(variable_set[2763] == 1) { return result;}
  result = ZERO;
  variable_set[2763] = 1;
  return result;
}

ExcelValue results_i136() {
  static ExcelValue result;
  if(variable_set[2764] == 1) { return result;}
  result = ZERO;
  variable_set[2764] = 1;
  return result;
}

ExcelValue results_j136() {
  static ExcelValue result;
  if(variable_set[2765] == 1) { return result;}
  result = ZERO;
  variable_set[2765] = 1;
  return result;
}

ExcelValue results_k136() {
  static ExcelValue result;
  if(variable_set[2766] == 1) { return result;}
  result = ZERO;
  variable_set[2766] = 1;
  return result;
}

ExcelValue results_l136() {
  static ExcelValue result;
  if(variable_set[2767] == 1) { return result;}
  result = ZERO;
  variable_set[2767] = 1;
  return result;
}

ExcelValue results_m136() {
  static ExcelValue result;
  if(variable_set[2768] == 1) { return result;}
  result = ZERO;
  variable_set[2768] = 1;
  return result;
}

ExcelValue results_n136() {
  static ExcelValue result;
  if(variable_set[2769] == 1) { return result;}
  result = ZERO;
  variable_set[2769] = 1;
  return result;
}

ExcelValue results_o136() {
  static ExcelValue result;
  if(variable_set[2770] == 1) { return result;}
  result = ZERO;
  variable_set[2770] = 1;
  return result;
}

ExcelValue results_p136() {
  static ExcelValue result;
  if(variable_set[2771] == 1) { return result;}
  result = ZERO;
  variable_set[2771] = 1;
  return result;
}

ExcelValue results_q136() {
  static ExcelValue result;
  if(variable_set[2772] == 1) { return result;}
  result = ZERO;
  variable_set[2772] = 1;
  return result;
}

ExcelValue results_r136() {
  static ExcelValue result;
  if(variable_set[2773] == 1) { return result;}
  result = ZERO;
  variable_set[2773] = 1;
  return result;
}

ExcelValue results_s136() {
  static ExcelValue result;
  if(variable_set[2774] == 1) { return result;}
  result = ZERO;
  variable_set[2774] = 1;
  return result;
}

ExcelValue results_t136() {
  static ExcelValue result;
  if(variable_set[2775] == 1) { return result;}
  result = ZERO;
  variable_set[2775] = 1;
  return result;
}

ExcelValue results_u136() {
  static ExcelValue result;
  if(variable_set[2776] == 1) { return result;}
  result = ZERO;
  variable_set[2776] = 1;
  return result;
}

ExcelValue results_v136() {
  static ExcelValue result;
  if(variable_set[2777] == 1) { return result;}
  result = ZERO;
  variable_set[2777] = 1;
  return result;
}

ExcelValue results_w136() {
  static ExcelValue result;
  if(variable_set[2778] == 1) { return result;}
  result = ZERO;
  variable_set[2778] = 1;
  return result;
}

ExcelValue results_e137() { return constant970; }
ExcelValue results_f137() { return constant971; }
ExcelValue results_g137() { return constant972; }
ExcelValue results_h137() { return constant973; }
ExcelValue results_i137() { return constant974; }
ExcelValue results_j137() { return constant975; }
ExcelValue results_k137() {
  static ExcelValue result;
  if(variable_set[2785] == 1) { return result;}
  result = ZERO;
  variable_set[2785] = 1;
  return result;
}

ExcelValue results_l137() {
  static ExcelValue result;
  if(variable_set[2786] == 1) { return result;}
  result = ZERO;
  variable_set[2786] = 1;
  return result;
}

ExcelValue results_m137() {
  static ExcelValue result;
  if(variable_set[2787] == 1) { return result;}
  result = ZERO;
  variable_set[2787] = 1;
  return result;
}

ExcelValue results_n137() {
  static ExcelValue result;
  if(variable_set[2788] == 1) { return result;}
  result = ZERO;
  variable_set[2788] = 1;
  return result;
}

ExcelValue results_o137() {
  static ExcelValue result;
  if(variable_set[2789] == 1) { return result;}
  result = ZERO;
  variable_set[2789] = 1;
  return result;
}

ExcelValue results_p137() {
  static ExcelValue result;
  if(variable_set[2790] == 1) { return result;}
  result = ZERO;
  variable_set[2790] = 1;
  return result;
}

ExcelValue results_q137() {
  static ExcelValue result;
  if(variable_set[2791] == 1) { return result;}
  result = ZERO;
  variable_set[2791] = 1;
  return result;
}

ExcelValue results_r137() {
  static ExcelValue result;
  if(variable_set[2792] == 1) { return result;}
  result = ZERO;
  variable_set[2792] = 1;
  return result;
}

ExcelValue results_s137() {
  static ExcelValue result;
  if(variable_set[2793] == 1) { return result;}
  result = ZERO;
  variable_set[2793] = 1;
  return result;
}

ExcelValue results_t137() {
  static ExcelValue result;
  if(variable_set[2794] == 1) { return result;}
  result = ZERO;
  variable_set[2794] = 1;
  return result;
}

ExcelValue results_u137() {
  static ExcelValue result;
  if(variable_set[2795] == 1) { return result;}
  result = ZERO;
  variable_set[2795] = 1;
  return result;
}

ExcelValue results_v137() {
  static ExcelValue result;
  if(variable_set[2796] == 1) { return result;}
  result = ZERO;
  variable_set[2796] = 1;
  return result;
}

ExcelValue results_w137() {
  static ExcelValue result;
  if(variable_set[2797] == 1) { return result;}
  result = ZERO;
  variable_set[2797] = 1;
  return result;
}

ExcelValue results_e138() { return constant976; }
ExcelValue results_f138() {
  static ExcelValue result;
  if(variable_set[2799] == 1) { return result;}
  result = ZERO;
  variable_set[2799] = 1;
  return result;
}

ExcelValue results_g138() {
  static ExcelValue result;
  if(variable_set[2800] == 1) { return result;}
  result = ZERO;
  variable_set[2800] = 1;
  return result;
}

ExcelValue results_h138() {
  static ExcelValue result;
  if(variable_set[2801] == 1) { return result;}
  result = ZERO;
  variable_set[2801] = 1;
  return result;
}

ExcelValue results_i138() {
  static ExcelValue result;
  if(variable_set[2802] == 1) { return result;}
  result = ZERO;
  variable_set[2802] = 1;
  return result;
}

ExcelValue results_j138() {
  static ExcelValue result;
  if(variable_set[2803] == 1) { return result;}
  result = ZERO;
  variable_set[2803] = 1;
  return result;
}

ExcelValue results_k138() {
  static ExcelValue result;
  if(variable_set[2804] == 1) { return result;}
  result = ZERO;
  variable_set[2804] = 1;
  return result;
}

ExcelValue results_l138() {
  static ExcelValue result;
  if(variable_set[2805] == 1) { return result;}
  result = ZERO;
  variable_set[2805] = 1;
  return result;
}

ExcelValue results_m138() {
  static ExcelValue result;
  if(variable_set[2806] == 1) { return result;}
  result = ZERO;
  variable_set[2806] = 1;
  return result;
}

ExcelValue results_n138() {
  static ExcelValue result;
  if(variable_set[2807] == 1) { return result;}
  result = ZERO;
  variable_set[2807] = 1;
  return result;
}

ExcelValue results_o138() {
  static ExcelValue result;
  if(variable_set[2808] == 1) { return result;}
  result = ZERO;
  variable_set[2808] = 1;
  return result;
}

ExcelValue results_p138() {
  static ExcelValue result;
  if(variable_set[2809] == 1) { return result;}
  result = ZERO;
  variable_set[2809] = 1;
  return result;
}

ExcelValue results_q138() {
  static ExcelValue result;
  if(variable_set[2810] == 1) { return result;}
  result = ZERO;
  variable_set[2810] = 1;
  return result;
}

ExcelValue results_r138() {
  static ExcelValue result;
  if(variable_set[2811] == 1) { return result;}
  result = ZERO;
  variable_set[2811] = 1;
  return result;
}

ExcelValue results_s138() {
  static ExcelValue result;
  if(variable_set[2812] == 1) { return result;}
  result = ZERO;
  variable_set[2812] = 1;
  return result;
}

ExcelValue results_t138() {
  static ExcelValue result;
  if(variable_set[2813] == 1) { return result;}
  result = ZERO;
  variable_set[2813] = 1;
  return result;
}

ExcelValue results_u138() {
  static ExcelValue result;
  if(variable_set[2814] == 1) { return result;}
  result = ZERO;
  variable_set[2814] = 1;
  return result;
}

ExcelValue results_v138() {
  static ExcelValue result;
  if(variable_set[2815] == 1) { return result;}
  result = ZERO;
  variable_set[2815] = 1;
  return result;
}

ExcelValue results_w138() {
  static ExcelValue result;
  if(variable_set[2816] == 1) { return result;}
  result = ZERO;
  variable_set[2816] = 1;
  return result;
}

ExcelValue results_e139() { return constant977; }
ExcelValue results_f139() {
  static ExcelValue result;
  if(variable_set[2818] == 1) { return result;}
  result = ZERO;
  variable_set[2818] = 1;
  return result;
}

ExcelValue results_g139() {
  static ExcelValue result;
  if(variable_set[2819] == 1) { return result;}
  result = ZERO;
  variable_set[2819] = 1;
  return result;
}

ExcelValue results_h139() {
  static ExcelValue result;
  if(variable_set[2820] == 1) { return result;}
  result = ZERO;
  variable_set[2820] = 1;
  return result;
}

ExcelValue results_i139() {
  static ExcelValue result;
  if(variable_set[2821] == 1) { return result;}
  result = ZERO;
  variable_set[2821] = 1;
  return result;
}

ExcelValue results_j139() {
  static ExcelValue result;
  if(variable_set[2822] == 1) { return result;}
  result = ZERO;
  variable_set[2822] = 1;
  return result;
}

ExcelValue results_k139() {
  static ExcelValue result;
  if(variable_set[2823] == 1) { return result;}
  result = ZERO;
  variable_set[2823] = 1;
  return result;
}

ExcelValue results_l139() {
  static ExcelValue result;
  if(variable_set[2824] == 1) { return result;}
  result = ZERO;
  variable_set[2824] = 1;
  return result;
}

ExcelValue results_m139() {
  static ExcelValue result;
  if(variable_set[2825] == 1) { return result;}
  result = ZERO;
  variable_set[2825] = 1;
  return result;
}

ExcelValue results_n139() {
  static ExcelValue result;
  if(variable_set[2826] == 1) { return result;}
  result = ZERO;
  variable_set[2826] = 1;
  return result;
}

ExcelValue results_o139() {
  static ExcelValue result;
  if(variable_set[2827] == 1) { return result;}
  result = ZERO;
  variable_set[2827] = 1;
  return result;
}

ExcelValue results_p139() {
  static ExcelValue result;
  if(variable_set[2828] == 1) { return result;}
  result = ZERO;
  variable_set[2828] = 1;
  return result;
}

ExcelValue results_q139() {
  static ExcelValue result;
  if(variable_set[2829] == 1) { return result;}
  result = ZERO;
  variable_set[2829] = 1;
  return result;
}

ExcelValue results_r139() {
  static ExcelValue result;
  if(variable_set[2830] == 1) { return result;}
  result = ZERO;
  variable_set[2830] = 1;
  return result;
}

ExcelValue results_s139() {
  static ExcelValue result;
  if(variable_set[2831] == 1) { return result;}
  result = ZERO;
  variable_set[2831] = 1;
  return result;
}

ExcelValue results_t139() {
  static ExcelValue result;
  if(variable_set[2832] == 1) { return result;}
  result = ZERO;
  variable_set[2832] = 1;
  return result;
}

ExcelValue results_u139() {
  static ExcelValue result;
  if(variable_set[2833] == 1) { return result;}
  result = ZERO;
  variable_set[2833] = 1;
  return result;
}

ExcelValue results_v139() {
  static ExcelValue result;
  if(variable_set[2834] == 1) { return result;}
  result = ZERO;
  variable_set[2834] = 1;
  return result;
}

ExcelValue results_w139() {
  static ExcelValue result;
  if(variable_set[2835] == 1) { return result;}
  result = ZERO;
  variable_set[2835] = 1;
  return result;
}

ExcelValue results_e140() { return constant978; }
ExcelValue results_f140() {
  static ExcelValue result;
  if(variable_set[2837] == 1) { return result;}
  result = ZERO;
  variable_set[2837] = 1;
  return result;
}

ExcelValue results_g140() {
  static ExcelValue result;
  if(variable_set[2838] == 1) { return result;}
  result = ZERO;
  variable_set[2838] = 1;
  return result;
}

ExcelValue results_h140() {
  static ExcelValue result;
  if(variable_set[2839] == 1) { return result;}
  result = ZERO;
  variable_set[2839] = 1;
  return result;
}

ExcelValue results_i140() {
  static ExcelValue result;
  if(variable_set[2840] == 1) { return result;}
  result = ZERO;
  variable_set[2840] = 1;
  return result;
}

ExcelValue results_j140() {
  static ExcelValue result;
  if(variable_set[2841] == 1) { return result;}
  result = ZERO;
  variable_set[2841] = 1;
  return result;
}

ExcelValue results_k140() {
  static ExcelValue result;
  if(variable_set[2842] == 1) { return result;}
  result = ZERO;
  variable_set[2842] = 1;
  return result;
}

ExcelValue results_l140() {
  static ExcelValue result;
  if(variable_set[2843] == 1) { return result;}
  result = ZERO;
  variable_set[2843] = 1;
  return result;
}

ExcelValue results_m140() {
  static ExcelValue result;
  if(variable_set[2844] == 1) { return result;}
  result = ZERO;
  variable_set[2844] = 1;
  return result;
}

ExcelValue results_n140() {
  static ExcelValue result;
  if(variable_set[2845] == 1) { return result;}
  result = ZERO;
  variable_set[2845] = 1;
  return result;
}

ExcelValue results_o140() {
  static ExcelValue result;
  if(variable_set[2846] == 1) { return result;}
  result = ZERO;
  variable_set[2846] = 1;
  return result;
}

ExcelValue results_p140() {
  static ExcelValue result;
  if(variable_set[2847] == 1) { return result;}
  result = ZERO;
  variable_set[2847] = 1;
  return result;
}

ExcelValue results_q140() {
  static ExcelValue result;
  if(variable_set[2848] == 1) { return result;}
  result = ZERO;
  variable_set[2848] = 1;
  return result;
}

ExcelValue results_r140() {
  static ExcelValue result;
  if(variable_set[2849] == 1) { return result;}
  result = ZERO;
  variable_set[2849] = 1;
  return result;
}

ExcelValue results_s140() {
  static ExcelValue result;
  if(variable_set[2850] == 1) { return result;}
  result = ZERO;
  variable_set[2850] = 1;
  return result;
}

ExcelValue results_t140() {
  static ExcelValue result;
  if(variable_set[2851] == 1) { return result;}
  result = ZERO;
  variable_set[2851] = 1;
  return result;
}

ExcelValue results_u140() {
  static ExcelValue result;
  if(variable_set[2852] == 1) { return result;}
  result = ZERO;
  variable_set[2852] = 1;
  return result;
}

ExcelValue results_v140() {
  static ExcelValue result;
  if(variable_set[2853] == 1) { return result;}
  result = ZERO;
  variable_set[2853] = 1;
  return result;
}

ExcelValue results_w140() {
  static ExcelValue result;
  if(variable_set[2854] == 1) { return result;}
  result = ZERO;
  variable_set[2854] = 1;
  return result;
}

ExcelValue results_e141() { return constant979; }
ExcelValue results_f141() {
  static ExcelValue result;
  if(variable_set[2856] == 1) { return result;}
  result = ZERO;
  variable_set[2856] = 1;
  return result;
}

ExcelValue results_g141() {
  static ExcelValue result;
  if(variable_set[2857] == 1) { return result;}
  result = ZERO;
  variable_set[2857] = 1;
  return result;
}

ExcelValue results_h141() {
  static ExcelValue result;
  if(variable_set[2858] == 1) { return result;}
  result = ZERO;
  variable_set[2858] = 1;
  return result;
}

ExcelValue results_i141() {
  static ExcelValue result;
  if(variable_set[2859] == 1) { return result;}
  result = ZERO;
  variable_set[2859] = 1;
  return result;
}

ExcelValue results_j141() {
  static ExcelValue result;
  if(variable_set[2860] == 1) { return result;}
  result = ZERO;
  variable_set[2860] = 1;
  return result;
}

ExcelValue results_k141() {
  static ExcelValue result;
  if(variable_set[2861] == 1) { return result;}
  result = ZERO;
  variable_set[2861] = 1;
  return result;
}

ExcelValue results_l141() {
  static ExcelValue result;
  if(variable_set[2862] == 1) { return result;}
  result = ZERO;
  variable_set[2862] = 1;
  return result;
}

ExcelValue results_m141() {
  static ExcelValue result;
  if(variable_set[2863] == 1) { return result;}
  result = ZERO;
  variable_set[2863] = 1;
  return result;
}

ExcelValue results_n141() {
  static ExcelValue result;
  if(variable_set[2864] == 1) { return result;}
  result = ZERO;
  variable_set[2864] = 1;
  return result;
}

ExcelValue results_o141() {
  static ExcelValue result;
  if(variable_set[2865] == 1) { return result;}
  result = ZERO;
  variable_set[2865] = 1;
  return result;
}

ExcelValue results_p141() {
  static ExcelValue result;
  if(variable_set[2866] == 1) { return result;}
  result = ZERO;
  variable_set[2866] = 1;
  return result;
}

ExcelValue results_q141() {
  static ExcelValue result;
  if(variable_set[2867] == 1) { return result;}
  result = ZERO;
  variable_set[2867] = 1;
  return result;
}

ExcelValue results_r141() {
  static ExcelValue result;
  if(variable_set[2868] == 1) { return result;}
  result = ZERO;
  variable_set[2868] = 1;
  return result;
}

ExcelValue results_s141() {
  static ExcelValue result;
  if(variable_set[2869] == 1) { return result;}
  result = ZERO;
  variable_set[2869] = 1;
  return result;
}

ExcelValue results_t141() {
  static ExcelValue result;
  if(variable_set[2870] == 1) { return result;}
  result = ZERO;
  variable_set[2870] = 1;
  return result;
}

ExcelValue results_u141() {
  static ExcelValue result;
  if(variable_set[2871] == 1) { return result;}
  result = ZERO;
  variable_set[2871] = 1;
  return result;
}

ExcelValue results_v141() {
  static ExcelValue result;
  if(variable_set[2872] == 1) { return result;}
  result = ZERO;
  variable_set[2872] = 1;
  return result;
}

ExcelValue results_w141() {
  static ExcelValue result;
  if(variable_set[2873] == 1) { return result;}
  result = ZERO;
  variable_set[2873] = 1;
  return result;
}

ExcelValue results_e142() { return constant980; }
ExcelValue results_f142() {
  static ExcelValue result;
  if(variable_set[2875] == 1) { return result;}
  result = ZERO;
  variable_set[2875] = 1;
  return result;
}

ExcelValue results_g142() {
  static ExcelValue result;
  if(variable_set[2876] == 1) { return result;}
  result = ZERO;
  variable_set[2876] = 1;
  return result;
}

ExcelValue results_h142() {
  static ExcelValue result;
  if(variable_set[2877] == 1) { return result;}
  result = ZERO;
  variable_set[2877] = 1;
  return result;
}

ExcelValue results_i142() {
  static ExcelValue result;
  if(variable_set[2878] == 1) { return result;}
  result = ZERO;
  variable_set[2878] = 1;
  return result;
}

ExcelValue results_j142() {
  static ExcelValue result;
  if(variable_set[2879] == 1) { return result;}
  result = ZERO;
  variable_set[2879] = 1;
  return result;
}

ExcelValue results_k142() {
  static ExcelValue result;
  if(variable_set[2880] == 1) { return result;}
  result = ZERO;
  variable_set[2880] = 1;
  return result;
}

ExcelValue results_l142() {
  static ExcelValue result;
  if(variable_set[2881] == 1) { return result;}
  result = ZERO;
  variable_set[2881] = 1;
  return result;
}

ExcelValue results_m142() {
  static ExcelValue result;
  if(variable_set[2882] == 1) { return result;}
  result = ZERO;
  variable_set[2882] = 1;
  return result;
}

ExcelValue results_n142() {
  static ExcelValue result;
  if(variable_set[2883] == 1) { return result;}
  result = ZERO;
  variable_set[2883] = 1;
  return result;
}

ExcelValue results_o142() {
  static ExcelValue result;
  if(variable_set[2884] == 1) { return result;}
  result = ZERO;
  variable_set[2884] = 1;
  return result;
}

ExcelValue results_p142() {
  static ExcelValue result;
  if(variable_set[2885] == 1) { return result;}
  result = ZERO;
  variable_set[2885] = 1;
  return result;
}

ExcelValue results_q142() {
  static ExcelValue result;
  if(variable_set[2886] == 1) { return result;}
  result = ZERO;
  variable_set[2886] = 1;
  return result;
}

ExcelValue results_r142() {
  static ExcelValue result;
  if(variable_set[2887] == 1) { return result;}
  result = ZERO;
  variable_set[2887] = 1;
  return result;
}

ExcelValue results_s142() {
  static ExcelValue result;
  if(variable_set[2888] == 1) { return result;}
  result = ZERO;
  variable_set[2888] = 1;
  return result;
}

ExcelValue results_t142() {
  static ExcelValue result;
  if(variable_set[2889] == 1) { return result;}
  result = ZERO;
  variable_set[2889] = 1;
  return result;
}

ExcelValue results_u142() {
  static ExcelValue result;
  if(variable_set[2890] == 1) { return result;}
  result = ZERO;
  variable_set[2890] = 1;
  return result;
}

ExcelValue results_v142() {
  static ExcelValue result;
  if(variable_set[2891] == 1) { return result;}
  result = ZERO;
  variable_set[2891] = 1;
  return result;
}

ExcelValue results_w142() {
  static ExcelValue result;
  if(variable_set[2892] == 1) { return result;}
  result = ZERO;
  variable_set[2892] = 1;
  return result;
}

ExcelValue results_e143() { return constant981; }
ExcelValue results_f143() {
  static ExcelValue result;
  if(variable_set[2894] == 1) { return result;}
  result = ZERO;
  variable_set[2894] = 1;
  return result;
}

ExcelValue results_g143() {
  static ExcelValue result;
  if(variable_set[2895] == 1) { return result;}
  result = ZERO;
  variable_set[2895] = 1;
  return result;
}

ExcelValue results_h143() {
  static ExcelValue result;
  if(variable_set[2896] == 1) { return result;}
  result = ZERO;
  variable_set[2896] = 1;
  return result;
}

ExcelValue results_i143() {
  static ExcelValue result;
  if(variable_set[2897] == 1) { return result;}
  result = ZERO;
  variable_set[2897] = 1;
  return result;
}

ExcelValue results_j143() {
  static ExcelValue result;
  if(variable_set[2898] == 1) { return result;}
  result = ZERO;
  variable_set[2898] = 1;
  return result;
}

ExcelValue results_k143() {
  static ExcelValue result;
  if(variable_set[2899] == 1) { return result;}
  result = ZERO;
  variable_set[2899] = 1;
  return result;
}

ExcelValue results_l143() {
  static ExcelValue result;
  if(variable_set[2900] == 1) { return result;}
  result = ZERO;
  variable_set[2900] = 1;
  return result;
}

ExcelValue results_m143() {
  static ExcelValue result;
  if(variable_set[2901] == 1) { return result;}
  result = ZERO;
  variable_set[2901] = 1;
  return result;
}

ExcelValue results_n143() {
  static ExcelValue result;
  if(variable_set[2902] == 1) { return result;}
  result = ZERO;
  variable_set[2902] = 1;
  return result;
}

ExcelValue results_o143() {
  static ExcelValue result;
  if(variable_set[2903] == 1) { return result;}
  result = ZERO;
  variable_set[2903] = 1;
  return result;
}

ExcelValue results_p143() {
  static ExcelValue result;
  if(variable_set[2904] == 1) { return result;}
  result = ZERO;
  variable_set[2904] = 1;
  return result;
}

ExcelValue results_q143() {
  static ExcelValue result;
  if(variable_set[2905] == 1) { return result;}
  result = ZERO;
  variable_set[2905] = 1;
  return result;
}

ExcelValue results_r143() {
  static ExcelValue result;
  if(variable_set[2906] == 1) { return result;}
  result = ZERO;
  variable_set[2906] = 1;
  return result;
}

ExcelValue results_s143() {
  static ExcelValue result;
  if(variable_set[2907] == 1) { return result;}
  result = ZERO;
  variable_set[2907] = 1;
  return result;
}

ExcelValue results_t143() {
  static ExcelValue result;
  if(variable_set[2908] == 1) { return result;}
  result = ZERO;
  variable_set[2908] = 1;
  return result;
}

ExcelValue results_u143() {
  static ExcelValue result;
  if(variable_set[2909] == 1) { return result;}
  result = ZERO;
  variable_set[2909] = 1;
  return result;
}

ExcelValue results_v143() {
  static ExcelValue result;
  if(variable_set[2910] == 1) { return result;}
  result = ZERO;
  variable_set[2910] = 1;
  return result;
}

ExcelValue results_w143() {
  static ExcelValue result;
  if(variable_set[2911] == 1) { return result;}
  result = ZERO;
  variable_set[2911] = 1;
  return result;
}

ExcelValue results_e144() { return constant982; }
ExcelValue results_f144() {
  static ExcelValue result;
  if(variable_set[2913] == 1) { return result;}
  result = ZERO;
  variable_set[2913] = 1;
  return result;
}

ExcelValue results_g144() {
  static ExcelValue result;
  if(variable_set[2914] == 1) { return result;}
  result = ZERO;
  variable_set[2914] = 1;
  return result;
}

ExcelValue results_h144() {
  static ExcelValue result;
  if(variable_set[2915] == 1) { return result;}
  result = ZERO;
  variable_set[2915] = 1;
  return result;
}

ExcelValue results_i144() {
  static ExcelValue result;
  if(variable_set[2916] == 1) { return result;}
  result = ZERO;
  variable_set[2916] = 1;
  return result;
}

ExcelValue results_j144() {
  static ExcelValue result;
  if(variable_set[2917] == 1) { return result;}
  result = ZERO;
  variable_set[2917] = 1;
  return result;
}

ExcelValue results_k144() {
  static ExcelValue result;
  if(variable_set[2918] == 1) { return result;}
  result = ZERO;
  variable_set[2918] = 1;
  return result;
}

ExcelValue results_l144() {
  static ExcelValue result;
  if(variable_set[2919] == 1) { return result;}
  result = ZERO;
  variable_set[2919] = 1;
  return result;
}

ExcelValue results_m144() {
  static ExcelValue result;
  if(variable_set[2920] == 1) { return result;}
  result = ZERO;
  variable_set[2920] = 1;
  return result;
}

ExcelValue results_n144() {
  static ExcelValue result;
  if(variable_set[2921] == 1) { return result;}
  result = ZERO;
  variable_set[2921] = 1;
  return result;
}

ExcelValue results_o144() {
  static ExcelValue result;
  if(variable_set[2922] == 1) { return result;}
  result = ZERO;
  variable_set[2922] = 1;
  return result;
}

ExcelValue results_p144() {
  static ExcelValue result;
  if(variable_set[2923] == 1) { return result;}
  result = ZERO;
  variable_set[2923] = 1;
  return result;
}

ExcelValue results_q144() {
  static ExcelValue result;
  if(variable_set[2924] == 1) { return result;}
  result = ZERO;
  variable_set[2924] = 1;
  return result;
}

ExcelValue results_r144() {
  static ExcelValue result;
  if(variable_set[2925] == 1) { return result;}
  result = ZERO;
  variable_set[2925] = 1;
  return result;
}

ExcelValue results_s144() {
  static ExcelValue result;
  if(variable_set[2926] == 1) { return result;}
  result = ZERO;
  variable_set[2926] = 1;
  return result;
}

ExcelValue results_t144() {
  static ExcelValue result;
  if(variable_set[2927] == 1) { return result;}
  result = ZERO;
  variable_set[2927] = 1;
  return result;
}

ExcelValue results_u144() {
  static ExcelValue result;
  if(variable_set[2928] == 1) { return result;}
  result = ZERO;
  variable_set[2928] = 1;
  return result;
}

ExcelValue results_v144() {
  static ExcelValue result;
  if(variable_set[2929] == 1) { return result;}
  result = ZERO;
  variable_set[2929] = 1;
  return result;
}

ExcelValue results_w144() {
  static ExcelValue result;
  if(variable_set[2930] == 1) { return result;}
  result = ZERO;
  variable_set[2930] = 1;
  return result;
}

ExcelValue results_e145() { return constant983; }
ExcelValue results_f145() {
  static ExcelValue result;
  if(variable_set[2932] == 1) { return result;}
  result = ZERO;
  variable_set[2932] = 1;
  return result;
}

ExcelValue results_g145() {
  static ExcelValue result;
  if(variable_set[2933] == 1) { return result;}
  result = ZERO;
  variable_set[2933] = 1;
  return result;
}

ExcelValue results_h145() {
  static ExcelValue result;
  if(variable_set[2934] == 1) { return result;}
  result = ZERO;
  variable_set[2934] = 1;
  return result;
}

ExcelValue results_i145() {
  static ExcelValue result;
  if(variable_set[2935] == 1) { return result;}
  result = ZERO;
  variable_set[2935] = 1;
  return result;
}

ExcelValue results_j145() {
  static ExcelValue result;
  if(variable_set[2936] == 1) { return result;}
  result = ZERO;
  variable_set[2936] = 1;
  return result;
}

ExcelValue results_k145() {
  static ExcelValue result;
  if(variable_set[2937] == 1) { return result;}
  result = ZERO;
  variable_set[2937] = 1;
  return result;
}

ExcelValue results_l145() {
  static ExcelValue result;
  if(variable_set[2938] == 1) { return result;}
  result = ZERO;
  variable_set[2938] = 1;
  return result;
}

ExcelValue results_m145() {
  static ExcelValue result;
  if(variable_set[2939] == 1) { return result;}
  result = ZERO;
  variable_set[2939] = 1;
  return result;
}

ExcelValue results_n145() {
  static ExcelValue result;
  if(variable_set[2940] == 1) { return result;}
  result = ZERO;
  variable_set[2940] = 1;
  return result;
}

ExcelValue results_o145() {
  static ExcelValue result;
  if(variable_set[2941] == 1) { return result;}
  result = ZERO;
  variable_set[2941] = 1;
  return result;
}

ExcelValue results_p145() {
  static ExcelValue result;
  if(variable_set[2942] == 1) { return result;}
  result = ZERO;
  variable_set[2942] = 1;
  return result;
}

ExcelValue results_q145() {
  static ExcelValue result;
  if(variable_set[2943] == 1) { return result;}
  result = ZERO;
  variable_set[2943] = 1;
  return result;
}

ExcelValue results_r145() {
  static ExcelValue result;
  if(variable_set[2944] == 1) { return result;}
  result = ZERO;
  variable_set[2944] = 1;
  return result;
}

ExcelValue results_s145() {
  static ExcelValue result;
  if(variable_set[2945] == 1) { return result;}
  result = ZERO;
  variable_set[2945] = 1;
  return result;
}

ExcelValue results_t145() {
  static ExcelValue result;
  if(variable_set[2946] == 1) { return result;}
  result = ZERO;
  variable_set[2946] = 1;
  return result;
}

ExcelValue results_u145() {
  static ExcelValue result;
  if(variable_set[2947] == 1) { return result;}
  result = ZERO;
  variable_set[2947] = 1;
  return result;
}

ExcelValue results_v145() {
  static ExcelValue result;
  if(variable_set[2948] == 1) { return result;}
  result = ZERO;
  variable_set[2948] = 1;
  return result;
}

ExcelValue results_w145() {
  static ExcelValue result;
  if(variable_set[2949] == 1) { return result;}
  result = ZERO;
  variable_set[2949] = 1;
  return result;
}

ExcelValue results_e146() { return constant984; }
ExcelValue results_f146() { return constant985; }
ExcelValue results_g146() { return constant986; }
ExcelValue results_h146() { return constant987; }
ExcelValue results_i146() { return constant988; }
ExcelValue results_j146() { return constant989; }
ExcelValue results_k146() {
  static ExcelValue result;
  if(variable_set[2956] == 1) { return result;}
  result = ZERO;
  variable_set[2956] = 1;
  return result;
}

ExcelValue results_l146() {
  static ExcelValue result;
  if(variable_set[2957] == 1) { return result;}
  result = ZERO;
  variable_set[2957] = 1;
  return result;
}

ExcelValue results_m146() {
  static ExcelValue result;
  if(variable_set[2958] == 1) { return result;}
  result = ZERO;
  variable_set[2958] = 1;
  return result;
}

ExcelValue results_n146() {
  static ExcelValue result;
  if(variable_set[2959] == 1) { return result;}
  result = ZERO;
  variable_set[2959] = 1;
  return result;
}

ExcelValue results_o146() {
  static ExcelValue result;
  if(variable_set[2960] == 1) { return result;}
  result = ZERO;
  variable_set[2960] = 1;
  return result;
}

ExcelValue results_p146() {
  static ExcelValue result;
  if(variable_set[2961] == 1) { return result;}
  result = ZERO;
  variable_set[2961] = 1;
  return result;
}

ExcelValue results_q146() {
  static ExcelValue result;
  if(variable_set[2962] == 1) { return result;}
  result = ZERO;
  variable_set[2962] = 1;
  return result;
}

ExcelValue results_r146() {
  static ExcelValue result;
  if(variable_set[2963] == 1) { return result;}
  result = ZERO;
  variable_set[2963] = 1;
  return result;
}

ExcelValue results_s146() {
  static ExcelValue result;
  if(variable_set[2964] == 1) { return result;}
  result = ZERO;
  variable_set[2964] = 1;
  return result;
}

ExcelValue results_t146() {
  static ExcelValue result;
  if(variable_set[2965] == 1) { return result;}
  result = ZERO;
  variable_set[2965] = 1;
  return result;
}

ExcelValue results_u146() {
  static ExcelValue result;
  if(variable_set[2966] == 1) { return result;}
  result = ZERO;
  variable_set[2966] = 1;
  return result;
}

ExcelValue results_v146() {
  static ExcelValue result;
  if(variable_set[2967] == 1) { return result;}
  result = ZERO;
  variable_set[2967] = 1;
  return result;
}

ExcelValue results_w146() {
  static ExcelValue result;
  if(variable_set[2968] == 1) { return result;}
  result = ZERO;
  variable_set[2968] = 1;
  return result;
}

ExcelValue results_e147() { return constant990; }
ExcelValue results_f147() {
  static ExcelValue result;
  if(variable_set[2970] == 1) { return result;}
  result = ZERO;
  variable_set[2970] = 1;
  return result;
}

ExcelValue results_g147() {
  static ExcelValue result;
  if(variable_set[2971] == 1) { return result;}
  result = ZERO;
  variable_set[2971] = 1;
  return result;
}

ExcelValue results_h147() { return constant991; }
ExcelValue results_i147() { return constant992; }
ExcelValue results_j147() { return constant993; }
ExcelValue results_k147() { return constant994; }
ExcelValue results_l147() { return constant995; }
ExcelValue results_m147() { return constant996; }
ExcelValue results_n147() { return constant997; }
ExcelValue results_o147() { return constant998; }
ExcelValue results_p147() { return constant999; }
ExcelValue results_q147() { return constant1000; }
ExcelValue results_r147() { return constant1001; }
ExcelValue results_s147() { return constant1002; }
ExcelValue results_t147() { return constant1003; }
ExcelValue results_u147() { return constant1004; }
ExcelValue results_v147() { return constant1005; }
ExcelValue results_w147() { return constant1006; }
ExcelValue results_e148() { return constant1007; }
ExcelValue results_f148() { return constant1008; }
ExcelValue results_g148() { return constant1009; }
ExcelValue results_h148() { return constant1010; }
ExcelValue results_i148() {
  static ExcelValue result;
  if(variable_set[2992] == 1) { return result;}
  result = ZERO;
  variable_set[2992] = 1;
  return result;
}

ExcelValue results_j148() {
  static ExcelValue result;
  if(variable_set[2993] == 1) { return result;}
  result = ZERO;
  variable_set[2993] = 1;
  return result;
}

ExcelValue results_k148() {
  static ExcelValue result;
  if(variable_set[2994] == 1) { return result;}
  result = ZERO;
  variable_set[2994] = 1;
  return result;
}

ExcelValue results_l148() {
  static ExcelValue result;
  if(variable_set[2995] == 1) { return result;}
  result = ZERO;
  variable_set[2995] = 1;
  return result;
}

ExcelValue results_m148() {
  static ExcelValue result;
  if(variable_set[2996] == 1) { return result;}
  result = ZERO;
  variable_set[2996] = 1;
  return result;
}

ExcelValue results_n148() {
  static ExcelValue result;
  if(variable_set[2997] == 1) { return result;}
  result = ZERO;
  variable_set[2997] = 1;
  return result;
}

ExcelValue results_o148() {
  static ExcelValue result;
  if(variable_set[2998] == 1) { return result;}
  result = ZERO;
  variable_set[2998] = 1;
  return result;
}

ExcelValue results_p148() {
  static ExcelValue result;
  if(variable_set[2999] == 1) { return result;}
  result = ZERO;
  variable_set[2999] = 1;
  return result;
}

ExcelValue results_q148() {
  static ExcelValue result;
  if(variable_set[3000] == 1) { return result;}
  result = ZERO;
  variable_set[3000] = 1;
  return result;
}

ExcelValue results_r148() {
  static ExcelValue result;
  if(variable_set[3001] == 1) { return result;}
  result = ZERO;
  variable_set[3001] = 1;
  return result;
}

ExcelValue results_s148() {
  static ExcelValue result;
  if(variable_set[3002] == 1) { return result;}
  result = ZERO;
  variable_set[3002] = 1;
  return result;
}

ExcelValue results_t148() {
  static ExcelValue result;
  if(variable_set[3003] == 1) { return result;}
  result = ZERO;
  variable_set[3003] = 1;
  return result;
}

ExcelValue results_u148() {
  static ExcelValue result;
  if(variable_set[3004] == 1) { return result;}
  result = ZERO;
  variable_set[3004] = 1;
  return result;
}

ExcelValue results_v148() {
  static ExcelValue result;
  if(variable_set[3005] == 1) { return result;}
  result = ZERO;
  variable_set[3005] = 1;
  return result;
}

ExcelValue results_w148() {
  static ExcelValue result;
  if(variable_set[3006] == 1) { return result;}
  result = ZERO;
  variable_set[3006] = 1;
  return result;
}

ExcelValue results_e149() { return constant1011; }
ExcelValue results_f149() { return constant1012; }
ExcelValue results_g149() { return constant1013; }
ExcelValue results_h149() { return constant1014; }
ExcelValue results_i149() { return constant1015; }
ExcelValue results_j149() { return constant1016; }
ExcelValue results_k149() { return constant1017; }
ExcelValue results_l149() { return constant1018; }
ExcelValue results_m149() { return constant1019; }
ExcelValue results_n149() { return constant1020; }
ExcelValue results_o149() { return constant1021; }
ExcelValue results_p149() { return constant1022; }
ExcelValue results_q149() { return constant1023; }
ExcelValue results_r149() { return constant1024; }
ExcelValue results_s149() { return constant1025; }
ExcelValue results_t149() { return constant1026; }
ExcelValue results_u149() { return constant1027; }
ExcelValue results_v149() { return constant1028; }
ExcelValue results_w149() { return constant1029; }
ExcelValue results_e150() { return constant1030; }
ExcelValue results_f150() { return constant1031; }
ExcelValue results_g150() { return constant1032; }
ExcelValue results_h150() { return constant1033; }
ExcelValue results_i150() { return constant1034; }
ExcelValue results_j150() { return constant1035; }
ExcelValue results_k150() { return constant1036; }
ExcelValue results_l150() { return constant1037; }
ExcelValue results_m150() { return constant1038; }
ExcelValue results_n150() { return constant1039; }
ExcelValue results_o150() { return constant1040; }
ExcelValue results_p150() { return constant1041; }
ExcelValue results_q150() { return constant1042; }
ExcelValue results_r150() { return constant1043; }
ExcelValue results_s150() { return constant1044; }
ExcelValue results_t150() { return constant1045; }
ExcelValue results_u150() { return constant1046; }
ExcelValue results_v150() { return constant1047; }
ExcelValue results_w150() { return constant1048; }
ExcelValue results_e151() { return constant1049; }
ExcelValue results_f151() {
  static ExcelValue result;
  if(variable_set[3046] == 1) { return result;}
  result = ZERO;
  variable_set[3046] = 1;
  return result;
}

ExcelValue results_g151() {
  static ExcelValue result;
  if(variable_set[3047] == 1) { return result;}
  result = ZERO;
  variable_set[3047] = 1;
  return result;
}

ExcelValue results_h151() {
  static ExcelValue result;
  if(variable_set[3048] == 1) { return result;}
  result = ZERO;
  variable_set[3048] = 1;
  return result;
}

ExcelValue results_i151() {
  static ExcelValue result;
  if(variable_set[3049] == 1) { return result;}
  result = ZERO;
  variable_set[3049] = 1;
  return result;
}

ExcelValue results_j151() {
  static ExcelValue result;
  if(variable_set[3050] == 1) { return result;}
  result = ZERO;
  variable_set[3050] = 1;
  return result;
}

ExcelValue results_k151() {
  static ExcelValue result;
  if(variable_set[3051] == 1) { return result;}
  result = ZERO;
  variable_set[3051] = 1;
  return result;
}

ExcelValue results_l151() {
  static ExcelValue result;
  if(variable_set[3052] == 1) { return result;}
  result = ZERO;
  variable_set[3052] = 1;
  return result;
}

ExcelValue results_m151() {
  static ExcelValue result;
  if(variable_set[3053] == 1) { return result;}
  result = ZERO;
  variable_set[3053] = 1;
  return result;
}

ExcelValue results_n151() {
  static ExcelValue result;
  if(variable_set[3054] == 1) { return result;}
  result = ZERO;
  variable_set[3054] = 1;
  return result;
}

ExcelValue results_o151() {
  static ExcelValue result;
  if(variable_set[3055] == 1) { return result;}
  result = ZERO;
  variable_set[3055] = 1;
  return result;
}

ExcelValue results_p151() {
  static ExcelValue result;
  if(variable_set[3056] == 1) { return result;}
  result = ZERO;
  variable_set[3056] = 1;
  return result;
}

ExcelValue results_q151() {
  static ExcelValue result;
  if(variable_set[3057] == 1) { return result;}
  result = ZERO;
  variable_set[3057] = 1;
  return result;
}

ExcelValue results_r151() {
  static ExcelValue result;
  if(variable_set[3058] == 1) { return result;}
  result = ZERO;
  variable_set[3058] = 1;
  return result;
}

ExcelValue results_s151() {
  static ExcelValue result;
  if(variable_set[3059] == 1) { return result;}
  result = ZERO;
  variable_set[3059] = 1;
  return result;
}

ExcelValue results_t151() {
  static ExcelValue result;
  if(variable_set[3060] == 1) { return result;}
  result = ZERO;
  variable_set[3060] = 1;
  return result;
}

ExcelValue results_u151() {
  static ExcelValue result;
  if(variable_set[3061] == 1) { return result;}
  result = ZERO;
  variable_set[3061] = 1;
  return result;
}

ExcelValue results_v151() {
  static ExcelValue result;
  if(variable_set[3062] == 1) { return result;}
  result = ZERO;
  variable_set[3062] = 1;
  return result;
}

ExcelValue results_w151() {
  static ExcelValue result;
  if(variable_set[3063] == 1) { return result;}
  result = ZERO;
  variable_set[3063] = 1;
  return result;
}

ExcelValue results_e152() { return constant1050; }
ExcelValue results_f152() {
  static ExcelValue result;
  if(variable_set[3065] == 1) { return result;}
  result = ZERO;
  variable_set[3065] = 1;
  return result;
}

ExcelValue results_g152() {
  static ExcelValue result;
  if(variable_set[3066] == 1) { return result;}
  result = ZERO;
  variable_set[3066] = 1;
  return result;
}

ExcelValue results_h152() {
  static ExcelValue result;
  if(variable_set[3067] == 1) { return result;}
  result = ZERO;
  variable_set[3067] = 1;
  return result;
}

ExcelValue results_i152() {
  static ExcelValue result;
  if(variable_set[3068] == 1) { return result;}
  result = ZERO;
  variable_set[3068] = 1;
  return result;
}

ExcelValue results_j152() {
  static ExcelValue result;
  if(variable_set[3069] == 1) { return result;}
  result = ZERO;
  variable_set[3069] = 1;
  return result;
}

ExcelValue results_k152() {
  static ExcelValue result;
  if(variable_set[3070] == 1) { return result;}
  result = ZERO;
  variable_set[3070] = 1;
  return result;
}

ExcelValue results_l152() {
  static ExcelValue result;
  if(variable_set[3071] == 1) { return result;}
  result = ZERO;
  variable_set[3071] = 1;
  return result;
}

ExcelValue results_m152() {
  static ExcelValue result;
  if(variable_set[3072] == 1) { return result;}
  result = ZERO;
  variable_set[3072] = 1;
  return result;
}

ExcelValue results_n152() {
  static ExcelValue result;
  if(variable_set[3073] == 1) { return result;}
  result = ZERO;
  variable_set[3073] = 1;
  return result;
}

ExcelValue results_o152() {
  static ExcelValue result;
  if(variable_set[3074] == 1) { return result;}
  result = ZERO;
  variable_set[3074] = 1;
  return result;
}

ExcelValue results_p152() {
  static ExcelValue result;
  if(variable_set[3075] == 1) { return result;}
  result = ZERO;
  variable_set[3075] = 1;
  return result;
}

ExcelValue results_q152() {
  static ExcelValue result;
  if(variable_set[3076] == 1) { return result;}
  result = ZERO;
  variable_set[3076] = 1;
  return result;
}

ExcelValue results_r152() {
  static ExcelValue result;
  if(variable_set[3077] == 1) { return result;}
  result = ZERO;
  variable_set[3077] = 1;
  return result;
}

ExcelValue results_s152() {
  static ExcelValue result;
  if(variable_set[3078] == 1) { return result;}
  result = ZERO;
  variable_set[3078] = 1;
  return result;
}

ExcelValue results_t152() {
  static ExcelValue result;
  if(variable_set[3079] == 1) { return result;}
  result = ZERO;
  variable_set[3079] = 1;
  return result;
}

ExcelValue results_u152() {
  static ExcelValue result;
  if(variable_set[3080] == 1) { return result;}
  result = ZERO;
  variable_set[3080] = 1;
  return result;
}

ExcelValue results_v152() {
  static ExcelValue result;
  if(variable_set[3081] == 1) { return result;}
  result = ZERO;
  variable_set[3081] = 1;
  return result;
}

ExcelValue results_w152() {
  static ExcelValue result;
  if(variable_set[3082] == 1) { return result;}
  result = ZERO;
  variable_set[3082] = 1;
  return result;
}

ExcelValue results_e153() { return constant1051; }
ExcelValue results_f153() {
  static ExcelValue result;
  if(variable_set[3084] == 1) { return result;}
  result = ZERO;
  variable_set[3084] = 1;
  return result;
}

ExcelValue results_g153() {
  static ExcelValue result;
  if(variable_set[3085] == 1) { return result;}
  result = ZERO;
  variable_set[3085] = 1;
  return result;
}

ExcelValue results_h153() {
  static ExcelValue result;
  if(variable_set[3086] == 1) { return result;}
  result = ZERO;
  variable_set[3086] = 1;
  return result;
}

ExcelValue results_i153() {
  static ExcelValue result;
  if(variable_set[3087] == 1) { return result;}
  result = ZERO;
  variable_set[3087] = 1;
  return result;
}

ExcelValue results_j153() {
  static ExcelValue result;
  if(variable_set[3088] == 1) { return result;}
  result = ZERO;
  variable_set[3088] = 1;
  return result;
}

ExcelValue results_k153() {
  static ExcelValue result;
  if(variable_set[3089] == 1) { return result;}
  result = ZERO;
  variable_set[3089] = 1;
  return result;
}

ExcelValue results_l153() {
  static ExcelValue result;
  if(variable_set[3090] == 1) { return result;}
  result = ZERO;
  variable_set[3090] = 1;
  return result;
}

ExcelValue results_m153() {
  static ExcelValue result;
  if(variable_set[3091] == 1) { return result;}
  result = ZERO;
  variable_set[3091] = 1;
  return result;
}

ExcelValue results_n153() {
  static ExcelValue result;
  if(variable_set[3092] == 1) { return result;}
  result = ZERO;
  variable_set[3092] = 1;
  return result;
}

ExcelValue results_o153() {
  static ExcelValue result;
  if(variable_set[3093] == 1) { return result;}
  result = ZERO;
  variable_set[3093] = 1;
  return result;
}

ExcelValue results_p153() {
  static ExcelValue result;
  if(variable_set[3094] == 1) { return result;}
  result = ZERO;
  variable_set[3094] = 1;
  return result;
}

ExcelValue results_q153() {
  static ExcelValue result;
  if(variable_set[3095] == 1) { return result;}
  result = ZERO;
  variable_set[3095] = 1;
  return result;
}

ExcelValue results_r153() {
  static ExcelValue result;
  if(variable_set[3096] == 1) { return result;}
  result = ZERO;
  variable_set[3096] = 1;
  return result;
}

ExcelValue results_s153() {
  static ExcelValue result;
  if(variable_set[3097] == 1) { return result;}
  result = ZERO;
  variable_set[3097] = 1;
  return result;
}

ExcelValue results_t153() {
  static ExcelValue result;
  if(variable_set[3098] == 1) { return result;}
  result = ZERO;
  variable_set[3098] = 1;
  return result;
}

ExcelValue results_u153() {
  static ExcelValue result;
  if(variable_set[3099] == 1) { return result;}
  result = ZERO;
  variable_set[3099] = 1;
  return result;
}

ExcelValue results_v153() {
  static ExcelValue result;
  if(variable_set[3100] == 1) { return result;}
  result = ZERO;
  variable_set[3100] = 1;
  return result;
}

ExcelValue results_w153() {
  static ExcelValue result;
  if(variable_set[3101] == 1) { return result;}
  result = ZERO;
  variable_set[3101] = 1;
  return result;
}

ExcelValue results_e154() { return constant1052; }
ExcelValue results_f154() {
  static ExcelValue result;
  if(variable_set[3103] == 1) { return result;}
  result = ZERO;
  variable_set[3103] = 1;
  return result;
}

ExcelValue results_g154() {
  static ExcelValue result;
  if(variable_set[3104] == 1) { return result;}
  result = ZERO;
  variable_set[3104] = 1;
  return result;
}

ExcelValue results_h154() {
  static ExcelValue result;
  if(variable_set[3105] == 1) { return result;}
  result = ZERO;
  variable_set[3105] = 1;
  return result;
}

ExcelValue results_i154() {
  static ExcelValue result;
  if(variable_set[3106] == 1) { return result;}
  result = ZERO;
  variable_set[3106] = 1;
  return result;
}

ExcelValue results_j154() {
  static ExcelValue result;
  if(variable_set[3107] == 1) { return result;}
  result = ZERO;
  variable_set[3107] = 1;
  return result;
}

ExcelValue results_k154() {
  static ExcelValue result;
  if(variable_set[3108] == 1) { return result;}
  result = ZERO;
  variable_set[3108] = 1;
  return result;
}

ExcelValue results_l154() {
  static ExcelValue result;
  if(variable_set[3109] == 1) { return result;}
  result = ZERO;
  variable_set[3109] = 1;
  return result;
}

ExcelValue results_m154() {
  static ExcelValue result;
  if(variable_set[3110] == 1) { return result;}
  result = ZERO;
  variable_set[3110] = 1;
  return result;
}

ExcelValue results_n154() {
  static ExcelValue result;
  if(variable_set[3111] == 1) { return result;}
  result = ZERO;
  variable_set[3111] = 1;
  return result;
}

ExcelValue results_o154() {
  static ExcelValue result;
  if(variable_set[3112] == 1) { return result;}
  result = ZERO;
  variable_set[3112] = 1;
  return result;
}

ExcelValue results_p154() {
  static ExcelValue result;
  if(variable_set[3113] == 1) { return result;}
  result = ZERO;
  variable_set[3113] = 1;
  return result;
}

ExcelValue results_q154() {
  static ExcelValue result;
  if(variable_set[3114] == 1) { return result;}
  result = ZERO;
  variable_set[3114] = 1;
  return result;
}

ExcelValue results_r154() {
  static ExcelValue result;
  if(variable_set[3115] == 1) { return result;}
  result = ZERO;
  variable_set[3115] = 1;
  return result;
}

ExcelValue results_s154() {
  static ExcelValue result;
  if(variable_set[3116] == 1) { return result;}
  result = ZERO;
  variable_set[3116] = 1;
  return result;
}

ExcelValue results_t154() {
  static ExcelValue result;
  if(variable_set[3117] == 1) { return result;}
  result = ZERO;
  variable_set[3117] = 1;
  return result;
}

ExcelValue results_u154() {
  static ExcelValue result;
  if(variable_set[3118] == 1) { return result;}
  result = ZERO;
  variable_set[3118] = 1;
  return result;
}

ExcelValue results_v154() {
  static ExcelValue result;
  if(variable_set[3119] == 1) { return result;}
  result = ZERO;
  variable_set[3119] = 1;
  return result;
}

ExcelValue results_w154() {
  static ExcelValue result;
  if(variable_set[3120] == 1) { return result;}
  result = ZERO;
  variable_set[3120] = 1;
  return result;
}

ExcelValue results_e155() { return constant1053; }
ExcelValue results_f155() {
  static ExcelValue result;
  if(variable_set[3122] == 1) { return result;}
  result = ZERO;
  variable_set[3122] = 1;
  return result;
}

ExcelValue results_g155() {
  static ExcelValue result;
  if(variable_set[3123] == 1) { return result;}
  result = ZERO;
  variable_set[3123] = 1;
  return result;
}

ExcelValue results_h155() {
  static ExcelValue result;
  if(variable_set[3124] == 1) { return result;}
  result = ZERO;
  variable_set[3124] = 1;
  return result;
}

ExcelValue results_i155() {
  static ExcelValue result;
  if(variable_set[3125] == 1) { return result;}
  result = ZERO;
  variable_set[3125] = 1;
  return result;
}

ExcelValue results_j155() {
  static ExcelValue result;
  if(variable_set[3126] == 1) { return result;}
  result = ZERO;
  variable_set[3126] = 1;
  return result;
}

ExcelValue results_k155() {
  static ExcelValue result;
  if(variable_set[3127] == 1) { return result;}
  result = ZERO;
  variable_set[3127] = 1;
  return result;
}

ExcelValue results_l155() {
  static ExcelValue result;
  if(variable_set[3128] == 1) { return result;}
  result = ZERO;
  variable_set[3128] = 1;
  return result;
}

ExcelValue results_m155() {
  static ExcelValue result;
  if(variable_set[3129] == 1) { return result;}
  result = ZERO;
  variable_set[3129] = 1;
  return result;
}

ExcelValue results_n155() {
  static ExcelValue result;
  if(variable_set[3130] == 1) { return result;}
  result = ZERO;
  variable_set[3130] = 1;
  return result;
}

ExcelValue results_o155() {
  static ExcelValue result;
  if(variable_set[3131] == 1) { return result;}
  result = ZERO;
  variable_set[3131] = 1;
  return result;
}

ExcelValue results_p155() {
  static ExcelValue result;
  if(variable_set[3132] == 1) { return result;}
  result = ZERO;
  variable_set[3132] = 1;
  return result;
}

ExcelValue results_q155() {
  static ExcelValue result;
  if(variable_set[3133] == 1) { return result;}
  result = ZERO;
  variable_set[3133] = 1;
  return result;
}

ExcelValue results_r155() {
  static ExcelValue result;
  if(variable_set[3134] == 1) { return result;}
  result = ZERO;
  variable_set[3134] = 1;
  return result;
}

ExcelValue results_s155() {
  static ExcelValue result;
  if(variable_set[3135] == 1) { return result;}
  result = ZERO;
  variable_set[3135] = 1;
  return result;
}

ExcelValue results_t155() {
  static ExcelValue result;
  if(variable_set[3136] == 1) { return result;}
  result = ZERO;
  variable_set[3136] = 1;
  return result;
}

ExcelValue results_u155() {
  static ExcelValue result;
  if(variable_set[3137] == 1) { return result;}
  result = ZERO;
  variable_set[3137] = 1;
  return result;
}

ExcelValue results_v155() {
  static ExcelValue result;
  if(variable_set[3138] == 1) { return result;}
  result = ZERO;
  variable_set[3138] = 1;
  return result;
}

ExcelValue results_w155() {
  static ExcelValue result;
  if(variable_set[3139] == 1) { return result;}
  result = ZERO;
  variable_set[3139] = 1;
  return result;
}

ExcelValue results_e156() { return constant1054; }
ExcelValue results_f156() {
  static ExcelValue result;
  if(variable_set[3141] == 1) { return result;}
  result = ZERO;
  variable_set[3141] = 1;
  return result;
}

ExcelValue results_g156() {
  static ExcelValue result;
  if(variable_set[3142] == 1) { return result;}
  result = ZERO;
  variable_set[3142] = 1;
  return result;
}

ExcelValue results_h156() {
  static ExcelValue result;
  if(variable_set[3143] == 1) { return result;}
  result = ZERO;
  variable_set[3143] = 1;
  return result;
}

ExcelValue results_i156() {
  static ExcelValue result;
  if(variable_set[3144] == 1) { return result;}
  result = ZERO;
  variable_set[3144] = 1;
  return result;
}

ExcelValue results_j156() {
  static ExcelValue result;
  if(variable_set[3145] == 1) { return result;}
  result = ZERO;
  variable_set[3145] = 1;
  return result;
}

ExcelValue results_k156() {
  static ExcelValue result;
  if(variable_set[3146] == 1) { return result;}
  result = ZERO;
  variable_set[3146] = 1;
  return result;
}

ExcelValue results_l156() {
  static ExcelValue result;
  if(variable_set[3147] == 1) { return result;}
  result = ZERO;
  variable_set[3147] = 1;
  return result;
}

ExcelValue results_m156() {
  static ExcelValue result;
  if(variable_set[3148] == 1) { return result;}
  result = ZERO;
  variable_set[3148] = 1;
  return result;
}

ExcelValue results_n156() {
  static ExcelValue result;
  if(variable_set[3149] == 1) { return result;}
  result = ZERO;
  variable_set[3149] = 1;
  return result;
}

ExcelValue results_o156() {
  static ExcelValue result;
  if(variable_set[3150] == 1) { return result;}
  result = ZERO;
  variable_set[3150] = 1;
  return result;
}

ExcelValue results_p156() {
  static ExcelValue result;
  if(variable_set[3151] == 1) { return result;}
  result = ZERO;
  variable_set[3151] = 1;
  return result;
}

ExcelValue results_q156() {
  static ExcelValue result;
  if(variable_set[3152] == 1) { return result;}
  result = ZERO;
  variable_set[3152] = 1;
  return result;
}

ExcelValue results_r156() {
  static ExcelValue result;
  if(variable_set[3153] == 1) { return result;}
  result = ZERO;
  variable_set[3153] = 1;
  return result;
}

ExcelValue results_s156() {
  static ExcelValue result;
  if(variable_set[3154] == 1) { return result;}
  result = ZERO;
  variable_set[3154] = 1;
  return result;
}

ExcelValue results_t156() {
  static ExcelValue result;
  if(variable_set[3155] == 1) { return result;}
  result = ZERO;
  variable_set[3155] = 1;
  return result;
}

ExcelValue results_u156() {
  static ExcelValue result;
  if(variable_set[3156] == 1) { return result;}
  result = ZERO;
  variable_set[3156] = 1;
  return result;
}

ExcelValue results_v156() {
  static ExcelValue result;
  if(variable_set[3157] == 1) { return result;}
  result = ZERO;
  variable_set[3157] = 1;
  return result;
}

ExcelValue results_w156() {
  static ExcelValue result;
  if(variable_set[3158] == 1) { return result;}
  result = ZERO;
  variable_set[3158] = 1;
  return result;
}

ExcelValue results_e157() { return constant1055; }
ExcelValue results_f157() { return constant1056; }
ExcelValue results_g157() { return constant1057; }
ExcelValue results_h157() { return constant1058; }
ExcelValue results_i157() { return constant1059; }
ExcelValue results_j157() { return constant1060; }
ExcelValue results_k157() { return constant1061; }
ExcelValue results_l157() { return constant1062; }
ExcelValue results_m157() { return constant1063; }
ExcelValue results_n157() { return constant1064; }
ExcelValue results_o157() { return constant1065; }
ExcelValue results_p157() { return constant1066; }
ExcelValue results_q157() { return constant1066; }
ExcelValue results_r157() { return constant1066; }
ExcelValue results_s157() { return constant1066; }
ExcelValue results_t157() { return constant1066; }
ExcelValue results_u157() { return constant1066; }
ExcelValue results_v157() { return constant1066; }
ExcelValue results_w157() { return constant1066; }
ExcelValue results_e158() { return constant1067; }
ExcelValue results_f158() { return constant1068; }
ExcelValue results_g158() { return constant1069; }
ExcelValue results_h158() { return constant1070; }
ExcelValue results_i158() { return constant1071; }
ExcelValue results_j158() { return constant1072; }
ExcelValue results_k158() { return constant1073; }
ExcelValue results_l158() { return constant1074; }
ExcelValue results_m158() { return constant1075; }
ExcelValue results_n158() { return constant1076; }
ExcelValue results_o158() { return constant1077; }
ExcelValue results_p158() { return constant1078; }
ExcelValue results_q158() { return constant1078; }
ExcelValue results_r158() { return constant1078; }
ExcelValue results_s158() { return constant1078; }
ExcelValue results_t158() { return constant1078; }
ExcelValue results_u158() { return constant1078; }
ExcelValue results_v158() { return constant1078; }
ExcelValue results_w158() { return constant1078; }
ExcelValue results_e159() { return constant1079; }
ExcelValue results_f159() { return constant1080; }
ExcelValue results_g159() { return constant1081; }
ExcelValue results_h159() { return constant1082; }
ExcelValue results_i159() { return constant1083; }
ExcelValue results_j159() { return constant1084; }
ExcelValue results_k159() { return constant1085; }
ExcelValue results_l159() { return constant1086; }
ExcelValue results_m159() { return constant1087; }
ExcelValue results_n159() { return constant1088; }
ExcelValue results_o159() { return constant1089; }
ExcelValue results_p159() { return constant1090; }
ExcelValue results_q159() { return constant1090; }
ExcelValue results_r159() { return constant1090; }
ExcelValue results_s159() { return constant1090; }
ExcelValue results_t159() { return constant1090; }
ExcelValue results_u159() { return constant1090; }
ExcelValue results_v159() { return constant1090; }
ExcelValue results_w159() { return constant1090; }
ExcelValue results_e160() { return constant1091; }
ExcelValue results_f160() { return constant1092; }
ExcelValue results_g160() { return constant1093; }
ExcelValue results_h160() { return constant1094; }
ExcelValue results_i160() { return constant1095; }
ExcelValue results_j160() { return constant1096; }
ExcelValue results_k160() { return constant1097; }
ExcelValue results_l160() { return constant1098; }
ExcelValue results_m160() { return constant1099; }
ExcelValue results_n160() { return constant1100; }
ExcelValue results_o160() { return constant1101; }
ExcelValue results_p160() { return constant1102; }
ExcelValue results_q160() { return constant1103; }
ExcelValue results_r160() { return constant1104; }
ExcelValue results_s160() { return constant1105; }
ExcelValue results_t160() { return constant1106; }
ExcelValue results_u160() { return constant1107; }
ExcelValue results_v160() { return constant1108; }
ExcelValue results_w160() { return constant1109; }
ExcelValue results_e161() { return constant1110; }
ExcelValue results_f161() { return constant1111; }
ExcelValue results_g161() { return constant1112; }
ExcelValue results_h161() { return constant1113; }
ExcelValue results_i161() { return constant1114; }
ExcelValue results_j161() { return constant1115; }
ExcelValue results_k161() { return constant1116; }
ExcelValue results_l161() { return constant1117; }
ExcelValue results_m161() { return constant1118; }
ExcelValue results_n161() { return constant1119; }
ExcelValue results_o161() { return constant1120; }
ExcelValue results_p161() { return constant1121; }
ExcelValue results_q161() { return constant1121; }
ExcelValue results_r161() { return constant1121; }
ExcelValue results_s161() { return constant1121; }
ExcelValue results_t161() { return constant1121; }
ExcelValue results_u161() { return constant1121; }
ExcelValue results_v161() { return constant1121; }
ExcelValue results_w161() { return constant1121; }
ExcelValue results_e644() { return constant1122; }
ExcelValue results_f644() {
  static ExcelValue result;
  if(variable_set[3255] == 1) { return result;}
  result = ZERO;
  variable_set[3255] = 1;
  return result;
}

ExcelValue results_g644() {
  static ExcelValue result;
  if(variable_set[3256] == 1) { return result;}
  result = ZERO;
  variable_set[3256] = 1;
  return result;
}

ExcelValue results_h644() { return constant1123; }
ExcelValue results_i644() { return constant1124; }
ExcelValue results_j644() { return constant1125; }
ExcelValue results_k644() { return constant1126; }
ExcelValue results_l644() { return constant1127; }
ExcelValue results_m644() { return constant1128; }
ExcelValue results_n644() { return constant1128; }
ExcelValue results_o644() { return constant1128; }
ExcelValue results_p644() { return constant1128; }
ExcelValue results_q644() { return constant1128; }
ExcelValue results_r644() { return constant1128; }
ExcelValue results_s644() { return constant1128; }
ExcelValue results_t644() { return constant1128; }
ExcelValue results_u644() { return constant1128; }
ExcelValue results_v644() { return constant1128; }
ExcelValue results_w644() { return constant1128; }
ExcelValue results_e645() { return constant1129; }
ExcelValue results_f645() { return constant1130; }
ExcelValue results_g645() { return constant1130; }
ExcelValue results_h645() { return constant1131; }
ExcelValue results_i645() { return constant1132; }
ExcelValue results_j645() { return constant1133; }
ExcelValue results_k645() { return constant1128; }
ExcelValue results_l645() { return constant1128; }
ExcelValue results_m645() { return constant1128; }
ExcelValue results_n645() { return constant1128; }
ExcelValue results_o645() { return constant1128; }
ExcelValue results_p645() { return constant1128; }
ExcelValue results_q645() { return constant1128; }
ExcelValue results_r645() { return constant1128; }
ExcelValue results_s645() { return constant1128; }
ExcelValue results_t645() { return constant1128; }
ExcelValue results_u645() { return constant1128; }
ExcelValue results_v645() { return constant1128; }
ExcelValue results_w645() { return constant1128; }
ExcelValue results_e646() { return constant1134; }
ExcelValue results_f646() { return constant1135; }
ExcelValue results_g646() { return constant1135; }
ExcelValue results_h646() { return constant1136; }
ExcelValue results_i646() { return constant1137; }
ExcelValue results_j646() { return constant1138; }
ExcelValue results_k646() { return constant1139; }
ExcelValue results_l646() { return constant1140; }
ExcelValue results_m646() {
  static ExcelValue result;
  if(variable_set[3300] == 1) { return result;}
  result = ZERO;
  variable_set[3300] = 1;
  return result;
}

ExcelValue results_n646() {
  static ExcelValue result;
  if(variable_set[3301] == 1) { return result;}
  result = ZERO;
  variable_set[3301] = 1;
  return result;
}

ExcelValue results_o646() {
  static ExcelValue result;
  if(variable_set[3302] == 1) { return result;}
  result = ZERO;
  variable_set[3302] = 1;
  return result;
}

ExcelValue results_p646() {
  static ExcelValue result;
  if(variable_set[3303] == 1) { return result;}
  result = ZERO;
  variable_set[3303] = 1;
  return result;
}

ExcelValue results_q646() {
  static ExcelValue result;
  if(variable_set[3304] == 1) { return result;}
  result = ZERO;
  variable_set[3304] = 1;
  return result;
}

ExcelValue results_r646() {
  static ExcelValue result;
  if(variable_set[3305] == 1) { return result;}
  result = ZERO;
  variable_set[3305] = 1;
  return result;
}

ExcelValue results_s646() {
  static ExcelValue result;
  if(variable_set[3306] == 1) { return result;}
  result = ZERO;
  variable_set[3306] = 1;
  return result;
}

ExcelValue results_t646() {
  static ExcelValue result;
  if(variable_set[3307] == 1) { return result;}
  result = ZERO;
  variable_set[3307] = 1;
  return result;
}

ExcelValue results_u646() {
  static ExcelValue result;
  if(variable_set[3308] == 1) { return result;}
  result = ZERO;
  variable_set[3308] = 1;
  return result;
}

ExcelValue results_v646() {
  static ExcelValue result;
  if(variable_set[3309] == 1) { return result;}
  result = ZERO;
  variable_set[3309] = 1;
  return result;
}

ExcelValue results_w646() {
  static ExcelValue result;
  if(variable_set[3310] == 1) { return result;}
  result = ZERO;
  variable_set[3310] = 1;
  return result;
}

ExcelValue results_e647() { return constant1141; }
ExcelValue results_f647() { return constant1142; }
ExcelValue results_g647() { return constant1142; }
ExcelValue results_h647() { return constant1143; }
ExcelValue results_i647() { return constant1144; }
ExcelValue results_j647() {
  static ExcelValue result;
  if(variable_set[3316] == 1) { return result;}
  result = ZERO;
  variable_set[3316] = 1;
  return result;
}

ExcelValue results_k647() {
  static ExcelValue result;
  if(variable_set[3317] == 1) { return result;}
  result = ZERO;
  variable_set[3317] = 1;
  return result;
}

ExcelValue results_l647() {
  static ExcelValue result;
  if(variable_set[3318] == 1) { return result;}
  result = ZERO;
  variable_set[3318] = 1;
  return result;
}

ExcelValue results_m647() {
  static ExcelValue result;
  if(variable_set[3319] == 1) { return result;}
  result = ZERO;
  variable_set[3319] = 1;
  return result;
}

ExcelValue results_n647() {
  static ExcelValue result;
  if(variable_set[3320] == 1) { return result;}
  result = ZERO;
  variable_set[3320] = 1;
  return result;
}

ExcelValue results_o647() {
  static ExcelValue result;
  if(variable_set[3321] == 1) { return result;}
  result = ZERO;
  variable_set[3321] = 1;
  return result;
}

ExcelValue results_p647() {
  static ExcelValue result;
  if(variable_set[3322] == 1) { return result;}
  result = ZERO;
  variable_set[3322] = 1;
  return result;
}

ExcelValue results_q647() {
  static ExcelValue result;
  if(variable_set[3323] == 1) { return result;}
  result = ZERO;
  variable_set[3323] = 1;
  return result;
}

ExcelValue results_r647() {
  static ExcelValue result;
  if(variable_set[3324] == 1) { return result;}
  result = ZERO;
  variable_set[3324] = 1;
  return result;
}

ExcelValue results_s647() {
  static ExcelValue result;
  if(variable_set[3325] == 1) { return result;}
  result = ZERO;
  variable_set[3325] = 1;
  return result;
}

ExcelValue results_t647() {
  static ExcelValue result;
  if(variable_set[3326] == 1) { return result;}
  result = ZERO;
  variable_set[3326] = 1;
  return result;
}

ExcelValue results_u647() {
  static ExcelValue result;
  if(variable_set[3327] == 1) { return result;}
  result = ZERO;
  variable_set[3327] = 1;
  return result;
}

ExcelValue results_v647() {
  static ExcelValue result;
  if(variable_set[3328] == 1) { return result;}
  result = ZERO;
  variable_set[3328] = 1;
  return result;
}

ExcelValue results_w647() {
  static ExcelValue result;
  if(variable_set[3329] == 1) { return result;}
  result = ZERO;
  variable_set[3329] = 1;
  return result;
}

ExcelValue results_e689() { return constant1145; }
ExcelValue results_f689() { return constant1146; }
ExcelValue results_g689() { return constant1147; }
ExcelValue results_h689() { return constant1148; }
ExcelValue results_i689() { return constant1149; }
ExcelValue results_j689() { return constant1150; }
ExcelValue results_k689() { return constant1151; }
ExcelValue results_l689() { return constant1152; }
ExcelValue results_m689() { return constant1153; }
ExcelValue results_n689() { return constant1154; }
ExcelValue results_o689() { return constant1155; }
ExcelValue results_p689() { return constant1156; }
ExcelValue results_q689() { return constant1157; }
ExcelValue results_r689() { return constant1158; }
ExcelValue results_s689() { return constant1159; }
ExcelValue results_t689() { return constant1160; }
ExcelValue results_u689() { return constant1161; }
ExcelValue results_v689() { return constant1162; }
ExcelValue results_w689() { return constant1163; }
ExcelValue results_e690() { return constant1164; }
ExcelValue results_f690() {
  static ExcelValue result;
  if(variable_set[3350] == 1) { return result;}
  result = ZERO;
  variable_set[3350] = 1;
  return result;
}

ExcelValue results_g690() {
  static ExcelValue result;
  if(variable_set[3351] == 1) { return result;}
  result = ZERO;
  variable_set[3351] = 1;
  return result;
}

ExcelValue results_h690() { return constant1165; }
ExcelValue results_i690() { return constant1166; }
ExcelValue results_j690() { return constant1167; }
ExcelValue results_k690() { return constant1168; }
ExcelValue results_l690() { return constant1169; }
ExcelValue results_m690() { return constant1170; }
ExcelValue results_n690() { return constant1171; }
ExcelValue results_o690() { return constant1172; }
ExcelValue results_p690() { return constant1173; }
ExcelValue results_q690() { return constant1174; }
ExcelValue results_r690() { return constant1175; }
ExcelValue results_s690() { return constant1176; }
ExcelValue results_t690() { return constant1177; }
ExcelValue results_u690() { return constant1178; }
ExcelValue results_v690() { return constant1179; }
ExcelValue results_w690() { return constant1180; }
ExcelValue results_e691() { return constant1181; }
ExcelValue results_f691() { return constant1182; }
ExcelValue results_g691() { return constant1183; }
ExcelValue results_h691() { return constant1184; }
ExcelValue results_i691() { return constant1185; }
ExcelValue results_j691() { return constant1186; }
ExcelValue results_k691() { return constant1187; }
ExcelValue results_l691() { return constant1188; }
ExcelValue results_m691() { return constant1189; }
ExcelValue results_n691() { return constant1190; }
ExcelValue results_o691() { return constant1191; }
ExcelValue results_p691() { return constant1192; }
ExcelValue results_q691() { return constant1193; }
ExcelValue results_r691() { return constant1194; }
ExcelValue results_s691() { return constant1195; }
ExcelValue results_t691() { return constant1196; }
ExcelValue results_u691() { return constant1197; }
ExcelValue results_v691() { return constant1198; }
ExcelValue results_w691() { return constant1199; }
ExcelValue results_e692() { return constant1200; }
ExcelValue results_f692() { return constant1201; }
ExcelValue results_g692() { return constant1202; }
ExcelValue results_h692() { return constant1203; }
ExcelValue results_i692() { return constant1204; }
ExcelValue results_j692() { return constant1205; }
ExcelValue results_k692() { return constant1206; }
ExcelValue results_l692() { return constant1207; }
ExcelValue results_m692() { return constant1208; }
ExcelValue results_n692() { return constant1209; }
ExcelValue results_o692() { return constant1210; }
ExcelValue results_p692() { return constant1211; }
ExcelValue results_q692() { return constant1212; }
ExcelValue results_r692() { return constant1213; }
ExcelValue results_s692() { return constant1214; }
ExcelValue results_t692() { return constant1215; }
ExcelValue results_u692() { return constant1216; }
ExcelValue results_v692() { return constant1217; }
ExcelValue results_w692() { return constant1218; }
// Start of named references
ExcelValue input_example_pathways() {
  static ExcelValue result;
  if(variable_set[3406] == 1) { return result;}
  static ExcelValue array0[49];
  array0[0] = constant10;
  array0[1] = ONE;
  array0[2] = ONE;
  array0[3] = ONE;
  array0[4] = ONE;
  array0[5] = ONE;
  array0[6] = ONE;
  array0[7] = ONE;
  array0[8] = ONE;
  array0[9] = ONE;
  array0[10] = ONE;
  array0[11] = ONE;
  array0[12] = ONE;
  array0[13] = ONE;
  array0[14] = ONE;
  array0[15] = ONE;
  array0[16] = ONE;
  array0[17] = ONE;
  array0[18] = ONE;
  array0[19] = ONE;
  array0[20] = ONE;
  array0[21] = ONE;
  array0[22] = ONE;
  array0[23] = ONE;
  array0[24] = ONE;
  array0[25] = ONE;
  array0[26] = ONE;
  array0[27] = ONE;
  array0[28] = ONE;
  array0[29] = ONE;
  array0[30] = ONE;
  array0[31] = ONE;
  array0[32] = ONE;
  array0[33] = ONE;
  array0[34] = ONE;
  array0[35] = ONE;
  array0[36] = ONE;
  array0[37] = ONE;
  array0[38] = ONE;
  array0[39] = ONE;
  array0[40] = ONE;
  array0[41] = ONE;
  array0[42] = ONE;
  array0[43] = ONE;
  array0[44] = ONE;
  array0[45] = ONE;
  array0[46] = constant11;
  array0[47] = constant12;
  array0[48] = constant13;
  ExcelValue array0_ev = EXCEL_RANGE(array0,49,1);
  result = array0_ev;
  variable_set[3406] = 1;
  return result;
}

ExcelValue input_lever_ambition() {
  static ExcelValue result;
  if(variable_set[3407] == 1) { return result;}
  static ExcelValue array0[45];
  array0[0] = control_d17();
  array0[1] = control_d18();
  array0[2] = control_d19();
  array0[3] = control_d20();
  array0[4] = control_d21();
  array0[5] = control_d22();
  array0[6] = control_d23();
  array0[7] = control_d24();
  array0[8] = control_d25();
  array0[9] = control_d26();
  array0[10] = control_d27();
  array0[11] = control_d28();
  array0[12] = control_d29();
  array0[13] = control_d30();
  array0[14] = control_d31();
  array0[15] = control_d32();
  array0[16] = control_d33();
  array0[17] = control_d34();
  array0[18] = control_d35();
  array0[19] = control_d36();
  array0[20] = control_d37();
  array0[21] = control_d38();
  array0[22] = control_d39();
  array0[23] = control_d40();
  array0[24] = control_d41();
  array0[25] = control_d42();
  array0[26] = control_d43();
  array0[27] = control_d44();
  array0[28] = control_d45();
  array0[29] = control_d46();
  array0[30] = control_d47();
  array0[31] = control_d48();
  array0[32] = control_d49();
  array0[33] = control_d50();
  array0[34] = control_d51();
  array0[35] = control_d52();
  array0[36] = control_d53();
  array0[37] = control_d54();
  array0[38] = control_d55();
  array0[39] = control_d56();
  array0[40] = control_d57();
  array0[41] = control_d58();
  array0[42] = control_d59();
  array0[43] = control_d60();
  array0[44] = control_d61();
  ExcelValue array0_ev = EXCEL_RANGE(array0,45,1);
  result = array0_ev;
  variable_set[3407] = 1;
  return result;
}

ExcelValue input_lever_end() {
  static ExcelValue result;
  if(variable_set[3408] == 1) { return result;}
  static ExcelValue array0[45];
  array0[0] = control_g17();
  array0[1] = control_g18();
  array0[2] = control_g19();
  array0[3] = control_g20();
  array0[4] = control_g21();
  array0[5] = control_g22();
  array0[6] = control_g23();
  array0[7] = control_g24();
  array0[8] = control_g25();
  array0[9] = control_g26();
  array0[10] = control_g27();
  array0[11] = control_g28();
  array0[12] = control_g29();
  array0[13] = control_g30();
  array0[14] = control_g31();
  array0[15] = control_g32();
  array0[16] = control_g33();
  array0[17] = control_g34();
  array0[18] = control_g35();
  array0[19] = control_g36();
  array0[20] = control_g37();
  array0[21] = control_g38();
  array0[22] = control_g39();
  array0[23] = control_g40();
  array0[24] = control_g41();
  array0[25] = control_g42();
  array0[26] = control_g43();
  array0[27] = control_g44();
  array0[28] = control_g45();
  array0[29] = control_g46();
  array0[30] = control_g47();
  array0[31] = control_g48();
  array0[32] = control_g49();
  array0[33] = control_g50();
  array0[34] = control_g51();
  array0[35] = control_g52();
  array0[36] = control_g53();
  array0[37] = control_g54();
  array0[38] = control_g55();
  array0[39] = control_g56();
  array0[40] = control_g57();
  array0[41] = control_g58();
  array0[42] = control_g59();
  array0[43] = control_g60();
  array0[44] = control_g61();
  ExcelValue array0_ev = EXCEL_RANGE(array0,45,1);
  result = array0_ev;
  variable_set[3408] = 1;
  return result;
}

ExcelValue input_lever_start() {
  static ExcelValue result;
  if(variable_set[3409] == 1) { return result;}
  static ExcelValue array0[46];
  array0[0] = control_e17();
  array0[1] = control_e18();
  array0[2] = control_e19();
  array0[3] = control_e20();
  array0[4] = control_e21();
  array0[5] = control_e22();
  array0[6] = control_e23();
  array0[7] = control_e24();
  array0[8] = control_e25();
  array0[9] = control_e26();
  array0[10] = control_e27();
  array0[11] = control_e28();
  array0[12] = control_e29();
  array0[13] = control_e30();
  array0[14] = control_e31();
  array0[15] = control_e32();
  array0[16] = control_e33();
  array0[17] = control_e34();
  array0[18] = control_e35();
  array0[19] = control_e36();
  array0[20] = control_e37();
  array0[21] = control_e38();
  array0[22] = control_e39();
  array0[23] = control_e40();
  array0[24] = control_e41();
  array0[25] = control_e42();
  array0[26] = control_e43();
  array0[27] = control_e44();
  array0[28] = control_e45();
  array0[29] = control_e46();
  array0[30] = control_e47();
  array0[31] = control_e48();
  array0[32] = control_e49();
  array0[33] = control_e50();
  array0[34] = control_e51();
  array0[35] = control_e52();
  array0[36] = control_e53();
  array0[37] = control_e54();
  array0[38] = control_e55();
  array0[39] = control_e56();
  array0[40] = control_e57();
  array0[41] = control_e58();
  array0[42] = control_e59();
  array0[43] = control_e60();
  array0[44] = control_e61();
  array0[45] = control_e62();
  ExcelValue array0_ev = EXCEL_RANGE(array0,46,1);
  result = array0_ev;
  variable_set[3409] = 1;
  return result;
}

ExcelValue input_long_descriptions() {
  static ExcelValue result;
  if(variable_set[3410] == 1) { return result;}
  static ExcelValue array0[184];
  array0[0] = ONE;
  array0[1] = TWO;
  array0[2] = THREE;
  array0[3] = FOUR;
  array0[4] = ONE;
  array0[5] = TWO;
  array0[6] = THREE;
  array0[7] = FOUR;
  array0[8] = ONE;
  array0[9] = TWO;
  array0[10] = THREE;
  array0[11] = FOUR;
  array0[12] = ONE;
  array0[13] = TWO;
  array0[14] = THREE;
  array0[15] = FOUR;
  array0[16] = ONE;
  array0[17] = TWO;
  array0[18] = THREE;
  array0[19] = FOUR;
  array0[20] = ONE;
  array0[21] = TWO;
  array0[22] = THREE;
  array0[23] = FOUR;
  array0[24] = ONE;
  array0[25] = TWO;
  array0[26] = THREE;
  array0[27] = FOUR;
  array0[28] = ONE;
  array0[29] = TWO;
  array0[30] = THREE;
  array0[31] = FOUR;
  array0[32] = ONE;
  array0[33] = TWO;
  array0[34] = THREE;
  array0[35] = FOUR;
  array0[36] = ONE;
  array0[37] = TWO;
  array0[38] = THREE;
  array0[39] = FOUR;
  array0[40] = ONE;
  array0[41] = TWO;
  array0[42] = THREE;
  array0[43] = FOUR;
  array0[44] = ONE;
  array0[45] = TWO;
  array0[46] = THREE;
  array0[47] = FOUR;
  array0[48] = ONE;
  array0[49] = TWO;
  array0[50] = THREE;
  array0[51] = FOUR;
  array0[52] = ONE;
  array0[53] = TWO;
  array0[54] = THREE;
  array0[55] = FOUR;
  array0[56] = ONE;
  array0[57] = TWO;
  array0[58] = THREE;
  array0[59] = FOUR;
  array0[60] = ONE;
  array0[61] = TWO;
  array0[62] = THREE;
  array0[63] = FOUR;
  array0[64] = ONE;
  array0[65] = TWO;
  array0[66] = THREE;
  array0[67] = FOUR;
  array0[68] = ONE;
  array0[69] = TWO;
  array0[70] = THREE;
  array0[71] = FOUR;
  array0[72] = ONE;
  array0[73] = TWO;
  array0[74] = THREE;
  array0[75] = FOUR;
  array0[76] = ONE;
  array0[77] = TWO;
  array0[78] = THREE;
  array0[79] = FOUR;
  array0[80] = ONE;
  array0[81] = TWO;
  array0[82] = THREE;
  array0[83] = FOUR;
  array0[84] = ONE;
  array0[85] = TWO;
  array0[86] = THREE;
  array0[87] = FOUR;
  array0[88] = ONE;
  array0[89] = TWO;
  array0[90] = THREE;
  array0[91] = FOUR;
  array0[92] = ONE;
  array0[93] = TWO;
  array0[94] = THREE;
  array0[95] = FOUR;
  array0[96] = ONE;
  array0[97] = TWO;
  array0[98] = THREE;
  array0[99] = FOUR;
  array0[100] = ONE;
  array0[101] = TWO;
  array0[102] = THREE;
  array0[103] = FOUR;
  array0[104] = ONE;
  array0[105] = TWO;
  array0[106] = THREE;
  array0[107] = FOUR;
  array0[108] = ONE;
  array0[109] = TWO;
  array0[110] = THREE;
  array0[111] = FOUR;
  array0[112] = ONE;
  array0[113] = TWO;
  array0[114] = THREE;
  array0[115] = FOUR;
  array0[116] = ONE;
  array0[117] = TWO;
  array0[118] = THREE;
  array0[119] = FOUR;
  array0[120] = ONE;
  array0[121] = TWO;
  array0[122] = THREE;
  array0[123] = FOUR;
  array0[124] = ONE;
  array0[125] = TWO;
  array0[126] = THREE;
  array0[127] = FOUR;
  array0[128] = ONE;
  array0[129] = TWO;
  array0[130] = THREE;
  array0[131] = FOUR;
  array0[132] = ONE;
  array0[133] = TWO;
  array0[134] = THREE;
  array0[135] = FOUR;
  array0[136] = ONE;
  array0[137] = TWO;
  array0[138] = THREE;
  array0[139] = FOUR;
  array0[140] = ONE;
  array0[141] = TWO;
  array0[142] = THREE;
  array0[143] = FOUR;
  array0[144] = ONE;
  array0[145] = TWO;
  array0[146] = THREE;
  array0[147] = FOUR;
  array0[148] = ONE;
  array0[149] = TWO;
  array0[150] = THREE;
  array0[151] = FOUR;
  array0[152] = ONE;
  array0[153] = TWO;
  array0[154] = THREE;
  array0[155] = FOUR;
  array0[156] = ONE;
  array0[157] = TWO;
  array0[158] = THREE;
  array0[159] = FOUR;
  array0[160] = ONE;
  array0[161] = TWO;
  array0[162] = THREE;
  array0[163] = FOUR;
  array0[164] = ONE;
  array0[165] = TWO;
  array0[166] = THREE;
  array0[167] = FOUR;
  array0[168] = ONE;
  array0[169] = TWO;
  array0[170] = THREE;
  array0[171] = FOUR;
  array0[172] = ONE;
  array0[173] = TWO;
  array0[174] = THREE;
  array0[175] = FOUR;
  array0[176] = ONE;
  array0[177] = TWO;
  array0[178] = THREE;
  array0[179] = FOUR;
  array0[180] = BLANK;
  array0[181] = BLANK;
  array0[182] = BLANK;
  array0[183] = BLANK;
  ExcelValue array0_ev = EXCEL_RANGE(array0,46,4);
  result = array0_ev;
  variable_set[3410] = 1;
  return result;
}

ExcelValue input_names() {
  static ExcelValue result;
  if(variable_set[3411] == 1) { return result;}
  static ExcelValue array0[46];
  array0[0] = constant14;
  array0[1] = constant15;
  array0[2] = constant16;
  array0[3] = constant17;
  array0[4] = constant18;
  array0[5] = constant19;
  array0[6] = constant20;
  array0[7] = constant21;
  array0[8] = constant22;
  array0[9] = constant23;
  array0[10] = constant24;
  array0[11] = constant25;
  array0[12] = constant26;
  array0[13] = constant27;
  array0[14] = constant28;
  array0[15] = constant29;
  array0[16] = constant30;
  array0[17] = constant31;
  array0[18] = constant32;
  array0[19] = constant33;
  array0[20] = constant34;
  array0[21] = constant35;
  array0[22] = constant36;
  array0[23] = constant37;
  array0[24] = constant38;
  array0[25] = constant39;
  array0[26] = constant40;
  array0[27] = constant41;
  array0[28] = constant42;
  array0[29] = constant43;
  array0[30] = constant44;
  array0[31] = constant45;
  array0[32] = constant46;
  array0[33] = constant47;
  array0[34] = constant48;
  array0[35] = constant49;
  array0[36] = constant50;
  array0[37] = constant51;
  array0[38] = constant52;
  array0[39] = constant53;
  array0[40] = constant54;
  array0[41] = constant55;
  array0[42] = constant56;
  array0[43] = constant57;
  array0[44] = constant58;
  array0[45] = BLANK;
  ExcelValue array0_ev = EXCEL_RANGE(array0,46,1);
  result = array0_ev;
  variable_set[3411] = 1;
  return result;
}

ExcelValue input_types() {
  static ExcelValue result;
  if(variable_set[3412] == 1) { return result;}
  static ExcelValue array0[46];
  array0[0] = FOUR;
  array0[1] = FOUR;
  array0[2] = FOUR;
  array0[3] = FOUR;
  array0[4] = FOUR;
  array0[5] = FOUR;
  array0[6] = FOUR;
  array0[7] = FOUR;
  array0[8] = FOUR;
  array0[9] = FOUR;
  array0[10] = FOUR;
  array0[11] = FOUR;
  array0[12] = FOUR;
  array0[13] = FOUR;
  array0[14] = FOUR;
  array0[15] = FOUR;
  array0[16] = FOUR;
  array0[17] = FOUR;
  array0[18] = FOUR;
  array0[19] = FOUR;
  array0[20] = FOUR;
  array0[21] = FOUR;
  array0[22] = FOUR;
  array0[23] = FOUR;
  array0[24] = FOUR;
  array0[25] = FOUR;
  array0[26] = FOUR;
  array0[27] = FOUR;
  array0[28] = FOUR;
  array0[29] = FOUR;
  array0[30] = FOUR;
  array0[31] = FOUR;
  array0[32] = FOUR;
  array0[33] = FOUR;
  array0[34] = FOUR;
  array0[35] = FOUR;
  array0[36] = FOUR;
  array0[37] = FOUR;
  array0[38] = FOUR;
  array0[39] = FOUR;
  array0[40] = FOUR;
  array0[41] = FOUR;
  array0[42] = FOUR;
  array0[43] = FOUR;
  array0[44] = FOUR;
  array0[45] = BLANK;
  ExcelValue array0_ev = EXCEL_RANGE(array0,46,1);
  result = array0_ev;
  variable_set[3412] = 1;
  return result;
}

ExcelValue output_electricity_capacity_type() {
  static ExcelValue result;
  if(variable_set[3413] == 1) { return result;}
  static ExcelValue array0[171];
  array0[0] = constant59;
  array0[1] = constant60;
  array0[2] = constant61;
  array0[3] = constant62;
  array0[4] = constant63;
  array0[5] = constant64;
  array0[6] = constant65;
  array0[7] = constant66;
  array0[8] = constant67;
  array0[9] = constant68;
  array0[10] = constant69;
  array0[11] = constant70;
  array0[12] = constant71;
  array0[13] = constant72;
  array0[14] = constant73;
  array0[15] = constant74;
  array0[16] = constant75;
  array0[17] = constant76;
  array0[18] = constant77;
  array0[19] = constant50;
  array0[20] = NINE;
  array0[21] = EIGHT;
  array0[22] = constant78;
  array0[23] = constant79;
  array0[24] = constant80;
  array0[25] = constant81;
  array0[26] = constant82;
  array0[27] = constant83;
  array0[28] = constant83;
  array0[29] = constant83;
  array0[30] = constant83;
  array0[31] = constant83;
  array0[32] = constant83;
  array0[33] = constant83;
  array0[34] = constant83;
  array0[35] = constant83;
  array0[36] = constant83;
  array0[37] = constant83;
  array0[38] = constant84;
  array0[39] = ZERO;
  array0[40] = ZERO;
  array0[41] = constant85;
  array0[42] = constant86;
  array0[43] = constant87;
  array0[44] = constant88;
  array0[45] = constant89;
  array0[46] = constant90;
  array0[47] = constant90;
  array0[48] = constant90;
  array0[49] = constant90;
  array0[50] = constant90;
  array0[51] = constant90;
  array0[52] = constant90;
  array0[53] = constant90;
  array0[54] = constant90;
  array0[55] = constant90;
  array0[56] = constant90;
  array0[57] = constant91;
  array0[58] = ZERO;
  array0[59] = ZERO;
  array0[60] = constant85;
  array0[61] = constant86;
  array0[62] = constant87;
  array0[63] = constant88;
  array0[64] = constant89;
  array0[65] = constant90;
  array0[66] = constant90;
  array0[67] = constant90;
  array0[68] = constant90;
  array0[69] = constant90;
  array0[70] = constant90;
  array0[71] = constant90;
  array0[72] = constant90;
  array0[73] = constant90;
  array0[74] = constant90;
  array0[75] = constant90;
  array0[76] = constant92;
  array0[77] = FOUR;
  array0[78] = FOUR;
  array0[79] = constant93;
  array0[80] = constant94;
  array0[81] = constant95;
  array0[82] = constant96;
  array0[83] = constant97;
  array0[84] = constant98;
  array0[85] = constant98;
  array0[86] = constant98;
  array0[87] = constant98;
  array0[88] = constant98;
  array0[89] = constant98;
  array0[90] = constant98;
  array0[91] = constant98;
  array0[92] = constant98;
  array0[93] = constant98;
  array0[94] = constant98;
  array0[95] = constant99;
  array0[96] = TWO;
  array0[97] = TWO;
  array0[98] = constant100;
  array0[99] = constant101;
  array0[100] = constant102;
  array0[101] = constant97;
  array0[102] = constant103;
  array0[103] = constant104;
  array0[104] = constant104;
  array0[105] = constant104;
  array0[106] = constant104;
  array0[107] = constant104;
  array0[108] = constant104;
  array0[109] = constant104;
  array0[110] = constant104;
  array0[111] = constant104;
  array0[112] = constant104;
  array0[113] = constant104;
  array0[114] = constant52;
  array0[115] = constant105;
  array0[116] = constant1;
  array0[117] = constant106;
  array0[118] = constant107;
  array0[119] = constant108;
  array0[120] = constant109;
  array0[121] = constant110;
  array0[122] = TWO;
  array0[123] = TWO;
  array0[124] = TWO;
  array0[125] = TWO;
  array0[126] = TWO;
  array0[127] = TWO;
  array0[128] = TWO;
  array0[129] = TWO;
  array0[130] = TWO;
  array0[131] = TWO;
  array0[132] = TWO;
  array0[133] = constant111;
  array0[134] = ZERO;
  array0[135] = ZERO;
  array0[136] = constant112;
  array0[137] = constant113;
  array0[138] = constant114;
  array0[139] = constant115;
  array0[140] = FIVE;
  array0[141] = constant116;
  array0[142] = constant116;
  array0[143] = constant116;
  array0[144] = constant116;
  array0[145] = constant116;
  array0[146] = constant116;
  array0[147] = constant116;
  array0[148] = constant116;
  array0[149] = constant116;
  array0[150] = constant116;
  array0[151] = constant116;
  array0[152] = constant117;
  array0[153] = constant118;
  array0[154] = constant118;
  array0[155] = constant118;
  array0[156] = constant119;
  array0[157] = constant119;
  array0[158] = constant119;
  array0[159] = constant119;
  array0[160] = constant119;
  array0[161] = constant119;
  array0[162] = constant120;
  array0[163] = constant120;
  array0[164] = constant120;
  array0[165] = constant1;
  array0[166] = constant1;
  array0[167] = constant120;
  array0[168] = constant120;
  array0[169] = constant120;
  array0[170] = constant120;
  ExcelValue array0_ev = EXCEL_RANGE(array0,9,19);
  result = array0_ev;
  variable_set[3413] = 1;
  return result;
}

ExcelValue output_electricity_generation_type() {
  static ExcelValue result;
  if(variable_set[3414] == 1) { return result;}
  static ExcelValue array0[190];
  array0[0] = constant59;
  array0[1] = constant121;
  array0[2] = constant122;
  array0[3] = constant123;
  array0[4] = constant124;
  array0[5] = constant125;
  array0[6] = constant126;
  array0[7] = constant127;
  array0[8] = constant128;
  array0[9] = constant129;
  array0[10] = constant130;
  array0[11] = constant131;
  array0[12] = constant132;
  array0[13] = constant133;
  array0[14] = constant134;
  array0[15] = constant135;
  array0[16] = constant136;
  array0[17] = constant137;
  array0[18] = constant138;
  array0[19] = constant50;
  array0[20] = constant139;
  array0[21] = constant140;
  array0[22] = constant141;
  array0[23] = constant142;
  array0[24] = constant143;
  array0[25] = constant144;
  array0[26] = constant145;
  array0[27] = constant146;
  array0[28] = constant147;
  array0[29] = constant148;
  array0[30] = constant149;
  array0[31] = constant150;
  array0[32] = constant151;
  array0[33] = constant152;
  array0[34] = constant153;
  array0[35] = constant154;
  array0[36] = constant155;
  array0[37] = constant156;
  array0[38] = constant84;
  array0[39] = ZERO;
  array0[40] = ZERO;
  array0[41] = constant157;
  array0[42] = constant158;
  array0[43] = constant159;
  array0[44] = constant160;
  array0[45] = constant161;
  array0[46] = constant162;
  array0[47] = constant163;
  array0[48] = constant164;
  array0[49] = constant165;
  array0[50] = constant166;
  array0[51] = constant167;
  array0[52] = constant168;
  array0[53] = constant169;
  array0[54] = constant170;
  array0[55] = constant171;
  array0[56] = constant172;
  array0[57] = constant91;
  array0[58] = ZERO;
  array0[59] = ZERO;
  array0[60] = constant157;
  array0[61] = constant158;
  array0[62] = constant173;
  array0[63] = constant174;
  array0[64] = constant175;
  array0[65] = constant176;
  array0[66] = constant177;
  array0[67] = constant178;
  array0[68] = constant179;
  array0[69] = constant180;
  array0[70] = constant181;
  array0[71] = constant182;
  array0[72] = constant183;
  array0[73] = constant184;
  array0[74] = constant185;
  array0[75] = constant186;
  array0[76] = constant92;
  array0[77] = constant187;
  array0[78] = constant188;
  array0[79] = constant189;
  array0[80] = constant190;
  array0[81] = constant191;
  array0[82] = constant192;
  array0[83] = constant193;
  array0[84] = constant194;
  array0[85] = constant195;
  array0[86] = constant196;
  array0[87] = constant197;
  array0[88] = constant198;
  array0[89] = constant199;
  array0[90] = constant200;
  array0[91] = constant201;
  array0[92] = constant202;
  array0[93] = constant203;
  array0[94] = constant204;
  array0[95] = constant99;
  array0[96] = constant205;
  array0[97] = constant206;
  array0[98] = constant207;
  array0[99] = constant208;
  array0[100] = constant209;
  array0[101] = constant210;
  array0[102] = constant211;
  array0[103] = constant212;
  array0[104] = constant213;
  array0[105] = constant214;
  array0[106] = constant215;
  array0[107] = constant216;
  array0[108] = constant217;
  array0[109] = constant218;
  array0[110] = constant219;
  array0[111] = constant220;
  array0[112] = constant221;
  array0[113] = constant222;
  array0[114] = constant52;
  array0[115] = constant223;
  array0[116] = constant224;
  array0[117] = constant225;
  array0[118] = constant226;
  array0[119] = constant227;
  array0[120] = constant228;
  array0[121] = constant229;
  array0[122] = constant230;
  array0[123] = constant231;
  array0[124] = constant232;
  array0[125] = constant233;
  array0[126] = constant234;
  array0[127] = constant235;
  array0[128] = constant236;
  array0[129] = constant237;
  array0[130] = constant238;
  array0[131] = constant239;
  array0[132] = constant240;
  array0[133] = constant111;
  array0[134] = ZERO;
  array0[135] = ZERO;
  array0[136] = constant241;
  array0[137] = constant158;
  array0[138] = constant242;
  array0[139] = constant160;
  array0[140] = constant161;
  array0[141] = constant243;
  array0[142] = constant244;
  array0[143] = constant245;
  array0[144] = constant165;
  array0[145] = constant166;
  array0[146] = constant167;
  array0[147] = constant246;
  array0[148] = constant169;
  array0[149] = constant247;
  array0[150] = constant248;
  array0[151] = constant249;
  array0[152] = constant117;
  array0[153] = constant250;
  array0[154] = constant251;
  array0[155] = constant252;
  array0[156] = constant253;
  array0[157] = constant254;
  array0[158] = constant255;
  array0[159] = constant256;
  array0[160] = constant257;
  array0[161] = constant258;
  array0[162] = constant259;
  array0[163] = constant260;
  array0[164] = constant261;
  array0[165] = constant262;
  array0[166] = constant263;
  array0[167] = constant264;
  array0[168] = constant265;
  array0[169] = constant266;
  array0[170] = constant267;
  array0[171] = constant268;
  array0[172] = constant269;
  array0[173] = constant270;
  array0[174] = constant271;
  array0[175] = constant272;
  array0[176] = constant273;
  array0[177] = constant274;
  array0[178] = ZERO;
  array0[179] = ZERO;
  array0[180] = ZERO;
  array0[181] = ZERO;
  array0[182] = ZERO;
  array0[183] = ZERO;
  array0[184] = ZERO;
  array0[185] = ZERO;
  array0[186] = ZERO;
  array0[187] = ZERO;
  array0[188] = ZERO;
  array0[189] = ZERO;
  ExcelValue array0_ev = EXCEL_RANGE(array0,10,19);
  result = array0_ev;
  variable_set[3414] = 1;
  return result;
}

ExcelValue output_emissions_by_sector() {
  static ExcelValue result;
  if(variable_set[3415] == 1) { return result;}
  static ExcelValue array0[2432];
  array0[0] = constant275;
  array0[1] = ZERO;
  array0[2] = ZERO;
  array0[3] = ZERO;
  array0[4] = ZERO;
  array0[5] = ZERO;
  array0[6] = ZERO;
  array0[7] = ZERO;
  array0[8] = ZERO;
  array0[9] = ZERO;
  array0[10] = ZERO;
  array0[11] = ZERO;
  array0[12] = ZERO;
  array0[13] = ZERO;
  array0[14] = ZERO;
  array0[15] = ZERO;
  array0[16] = ZERO;
  array0[17] = ZERO;
  array0[18] = ZERO;
  array0[19] = constant276;
  array0[20] = ZERO;
  array0[21] = ZERO;
  array0[22] = ZERO;
  array0[23] = ZERO;
  array0[24] = ZERO;
  array0[25] = ZERO;
  array0[26] = ZERO;
  array0[27] = ZERO;
  array0[28] = ZERO;
  array0[29] = ZERO;
  array0[30] = ZERO;
  array0[31] = ZERO;
  array0[32] = ZERO;
  array0[33] = ZERO;
  array0[34] = ZERO;
  array0[35] = ZERO;
  array0[36] = ZERO;
  array0[37] = ZERO;
  array0[38] = constant277;
  array0[39] = ZERO;
  array0[40] = ZERO;
  array0[41] = ZERO;
  array0[42] = ZERO;
  array0[43] = ZERO;
  array0[44] = ZERO;
  array0[45] = ZERO;
  array0[46] = ZERO;
  array0[47] = ZERO;
  array0[48] = ZERO;
  array0[49] = ZERO;
  array0[50] = ZERO;
  array0[51] = ZERO;
  array0[52] = ZERO;
  array0[53] = ZERO;
  array0[54] = ZERO;
  array0[55] = ZERO;
  array0[56] = ZERO;
  array0[57] = constant278;
  array0[58] = ZERO;
  array0[59] = ZERO;
  array0[60] = constant279;
  array0[61] = constant280;
  array0[62] = constant281;
  array0[63] = constant282;
  array0[64] = constant283;
  array0[65] = constant284;
  array0[66] = constant285;
  array0[67] = constant286;
  array0[68] = constant287;
  array0[69] = constant288;
  array0[70] = constant289;
  array0[71] = constant290;
  array0[72] = constant291;
  array0[73] = constant292;
  array0[74] = constant293;
  array0[75] = constant294;
  array0[76] = constant295;
  array0[77] = constant296;
  array0[78] = constant297;
  array0[79] = constant298;
  array0[80] = constant299;
  array0[81] = constant300;
  array0[82] = constant301;
  array0[83] = constant302;
  array0[84] = constant303;
  array0[85] = constant304;
  array0[86] = constant305;
  array0[87] = constant306;
  array0[88] = constant307;
  array0[89] = constant308;
  array0[90] = constant309;
  array0[91] = constant310;
  array0[92] = constant311;
  array0[93] = constant312;
  array0[94] = constant313;
  array0[95] = constant314;
  array0[96] = constant315;
  array0[97] = constant316;
  array0[98] = constant317;
  array0[99] = constant318;
  array0[100] = constant319;
  array0[101] = constant320;
  array0[102] = constant321;
  array0[103] = constant322;
  array0[104] = constant323;
  array0[105] = constant324;
  array0[106] = constant325;
  array0[107] = constant326;
  array0[108] = constant327;
  array0[109] = constant328;
  array0[110] = constant329;
  array0[111] = constant330;
  array0[112] = constant331;
  array0[113] = constant332;
  array0[114] = constant333;
  array0[115] = constant334;
  array0[116] = constant335;
  array0[117] = constant336;
  array0[118] = constant337;
  array0[119] = constant338;
  array0[120] = constant339;
  array0[121] = constant340;
  array0[122] = constant341;
  array0[123] = constant342;
  array0[124] = constant343;
  array0[125] = constant344;
  array0[126] = constant345;
  array0[127] = constant346;
  array0[128] = constant347;
  array0[129] = constant348;
  array0[130] = constant349;
  array0[131] = constant350;
  array0[132] = constant351;
  array0[133] = constant352;
  array0[134] = ZERO;
  array0[135] = ZERO;
  array0[136] = ZERO;
  array0[137] = ZERO;
  array0[138] = ZERO;
  array0[139] = ZERO;
  array0[140] = ZERO;
  array0[141] = ZERO;
  array0[142] = ZERO;
  array0[143] = ZERO;
  array0[144] = ZERO;
  array0[145] = ZERO;
  array0[146] = ZERO;
  array0[147] = ZERO;
  array0[148] = ZERO;
  array0[149] = ZERO;
  array0[150] = ZERO;
  array0[151] = ZERO;
  array0[152] = constant353;
  array0[153] = ZERO;
  array0[154] = ZERO;
  array0[155] = ZERO;
  array0[156] = ZERO;
  array0[157] = ZERO;
  array0[158] = ZERO;
  array0[159] = ZERO;
  array0[160] = ZERO;
  array0[161] = ZERO;
  array0[162] = ZERO;
  array0[163] = ZERO;
  array0[164] = ZERO;
  array0[165] = ZERO;
  array0[166] = ZERO;
  array0[167] = ZERO;
  array0[168] = ZERO;
  array0[169] = ZERO;
  array0[170] = ZERO;
  array0[171] = constant354;
  array0[172] = ZERO;
  array0[173] = ZERO;
  array0[174] = ZERO;
  array0[175] = ZERO;
  array0[176] = ZERO;
  array0[177] = ZERO;
  array0[178] = ZERO;
  array0[179] = ZERO;
  array0[180] = ZERO;
  array0[181] = ZERO;
  array0[182] = ZERO;
  array0[183] = ZERO;
  array0[184] = ZERO;
  array0[185] = ZERO;
  array0[186] = ZERO;
  array0[187] = ZERO;
  array0[188] = ZERO;
  array0[189] = ZERO;
  array0[190] = constant355;
  array0[191] = ZERO;
  array0[192] = ZERO;
  array0[193] = ZERO;
  array0[194] = ZERO;
  array0[195] = ZERO;
  array0[196] = ZERO;
  array0[197] = ZERO;
  array0[198] = ZERO;
  array0[199] = ZERO;
  array0[200] = ZERO;
  array0[201] = ZERO;
  array0[202] = ZERO;
  array0[203] = ZERO;
  array0[204] = ZERO;
  array0[205] = ZERO;
  array0[206] = ZERO;
  array0[207] = ZERO;
  array0[208] = ZERO;
  array0[209] = constant356;
  array0[210] = ZERO;
  array0[211] = ZERO;
  array0[212] = constant357;
  array0[213] = constant358;
  array0[214] = constant359;
  array0[215] = constant360;
  array0[216] = constant361;
  array0[217] = constant362;
  array0[218] = constant363;
  array0[219] = constant364;
  array0[220] = constant365;
  array0[221] = constant366;
  array0[222] = constant367;
  array0[223] = constant368;
  array0[224] = constant369;
  array0[225] = constant370;
  array0[226] = constant371;
  array0[227] = constant372;
  array0[228] = constant373;
  array0[229] = constant374;
  array0[230] = constant375;
  array0[231] = constant376;
  array0[232] = constant377;
  array0[233] = constant378;
  array0[234] = constant379;
  array0[235] = constant380;
  array0[236] = constant381;
  array0[237] = constant382;
  array0[238] = constant383;
  array0[239] = constant384;
  array0[240] = constant385;
  array0[241] = constant386;
  array0[242] = constant387;
  array0[243] = constant388;
  array0[244] = constant389;
  array0[245] = constant390;
  array0[246] = constant391;
  array0[247] = constant392;
  array0[248] = constant393;
  array0[249] = constant394;
  array0[250] = constant395;
  array0[251] = constant396;
  array0[252] = constant397;
  array0[253] = constant398;
  array0[254] = constant399;
  array0[255] = constant400;
  array0[256] = constant401;
  array0[257] = constant402;
  array0[258] = constant403;
  array0[259] = constant404;
  array0[260] = constant405;
  array0[261] = constant406;
  array0[262] = constant407;
  array0[263] = constant408;
  array0[264] = constant409;
  array0[265] = constant410;
  array0[266] = constant411;
  array0[267] = constant412;
  array0[268] = constant413;
  array0[269] = constant414;
  array0[270] = constant415;
  array0[271] = constant416;
  array0[272] = constant417;
  array0[273] = constant418;
  array0[274] = constant419;
  array0[275] = constant420;
  array0[276] = constant421;
  array0[277] = constant422;
  array0[278] = constant423;
  array0[279] = constant424;
  array0[280] = constant425;
  array0[281] = constant426;
  array0[282] = constant427;
  array0[283] = constant428;
  array0[284] = constant429;
  array0[285] = constant430;
  array0[286] = ZERO;
  array0[287] = ZERO;
  array0[288] = ZERO;
  array0[289] = ZERO;
  array0[290] = ZERO;
  array0[291] = ZERO;
  array0[292] = ZERO;
  array0[293] = ZERO;
  array0[294] = ZERO;
  array0[295] = ZERO;
  array0[296] = ZERO;
  array0[297] = ZERO;
  array0[298] = ZERO;
  array0[299] = ZERO;
  array0[300] = ZERO;
  array0[301] = ZERO;
  array0[302] = ZERO;
  array0[303] = ZERO;
  array0[304] = constant431;
  array0[305] = constant432;
  array0[306] = constant433;
  array0[307] = constant434;
  array0[308] = constant435;
  array0[309] = constant436;
  array0[310] = constant437;
  array0[311] = constant438;
  array0[312] = constant439;
  array0[313] = constant440;
  array0[314] = constant441;
  array0[315] = constant442;
  array0[316] = constant443;
  array0[317] = constant444;
  array0[318] = constant445;
  array0[319] = constant446;
  array0[320] = constant447;
  array0[321] = constant448;
  array0[322] = constant449;
  array0[323] = constant450;
  array0[324] = ZERO;
  array0[325] = ZERO;
  array0[326] = ZERO;
  array0[327] = ZERO;
  array0[328] = ZERO;
  array0[329] = ZERO;
  array0[330] = ZERO;
  array0[331] = ZERO;
  array0[332] = ZERO;
  array0[333] = ZERO;
  array0[334] = ZERO;
  array0[335] = ZERO;
  array0[336] = ZERO;
  array0[337] = ZERO;
  array0[338] = ZERO;
  array0[339] = ZERO;
  array0[340] = ZERO;
  array0[341] = ZERO;
  array0[342] = constant451;
  array0[343] = constant452;
  array0[344] = constant453;
  array0[345] = constant454;
  array0[346] = constant455;
  array0[347] = constant456;
  array0[348] = constant457;
  array0[349] = constant458;
  array0[350] = constant459;
  array0[351] = constant460;
  array0[352] = constant461;
  array0[353] = constant462;
  array0[354] = constant463;
  array0[355] = constant464;
  array0[356] = constant465;
  array0[357] = constant466;
  array0[358] = constant467;
  array0[359] = constant468;
  array0[360] = constant469;
  array0[361] = constant470;
  array0[362] = ZERO;
  array0[363] = ZERO;
  array0[364] = ZERO;
  array0[365] = ZERO;
  array0[366] = ZERO;
  array0[367] = ZERO;
  array0[368] = ZERO;
  array0[369] = ZERO;
  array0[370] = ZERO;
  array0[371] = ZERO;
  array0[372] = ZERO;
  array0[373] = ZERO;
  array0[374] = ZERO;
  array0[375] = ZERO;
  array0[376] = ZERO;
  array0[377] = ZERO;
  array0[378] = ZERO;
  array0[379] = ZERO;
  array0[380] = constant471;
  array0[381] = ZERO;
  array0[382] = ZERO;
  array0[383] = ZERO;
  array0[384] = ZERO;
  array0[385] = ZERO;
  array0[386] = ZERO;
  array0[387] = ZERO;
  array0[388] = ZERO;
  array0[389] = ZERO;
  array0[390] = ZERO;
  array0[391] = ZERO;
  array0[392] = ZERO;
  array0[393] = ZERO;
  array0[394] = ZERO;
  array0[395] = ZERO;
  array0[396] = ZERO;
  array0[397] = ZERO;
  array0[398] = ZERO;
  array0[399] = constant472;
  array0[400] = ZERO;
  array0[401] = ZERO;
  array0[402] = ZERO;
  array0[403] = ZERO;
  array0[404] = ZERO;
  array0[405] = ZERO;
  array0[406] = ZERO;
  array0[407] = ZERO;
  array0[408] = ZERO;
  array0[409] = ZERO;
  array0[410] = ZERO;
  array0[411] = ZERO;
  array0[412] = ZERO;
  array0[413] = ZERO;
  array0[414] = ZERO;
  array0[415] = ZERO;
  array0[416] = ZERO;
  array0[417] = ZERO;
  array0[418] = constant473;
  array0[419] = constant474;
  array0[420] = constant475;
  array0[421] = constant476;
  array0[422] = constant477;
  array0[423] = constant478;
  array0[424] = constant478;
  array0[425] = constant478;
  array0[426] = constant478;
  array0[427] = constant478;
  array0[428] = constant478;
  array0[429] = constant478;
  array0[430] = constant478;
  array0[431] = constant478;
  array0[432] = constant478;
  array0[433] = constant478;
  array0[434] = constant478;
  array0[435] = constant478;
  array0[436] = constant478;
  array0[437] = constant479;
  array0[438] = constant480;
  array0[439] = constant481;
  array0[440] = constant482;
  array0[441] = constant483;
  array0[442] = constant484;
  array0[443] = constant484;
  array0[444] = constant484;
  array0[445] = constant484;
  array0[446] = constant484;
  array0[447] = constant484;
  array0[448] = constant484;
  array0[449] = constant484;
  array0[450] = constant484;
  array0[451] = constant484;
  array0[452] = constant484;
  array0[453] = constant484;
  array0[454] = constant484;
  array0[455] = constant484;
  array0[456] = constant485;
  array0[457] = ZERO;
  array0[458] = ZERO;
  array0[459] = ZERO;
  array0[460] = ZERO;
  array0[461] = ZERO;
  array0[462] = ZERO;
  array0[463] = ZERO;
  array0[464] = ZERO;
  array0[465] = ZERO;
  array0[466] = ZERO;
  array0[467] = ZERO;
  array0[468] = ZERO;
  array0[469] = ZERO;
  array0[470] = ZERO;
  array0[471] = ZERO;
  array0[472] = ZERO;
  array0[473] = ZERO;
  array0[474] = ZERO;
  array0[475] = constant486;
  array0[476] = ZERO;
  array0[477] = ZERO;
  array0[478] = ZERO;
  array0[479] = ZERO;
  array0[480] = ZERO;
  array0[481] = ZERO;
  array0[482] = ZERO;
  array0[483] = ZERO;
  array0[484] = ZERO;
  array0[485] = ZERO;
  array0[486] = ZERO;
  array0[487] = ZERO;
  array0[488] = ZERO;
  array0[489] = ZERO;
  array0[490] = ZERO;
  array0[491] = ZERO;
  array0[492] = ZERO;
  array0[493] = ZERO;
  array0[494] = constant487;
  array0[495] = ZERO;
  array0[496] = ZERO;
  array0[497] = ZERO;
  array0[498] = ZERO;
  array0[499] = ZERO;
  array0[500] = ZERO;
  array0[501] = ZERO;
  array0[502] = ZERO;
  array0[503] = ZERO;
  array0[504] = ZERO;
  array0[505] = ZERO;
  array0[506] = ZERO;
  array0[507] = ZERO;
  array0[508] = ZERO;
  array0[509] = ZERO;
  array0[510] = ZERO;
  array0[511] = ZERO;
  array0[512] = ZERO;
  array0[513] = constant488;
  array0[514] = ZERO;
  array0[515] = ZERO;
  array0[516] = ZERO;
  array0[517] = ZERO;
  array0[518] = ZERO;
  array0[519] = ZERO;
  array0[520] = ZERO;
  array0[521] = ZERO;
  array0[522] = ZERO;
  array0[523] = ZERO;
  array0[524] = ZERO;
  array0[525] = ZERO;
  array0[526] = ZERO;
  array0[527] = ZERO;
  array0[528] = ZERO;
  array0[529] = ZERO;
  array0[530] = ZERO;
  array0[531] = ZERO;
  array0[532] = constant489;
  array0[533] = ZERO;
  array0[534] = ZERO;
  array0[535] = ZERO;
  array0[536] = ZERO;
  array0[537] = ZERO;
  array0[538] = ZERO;
  array0[539] = ZERO;
  array0[540] = ZERO;
  array0[541] = ZERO;
  array0[542] = ZERO;
  array0[543] = ZERO;
  array0[544] = ZERO;
  array0[545] = ZERO;
  array0[546] = ZERO;
  array0[547] = ZERO;
  array0[548] = ZERO;
  array0[549] = ZERO;
  array0[550] = ZERO;
  array0[551] = constant490;
  array0[552] = constant491;
  array0[553] = constant491;
  array0[554] = constant492;
  array0[555] = constant493;
  array0[556] = constant494;
  array0[557] = constant495;
  array0[558] = ZERO;
  array0[559] = ZERO;
  array0[560] = constant496;
  array0[561] = constant497;
  array0[562] = constant496;
  array0[563] = constant497;
  array0[564] = ZERO;
  array0[565] = constant496;
  array0[566] = ZERO;
  array0[567] = constant496;
  array0[568] = ZERO;
  array0[569] = ZERO;
  array0[570] = constant498;
  array0[571] = constant499;
  array0[572] = constant500;
  array0[573] = constant501;
  array0[574] = ZERO;
  array0[575] = ZERO;
  array0[576] = ZERO;
  array0[577] = ZERO;
  array0[578] = ZERO;
  array0[579] = ZERO;
  array0[580] = ZERO;
  array0[581] = ZERO;
  array0[582] = ZERO;
  array0[583] = ZERO;
  array0[584] = ZERO;
  array0[585] = ZERO;
  array0[586] = ZERO;
  array0[587] = ZERO;
  array0[588] = ZERO;
  array0[589] = constant502;
  array0[590] = constant503;
  array0[591] = constant504;
  array0[592] = constant505;
  array0[593] = constant506;
  array0[594] = constant507;
  array0[595] = constant508;
  array0[596] = constant509;
  array0[597] = constant510;
  array0[598] = constant511;
  array0[599] = constant512;
  array0[600] = constant513;
  array0[601] = constant514;
  array0[602] = constant515;
  array0[603] = constant516;
  array0[604] = constant517;
  array0[605] = constant518;
  array0[606] = constant519;
  array0[607] = constant520;
  array0[608] = constant521;
  array0[609] = constant522;
  array0[610] = constant523;
  array0[611] = constant524;
  array0[612] = constant525;
  array0[613] = constant526;
  array0[614] = constant527;
  array0[615] = constant528;
  array0[616] = constant529;
  array0[617] = constant530;
  array0[618] = constant531;
  array0[619] = constant532;
  array0[620] = constant533;
  array0[621] = constant534;
  array0[622] = constant535;
  array0[623] = constant536;
  array0[624] = constant537;
  array0[625] = constant538;
  array0[626] = constant539;
  array0[627] = constant540;
  array0[628] = ZERO;
  array0[629] = ZERO;
  array0[630] = ZERO;
  array0[631] = ZERO;
  array0[632] = ZERO;
  array0[633] = ZERO;
  array0[634] = ZERO;
  array0[635] = ZERO;
  array0[636] = ZERO;
  array0[637] = ZERO;
  array0[638] = ZERO;
  array0[639] = ZERO;
  array0[640] = ZERO;
  array0[641] = ZERO;
  array0[642] = ZERO;
  array0[643] = ZERO;
  array0[644] = ZERO;
  array0[645] = ZERO;
  array0[646] = constant541;
  array0[647] = ZERO;
  array0[648] = ZERO;
  array0[649] = ZERO;
  array0[650] = ZERO;
  array0[651] = ZERO;
  array0[652] = ZERO;
  array0[653] = ZERO;
  array0[654] = ZERO;
  array0[655] = ZERO;
  array0[656] = ZERO;
  array0[657] = ZERO;
  array0[658] = ZERO;
  array0[659] = ZERO;
  array0[660] = ZERO;
  array0[661] = ZERO;
  array0[662] = ZERO;
  array0[663] = ZERO;
  array0[664] = ZERO;
  array0[665] = constant542;
  array0[666] = constant543;
  array0[667] = constant544;
  array0[668] = constant545;
  array0[669] = constant546;
  array0[670] = constant547;
  array0[671] = constant548;
  array0[672] = constant549;
  array0[673] = ZERO;
  array0[674] = ZERO;
  array0[675] = ZERO;
  array0[676] = ZERO;
  array0[677] = ZERO;
  array0[678] = ZERO;
  array0[679] = ZERO;
  array0[680] = ZERO;
  array0[681] = ZERO;
  array0[682] = ZERO;
  array0[683] = ZERO;
  array0[684] = constant550;
  array0[685] = constant551;
  array0[686] = constant552;
  array0[687] = constant553;
  array0[688] = constant554;
  array0[689] = ZERO;
  array0[690] = ZERO;
  array0[691] = ZERO;
  array0[692] = ZERO;
  array0[693] = ZERO;
  array0[694] = ZERO;
  array0[695] = ZERO;
  array0[696] = ZERO;
  array0[697] = ZERO;
  array0[698] = ZERO;
  array0[699] = ZERO;
  array0[700] = ZERO;
  array0[701] = ZERO;
  array0[702] = ZERO;
  array0[703] = constant555;
  array0[704] = ZERO;
  array0[705] = ZERO;
  array0[706] = ZERO;
  array0[707] = ZERO;
  array0[708] = ZERO;
  array0[709] = ZERO;
  array0[710] = ZERO;
  array0[711] = ZERO;
  array0[712] = ZERO;
  array0[713] = ZERO;
  array0[714] = ZERO;
  array0[715] = ZERO;
  array0[716] = ZERO;
  array0[717] = ZERO;
  array0[718] = ZERO;
  array0[719] = ZERO;
  array0[720] = ZERO;
  array0[721] = ZERO;
  array0[722] = constant556;
  array0[723] = ZERO;
  array0[724] = ZERO;
  array0[725] = ZERO;
  array0[726] = ZERO;
  array0[727] = ZERO;
  array0[728] = ZERO;
  array0[729] = ZERO;
  array0[730] = ZERO;
  array0[731] = ZERO;
  array0[732] = ZERO;
  array0[733] = ZERO;
  array0[734] = ZERO;
  array0[735] = ZERO;
  array0[736] = ZERO;
  array0[737] = ZERO;
  array0[738] = ZERO;
  array0[739] = ZERO;
  array0[740] = ZERO;
  array0[741] = constant557;
  array0[742] = constant558;
  array0[743] = constant559;
  array0[744] = constant560;
  array0[745] = constant561;
  array0[746] = constant562;
  array0[747] = constant563;
  array0[748] = constant564;
  array0[749] = ZERO;
  array0[750] = ZERO;
  array0[751] = ZERO;
  array0[752] = ZERO;
  array0[753] = ZERO;
  array0[754] = ZERO;
  array0[755] = ZERO;
  array0[756] = ZERO;
  array0[757] = ZERO;
  array0[758] = ZERO;
  array0[759] = ZERO;
  array0[760] = constant565;
  array0[761] = constant566;
  array0[762] = constant567;
  array0[763] = constant568;
  array0[764] = constant569;
  array0[765] = ZERO;
  array0[766] = ZERO;
  array0[767] = ZERO;
  array0[768] = ZERO;
  array0[769] = ZERO;
  array0[770] = ZERO;
  array0[771] = ZERO;
  array0[772] = ZERO;
  array0[773] = ZERO;
  array0[774] = ZERO;
  array0[775] = ZERO;
  array0[776] = ZERO;
  array0[777] = ZERO;
  array0[778] = ZERO;
  array0[779] = constant570;
  array0[780] = ZERO;
  array0[781] = ZERO;
  array0[782] = ZERO;
  array0[783] = ZERO;
  array0[784] = ZERO;
  array0[785] = ZERO;
  array0[786] = ZERO;
  array0[787] = ZERO;
  array0[788] = ZERO;
  array0[789] = ZERO;
  array0[790] = ZERO;
  array0[791] = ZERO;
  array0[792] = ZERO;
  array0[793] = ZERO;
  array0[794] = ZERO;
  array0[795] = ZERO;
  array0[796] = ZERO;
  array0[797] = ZERO;
  array0[798] = constant571;
  array0[799] = ZERO;
  array0[800] = ZERO;
  array0[801] = ZERO;
  array0[802] = ZERO;
  array0[803] = ZERO;
  array0[804] = ZERO;
  array0[805] = ZERO;
  array0[806] = ZERO;
  array0[807] = ZERO;
  array0[808] = ZERO;
  array0[809] = ZERO;
  array0[810] = ZERO;
  array0[811] = ZERO;
  array0[812] = ZERO;
  array0[813] = ZERO;
  array0[814] = ZERO;
  array0[815] = ZERO;
  array0[816] = ZERO;
  array0[817] = constant572;
  array0[818] = constant573;
  array0[819] = constant574;
  array0[820] = constant575;
  array0[821] = constant576;
  array0[822] = constant577;
  array0[823] = constant578;
  array0[824] = constant579;
  array0[825] = ZERO;
  array0[826] = ZERO;
  array0[827] = ZERO;
  array0[828] = ZERO;
  array0[829] = ZERO;
  array0[830] = ZERO;
  array0[831] = ZERO;
  array0[832] = ZERO;
  array0[833] = ZERO;
  array0[834] = ZERO;
  array0[835] = ZERO;
  array0[836] = constant580;
  array0[837] = constant581;
  array0[838] = constant582;
  array0[839] = constant583;
  array0[840] = constant584;
  array0[841] = ZERO;
  array0[842] = ZERO;
  array0[843] = ZERO;
  array0[844] = ZERO;
  array0[845] = ZERO;
  array0[846] = ZERO;
  array0[847] = ZERO;
  array0[848] = ZERO;
  array0[849] = ZERO;
  array0[850] = ZERO;
  array0[851] = ZERO;
  array0[852] = ZERO;
  array0[853] = ZERO;
  array0[854] = ZERO;
  array0[855] = constant585;
  array0[856] = ZERO;
  array0[857] = ZERO;
  array0[858] = ZERO;
  array0[859] = ZERO;
  array0[860] = ZERO;
  array0[861] = ZERO;
  array0[862] = ZERO;
  array0[863] = ZERO;
  array0[864] = ZERO;
  array0[865] = ZERO;
  array0[866] = ZERO;
  array0[867] = ZERO;
  array0[868] = ZERO;
  array0[869] = ZERO;
  array0[870] = ZERO;
  array0[871] = ZERO;
  array0[872] = ZERO;
  array0[873] = ZERO;
  array0[874] = constant586;
  array0[875] = ZERO;
  array0[876] = ZERO;
  array0[877] = ZERO;
  array0[878] = ZERO;
  array0[879] = ZERO;
  array0[880] = ZERO;
  array0[881] = ZERO;
  array0[882] = ZERO;
  array0[883] = ZERO;
  array0[884] = ZERO;
  array0[885] = ZERO;
  array0[886] = ZERO;
  array0[887] = ZERO;
  array0[888] = ZERO;
  array0[889] = ZERO;
  array0[890] = ZERO;
  array0[891] = ZERO;
  array0[892] = ZERO;
  array0[893] = constant587;
  array0[894] = constant588;
  array0[895] = constant589;
  array0[896] = constant590;
  array0[897] = constant591;
  array0[898] = constant592;
  array0[899] = constant593;
  array0[900] = constant594;
  array0[901] = ZERO;
  array0[902] = ZERO;
  array0[903] = ZERO;
  array0[904] = ZERO;
  array0[905] = ZERO;
  array0[906] = ZERO;
  array0[907] = ZERO;
  array0[908] = ZERO;
  array0[909] = ZERO;
  array0[910] = ZERO;
  array0[911] = ZERO;
  array0[912] = constant595;
  array0[913] = constant596;
  array0[914] = constant597;
  array0[915] = constant598;
  array0[916] = constant599;
  array0[917] = constant600;
  array0[918] = ZERO;
  array0[919] = ZERO;
  array0[920] = ZERO;
  array0[921] = ZERO;
  array0[922] = ZERO;
  array0[923] = ZERO;
  array0[924] = ZERO;
  array0[925] = ZERO;
  array0[926] = ZERO;
  array0[927] = ZERO;
  array0[928] = ZERO;
  array0[929] = ZERO;
  array0[930] = ZERO;
  array0[931] = constant601;
  array0[932] = ZERO;
  array0[933] = ZERO;
  array0[934] = ZERO;
  array0[935] = ZERO;
  array0[936] = ZERO;
  array0[937] = ZERO;
  array0[938] = ZERO;
  array0[939] = ZERO;
  array0[940] = ZERO;
  array0[941] = ZERO;
  array0[942] = ZERO;
  array0[943] = ZERO;
  array0[944] = ZERO;
  array0[945] = ZERO;
  array0[946] = ZERO;
  array0[947] = ZERO;
  array0[948] = ZERO;
  array0[949] = ZERO;
  array0[950] = constant602;
  array0[951] = ZERO;
  array0[952] = ZERO;
  array0[953] = ZERO;
  array0[954] = ZERO;
  array0[955] = ZERO;
  array0[956] = ZERO;
  array0[957] = ZERO;
  array0[958] = ZERO;
  array0[959] = ZERO;
  array0[960] = ZERO;
  array0[961] = ZERO;
  array0[962] = ZERO;
  array0[963] = ZERO;
  array0[964] = ZERO;
  array0[965] = ZERO;
  array0[966] = ZERO;
  array0[967] = ZERO;
  array0[968] = ZERO;
  array0[969] = constant603;
  array0[970] = constant604;
  array0[971] = constant605;
  array0[972] = constant606;
  array0[973] = constant607;
  array0[974] = constant608;
  array0[975] = constant609;
  array0[976] = constant610;
  array0[977] = ZERO;
  array0[978] = ZERO;
  array0[979] = ZERO;
  array0[980] = ZERO;
  array0[981] = ZERO;
  array0[982] = ZERO;
  array0[983] = ZERO;
  array0[984] = ZERO;
  array0[985] = ZERO;
  array0[986] = ZERO;
  array0[987] = ZERO;
  array0[988] = constant611;
  array0[989] = constant612;
  array0[990] = constant613;
  array0[991] = constant614;
  array0[992] = constant615;
  array0[993] = constant616;
  array0[994] = ZERO;
  array0[995] = ZERO;
  array0[996] = ZERO;
  array0[997] = ZERO;
  array0[998] = ZERO;
  array0[999] = ZERO;
  array0[1000] = ZERO;
  array0[1001] = ZERO;
  array0[1002] = ZERO;
  array0[1003] = ZERO;
  array0[1004] = ZERO;
  array0[1005] = ZERO;
  array0[1006] = ZERO;
  array0[1007] = constant617;
  array0[1008] = ZERO;
  array0[1009] = ZERO;
  array0[1010] = ZERO;
  array0[1011] = ZERO;
  array0[1012] = ZERO;
  array0[1013] = ZERO;
  array0[1014] = ZERO;
  array0[1015] = ZERO;
  array0[1016] = ZERO;
  array0[1017] = ZERO;
  array0[1018] = ZERO;
  array0[1019] = ZERO;
  array0[1020] = ZERO;
  array0[1021] = ZERO;
  array0[1022] = ZERO;
  array0[1023] = ZERO;
  array0[1024] = ZERO;
  array0[1025] = ZERO;
  array0[1026] = constant618;
  array0[1027] = constant619;
  array0[1028] = constant620;
  array0[1029] = constant621;
  array0[1030] = constant622;
  array0[1031] = constant623;
  array0[1032] = constant624;
  array0[1033] = constant625;
  array0[1034] = constant626;
  array0[1035] = constant627;
  array0[1036] = constant628;
  array0[1037] = constant629;
  array0[1038] = constant630;
  array0[1039] = constant631;
  array0[1040] = constant632;
  array0[1041] = constant633;
  array0[1042] = constant634;
  array0[1043] = constant635;
  array0[1044] = constant636;
  array0[1045] = constant637;
  array0[1046] = ZERO;
  array0[1047] = ZERO;
  array0[1048] = ZERO;
  array0[1049] = ZERO;
  array0[1050] = ZERO;
  array0[1051] = ZERO;
  array0[1052] = ZERO;
  array0[1053] = ZERO;
  array0[1054] = ZERO;
  array0[1055] = ZERO;
  array0[1056] = ZERO;
  array0[1057] = ZERO;
  array0[1058] = ZERO;
  array0[1059] = ZERO;
  array0[1060] = ZERO;
  array0[1061] = ZERO;
  array0[1062] = ZERO;
  array0[1063] = ZERO;
  array0[1064] = constant638;
  array0[1065] = constant639;
  array0[1066] = constant640;
  array0[1067] = constant641;
  array0[1068] = constant642;
  array0[1069] = constant643;
  array0[1070] = constant644;
  array0[1071] = constant645;
  array0[1072] = constant646;
  array0[1073] = constant646;
  array0[1074] = constant646;
  array0[1075] = constant646;
  array0[1076] = constant646;
  array0[1077] = constant646;
  array0[1078] = constant646;
  array0[1079] = constant646;
  array0[1080] = constant646;
  array0[1081] = constant646;
  array0[1082] = constant646;
  array0[1083] = constant647;
  array0[1084] = constant648;
  array0[1085] = constant648;
  array0[1086] = constant649;
  array0[1087] = constant650;
  array0[1088] = constant651;
  array0[1089] = constant652;
  array0[1090] = constant653;
  array0[1091] = constant654;
  array0[1092] = constant654;
  array0[1093] = constant654;
  array0[1094] = constant654;
  array0[1095] = constant654;
  array0[1096] = constant654;
  array0[1097] = constant654;
  array0[1098] = constant654;
  array0[1099] = constant654;
  array0[1100] = constant654;
  array0[1101] = constant654;
  array0[1102] = constant655;
  array0[1103] = constant656;
  array0[1104] = constant657;
  array0[1105] = constant658;
  array0[1106] = constant659;
  array0[1107] = constant660;
  array0[1108] = constant661;
  array0[1109] = constant662;
  array0[1110] = constant663;
  array0[1111] = constant663;
  array0[1112] = constant663;
  array0[1113] = constant663;
  array0[1114] = constant663;
  array0[1115] = constant663;
  array0[1116] = constant663;
  array0[1117] = constant663;
  array0[1118] = constant663;
  array0[1119] = constant663;
  array0[1120] = constant663;
  array0[1121] = constant664;
  array0[1122] = constant665;
  array0[1123] = constant666;
  array0[1124] = constant667;
  array0[1125] = constant668;
  array0[1126] = constant669;
  array0[1127] = constant670;
  array0[1128] = constant671;
  array0[1129] = constant672;
  array0[1130] = constant672;
  array0[1131] = constant672;
  array0[1132] = constant672;
  array0[1133] = constant672;
  array0[1134] = constant672;
  array0[1135] = constant672;
  array0[1136] = constant672;
  array0[1137] = constant672;
  array0[1138] = constant672;
  array0[1139] = constant672;
  array0[1140] = constant673;
  array0[1141] = ZERO;
  array0[1142] = ZERO;
  array0[1143] = ZERO;
  array0[1144] = ZERO;
  array0[1145] = ZERO;
  array0[1146] = ZERO;
  array0[1147] = ZERO;
  array0[1148] = ZERO;
  array0[1149] = ZERO;
  array0[1150] = ZERO;
  array0[1151] = ZERO;
  array0[1152] = ZERO;
  array0[1153] = ZERO;
  array0[1154] = ZERO;
  array0[1155] = ZERO;
  array0[1156] = ZERO;
  array0[1157] = ZERO;
  array0[1158] = ZERO;
  array0[1159] = constant674;
  array0[1160] = ZERO;
  array0[1161] = ZERO;
  array0[1162] = ZERO;
  array0[1163] = ZERO;
  array0[1164] = ZERO;
  array0[1165] = ZERO;
  array0[1166] = ZERO;
  array0[1167] = ZERO;
  array0[1168] = ZERO;
  array0[1169] = ZERO;
  array0[1170] = ZERO;
  array0[1171] = ZERO;
  array0[1172] = ZERO;
  array0[1173] = ZERO;
  array0[1174] = ZERO;
  array0[1175] = ZERO;
  array0[1176] = ZERO;
  array0[1177] = ZERO;
  array0[1178] = constant675;
  array0[1179] = ZERO;
  array0[1180] = ZERO;
  array0[1181] = ZERO;
  array0[1182] = ZERO;
  array0[1183] = ZERO;
  array0[1184] = ZERO;
  array0[1185] = ZERO;
  array0[1186] = ZERO;
  array0[1187] = ZERO;
  array0[1188] = ZERO;
  array0[1189] = ZERO;
  array0[1190] = ZERO;
  array0[1191] = ZERO;
  array0[1192] = ZERO;
  array0[1193] = ZERO;
  array0[1194] = ZERO;
  array0[1195] = ZERO;
  array0[1196] = ZERO;
  array0[1197] = constant676;
  array0[1198] = ZERO;
  array0[1199] = ZERO;
  array0[1200] = ZERO;
  array0[1201] = ZERO;
  array0[1202] = ZERO;
  array0[1203] = ZERO;
  array0[1204] = ZERO;
  array0[1205] = ZERO;
  array0[1206] = ZERO;
  array0[1207] = ZERO;
  array0[1208] = ZERO;
  array0[1209] = ZERO;
  array0[1210] = ZERO;
  array0[1211] = ZERO;
  array0[1212] = ZERO;
  array0[1213] = ZERO;
  array0[1214] = ZERO;
  array0[1215] = ZERO;
  array0[1216] = constant677;
  array0[1217] = ZERO;
  array0[1218] = ZERO;
  array0[1219] = ZERO;
  array0[1220] = ZERO;
  array0[1221] = ZERO;
  array0[1222] = ZERO;
  array0[1223] = ZERO;
  array0[1224] = ZERO;
  array0[1225] = ZERO;
  array0[1226] = ZERO;
  array0[1227] = ZERO;
  array0[1228] = ZERO;
  array0[1229] = ZERO;
  array0[1230] = ZERO;
  array0[1231] = ZERO;
  array0[1232] = ZERO;
  array0[1233] = ZERO;
  array0[1234] = ZERO;
  array0[1235] = constant678;
  array0[1236] = ZERO;
  array0[1237] = ZERO;
  array0[1238] = ZERO;
  array0[1239] = ZERO;
  array0[1240] = ZERO;
  array0[1241] = ZERO;
  array0[1242] = ZERO;
  array0[1243] = ZERO;
  array0[1244] = ZERO;
  array0[1245] = ZERO;
  array0[1246] = ZERO;
  array0[1247] = ZERO;
  array0[1248] = ZERO;
  array0[1249] = ZERO;
  array0[1250] = ZERO;
  array0[1251] = ZERO;
  array0[1252] = ZERO;
  array0[1253] = ZERO;
  array0[1254] = constant679;
  array0[1255] = ZERO;
  array0[1256] = ZERO;
  array0[1257] = ZERO;
  array0[1258] = ZERO;
  array0[1259] = ZERO;
  array0[1260] = ZERO;
  array0[1261] = ZERO;
  array0[1262] = ZERO;
  array0[1263] = ZERO;
  array0[1264] = ZERO;
  array0[1265] = ZERO;
  array0[1266] = ZERO;
  array0[1267] = ZERO;
  array0[1268] = ZERO;
  array0[1269] = ZERO;
  array0[1270] = ZERO;
  array0[1271] = ZERO;
  array0[1272] = ZERO;
  array0[1273] = constant680;
  array0[1274] = ZERO;
  array0[1275] = ZERO;
  array0[1276] = ZERO;
  array0[1277] = ZERO;
  array0[1278] = ZERO;
  array0[1279] = ZERO;
  array0[1280] = ZERO;
  array0[1281] = ZERO;
  array0[1282] = ZERO;
  array0[1283] = ZERO;
  array0[1284] = ZERO;
  array0[1285] = ZERO;
  array0[1286] = ZERO;
  array0[1287] = ZERO;
  array0[1288] = ZERO;
  array0[1289] = ZERO;
  array0[1290] = ZERO;
  array0[1291] = ZERO;
  array0[1292] = constant681;
  array0[1293] = ZERO;
  array0[1294] = ZERO;
  array0[1295] = ZERO;
  array0[1296] = ZERO;
  array0[1297] = ZERO;
  array0[1298] = ZERO;
  array0[1299] = ZERO;
  array0[1300] = ZERO;
  array0[1301] = ZERO;
  array0[1302] = ZERO;
  array0[1303] = ZERO;
  array0[1304] = ZERO;
  array0[1305] = ZERO;
  array0[1306] = ZERO;
  array0[1307] = ZERO;
  array0[1308] = ZERO;
  array0[1309] = ZERO;
  array0[1310] = ZERO;
  array0[1311] = constant682;
  array0[1312] = ZERO;
  array0[1313] = ZERO;
  array0[1314] = ZERO;
  array0[1315] = ZERO;
  array0[1316] = ZERO;
  array0[1317] = ZERO;
  array0[1318] = ZERO;
  array0[1319] = ZERO;
  array0[1320] = ZERO;
  array0[1321] = ZERO;
  array0[1322] = ZERO;
  array0[1323] = ZERO;
  array0[1324] = ZERO;
  array0[1325] = ZERO;
  array0[1326] = ZERO;
  array0[1327] = ZERO;
  array0[1328] = ZERO;
  array0[1329] = ZERO;
  array0[1330] = constant683;
  array0[1331] = constant684;
  array0[1332] = constant685;
  array0[1333] = constant686;
  array0[1334] = constant687;
  array0[1335] = constant688;
  array0[1336] = constant689;
  array0[1337] = constant690;
  array0[1338] = constant691;
  array0[1339] = constant692;
  array0[1340] = constant693;
  array0[1341] = constant694;
  array0[1342] = constant695;
  array0[1343] = constant696;
  array0[1344] = constant697;
  array0[1345] = constant698;
  array0[1346] = constant699;
  array0[1347] = constant700;
  array0[1348] = constant701;
  array0[1349] = constant702;
  array0[1350] = ZERO;
  array0[1351] = ZERO;
  array0[1352] = ZERO;
  array0[1353] = ZERO;
  array0[1354] = ZERO;
  array0[1355] = ZERO;
  array0[1356] = ZERO;
  array0[1357] = ZERO;
  array0[1358] = ZERO;
  array0[1359] = ZERO;
  array0[1360] = ZERO;
  array0[1361] = ZERO;
  array0[1362] = ZERO;
  array0[1363] = ZERO;
  array0[1364] = ZERO;
  array0[1365] = ZERO;
  array0[1366] = ZERO;
  array0[1367] = ZERO;
  array0[1368] = constant703;
  array0[1369] = constant704;
  array0[1370] = constant705;
  array0[1371] = constant706;
  array0[1372] = constant707;
  array0[1373] = constant708;
  array0[1374] = constant709;
  array0[1375] = constant710;
  array0[1376] = constant711;
  array0[1377] = constant712;
  array0[1378] = constant713;
  array0[1379] = constant714;
  array0[1380] = constant715;
  array0[1381] = constant716;
  array0[1382] = constant717;
  array0[1383] = constant718;
  array0[1384] = constant719;
  array0[1385] = constant720;
  array0[1386] = constant721;
  array0[1387] = constant722;
  array0[1388] = constant723;
  array0[1389] = constant724;
  array0[1390] = constant725;
  array0[1391] = constant726;
  array0[1392] = constant727;
  array0[1393] = constant728;
  array0[1394] = constant729;
  array0[1395] = constant730;
  array0[1396] = constant731;
  array0[1397] = constant732;
  array0[1398] = constant733;
  array0[1399] = constant734;
  array0[1400] = constant734;
  array0[1401] = constant734;
  array0[1402] = constant734;
  array0[1403] = constant734;
  array0[1404] = constant734;
  array0[1405] = constant734;
  array0[1406] = constant735;
  array0[1407] = constant736;
  array0[1408] = constant736;
  array0[1409] = constant737;
  array0[1410] = constant738;
  array0[1411] = constant739;
  array0[1412] = constant740;
  array0[1413] = constant741;
  array0[1414] = constant742;
  array0[1415] = constant742;
  array0[1416] = constant742;
  array0[1417] = constant742;
  array0[1418] = constant742;
  array0[1419] = constant742;
  array0[1420] = constant742;
  array0[1421] = constant742;
  array0[1422] = constant742;
  array0[1423] = constant742;
  array0[1424] = constant742;
  array0[1425] = constant743;
  array0[1426] = constant744;
  array0[1427] = constant745;
  array0[1428] = constant746;
  array0[1429] = constant747;
  array0[1430] = constant748;
  array0[1431] = constant749;
  array0[1432] = constant750;
  array0[1433] = constant751;
  array0[1434] = constant751;
  array0[1435] = constant751;
  array0[1436] = constant751;
  array0[1437] = constant751;
  array0[1438] = constant751;
  array0[1439] = constant751;
  array0[1440] = constant751;
  array0[1441] = constant751;
  array0[1442] = constant751;
  array0[1443] = constant751;
  array0[1444] = constant752;
  array0[1445] = constant753;
  array0[1446] = constant753;
  array0[1447] = constant753;
  array0[1448] = constant753;
  array0[1449] = constant753;
  array0[1450] = constant753;
  array0[1451] = constant753;
  array0[1452] = constant753;
  array0[1453] = constant753;
  array0[1454] = constant753;
  array0[1455] = constant753;
  array0[1456] = constant753;
  array0[1457] = constant753;
  array0[1458] = constant753;
  array0[1459] = constant753;
  array0[1460] = constant753;
  array0[1461] = constant753;
  array0[1462] = constant753;
  array0[1463] = constant754;
  array0[1464] = constant755;
  array0[1465] = constant755;
  array0[1466] = constant755;
  array0[1467] = constant755;
  array0[1468] = constant755;
  array0[1469] = constant755;
  array0[1470] = constant755;
  array0[1471] = constant755;
  array0[1472] = constant755;
  array0[1473] = constant755;
  array0[1474] = constant755;
  array0[1475] = constant755;
  array0[1476] = constant755;
  array0[1477] = constant755;
  array0[1478] = constant755;
  array0[1479] = constant755;
  array0[1480] = constant755;
  array0[1481] = constant755;
  array0[1482] = constant756;
  array0[1483] = constant757;
  array0[1484] = constant758;
  array0[1485] = constant759;
  array0[1486] = constant760;
  array0[1487] = constant761;
  array0[1488] = constant762;
  array0[1489] = constant763;
  array0[1490] = constant764;
  array0[1491] = constant764;
  array0[1492] = constant764;
  array0[1493] = constant764;
  array0[1494] = constant764;
  array0[1495] = constant764;
  array0[1496] = constant764;
  array0[1497] = constant764;
  array0[1498] = constant764;
  array0[1499] = constant764;
  array0[1500] = constant764;
  array0[1501] = constant765;
  array0[1502] = constant766;
  array0[1503] = constant767;
  array0[1504] = constant768;
  array0[1505] = constant769;
  array0[1506] = constant770;
  array0[1507] = constant771;
  array0[1508] = constant772;
  array0[1509] = constant773;
  array0[1510] = constant773;
  array0[1511] = constant773;
  array0[1512] = constant773;
  array0[1513] = constant773;
  array0[1514] = constant773;
  array0[1515] = constant773;
  array0[1516] = constant773;
  array0[1517] = constant773;
  array0[1518] = constant773;
  array0[1519] = constant773;
  array0[1520] = constant774;
  array0[1521] = constant775;
  array0[1522] = constant776;
  array0[1523] = constant777;
  array0[1524] = constant778;
  array0[1525] = constant779;
  array0[1526] = constant780;
  array0[1527] = constant781;
  array0[1528] = constant782;
  array0[1529] = constant782;
  array0[1530] = constant782;
  array0[1531] = constant782;
  array0[1532] = constant782;
  array0[1533] = constant782;
  array0[1534] = constant782;
  array0[1535] = constant782;
  array0[1536] = constant782;
  array0[1537] = constant782;
  array0[1538] = constant782;
  array0[1539] = constant783;
  array0[1540] = constant784;
  array0[1541] = constant785;
  array0[1542] = constant786;
  array0[1543] = constant787;
  array0[1544] = constant788;
  array0[1545] = constant789;
  array0[1546] = constant790;
  array0[1547] = constant791;
  array0[1548] = constant792;
  array0[1549] = constant793;
  array0[1550] = constant793;
  array0[1551] = constant793;
  array0[1552] = constant793;
  array0[1553] = constant793;
  array0[1554] = constant793;
  array0[1555] = constant793;
  array0[1556] = constant793;
  array0[1557] = constant793;
  array0[1558] = constant794;
  array0[1559] = constant795;
  array0[1560] = constant796;
  array0[1561] = constant797;
  array0[1562] = constant798;
  array0[1563] = constant799;
  array0[1564] = constant800;
  array0[1565] = constant801;
  array0[1566] = constant802;
  array0[1567] = constant803;
  array0[1568] = constant804;
  array0[1569] = constant805;
  array0[1570] = constant806;
  array0[1571] = constant806;
  array0[1572] = constant806;
  array0[1573] = constant806;
  array0[1574] = constant806;
  array0[1575] = constant806;
  array0[1576] = constant806;
  array0[1577] = constant807;
  array0[1578] = constant808;
  array0[1579] = constant808;
  array0[1580] = constant808;
  array0[1581] = constant808;
  array0[1582] = constant808;
  array0[1583] = constant808;
  array0[1584] = constant808;
  array0[1585] = constant808;
  array0[1586] = constant808;
  array0[1587] = constant808;
  array0[1588] = constant808;
  array0[1589] = constant808;
  array0[1590] = constant808;
  array0[1591] = constant808;
  array0[1592] = constant808;
  array0[1593] = constant808;
  array0[1594] = constant808;
  array0[1595] = constant808;
  array0[1596] = constant809;
  array0[1597] = constant810;
  array0[1598] = constant811;
  array0[1599] = constant812;
  array0[1600] = constant813;
  array0[1601] = constant814;
  array0[1602] = constant815;
  array0[1603] = constant816;
  array0[1604] = constant817;
  array0[1605] = constant817;
  array0[1606] = constant817;
  array0[1607] = constant817;
  array0[1608] = constant817;
  array0[1609] = constant817;
  array0[1610] = constant817;
  array0[1611] = constant817;
  array0[1612] = constant817;
  array0[1613] = constant817;
  array0[1614] = constant817;
  array0[1615] = constant818;
  array0[1616] = constant819;
  array0[1617] = constant820;
  array0[1618] = constant821;
  array0[1619] = constant822;
  array0[1620] = constant823;
  array0[1621] = constant824;
  array0[1622] = constant825;
  array0[1623] = constant826;
  array0[1624] = constant826;
  array0[1625] = constant826;
  array0[1626] = constant826;
  array0[1627] = constant826;
  array0[1628] = constant826;
  array0[1629] = constant826;
  array0[1630] = constant826;
  array0[1631] = constant826;
  array0[1632] = constant826;
  array0[1633] = constant826;
  array0[1634] = constant827;
  array0[1635] = ZERO;
  array0[1636] = ZERO;
  array0[1637] = ZERO;
  array0[1638] = ZERO;
  array0[1639] = ZERO;
  array0[1640] = ZERO;
  array0[1641] = ZERO;
  array0[1642] = ZERO;
  array0[1643] = ZERO;
  array0[1644] = ZERO;
  array0[1645] = ZERO;
  array0[1646] = ZERO;
  array0[1647] = ZERO;
  array0[1648] = ZERO;
  array0[1649] = ZERO;
  array0[1650] = ZERO;
  array0[1651] = ZERO;
  array0[1652] = ZERO;
  array0[1653] = constant828;
  array0[1654] = ZERO;
  array0[1655] = ZERO;
  array0[1656] = ZERO;
  array0[1657] = ZERO;
  array0[1658] = ZERO;
  array0[1659] = ZERO;
  array0[1660] = ZERO;
  array0[1661] = ZERO;
  array0[1662] = ZERO;
  array0[1663] = ZERO;
  array0[1664] = ZERO;
  array0[1665] = ZERO;
  array0[1666] = ZERO;
  array0[1667] = ZERO;
  array0[1668] = ZERO;
  array0[1669] = ZERO;
  array0[1670] = ZERO;
  array0[1671] = ZERO;
  array0[1672] = constant829;
  array0[1673] = ZERO;
  array0[1674] = ZERO;
  array0[1675] = constant830;
  array0[1676] = constant831;
  array0[1677] = constant832;
  array0[1678] = constant833;
  array0[1679] = constant834;
  array0[1680] = constant835;
  array0[1681] = constant836;
  array0[1682] = constant837;
  array0[1683] = constant838;
  array0[1684] = constant839;
  array0[1685] = constant840;
  array0[1686] = constant841;
  array0[1687] = constant842;
  array0[1688] = constant843;
  array0[1689] = constant844;
  array0[1690] = constant845;
  array0[1691] = constant846;
  array0[1692] = ZERO;
  array0[1693] = ZERO;
  array0[1694] = ZERO;
  array0[1695] = ZERO;
  array0[1696] = ZERO;
  array0[1697] = ZERO;
  array0[1698] = ZERO;
  array0[1699] = ZERO;
  array0[1700] = ZERO;
  array0[1701] = ZERO;
  array0[1702] = ZERO;
  array0[1703] = ZERO;
  array0[1704] = ZERO;
  array0[1705] = ZERO;
  array0[1706] = ZERO;
  array0[1707] = ZERO;
  array0[1708] = ZERO;
  array0[1709] = ZERO;
  array0[1710] = constant847;
  array0[1711] = constant848;
  array0[1712] = constant849;
  array0[1713] = constant850;
  array0[1714] = constant851;
  array0[1715] = constant852;
  array0[1716] = constant853;
  array0[1717] = constant854;
  array0[1718] = constant855;
  array0[1719] = constant856;
  array0[1720] = constant857;
  array0[1721] = constant858;
  array0[1722] = constant859;
  array0[1723] = constant860;
  array0[1724] = constant861;
  array0[1725] = constant862;
  array0[1726] = constant863;
  array0[1727] = constant864;
  array0[1728] = constant865;
  array0[1729] = constant866;
  array0[1730] = constant867;
  array0[1731] = constant868;
  array0[1732] = constant869;
  array0[1733] = constant870;
  array0[1734] = constant871;
  array0[1735] = constant872;
  array0[1736] = constant873;
  array0[1737] = constant874;
  array0[1738] = constant875;
  array0[1739] = constant876;
  array0[1740] = constant877;
  array0[1741] = constant878;
  array0[1742] = constant879;
  array0[1743] = constant880;
  array0[1744] = constant881;
  array0[1745] = constant882;
  array0[1746] = constant883;
  array0[1747] = constant883;
  array0[1748] = constant884;
  array0[1749] = ZERO;
  array0[1750] = ZERO;
  array0[1751] = ZERO;
  array0[1752] = ZERO;
  array0[1753] = ZERO;
  array0[1754] = ZERO;
  array0[1755] = ZERO;
  array0[1756] = ZERO;
  array0[1757] = ZERO;
  array0[1758] = ZERO;
  array0[1759] = ZERO;
  array0[1760] = ZERO;
  array0[1761] = ZERO;
  array0[1762] = ZERO;
  array0[1763] = ZERO;
  array0[1764] = ZERO;
  array0[1765] = ZERO;
  array0[1766] = ZERO;
  array0[1767] = constant885;
  array0[1768] = ZERO;
  array0[1769] = ZERO;
  array0[1770] = ZERO;
  array0[1771] = ZERO;
  array0[1772] = constant886;
  array0[1773] = constant887;
  array0[1774] = constant888;
  array0[1775] = constant889;
  array0[1776] = constant890;
  array0[1777] = constant891;
  array0[1778] = constant892;
  array0[1779] = constant893;
  array0[1780] = constant894;
  array0[1781] = constant895;
  array0[1782] = constant896;
  array0[1783] = constant897;
  array0[1784] = constant898;
  array0[1785] = constant899;
  array0[1786] = constant900;
  array0[1787] = ZERO;
  array0[1788] = ZERO;
  array0[1789] = ZERO;
  array0[1790] = ZERO;
  array0[1791] = constant901;
  array0[1792] = constant902;
  array0[1793] = constant903;
  array0[1794] = constant904;
  array0[1795] = constant905;
  array0[1796] = constant906;
  array0[1797] = constant907;
  array0[1798] = constant908;
  array0[1799] = constant909;
  array0[1800] = constant910;
  array0[1801] = constant911;
  array0[1802] = constant912;
  array0[1803] = constant913;
  array0[1804] = constant913;
  array0[1805] = constant914;
  array0[1806] = constant915;
  array0[1807] = constant916;
  array0[1808] = constant917;
  array0[1809] = constant918;
  array0[1810] = constant919;
  array0[1811] = constant920;
  array0[1812] = constant921;
  array0[1813] = constant922;
  array0[1814] = constant923;
  array0[1815] = constant924;
  array0[1816] = constant925;
  array0[1817] = constant926;
  array0[1818] = constant927;
  array0[1819] = constant928;
  array0[1820] = constant929;
  array0[1821] = constant930;
  array0[1822] = constant931;
  array0[1823] = constant932;
  array0[1824] = constant933;
  array0[1825] = ZERO;
  array0[1826] = ZERO;
  array0[1827] = ZERO;
  array0[1828] = ZERO;
  array0[1829] = ZERO;
  array0[1830] = ZERO;
  array0[1831] = constant934;
  array0[1832] = constant935;
  array0[1833] = constant936;
  array0[1834] = constant937;
  array0[1835] = constant938;
  array0[1836] = constant939;
  array0[1837] = constant939;
  array0[1838] = constant939;
  array0[1839] = constant939;
  array0[1840] = constant939;
  array0[1841] = constant939;
  array0[1842] = constant939;
  array0[1843] = constant940;
  array0[1844] = ZERO;
  array0[1845] = ZERO;
  array0[1846] = ZERO;
  array0[1847] = ZERO;
  array0[1848] = ZERO;
  array0[1849] = ZERO;
  array0[1850] = constant941;
  array0[1851] = constant942;
  array0[1852] = constant943;
  array0[1853] = constant944;
  array0[1854] = constant945;
  array0[1855] = constant946;
  array0[1856] = constant946;
  array0[1857] = constant946;
  array0[1858] = constant946;
  array0[1859] = constant946;
  array0[1860] = constant946;
  array0[1861] = constant946;
  array0[1862] = constant947;
  array0[1863] = ZERO;
  array0[1864] = ZERO;
  array0[1865] = constant948;
  array0[1866] = constant949;
  array0[1867] = constant950;
  array0[1868] = constant951;
  array0[1869] = constant952;
  array0[1870] = constant953;
  array0[1871] = constant954;
  array0[1872] = constant955;
  array0[1873] = constant956;
  array0[1874] = constant956;
  array0[1875] = constant956;
  array0[1876] = constant956;
  array0[1877] = constant956;
  array0[1878] = constant956;
  array0[1879] = constant956;
  array0[1880] = constant956;
  array0[1881] = constant957;
  array0[1882] = ZERO;
  array0[1883] = ZERO;
  array0[1884] = constant958;
  array0[1885] = constant959;
  array0[1886] = constant960;
  array0[1887] = constant961;
  array0[1888] = constant962;
  array0[1889] = constant963;
  array0[1890] = constant964;
  array0[1891] = constant965;
  array0[1892] = constant966;
  array0[1893] = constant966;
  array0[1894] = constant966;
  array0[1895] = constant966;
  array0[1896] = constant966;
  array0[1897] = constant966;
  array0[1898] = constant966;
  array0[1899] = constant966;
  array0[1900] = constant967;
  array0[1901] = ZERO;
  array0[1902] = ZERO;
  array0[1903] = ZERO;
  array0[1904] = ZERO;
  array0[1905] = ZERO;
  array0[1906] = ZERO;
  array0[1907] = ZERO;
  array0[1908] = ZERO;
  array0[1909] = ZERO;
  array0[1910] = ZERO;
  array0[1911] = ZERO;
  array0[1912] = ZERO;
  array0[1913] = ZERO;
  array0[1914] = ZERO;
  array0[1915] = ZERO;
  array0[1916] = ZERO;
  array0[1917] = ZERO;
  array0[1918] = ZERO;
  array0[1919] = constant968;
  array0[1920] = ZERO;
  array0[1921] = ZERO;
  array0[1922] = ZERO;
  array0[1923] = ZERO;
  array0[1924] = ZERO;
  array0[1925] = ZERO;
  array0[1926] = ZERO;
  array0[1927] = ZERO;
  array0[1928] = ZERO;
  array0[1929] = ZERO;
  array0[1930] = ZERO;
  array0[1931] = ZERO;
  array0[1932] = ZERO;
  array0[1933] = ZERO;
  array0[1934] = ZERO;
  array0[1935] = ZERO;
  array0[1936] = ZERO;
  array0[1937] = ZERO;
  array0[1938] = constant969;
  array0[1939] = ZERO;
  array0[1940] = ZERO;
  array0[1941] = ZERO;
  array0[1942] = ZERO;
  array0[1943] = ZERO;
  array0[1944] = ZERO;
  array0[1945] = ZERO;
  array0[1946] = ZERO;
  array0[1947] = ZERO;
  array0[1948] = ZERO;
  array0[1949] = ZERO;
  array0[1950] = ZERO;
  array0[1951] = ZERO;
  array0[1952] = ZERO;
  array0[1953] = ZERO;
  array0[1954] = ZERO;
  array0[1955] = ZERO;
  array0[1956] = ZERO;
  array0[1957] = constant970;
  array0[1958] = constant971;
  array0[1959] = constant972;
  array0[1960] = constant973;
  array0[1961] = constant974;
  array0[1962] = constant975;
  array0[1963] = ZERO;
  array0[1964] = ZERO;
  array0[1965] = ZERO;
  array0[1966] = ZERO;
  array0[1967] = ZERO;
  array0[1968] = ZERO;
  array0[1969] = ZERO;
  array0[1970] = ZERO;
  array0[1971] = ZERO;
  array0[1972] = ZERO;
  array0[1973] = ZERO;
  array0[1974] = ZERO;
  array0[1975] = ZERO;
  array0[1976] = constant976;
  array0[1977] = ZERO;
  array0[1978] = ZERO;
  array0[1979] = ZERO;
  array0[1980] = ZERO;
  array0[1981] = ZERO;
  array0[1982] = ZERO;
  array0[1983] = ZERO;
  array0[1984] = ZERO;
  array0[1985] = ZERO;
  array0[1986] = ZERO;
  array0[1987] = ZERO;
  array0[1988] = ZERO;
  array0[1989] = ZERO;
  array0[1990] = ZERO;
  array0[1991] = ZERO;
  array0[1992] = ZERO;
  array0[1993] = ZERO;
  array0[1994] = ZERO;
  array0[1995] = constant977;
  array0[1996] = ZERO;
  array0[1997] = ZERO;
  array0[1998] = ZERO;
  array0[1999] = ZERO;
  array0[2000] = ZERO;
  array0[2001] = ZERO;
  array0[2002] = ZERO;
  array0[2003] = ZERO;
  array0[2004] = ZERO;
  array0[2005] = ZERO;
  array0[2006] = ZERO;
  array0[2007] = ZERO;
  array0[2008] = ZERO;
  array0[2009] = ZERO;
  array0[2010] = ZERO;
  array0[2011] = ZERO;
  array0[2012] = ZERO;
  array0[2013] = ZERO;
  array0[2014] = constant978;
  array0[2015] = ZERO;
  array0[2016] = ZERO;
  array0[2017] = ZERO;
  array0[2018] = ZERO;
  array0[2019] = ZERO;
  array0[2020] = ZERO;
  array0[2021] = ZERO;
  array0[2022] = ZERO;
  array0[2023] = ZERO;
  array0[2024] = ZERO;
  array0[2025] = ZERO;
  array0[2026] = ZERO;
  array0[2027] = ZERO;
  array0[2028] = ZERO;
  array0[2029] = ZERO;
  array0[2030] = ZERO;
  array0[2031] = ZERO;
  array0[2032] = ZERO;
  array0[2033] = constant979;
  array0[2034] = ZERO;
  array0[2035] = ZERO;
  array0[2036] = ZERO;
  array0[2037] = ZERO;
  array0[2038] = ZERO;
  array0[2039] = ZERO;
  array0[2040] = ZERO;
  array0[2041] = ZERO;
  array0[2042] = ZERO;
  array0[2043] = ZERO;
  array0[2044] = ZERO;
  array0[2045] = ZERO;
  array0[2046] = ZERO;
  array0[2047] = ZERO;
  array0[2048] = ZERO;
  array0[2049] = ZERO;
  array0[2050] = ZERO;
  array0[2051] = ZERO;
  array0[2052] = constant980;
  array0[2053] = ZERO;
  array0[2054] = ZERO;
  array0[2055] = ZERO;
  array0[2056] = ZERO;
  array0[2057] = ZERO;
  array0[2058] = ZERO;
  array0[2059] = ZERO;
  array0[2060] = ZERO;
  array0[2061] = ZERO;
  array0[2062] = ZERO;
  array0[2063] = ZERO;
  array0[2064] = ZERO;
  array0[2065] = ZERO;
  array0[2066] = ZERO;
  array0[2067] = ZERO;
  array0[2068] = ZERO;
  array0[2069] = ZERO;
  array0[2070] = ZERO;
  array0[2071] = constant981;
  array0[2072] = ZERO;
  array0[2073] = ZERO;
  array0[2074] = ZERO;
  array0[2075] = ZERO;
  array0[2076] = ZERO;
  array0[2077] = ZERO;
  array0[2078] = ZERO;
  array0[2079] = ZERO;
  array0[2080] = ZERO;
  array0[2081] = ZERO;
  array0[2082] = ZERO;
  array0[2083] = ZERO;
  array0[2084] = ZERO;
  array0[2085] = ZERO;
  array0[2086] = ZERO;
  array0[2087] = ZERO;
  array0[2088] = ZERO;
  array0[2089] = ZERO;
  array0[2090] = constant982;
  array0[2091] = ZERO;
  array0[2092] = ZERO;
  array0[2093] = ZERO;
  array0[2094] = ZERO;
  array0[2095] = ZERO;
  array0[2096] = ZERO;
  array0[2097] = ZERO;
  array0[2098] = ZERO;
  array0[2099] = ZERO;
  array0[2100] = ZERO;
  array0[2101] = ZERO;
  array0[2102] = ZERO;
  array0[2103] = ZERO;
  array0[2104] = ZERO;
  array0[2105] = ZERO;
  array0[2106] = ZERO;
  array0[2107] = ZERO;
  array0[2108] = ZERO;
  array0[2109] = constant983;
  array0[2110] = ZERO;
  array0[2111] = ZERO;
  array0[2112] = ZERO;
  array0[2113] = ZERO;
  array0[2114] = ZERO;
  array0[2115] = ZERO;
  array0[2116] = ZERO;
  array0[2117] = ZERO;
  array0[2118] = ZERO;
  array0[2119] = ZERO;
  array0[2120] = ZERO;
  array0[2121] = ZERO;
  array0[2122] = ZERO;
  array0[2123] = ZERO;
  array0[2124] = ZERO;
  array0[2125] = ZERO;
  array0[2126] = ZERO;
  array0[2127] = ZERO;
  array0[2128] = constant984;
  array0[2129] = constant985;
  array0[2130] = constant986;
  array0[2131] = constant987;
  array0[2132] = constant988;
  array0[2133] = constant989;
  array0[2134] = ZERO;
  array0[2135] = ZERO;
  array0[2136] = ZERO;
  array0[2137] = ZERO;
  array0[2138] = ZERO;
  array0[2139] = ZERO;
  array0[2140] = ZERO;
  array0[2141] = ZERO;
  array0[2142] = ZERO;
  array0[2143] = ZERO;
  array0[2144] = ZERO;
  array0[2145] = ZERO;
  array0[2146] = ZERO;
  array0[2147] = constant990;
  array0[2148] = ZERO;
  array0[2149] = ZERO;
  array0[2150] = constant991;
  array0[2151] = constant992;
  array0[2152] = constant993;
  array0[2153] = constant994;
  array0[2154] = constant995;
  array0[2155] = constant996;
  array0[2156] = constant997;
  array0[2157] = constant998;
  array0[2158] = constant999;
  array0[2159] = constant1000;
  array0[2160] = constant1001;
  array0[2161] = constant1002;
  array0[2162] = constant1003;
  array0[2163] = constant1004;
  array0[2164] = constant1005;
  array0[2165] = constant1006;
  array0[2166] = constant1007;
  array0[2167] = constant1008;
  array0[2168] = constant1009;
  array0[2169] = constant1010;
  array0[2170] = ZERO;
  array0[2171] = ZERO;
  array0[2172] = ZERO;
  array0[2173] = ZERO;
  array0[2174] = ZERO;
  array0[2175] = ZERO;
  array0[2176] = ZERO;
  array0[2177] = ZERO;
  array0[2178] = ZERO;
  array0[2179] = ZERO;
  array0[2180] = ZERO;
  array0[2181] = ZERO;
  array0[2182] = ZERO;
  array0[2183] = ZERO;
  array0[2184] = ZERO;
  array0[2185] = constant1011;
  array0[2186] = constant1012;
  array0[2187] = constant1013;
  array0[2188] = constant1014;
  array0[2189] = constant1015;
  array0[2190] = constant1016;
  array0[2191] = constant1017;
  array0[2192] = constant1018;
  array0[2193] = constant1019;
  array0[2194] = constant1020;
  array0[2195] = constant1021;
  array0[2196] = constant1022;
  array0[2197] = constant1023;
  array0[2198] = constant1024;
  array0[2199] = constant1025;
  array0[2200] = constant1026;
  array0[2201] = constant1027;
  array0[2202] = constant1028;
  array0[2203] = constant1029;
  array0[2204] = constant1030;
  array0[2205] = constant1031;
  array0[2206] = constant1032;
  array0[2207] = constant1033;
  array0[2208] = constant1034;
  array0[2209] = constant1035;
  array0[2210] = constant1036;
  array0[2211] = constant1037;
  array0[2212] = constant1038;
  array0[2213] = constant1039;
  array0[2214] = constant1040;
  array0[2215] = constant1041;
  array0[2216] = constant1042;
  array0[2217] = constant1043;
  array0[2218] = constant1044;
  array0[2219] = constant1045;
  array0[2220] = constant1046;
  array0[2221] = constant1047;
  array0[2222] = constant1048;
  array0[2223] = constant1049;
  array0[2224] = ZERO;
  array0[2225] = ZERO;
  array0[2226] = ZERO;
  array0[2227] = ZERO;
  array0[2228] = ZERO;
  array0[2229] = ZERO;
  array0[2230] = ZERO;
  array0[2231] = ZERO;
  array0[2232] = ZERO;
  array0[2233] = ZERO;
  array0[2234] = ZERO;
  array0[2235] = ZERO;
  array0[2236] = ZERO;
  array0[2237] = ZERO;
  array0[2238] = ZERO;
  array0[2239] = ZERO;
  array0[2240] = ZERO;
  array0[2241] = ZERO;
  array0[2242] = constant1050;
  array0[2243] = ZERO;
  array0[2244] = ZERO;
  array0[2245] = ZERO;
  array0[2246] = ZERO;
  array0[2247] = ZERO;
  array0[2248] = ZERO;
  array0[2249] = ZERO;
  array0[2250] = ZERO;
  array0[2251] = ZERO;
  array0[2252] = ZERO;
  array0[2253] = ZERO;
  array0[2254] = ZERO;
  array0[2255] = ZERO;
  array0[2256] = ZERO;
  array0[2257] = ZERO;
  array0[2258] = ZERO;
  array0[2259] = ZERO;
  array0[2260] = ZERO;
  array0[2261] = constant1051;
  array0[2262] = ZERO;
  array0[2263] = ZERO;
  array0[2264] = ZERO;
  array0[2265] = ZERO;
  array0[2266] = ZERO;
  array0[2267] = ZERO;
  array0[2268] = ZERO;
  array0[2269] = ZERO;
  array0[2270] = ZERO;
  array0[2271] = ZERO;
  array0[2272] = ZERO;
  array0[2273] = ZERO;
  array0[2274] = ZERO;
  array0[2275] = ZERO;
  array0[2276] = ZERO;
  array0[2277] = ZERO;
  array0[2278] = ZERO;
  array0[2279] = ZERO;
  array0[2280] = constant1052;
  array0[2281] = ZERO;
  array0[2282] = ZERO;
  array0[2283] = ZERO;
  array0[2284] = ZERO;
  array0[2285] = ZERO;
  array0[2286] = ZERO;
  array0[2287] = ZERO;
  array0[2288] = ZERO;
  array0[2289] = ZERO;
  array0[2290] = ZERO;
  array0[2291] = ZERO;
  array0[2292] = ZERO;
  array0[2293] = ZERO;
  array0[2294] = ZERO;
  array0[2295] = ZERO;
  array0[2296] = ZERO;
  array0[2297] = ZERO;
  array0[2298] = ZERO;
  array0[2299] = constant1053;
  array0[2300] = ZERO;
  array0[2301] = ZERO;
  array0[2302] = ZERO;
  array0[2303] = ZERO;
  array0[2304] = ZERO;
  array0[2305] = ZERO;
  array0[2306] = ZERO;
  array0[2307] = ZERO;
  array0[2308] = ZERO;
  array0[2309] = ZERO;
  array0[2310] = ZERO;
  array0[2311] = ZERO;
  array0[2312] = ZERO;
  array0[2313] = ZERO;
  array0[2314] = ZERO;
  array0[2315] = ZERO;
  array0[2316] = ZERO;
  array0[2317] = ZERO;
  array0[2318] = constant1054;
  array0[2319] = ZERO;
  array0[2320] = ZERO;
  array0[2321] = ZERO;
  array0[2322] = ZERO;
  array0[2323] = ZERO;
  array0[2324] = ZERO;
  array0[2325] = ZERO;
  array0[2326] = ZERO;
  array0[2327] = ZERO;
  array0[2328] = ZERO;
  array0[2329] = ZERO;
  array0[2330] = ZERO;
  array0[2331] = ZERO;
  array0[2332] = ZERO;
  array0[2333] = ZERO;
  array0[2334] = ZERO;
  array0[2335] = ZERO;
  array0[2336] = ZERO;
  array0[2337] = constant1055;
  array0[2338] = constant1056;
  array0[2339] = constant1057;
  array0[2340] = constant1058;
  array0[2341] = constant1059;
  array0[2342] = constant1060;
  array0[2343] = constant1061;
  array0[2344] = constant1062;
  array0[2345] = constant1063;
  array0[2346] = constant1064;
  array0[2347] = constant1065;
  array0[2348] = constant1066;
  array0[2349] = constant1066;
  array0[2350] = constant1066;
  array0[2351] = constant1066;
  array0[2352] = constant1066;
  array0[2353] = constant1066;
  array0[2354] = constant1066;
  array0[2355] = constant1066;
  array0[2356] = constant1067;
  array0[2357] = constant1068;
  array0[2358] = constant1069;
  array0[2359] = constant1070;
  array0[2360] = constant1071;
  array0[2361] = constant1072;
  array0[2362] = constant1073;
  array0[2363] = constant1074;
  array0[2364] = constant1075;
  array0[2365] = constant1076;
  array0[2366] = constant1077;
  array0[2367] = constant1078;
  array0[2368] = constant1078;
  array0[2369] = constant1078;
  array0[2370] = constant1078;
  array0[2371] = constant1078;
  array0[2372] = constant1078;
  array0[2373] = constant1078;
  array0[2374] = constant1078;
  array0[2375] = constant1079;
  array0[2376] = constant1080;
  array0[2377] = constant1081;
  array0[2378] = constant1082;
  array0[2379] = constant1083;
  array0[2380] = constant1084;
  array0[2381] = constant1085;
  array0[2382] = constant1086;
  array0[2383] = constant1087;
  array0[2384] = constant1088;
  array0[2385] = constant1089;
  array0[2386] = constant1090;
  array0[2387] = constant1090;
  array0[2388] = constant1090;
  array0[2389] = constant1090;
  array0[2390] = constant1090;
  array0[2391] = constant1090;
  array0[2392] = constant1090;
  array0[2393] = constant1090;
  array0[2394] = constant1091;
  array0[2395] = constant1092;
  array0[2396] = constant1093;
  array0[2397] = constant1094;
  array0[2398] = constant1095;
  array0[2399] = constant1096;
  array0[2400] = constant1097;
  array0[2401] = constant1098;
  array0[2402] = constant1099;
  array0[2403] = constant1100;
  array0[2404] = constant1101;
  array0[2405] = constant1102;
  array0[2406] = constant1103;
  array0[2407] = constant1104;
  array0[2408] = constant1105;
  array0[2409] = constant1106;
  array0[2410] = constant1107;
  array0[2411] = constant1108;
  array0[2412] = constant1109;
  array0[2413] = constant1110;
  array0[2414] = constant1111;
  array0[2415] = constant1112;
  array0[2416] = constant1113;
  array0[2417] = constant1114;
  array0[2418] = constant1115;
  array0[2419] = constant1116;
  array0[2420] = constant1117;
  array0[2421] = constant1118;
  array0[2422] = constant1119;
  array0[2423] = constant1120;
  array0[2424] = constant1121;
  array0[2425] = constant1121;
  array0[2426] = constant1121;
  array0[2427] = constant1121;
  array0[2428] = constant1121;
  array0[2429] = constant1121;
  array0[2430] = constant1121;
  array0[2431] = constant1121;
  ExcelValue array0_ev = EXCEL_RANGE(array0,128,19);
  result = array0_ev;
  variable_set[3415] = 1;
  return result;
}

ExcelValue output_tra_car_type_share() {
  static ExcelValue result;
  if(variable_set[3416] == 1) { return result;}
  static ExcelValue array0[76];
  array0[0] = constant1122;
  array0[1] = ZERO;
  array0[2] = ZERO;
  array0[3] = constant1123;
  array0[4] = constant1124;
  array0[5] = constant1125;
  array0[6] = constant1126;
  array0[7] = constant1127;
  array0[8] = constant1128;
  array0[9] = constant1128;
  array0[10] = constant1128;
  array0[11] = constant1128;
  array0[12] = constant1128;
  array0[13] = constant1128;
  array0[14] = constant1128;
  array0[15] = constant1128;
  array0[16] = constant1128;
  array0[17] = constant1128;
  array0[18] = constant1128;
  array0[19] = constant1129;
  array0[20] = constant1130;
  array0[21] = constant1130;
  array0[22] = constant1131;
  array0[23] = constant1132;
  array0[24] = constant1133;
  array0[25] = constant1128;
  array0[26] = constant1128;
  array0[27] = constant1128;
  array0[28] = constant1128;
  array0[29] = constant1128;
  array0[30] = constant1128;
  array0[31] = constant1128;
  array0[32] = constant1128;
  array0[33] = constant1128;
  array0[34] = constant1128;
  array0[35] = constant1128;
  array0[36] = constant1128;
  array0[37] = constant1128;
  array0[38] = constant1134;
  array0[39] = constant1135;
  array0[40] = constant1135;
  array0[41] = constant1136;
  array0[42] = constant1137;
  array0[43] = constant1138;
  array0[44] = constant1139;
  array0[45] = constant1140;
  array0[46] = ZERO;
  array0[47] = ZERO;
  array0[48] = ZERO;
  array0[49] = ZERO;
  array0[50] = ZERO;
  array0[51] = ZERO;
  array0[52] = ZERO;
  array0[53] = ZERO;
  array0[54] = ZERO;
  array0[55] = ZERO;
  array0[56] = ZERO;
  array0[57] = constant1141;
  array0[58] = constant1142;
  array0[59] = constant1142;
  array0[60] = constant1143;
  array0[61] = constant1144;
  array0[62] = ZERO;
  array0[63] = ZERO;
  array0[64] = ZERO;
  array0[65] = ZERO;
  array0[66] = ZERO;
  array0[67] = ZERO;
  array0[68] = ZERO;
  array0[69] = ZERO;
  array0[70] = ZERO;
  array0[71] = ZERO;
  array0[72] = ZERO;
  array0[73] = ZERO;
  array0[74] = ZERO;
  array0[75] = ZERO;
  ExcelValue array0_ev = EXCEL_RANGE(array0,4,19);
  result = array0_ev;
  variable_set[3416] = 1;
  return result;
}

ExcelValue output_tra_energy_consumption() {
  static ExcelValue result;
  if(variable_set[3417] == 1) { return result;}
  static ExcelValue array0[76];
  array0[0] = constant1145;
  array0[1] = constant1146;
  array0[2] = constant1147;
  array0[3] = constant1148;
  array0[4] = constant1149;
  array0[5] = constant1150;
  array0[6] = constant1151;
  array0[7] = constant1152;
  array0[8] = constant1153;
  array0[9] = constant1154;
  array0[10] = constant1155;
  array0[11] = constant1156;
  array0[12] = constant1157;
  array0[13] = constant1158;
  array0[14] = constant1159;
  array0[15] = constant1160;
  array0[16] = constant1161;
  array0[17] = constant1162;
  array0[18] = constant1163;
  array0[19] = constant1164;
  array0[20] = ZERO;
  array0[21] = ZERO;
  array0[22] = constant1165;
  array0[23] = constant1166;
  array0[24] = constant1167;
  array0[25] = constant1168;
  array0[26] = constant1169;
  array0[27] = constant1170;
  array0[28] = constant1171;
  array0[29] = constant1172;
  array0[30] = constant1173;
  array0[31] = constant1174;
  array0[32] = constant1175;
  array0[33] = constant1176;
  array0[34] = constant1177;
  array0[35] = constant1178;
  array0[36] = constant1179;
  array0[37] = constant1180;
  array0[38] = constant1181;
  array0[39] = constant1182;
  array0[40] = constant1183;
  array0[41] = constant1184;
  array0[42] = constant1185;
  array0[43] = constant1186;
  array0[44] = constant1187;
  array0[45] = constant1188;
  array0[46] = constant1189;
  array0[47] = constant1190;
  array0[48] = constant1191;
  array0[49] = constant1192;
  array0[50] = constant1193;
  array0[51] = constant1194;
  array0[52] = constant1195;
  array0[53] = constant1196;
  array0[54] = constant1197;
  array0[55] = constant1198;
  array0[56] = constant1199;
  array0[57] = constant1200;
  array0[58] = constant1201;
  array0[59] = constant1202;
  array0[60] = constant1203;
  array0[61] = constant1204;
  array0[62] = constant1205;
  array0[63] = constant1206;
  array0[64] = constant1207;
  array0[65] = constant1208;
  array0[66] = constant1209;
  array0[67] = constant1210;
  array0[68] = constant1211;
  array0[69] = constant1212;
  array0[70] = constant1213;
  array0[71] = constant1214;
  array0[72] = constant1215;
  array0[73] = constant1216;
  array0[74] = constant1217;
  array0[75] = constant1218;
  ExcelValue array0_ev = EXCEL_RANGE(array0,4,19);
  result = array0_ev;
  variable_set[3417] = 1;
  return result;
}

void set_input_lever_ambition(ExcelValue newValue) {
  ExcelValue *array = newValue.array;
  set_control_d17(array[0]);
  set_control_d18(array[1]);
  set_control_d19(array[2]);
  set_control_d20(array[3]);
  set_control_d21(array[4]);
  set_control_d22(array[5]);
  set_control_d23(array[6]);
  set_control_d24(array[7]);
  set_control_d25(array[8]);
  set_control_d26(array[9]);
  set_control_d27(array[10]);
  set_control_d28(array[11]);
  set_control_d29(array[12]);
  set_control_d30(array[13]);
  set_control_d31(array[14]);
  set_control_d32(array[15]);
  set_control_d33(array[16]);
  set_control_d34(array[17]);
  set_control_d35(array[18]);
  set_control_d36(array[19]);
  set_control_d37(array[20]);
  set_control_d38(array[21]);
  set_control_d39(array[22]);
  set_control_d40(array[23]);
  set_control_d41(array[24]);
  set_control_d42(array[25]);
  set_control_d43(array[26]);
  set_control_d44(array[27]);
  set_control_d45(array[28]);
  set_control_d46(array[29]);
  set_control_d47(array[30]);
  set_control_d48(array[31]);
  set_control_d49(array[32]);
  set_control_d50(array[33]);
  set_control_d51(array[34]);
  set_control_d52(array[35]);
  set_control_d53(array[36]);
  set_control_d54(array[37]);
  set_control_d55(array[38]);
  set_control_d56(array[39]);
  set_control_d57(array[40]);
  set_control_d58(array[41]);
  set_control_d59(array[42]);
  set_control_d60(array[43]);
  set_control_d61(array[44]);
}

void set_input_lever_end(ExcelValue newValue) {
  ExcelValue *array = newValue.array;
  set_control_g17(array[0]);
  set_control_g18(array[1]);
  set_control_g19(array[2]);
  set_control_g20(array[3]);
  set_control_g21(array[4]);
  set_control_g22(array[5]);
  set_control_g23(array[6]);
  set_control_g24(array[7]);
  set_control_g25(array[8]);
  set_control_g26(array[9]);
  set_control_g27(array[10]);
  set_control_g28(array[11]);
  set_control_g29(array[12]);
  set_control_g30(array[13]);
  set_control_g31(array[14]);
  set_control_g32(array[15]);
  set_control_g33(array[16]);
  set_control_g34(array[17]);
  set_control_g35(array[18]);
  set_control_g36(array[19]);
  set_control_g37(array[20]);
  set_control_g38(array[21]);
  set_control_g39(array[22]);
  set_control_g40(array[23]);
  set_control_g41(array[24]);
  set_control_g42(array[25]);
  set_control_g43(array[26]);
  set_control_g44(array[27]);
  set_control_g45(array[28]);
  set_control_g46(array[29]);
  set_control_g47(array[30]);
  set_control_g48(array[31]);
  set_control_g49(array[32]);
  set_control_g50(array[33]);
  set_control_g51(array[34]);
  set_control_g52(array[35]);
  set_control_g53(array[36]);
  set_control_g54(array[37]);
  set_control_g55(array[38]);
  set_control_g56(array[39]);
  set_control_g57(array[40]);
  set_control_g58(array[41]);
  set_control_g59(array[42]);
  set_control_g60(array[43]);
  set_control_g61(array[44]);
}

void set_input_lever_start(ExcelValue newValue) {
  ExcelValue *array = newValue.array;
  set_control_e17(array[0]);
  set_control_e18(array[1]);
  set_control_e19(array[2]);
  set_control_e20(array[3]);
  set_control_e21(array[4]);
  set_control_e22(array[5]);
  set_control_e23(array[6]);
  set_control_e24(array[7]);
  set_control_e25(array[8]);
  set_control_e26(array[9]);
  set_control_e27(array[10]);
  set_control_e28(array[11]);
  set_control_e29(array[12]);
  set_control_e30(array[13]);
  set_control_e31(array[14]);
  set_control_e32(array[15]);
  set_control_e33(array[16]);
  set_control_e34(array[17]);
  set_control_e35(array[18]);
  set_control_e36(array[19]);
  set_control_e37(array[20]);
  set_control_e38(array[21]);
  set_control_e39(array[22]);
  set_control_e40(array[23]);
  set_control_e41(array[24]);
  set_control_e42(array[25]);
  set_control_e43(array[26]);
  set_control_e44(array[27]);
  set_control_e45(array[28]);
  set_control_e46(array[29]);
  set_control_e47(array[30]);
  set_control_e48(array[31]);
  set_control_e49(array[32]);
  set_control_e50(array[33]);
  set_control_e51(array[34]);
  set_control_e52(array[35]);
  set_control_e53(array[36]);
  set_control_e54(array[37]);
  set_control_e55(array[38]);
  set_control_e56(array[39]);
  set_control_e57(array[40]);
  set_control_e58(array[41]);
  set_control_e59(array[42]);
  set_control_e60(array[43]);
  set_control_e61(array[44]);
  set_control_e62(array[45]);
}

// End of named references
