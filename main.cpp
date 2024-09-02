#include <pthread.h> //allows use of pthread library 
#include <iostream>
#include <string>
#include <vector>

struct arguments
{
	int value;
	std::string binCode;
};

// Write the child thread function here

// points to any data type, takes in any data type 
void* dec2bin(void* void_ptr) {
    // type cast converts void_ptr into a pointer of type arguments*
    arguments* ptr = (arguments*) void_ptr;

    // value stored in ptr->value is assigned to local variable 'value'
    int value = ptr->value;

    // sets binCode to an empty string 
    ptr->binCode = "";

    
    do {
        // digit holds remainder value of 'value'
        int digit = value % 2;

        //converts integer to string
        //concatenates to front of string in structure, binary numbers are built right to left 
        ptr->binCode = std::to_string(digit) + ptr->binCode;
        
        //divides value by 2, updates loop condition
        value = value/2;
    }
    while (value > 0);

    //returns structure 
    return ptr;
    
}


int main()
{
    std::vector<arguments> arg; // vector of arguments structure 

    arguments tempHolder;
    tempHolder.binCode = "";

    //reads value from user and places into vector
	while (std::cin >> tempHolder.value)
    {
        arg.push_back(tempHolder);
    }

    //determines how many values the user inputs by looking at vetcor size 
    int nValues = arg.size();

    //dynamically allocates memory for thread IDs
    //tid points to array of input values 
 	pthread_t *tid = new pthread_t[nValues];
    
    //loops through all input values 
	for(int i=0;i<nValues;i++)
	{
        //checks if thread creation was successful, if successful it returns 0 
        
        //&tid[i] -- ID of the new thread will be stored here
            //tid is an array that stored the IDs for all the threads  
        //nullptr -- allows for default attributes to be used
        //dec2bin -- function that the thread will run, each thread will execute this function
        //(void*) &arg.at(i) -- input data for the thread, converts the ith element into a generic (void*) pointer
            //this is because pthread_create() requres a (void*) as the input 

		if(pthread_create(&tid[i], nullptr, dec2bin, (void*) &arg.at(i)) != 0) 
		{
            //prints error to the standard error stream
			std::cerr << "Error creating thread" << std::endl;
			return 1;
		}		
	}
	
	// Wait for the other threads to finish.
    // Call pthread_join here
    for(int i =0; i<nValues; i++)
        // pthread_join() -- waits for a specific thread to finish its execution
        //tid[i] -- holds the thread ID that we are waiting for 
        //nullptr -- this parameter is notmally used to obtain the return status of the thread 
            //passing nullptr means we are not interested in the return status 
        pthread_join(tid[i],nullptr);
    //this ensures that the main program waits for all the threads to complete before continuing
    

    for (int i = 0; i < nValues; i++)
    {
        std::cout << arg[i].value << " base 10 is equal to " << arg[i].binCode << std::endl;
    }


    //frees allocated memory 
    if (tid!=nullptr) {
        delete[] tid;
    }


    return 0;
}