/**
 * This example shows how to use class list
 */

#include <MaterializeLayout.h>

MaterializeLayout page("Class list example");

void setup()
{
  Serial.begin(115200);
  Serial.print("\n\n\n");

  delay(5000);

  /**
   * The classList property has every element that is inherited from HTMLElement class
   */
  auto rootLayout = page.createAndAppendComponent<Layout>();

  /**
   * Class list is used to operate with element's css classes (add or remove them)
   * class list has next methods:
   *  add: adds class to the element's class list if it wasn't added before
   *  contains: returns 'true' if the passed class is found in the class list
   *  item: returns class name which is kept by specified index, returns an empty string if error occurs
   *  remove: removes class from the class list
   *  replace: replaces specified class name with provided ones
   *  toggle: adds class name if it isn't in class list, and adds if not
   *  value: returns string containing element's classes separated with space(' ') and ready to be inserted into HTML template
   */

  /**
   * Let's add classes 'my-class1' and 'my-class2' to the rootLayout and see what result we'll get
   */
  rootLayout->classList.add("my-class1");
  rootLayout->classList.add("my-class2");

  /**
   * Print rootLayout css classes
   */
  Serial.println("After addition: " + rootLayout->classList.value());

  /**
   * Check if rootLayout contains 'my-class1' and 'my-class3' classes
   */
  Serial.println("rootLayout contains 'my-class1' class: " + String(rootLayout->classList.contains("my-class1")));
  Serial.println("rootLayout contains 'my-class3' class: " + String(rootLayout->classList.contains("my-class3")));

  /**
   * Get class that is kept under index 1
   */
  Serial.println("rootLayout under index 1 has the following class: " + rootLayout->classList.item(1));

  /**
   * Remove 'my-class1' from the class list and print the result
   */
  rootLayout->classList.remove("my-class1");
  Serial.println("After deletion: " + rootLayout->classList.value());

  /**
   * Return 'my-class1' back and replace 'my-class2' with 'my-class3' then print the result
   */
  rootLayout->classList.add("my-class1");
  Serial.println("After addition: " + rootLayout->classList.value());
  rootLayout->classList.replace("my-class2", "my-class3");
  Serial.println("After replacement: " + rootLayout->classList.value());

  /**
   * Let's toggle 'my-class2' and 'my-class3'
   */
  rootLayout->classList.toggle("my-class2");
  Serial.println("After 'my-class2' toggle: " + rootLayout->classList.value());
  rootLayout->classList.toggle("my-class3");
  Serial.println("After 'my-class3' toggle: " + rootLayout->classList.value());
}

void loop()
{
}