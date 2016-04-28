# libKF
libKF is a easily customizable Kalman filter implementation in the C language. libKF also provides a small collection of vector and matrix operation functions. These are intended for use internally, but you are encouraged to use them within your project as well.

### Requirements

* Unix / Linux
* Modern C compiler (tested with GCC 4.8.4)
* Make (tested with GNU Make 3.81)
* Python >= 3.5.0 (for test runner)
* [indicurses](http://github.com/mrpossoms/indicurses) (for test visualization)

### Installation

```Bash
make
sudo make install
```

### Usage
Here I provide an example of simple, one dimensional usage. First we will start by allocating the matrices and vectors that are used internally by the algorithm.

```C
kf_t filter = {};

if(kfCreateFilter(&filter, 1)){
	// Allocation error occured
}
```

Next, we set the statistical traits of the system. Here we set the measurement noise co-variance matrix (matR) and the process noise co-variance matrix (matQ).
```C
	filter.matB.col[0][0] = 0; // No controls inputs exist in this example.
	filter.matR.col[0][0] = 10;
	filter.matQ.col[0][0] = 0.001;
```
Since this example only has a single dimension, all of these matrices only have a single row and column. All matrices in the filter are initialized to the identity. Other matrices availabe for you to customize include the following.
* matF   // 'F' State transition matrix, maps previous state onto new state ( user defined )
* matB   // 'B' Control input matrix, maps effect of control input onto the state ( user defined )
* matH;   // 'H' Transformation matrix maps measurements into the state domain ( user defined )
In this very simple example, we leave all of these as the identity.

Next, run the filter. Here we will filter a constat value just for the sake of example.
```C
float state[] = { 0 }; // currently estimated state

while(1){
	float reading = 100;
	float measurement[] = { reading }; 
	
	kfPredict(&filter, NULL);
	kfUpdate(&filter, state, measurement);
}
```

After many iterations `state` will gradually approach 100. If the convergence of this filter were plotted, it would look like this.

```
  |200
  |
  |
  |
  |
  |
  |
  |....................................................
  |                             ***********************
  |             ****************
  |      *******
  |   ***
  |  *
  |**
  *0% 
```
You can find a full, single file example [here](https://gist.github.com/mrpossoms/772681d7b9b822125a91b54a2cf66997).
