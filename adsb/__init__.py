import streampu as spu

import sys
import os

# Add the build directory to Python path for compiled modules
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "build"))

import _cpp

from .sequence_runner import SequenceRunner

def filter(x, h, complex_data=False):
    Lx = len(x)
    if complex_data:
        Lx //= 2
    filter_class_name = f"Filter_FIR_{x.dtype.name}_{"C" if complex_data else "R"}"
    flt = getattr(_cpp, filter_class_name)(Lx, h)
    spu._context_manager.store_task(flt.filter)
    return flt.filter(x)


def abs2(x):
    Lx = len(x) // 2
    abs2_class_name = f"Abs2_{x.dtype.name}"
    abs2_inst = getattr(_cpp, abs2_class_name)(Lx)
    spu._context_manager.store_task(abs2_inst.perform)
    return abs2_inst.perform(x)


def delay(x, d, complex_data=False):
    Lx = len(x)
    if complex_data:
        Lx //= 2
    delay_class_name = f"Filter_delay_{x.dtype.name}_{"C" if complex_data else "R"}"
    delay_inst = getattr(_cpp, delay_class_name)(Lx, d)
    spu._context_manager.store_task(delay_inst.filter)
    return delay_inst.filter(x)


def multirate_split(x, M, complex_data=False):
    Lx = len(x)
    if complex_data:
        Lx //= 2
    mlt_class_name = f"Multirate_{x.dtype.name}_{"C" if complex_data else "R"}"
    mlt_inst = getattr(_cpp, mlt_class_name)(Lx, M)
    spu._context_manager.store_task(mlt_inst.split)
    return mlt_inst.split(x)


def multirate_merge(x_list, complex_data=False):
    Lx = len(x_list[0])
    if complex_data:
        Lx //= 2
    mlt_class_name = f"Multirate_{x_list[0].dtype.name}_{"C" if complex_data else "R"}"
    mlt_inst = getattr(_cpp, mlt_class_name)(Lx, len(x_list))
    spu._context_manager.store_task(mlt_inst.merge)
    return mlt_inst.merge(x_list)
