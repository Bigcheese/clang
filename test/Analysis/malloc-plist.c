// RUN: %clang_cc1 -analyze -analyzer-checker=unix.Malloc -analyzer-output=plist -o - %s | FileCheck %s

typedef __typeof(sizeof(int)) size_t;
void *malloc(size_t);
void free(void *);
void *realloc(void *ptr, size_t size);

void diagnosticTest(int in) {
    if (in > 5) {
        int *p = malloc(12);
        (*p)++;
    }
    in++; // expected-warning {{leak}}
}

void myArrayAllocation() {
    int **A;
    A = malloc(2*sizeof(int*));
    A[0] = 0;// expected-warning {{leak}}
}

void reallocDiagnostics() {
    char * buf = malloc(100);
    char * tmp;
    tmp = (char*)realloc(buf, 0x1000000);
    if (!tmp) {
        return;// expected-warning {{leak}}
    }
    buf = tmp;
    free(buf);
}

// CHECK: <?xml version="1.0" encoding="UTF-8"?>
// CHECK: <!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
// CHECK: <plist version="1.0">
// CHECK: <dict>
// CHECK:  <key>files</key>
// CHECK:  <array>
// CHECK:  </array>
// CHECK:  <key>diagnostics</key>
// CHECK:  <array>
// CHECK:   <dict>
// CHECK:    <key>path</key>
// CHECK:    <array>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>9</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>9</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>9</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>9</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>9</integer>
// CHECK:       <key>col</key><integer>9</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>9</integer>
// CHECK:          <key>col</key><integer>9</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>9</integer>
// CHECK:          <key>col</key><integer>14</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Assuming &apos;in&apos; is &gt; 5</string>
// CHECK:      <key>message</key>
// CHECK: <string>Assuming &apos;in&apos; is &gt; 5</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>9</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>9</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>18</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>27</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>10</integer>
// CHECK:       <key>col</key><integer>18</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>10</integer>
// CHECK:          <key>col</key><integer>18</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>10</integer>
// CHECK:          <key>col</key><integer>27</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Memory is allocated</string>
// CHECK:      <key>message</key>
// CHECK: <string>Memory is allocated</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>18</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>10</integer>
// CHECK:            <key>col</key><integer>27</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>13</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>13</integer>
// CHECK:            <key>col</key><integer>6</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>13</integer>
// CHECK:       <key>col</key><integer>5</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>13</integer>
// CHECK:          <key>col</key><integer>5</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>13</integer>
// CHECK:          <key>col</key><integer>6</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Memory is never released; potential memory leak</string>
// CHECK:      <key>message</key>
// CHECK: <string>Memory is never released; potential memory leak</string>
// CHECK:     </dict>
// CHECK:    </array>
// CHECK:    <key>description</key><string>Memory is never released; potential memory leak</string>
// CHECK:    <key>category</key><string>Memory Error</string>
// CHECK:    <key>type</key><string>Memory leak</string>
// CHECK:   <key>location</key>
// CHECK:   <dict>
// CHECK:    <key>line</key><integer>13</integer>
// CHECK:    <key>col</key><integer>5</integer>
// CHECK:    <key>file</key><integer>0</integer>
// CHECK:   </dict>
// CHECK:   </dict>
// CHECK:   <dict>
// CHECK:    <key>path</key>
// CHECK:    <array>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>17</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>17</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>30</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>18</integer>
// CHECK:       <key>col</key><integer>9</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>18</integer>
// CHECK:          <key>col</key><integer>9</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>18</integer>
// CHECK:          <key>col</key><integer>30</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Memory is allocated</string>
// CHECK:      <key>message</key>
// CHECK: <string>Memory is allocated</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>18</integer>
// CHECK:            <key>col</key><integer>30</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>20</integer>
// CHECK:            <key>col</key><integer>1</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>20</integer>
// CHECK:            <key>col</key><integer>1</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>20</integer>
// CHECK:       <key>col</key><integer>1</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Memory is never released; potential memory leak</string>
// CHECK:      <key>message</key>
// CHECK: <string>Memory is never released; potential memory leak</string>
// CHECK:     </dict>
// CHECK:    </array>
// CHECK:    <key>description</key><string>Memory is never released; potential memory leak</string>
// CHECK:    <key>category</key><string>Memory Error</string>
// CHECK:    <key>type</key><string>Memory leak</string>
// CHECK:   <key>location</key>
// CHECK:   <dict>
// CHECK:    <key>line</key><integer>20</integer>
// CHECK:    <key>col</key><integer>1</integer>
// CHECK:    <key>file</key><integer>0</integer>
// CHECK:   </dict>
// CHECK:   </dict>
// CHECK:   <dict>
// CHECK:    <key>path</key>
// CHECK:    <array>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>23</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>23</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>23</integer>
// CHECK:            <key>col</key><integer>18</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>23</integer>
// CHECK:            <key>col</key><integer>28</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>23</integer>
// CHECK:       <key>col</key><integer>18</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>23</integer>
// CHECK:          <key>col</key><integer>18</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>23</integer>
// CHECK:          <key>col</key><integer>28</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Memory is allocated</string>
// CHECK:      <key>message</key>
// CHECK: <string>Memory is allocated</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>23</integer>
// CHECK:            <key>col</key><integer>18</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>23</integer>
// CHECK:            <key>col</key><integer>28</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>18</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>40</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>25</integer>
// CHECK:       <key>col</key><integer>18</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>25</integer>
// CHECK:          <key>col</key><integer>18</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>25</integer>
// CHECK:          <key>col</key><integer>40</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Attempt to reallocate memory</string>
// CHECK:      <key>message</key>
// CHECK: <string>Attempt to reallocate memory</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>18</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>25</integer>
// CHECK:            <key>col</key><integer>40</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>6</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>26</integer>
// CHECK:       <key>col</key><integer>5</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>26</integer>
// CHECK:          <key>col</key><integer>5</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>26</integer>
// CHECK:          <key>col</key><integer>6</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Reallocation failed</string>
// CHECK:      <key>message</key>
// CHECK: <string>Reallocation failed</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>5</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>6</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>12</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>26</integer>
// CHECK:       <key>col</key><integer>9</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>26</integer>
// CHECK:          <key>col</key><integer>9</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>26</integer>
// CHECK:          <key>col</key><integer>12</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Assuming &apos;tmp&apos; is null</string>
// CHECK:      <key>message</key>
// CHECK: <string>Assuming &apos;tmp&apos; is null</string>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>control</string>
// CHECK:      <key>edges</key>
// CHECK:       <array>
// CHECK:        <dict>
// CHECK:         <key>start</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>26</integer>
// CHECK:            <key>col</key><integer>12</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:         <key>end</key>
// CHECK:          <array>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>27</integer>
// CHECK:            <key>col</key><integer>9</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:           <dict>
// CHECK:            <key>line</key><integer>27</integer>
// CHECK:            <key>col</key><integer>14</integer>
// CHECK:            <key>file</key><integer>0</integer>
// CHECK:           </dict>
// CHECK:          </array>
// CHECK:        </dict>
// CHECK:       </array>
// CHECK:     </dict>
// CHECK:     <dict>
// CHECK:      <key>kind</key><string>event</string>
// CHECK:      <key>location</key>
// CHECK:      <dict>
// CHECK:       <key>line</key><integer>27</integer>
// CHECK:       <key>col</key><integer>9</integer>
// CHECK:       <key>file</key><integer>0</integer>
// CHECK:      </dict>
// CHECK:      <key>ranges</key>
// CHECK:      <array>
// CHECK:        <array>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>27</integer>
// CHECK:          <key>col</key><integer>9</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:         <dict>
// CHECK:          <key>line</key><integer>27</integer>
// CHECK:          <key>col</key><integer>14</integer>
// CHECK:          <key>file</key><integer>0</integer>
// CHECK:         </dict>
// CHECK:        </array>
// CHECK:      </array>
// CHECK:      <key>extended_message</key>
// CHECK:      <string>Memory is never released; potential memory leak</string>
// CHECK:      <key>message</key>
// CHECK: <string>Memory is never released; potential memory leak</string>
// CHECK:     </dict>
// CHECK:    </array>
// CHECK:    <key>description</key><string>Memory is never released; potential memory leak</string>
// CHECK:    <key>category</key><string>Memory Error</string>
// CHECK:    <key>type</key><string>Memory leak</string>
// CHECK:   <key>location</key>
// CHECK:   <dict>
// CHECK:    <key>line</key><integer>27</integer>
// CHECK:    <key>col</key><integer>9</integer>
// CHECK:    <key>file</key><integer>0</integer>
// CHECK:   </dict>
// CHECK:   </dict>
// CHECK:  </array>
// CHECK: </dict>
// CHECK: </plist>
