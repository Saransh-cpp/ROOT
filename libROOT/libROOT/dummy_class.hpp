#ifndef DUMMY_CLASS_HPP
#define DUMMY_CLASS_HPP

/**
 * \class DummyClass
 * \brief A cool dummy class to check whether Doxygen is working properly
 *
 * Let's rock, rock, rock.
 */
class DummyClass {
  public:
    /**
     * \brief Default constructor
     */
    DummyClass();

    /**
     * \brief Returns something dummy
     * \return Always returns 42
     */
    int return_dummy_value() const;
};

#endif
