cmake:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build/
	rm -rf *.so
	rm -rf *.egg-info/
	find . -name "*.pyc" -delete
	find . -name "__pycache__" -type d -exec rm -rf {} +

test:
	python coref/test.py

rebuild:
	make clean
	make cmake
	python setup.py build_ext --inplace 

publish:
	python -c "import toml; d=toml.load('pyproject.toml'); v=d['project']['version'].split('.'); v[-1]=str(int(v[-1])+1); d['project']['version']='.'.join(v); open('pyproject.toml','w').write(toml.dumps(d))"
	poetry publish --build
	