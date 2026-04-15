#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 100
// declare max size of array of structs to be 100
// size of array shoudl be 100 * size of struct

// struct to store input kv pairs for map to use
typedef struct {
  int line_number; // key - order of which value entered
  int value;       // value - terminal entered value
} Input;

// stores inter kv pairs produced by map and consumed by group
typedef struct {
  int line_number;
  int doubled_value; // map doubles the initial value
} IntermediateInput;

// group produces this, and reduce uses this to produce final output
typedef struct {
  int doubled_value;           // key becomes the doubled value
  int line_numbers[MAX_INPUT]; // value becomes instance of the double value
  int count;                   // count is num instances
} Output;

void map(Input *input, IntermediateInput *intermediate_input);
void groupByKey(IntermediateInput *input, Output *output, int *result_count);
void reduce(Output *output);

int main() {
  Input input_data[MAX_INPUT]; // assoc array that stores our kv pairs
  int input_size = 0; // tracks num values read from terminal - becomes keys
  int value;          // tha value read from terminal

  // Read input values until "end" is encountered
  printf("Enter values (one per line). Type 'end' to finish:\n");
  while (input_size < MAX_INPUT) {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      break;
    }
    if (sscanf(buffer, "%d", &value) == 1) {
      input_data[input_size].line_number = input_size + 1;
      input_data[input_size].value = value;
      input_size++;
    } else if (sscanf(buffer, "end") == 0) {
      break;
    } else {
      printf("Invalid input. Please enter an integer or 'end' to finish.\n");
    }
  }

  // Step 1: Map phase

  IntermediateInput mapped_results[MAX_INPUT] = {0};

  for (int i = 0; i < input_size;
       i++) { // call map for each kv pair in input list
    map(&input_data[i], &mapped_results[i]);
    printf("intermediate input line = %d\n", mapped_results[i].line_number);
  }

  // Step 2: Grouping phase

  Output output_results[MAX_INPUT] = {0};
  int result_count = 0;

  for (int i = 0; i < input_size; i++) {
    groupByKey(&mapped_results[i], output_results, &result_count);
  } // maps intermediate key to output and updates the result/num grouped keys
    // generated

  // Step 3: Reduce phase
  for (int i = 0; i < result_count; i++) {
    if (output_results[i].count > 0) {
      reduce(&output_results[i]);
    }
  } // call reduce for each output kv pairs

  return 0;
}

void map(Input *input, IntermediateInput *intermediate_input) {
  // TODO:
  // Double the value of the input
  int ln = input->line_number;
  int val = input->value * 2;

  intermediate_input->line_number = ln;    // store ln in inter kv list
  intermediate_input->doubled_value = val; // stored doubled value
}

void groupByKey(IntermediateInput *input, Output *output, int *result_count) {
  // TODO:
  // Group entries
  // If the doubled value exists in the output entries, add it to the output's

  int doubled_value = input->doubled_value; // key
  int line_number = (*input).line_number;   // value

  int addedEntry = 0; //
  for (int i = 0; i < *result_count; i++) {
    if (output[i].doubled_value ==
        input->doubled_value) { // check if key exists in output list
      output[i].line_numbers[output[i].count] =
          input->line_number; // adds value to kv pair
      output[i].count++;      // incr values in the kv pair
      addedEntry = 1;         // flag
      break;
    } // loops thru the entire output list and checks if key exists, add another
      // key
  }
  // line_numbers list If it doesn't exist, add a new entry to the output
  if (addedEntry == 0) {
    output[*result_count].doubled_value =
        input->doubled_value; // add at last index
    output[*result_count].line_numbers[0] =
        input->line_number;           // add first val encounterd
    output[*result_count].count += 1; // update count, nym values
    (*result_count)++;                // update items in list
  }

  printf("result count = %d, added entry = %d, output val = %d\n",
         *result_count, addedEntry, output->doubled_value);
}
// if adding val to struct use dot(.)
// once val added can deref to acces val using ->
void reduce(Output *output) {
  // TODO:
  // Print the doubled number and line numbers
  printf("(%d, [%d", output->doubled_value, output->line_numbers[0]);
  for (int i = 1; i < output->count; i++) {
    printf(", %d", output->line_numbers[i]);
  }
  printf("])\n");
}
