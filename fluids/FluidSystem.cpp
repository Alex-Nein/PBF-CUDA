#include "FluidSystem.h"
#include "FixedCubeSource.h"
#include "FluidParams.h"

FluidSystem::FluidSystem()
{
	m_tictoc = 0;
	m_nextFrame = false;

	/* Initialize Component */
	FluidParams fluidParams;
	fluidParams.g = 9.8f,
	fluidParams.h = .1f,
	fluidParams.dt = 0.0083f,
	fluidParams.pho0 = 8000.f,
	fluidParams.lambda_eps = 1000.f,
	fluidParams.delta_q = 0.3 * fluidParams.h,
	fluidParams.k_corr = 0.001f,
	fluidParams.n_corr = 4,
	fluidParams.k_boundaryDensity = 50.f,
	fluidParams.c_XSPH = 0.01f;
	fluidParams.niter = 4;

	const float3 ulim = make_float3(1.5f, 1.5f, 3.f), llim = make_float3(-1.5f, -1.5f, 0.f);

	m_simulator = new Simulator(fluidParams, ulim, llim);
	m_renderer = new SimpleRenderer(fluidParams, ulim, llim, [this]() { m_nextFrame = true; });
	m_source = new FixedCubeSource(
		/* limits */  make_float3(1.f, 1.f, 2.f), make_float3(-1.f, -1.f, 1.f),
		/* numbers */ make_int3(20, 20, 10));
	m_nparticle = 20 * 20 * 10;

	/* Initialize vertex buffer */
	glGenBuffers(1, &d_pos);
	glGenBuffers(1, &d_npos);
	glGenBuffers(1, &d_vel);
	glGenBuffers(1, &d_nvel);
	glGenBuffers(1, &d_iid);
	glGenBuffers(1, &d_niid);

	glBindBuffer(GL_ARRAY_BUFFER, d_pos);
	glBufferData(GL_ARRAY_BUFFER,  MAX_PARTICLE_NUM * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
	checkGLErr();
	glBindBuffer(GL_ARRAY_BUFFER, d_npos);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
	checkGLErr();
	glBindBuffer(GL_ARRAY_BUFFER, d_vel);
	glBufferData(GL_ARRAY_BUFFER,  MAX_PARTICLE_NUM * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
	checkGLErr();
	glBindBuffer(GL_ARRAY_BUFFER, d_nvel);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
	checkGLErr();
	glBindBuffer(GL_ARRAY_BUFFER, d_iid);
	glBufferData(GL_ARRAY_BUFFER,  MAX_PARTICLE_NUM * sizeof(uint),   NULL, GL_DYNAMIC_DRAW);
	checkGLErr();
	glBindBuffer(GL_ARRAY_BUFFER, d_niid);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * sizeof(uint),   NULL, GL_DYNAMIC_DRAW);
	checkGLErr();
}

void FluidSystem::initSource() 
{
	m_source->initialize(d_pos, d_vel, d_iid, MAX_PARTICLE_NUM);
}

void FluidSystem::stepSource() {
	if (!m_tictoc)
		m_nparticle = m_source->update(d_pos,  d_vel,  d_iid,  MAX_PARTICLE_NUM);
	else 
		m_nparticle = m_source->update(d_npos, d_nvel, d_niid, MAX_PARTICLE_NUM);
}

void FluidSystem::stepSimulate() {
	if (!(m_renderer->m_input->running || m_nextFrame)) return;

	m_simulator->loadParams(m_renderer->m_input->fluidParams);

	if (!m_tictoc)
		m_simulator->step(d_pos, d_npos, d_vel, d_nvel, d_iid, d_niid, m_nparticle);
	else 
		m_simulator->step(d_npos, d_pos, d_nvel, d_vel, d_niid, d_iid, m_nparticle);
	
	m_tictoc = !m_tictoc;
	m_nextFrame = false;
}

void FluidSystem::render() {
	if (!m_tictoc)
		m_renderer->render(d_pos, m_nparticle);
	else
		m_renderer->render(d_npos, m_nparticle);
}

FluidSystem::~FluidSystem()
{
	if (m_renderer) delete m_renderer;
	if (m_simulator) delete m_simulator;
	if (m_source) delete m_source;
}
