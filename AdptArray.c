#include "AdptArray.h"
#include <stdlib.h>

struct AdptArray_
{
    int size; // The size of the array
    PElement * pElementArr; // Pointer that represents an array of pointers to generic value (void)
    DEL_FUNC deleteFunction; // Pointer to the delete function as declared in AdptArray.h
    COPY_FUNC copyFunction; // Pointer to copy function as declared in AdptArray.h
    PRINT_FUNC printFunction; // pointer to print function as declared in AdptArray.h
}AdptArray;

PAdptArray CreateAdptArray(COPY_FUNC cp_func, DEL_FUNC del_func, PRINT_FUNC pr_func)
{
    // Allocate a new adaptive array
    PAdptArray new_AdptArray = (PAdptArray)malloc(sizeof(AdptArray));
    if (!new_AdptArray)
    {
        return NULL;
    }
    new_AdptArray->size = 0;
    new_AdptArray->pElementArr = NULL; // Initial the array to be null
    new_AdptArray->deleteFunction = del_func;
    new_AdptArray->copyFunction = cp_func;
    new_AdptArray->printFunction = pr_func;
    return new_AdptArray;
}

void DeleteAdptArray(PAdptArray arr)
{
    // Verify that the PAdptArray arr is not null
    if (!arr)
    {
        return;
    }

    // Free all element in the pElement Array
    for (size_t i = 0; i < arr->size; i++)
    {
        if(arr->pElementArr[i]){
            arr->deleteFunction(arr->pElementArr[i]);
        }
    }

    // Free the array that its size allocted dynamically
    free(arr->pElementArr);
    // Free the adaptive array
    free(arr);
    
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement element)
{
    if (arr == NULL)
    {
        return FAIL;
    }

    PElement * new_elementsArr = NULL; 

    // In case that the array size is lower than the given index
    if (index >= arr->size)
    {
        new_elementsArr = (PElement *)malloc((index + 1) * sizeof(PElement));
        if(new_elementsArr == NULL)
        {
            return FAIL;
        }

        //Init the new array
        for (size_t i = 0; i < index + 1; i++)
        {
            new_elementsArr[i] = NULL;
        }
        

        // Copy the values from old array
        for (size_t i = 0; i < arr->size; i++)
        {
            new_elementsArr[i] = arr->pElementArr[i];
        }
        
        // Free the old array
        free(arr->pElementArr);

        // Change the arr->pElementArr to hold the new_elementsArr which crated right now
        arr->pElementArr = new_elementsArr;

        // Update the arr->size value
        arr->size = index + 1;
    }

    // Delete the element exists in the required index to be set with a new element
    if (arr->pElementArr[index] != NULL)
    {
        arr->deleteFunction(arr->pElementArr[index]);
    }
    
    // Copy the required element tp be set in the index and assign it to the array
    arr->pElementArr[index] = arr->copyFunction(element);

    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray arr, int index)
{
    if(index < 0)
    {
        return NULL;
    }
    
    if (arr == NULL)
    {
        return NULL;
    }

    if (index >= arr->size)
    {
        return NULL;
    }
    
    // Return a copy of the element strored in the requred index
    return (arr->pElementArr[index] != NULL) ? arr->copyFunction(arr->pElementArr[index]) : NULL;
}

int GetAdptArraySize(PAdptArray arr)
{
    if (arr->size == 0){
        return 0;
    }

    if (arr == NULL){
        return -1;
    }

    return arr->size;
}

void PrintDB(PAdptArray arr)
{
    for (size_t i = 0; i < arr->size; i++)
    {
        if(arr->pElementArr[i] != NULL)
        {
            arr->printFunction(arr->pElementArr[i]);
        }
    }
}
