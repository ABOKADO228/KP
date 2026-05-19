
SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', 'public', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 5 (class 2615 OID 24577)
-- Name: farm_association; Type: SCHEMA; Schema: -; Owner: admin
--


--
-- TOC entry 255 (class 1259 OID 24742)
-- Name: association_employee_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.association_employee_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 226 (class 1259 OID 24619)
-- Name: association_employee; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.association_employee (
    id integer DEFAULT nextval('public.association_employee_id_seq'::regclass) NOT NULL,
    person_id integer NOT NULL,
    association_id integer NOT NULL,
    role_id smallint NOT NULL,
    employment_status_id smallint NOT NULL,
    hire_date date NOT NULL,
    dismissal_date date,
    salary numeric(10,2),
    contract_number character varying(50)
);



--
-- TOC entry 227 (class 1259 OID 24622)
-- Name: association_farms; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.association_farms (
    farm_id integer NOT NULL,
    association_id integer NOT NULL,
    join_date date NOT NULL,
    status character varying(9),
    notes text
);



--
-- TOC entry 250 (class 1259 OID 24732)
-- Name: association_member_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.association_member_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 228 (class 1259 OID 24627)
-- Name: association_member; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.association_member (
    id integer DEFAULT nextval('public.association_member_id_seq'::regclass) NOT NULL,
    association_id integer NOT NULL,
    person_id integer NOT NULL,
    membership_number character varying(20),
    joined_date date NOT NULL,
    status character varying(8),
    notes text
);



--
-- TOC entry 215 (class 1259 OID 24578)
-- Name: association_role; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.association_role (
    id smallint NOT NULL,
    code character varying(20),
    name character varying(50),
    description character varying(255)
);



--
-- TOC entry 259 (class 1259 OID 24750)
-- Name: contract_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.contract_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 229 (class 1259 OID 24632)
-- Name: contract; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.contract (
    id integer DEFAULT nextval('public.contract_id_seq'::regclass) NOT NULL,
    supplier_id integer,
    farm_id integer,
    association_id integer NOT NULL,
    contract_number character varying(50),
    sign_date date NOT NULL,
    start_date date NOT NULL,
    end_date date,
    status character varying(10),
    description character varying(255)
);



--
-- TOC entry 256 (class 1259 OID 24744)
-- Name: employee_plot_assignment_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.employee_plot_assignment_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 242 (class 1259 OID 24677)
-- Name: employee_plot_assignment; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.employee_plot_assignment (
    id integer DEFAULT nextval('public.employee_plot_assignment_id_seq'::regclass) NOT NULL,
    farm_employee_id integer NOT NULL,
    farm_plot_id integer NOT NULL,
    assignment_type character varying(9),
    assigned_at date NOT NULL,
    unassigned_at date,
    notes text
);



--
-- TOC entry 216 (class 1259 OID 24581)
-- Name: employment_status; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.employment_status (
    id smallint NOT NULL,
    name character varying(30)
);



--
-- TOC entry 247 (class 1259 OID 24726)
-- Name: farm_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 217 (class 1259 OID 24584)
-- Name: farm; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm (
    id integer DEFAULT nextval('public.farm_id_seq'::regclass) NOT NULL,
    name character varying(100),
    legal_name character varying(200),
    legal_address character varying(300),
    actual_address character varying(300),
    inn character varying(12),
    ogrn character varying(15),
    status character varying(9),
    farm_type character varying(23)
);



--
-- TOC entry 246 (class 1259 OID 24724)
-- Name: farm_association_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_association_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 218 (class 1259 OID 24589)
-- Name: farm_association; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_association (
    id integer DEFAULT nextval('public.farm_association_id_seq'::regclass) NOT NULL,
    inn character varying(12),
    ogrn character varying(15),
    name character varying(200),
    legal_address character varying(300),
    status character varying(9)
);



--
-- TOC entry 254 (class 1259 OID 24740)
-- Name: farm_employee_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_employee_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 230 (class 1259 OID 24635)
-- Name: farm_employee; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_employee (
    id integer DEFAULT nextval('public.farm_employee_id_seq'::regclass) NOT NULL,
    person_id integer NOT NULL,
    farm_id integer NOT NULL,
    role_id smallint NOT NULL,
    employment_status_id smallint NOT NULL,
    hire_date date NOT NULL,
    dismissal_date date,
    salary numeric(10,2),
    employment_contract_number character varying(50),
    is_primary_workplace integer
);



--
-- TOC entry 266 (class 1259 OID 24762)
-- Name: farm_inspection_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_inspection_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 251 (class 1259 OID 24734)
-- Name: farm_owner_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_owner_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 231 (class 1259 OID 24638)
-- Name: farm_owner; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_owner (
    id integer DEFAULT nextval('public.farm_owner_id_seq'::regclass) NOT NULL,
    person_id integer NOT NULL,
    status character varying(8),
    rating numeric(5,2)
);



--
-- TOC entry 257 (class 1259 OID 24746)
-- Name: farm_ownership_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_ownership_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 232 (class 1259 OID 24641)
-- Name: farm_ownership; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_ownership (
    id integer DEFAULT nextval('public.farm_ownership_id_seq'::regclass) NOT NULL,
    farm_id integer NOT NULL,
    farm_owner_id integer NOT NULL,
    ownership_percentage numeric(5,2),
    started_at date NOT NULL,
    ended_at date
);



--
-- TOC entry 253 (class 1259 OID 24738)
-- Name: farm_plot_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.farm_plot_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 233 (class 1259 OID 24644)
-- Name: farm_plot; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_plot (
    id integer DEFAULT nextval('public.farm_plot_id_seq'::regclass) NOT NULL,
    farm_plot_type_id integer,
    name character varying(100),
    area numeric(10,2),
    location character varying(300),
    cadastral_number character varying(50),
    status character varying(9)
);



--
-- TOC entry 234 (class 1259 OID 24647)
-- Name: farm_plot_assignment; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_plot_assignment (
    farm_id integer NOT NULL,
    farm_plot_id integer NOT NULL,
    status character varying(9),
    notes text
);



--
-- TOC entry 243 (class 1259 OID 24682)
-- Name: farm_plot_consumption_product; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_plot_consumption_product (
    product_id integer NOT NULL,
    farm_plot_id integer NOT NULL,
    quantity integer NOT NULL,
    consumption_now integer NOT NULL
);



--
-- TOC entry 244 (class 1259 OID 24685)
-- Name: farm_plot_production_product; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_plot_production_product (
    product_id integer NOT NULL,
    farm_plot_id integer NOT NULL,
    quantity integer NOT NULL,
    production_now integer NOT NULL
);



--
-- TOC entry 219 (class 1259 OID 24594)
-- Name: farm_plot_type; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_plot_type (
    id integer NOT NULL,
    name character varying(128),
    description character varying(255),
    farm_plot_level integer NOT NULL,
    parent_id integer
);



--
-- TOC entry 220 (class 1259 OID 24597)
-- Name: farm_role; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.farm_role (
    id smallint NOT NULL,
    name character varying(50),
    description character varying(255)
);



--
-- TOC entry 221 (class 1259 OID 24600)
-- Name: identity_document_type; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.identity_document_type (
    id smallint NOT NULL,
    code character varying(20),
    name character varying(50),
    description character varying(255)
);



--
-- TOC entry 265 (class 1259 OID 24761)
-- Name: inspection_result_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.inspection_result_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 264 (class 1259 OID 24760)
-- Name: inspection_type_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.inspection_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 245 (class 1259 OID 24722)
-- Name: person_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.person_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 222 (class 1259 OID 24603)
-- Name: person; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.person (
    id integer DEFAULT nextval('public.person_id_seq'::regclass) NOT NULL,
    first_name character varying(100),
    last_name character varying(100),
    middle_name character varying(100),
    birth_date date,
    phone character varying(20),
    email character varying(100),
    address character varying(300)
);



--
-- TOC entry 249 (class 1259 OID 24730)
-- Name: person_document_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.person_document_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 235 (class 1259 OID 24652)
-- Name: person_document; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.person_document (
    id integer DEFAULT nextval('public.person_document_id_seq'::regclass) NOT NULL,
    person_id integer NOT NULL,
    document_type_id smallint NOT NULL,
    document_number character varying(50),
    issued_by character varying(200),
    issued_date date,
    expiration_date date,
    is_primary integer
);



--
-- TOC entry 252 (class 1259 OID 24736)
-- Name: product_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.product_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 236 (class 1259 OID 24655)
-- Name: product; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.product (
    id integer DEFAULT nextval('public.product_id_seq'::regclass) NOT NULL,
    type_id integer NOT NULL,
    name character varying(100),
    unit_id smallint
);



--
-- TOC entry 223 (class 1259 OID 24608)
-- Name: product_type; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.product_type (
    id integer NOT NULL,
    parent_id integer,
    sku character varying(128),
    product_level integer NOT NULL,
    name character varying(128),
    description character varying(255)
);



--
-- TOC entry 260 (class 1259 OID 24752)
-- Name: purchase_order_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.purchase_order_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 237 (class 1259 OID 24658)
-- Name: purchase_order; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.purchase_order (
    id integer DEFAULT nextval('public.purchase_order_id_seq'::regclass) NOT NULL,
    association_id integer NOT NULL,
    supplier_id integer NOT NULL,
    delivery_address character varying(300),
    order_date date NOT NULL,
    expected_delivery_date date,
    status character varying(9),
    total_amount numeric(12,2),
    received_at date,
    created_by integer
);



--
-- TOC entry 261 (class 1259 OID 24754)
-- Name: purchase_order_item_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.purchase_order_item_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 238 (class 1259 OID 24661)
-- Name: purchase_order_item; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.purchase_order_item (
    id integer DEFAULT nextval('public.purchase_order_item_id_seq'::regclass) NOT NULL,
    purchase_order_id integer NOT NULL,
    product_id integer NOT NULL,
    quantity numeric(12,3),
    unit_price numeric(10,2),
    vat_rate numeric(4,2),
    currency character varying(3)
);



--
-- TOC entry 262 (class 1259 OID 24756)
-- Name: purchase_requisition_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.purchase_requisition_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 239 (class 1259 OID 24664)
-- Name: purchase_requisition; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.purchase_requisition (
    id integer DEFAULT nextval('public.purchase_requisition_id_seq'::regclass) NOT NULL,
    farm_id integer NOT NULL,
    product_id integer NOT NULL,
    quantity integer NOT NULL,
    max_price_per_unit numeric(12,2),
    offer_date date,
    required_date date,
    priority integer,
    valid_until date,
    status character varying(10),
    notes text
);



--
-- TOC entry 263 (class 1259 OID 24758)
-- Name: sales_requisition_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.sales_requisition_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 240 (class 1259 OID 24669)
-- Name: sales_requisition; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.sales_requisition (
    id integer DEFAULT nextval('public.sales_requisition_id_seq'::regclass) NOT NULL,
    farm_id integer,
    product_id integer NOT NULL,
    quantity integer NOT NULL,
    price_per_unit numeric(12,2),
    offer_date date NOT NULL,
    valid_until date,
    status character varying(10),
    notes text
);



--
-- TOC entry 248 (class 1259 OID 24728)
-- Name: supplier_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.supplier_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 224 (class 1259 OID 24613)
-- Name: supplier; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.supplier (
    id integer DEFAULT nextval('public.supplier_id_seq'::regclass) NOT NULL,
    name character varying(100),
    legal_address character varying(300),
    status character varying(9)
);



--
-- TOC entry 258 (class 1259 OID 24748)
-- Name: supplier_product_price_id_seq; Type: SEQUENCE; Schema: farm_association; Owner: admin
--

CREATE SEQUENCE public.supplier_product_price_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;



--
-- TOC entry 241 (class 1259 OID 24674)
-- Name: supplier_product_price; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.supplier_product_price (
    id integer DEFAULT nextval('public.supplier_product_price_id_seq'::regclass) NOT NULL,
    supplier_id integer NOT NULL,
    product_id integer NOT NULL,
    purchase_price numeric(10,2),
    valid_from date NOT NULL,
    valid_until date
);



--
-- TOC entry 225 (class 1259 OID 24616)
-- Name: unit; Type: TABLE; Schema: farm_association; Owner: admin
--

CREATE TABLE public.unit (
    id smallint NOT NULL,
    code character varying(10),
    name character varying(50)
);



--
-- TOC entry 3683 (class 0 OID 24619)
-- Dependencies: 226
-- Data for Name: association_employee; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.association_employee (id, person_id, association_id, role_id, employment_status_id, hire_date, dismissal_date, salary, contract_number) FROM stdin;
1	12	1	1	1	2024-05-18	\N	78271.49	AE-01-0001
2	23	2	2	1	2019-07-14	\N	73413.87	AE-02-0002
3	34	3	3	1	2018-12-27	\N	173232.51	AE-03-0003
4	45	4	4	1	2017-05-05	\N	176631.98	AE-04-0004
5	56	5	5	4	2023-02-17	\N	186928.53	AE-05-0005
6	67	6	6	1	2024-07-22	\N	148688.83	AE-06-0006
7	78	7	7	1	2023-07-20	\N	117323.87	AE-07-0007
8	89	8	8	1	2023-03-17	\N	105128.41	AE-08-0008
9	100	1	1	1	2014-04-12	\N	142439.78	AE-01-0009
10	111	2	2	1	2018-11-21	\N	55570.36	AE-02-0010
11	122	3	3	1	2016-01-14	\N	37469.18	AE-03-0011
12	133	4	4	1	2017-08-18	\N	144012.20	AE-04-0012
13	144	5	5	3	2014-07-19	\N	186345.22	AE-05-0013
14	155	6	6	1	2017-02-09	\N	170500.85	AE-06-0014
15	166	7	7	1	2024-11-24	\N	170090.53	AE-07-0015
16	177	8	8	1	2014-01-12	\N	168359.31	AE-08-0016
17	188	1	1	1	2021-06-20	\N	117292.65	AE-01-0017
18	199	2	2	1	2020-01-20	\N	143198.52	AE-02-0018
19	210	3	3	1	2025-01-12	\N	158144.21	AE-03-0019
20	221	4	4	2	2015-10-26	\N	131170.56	AE-04-0020
21	232	5	5	1	2015-03-26	\N	114784.15	AE-05-0021
22	243	6	6	4	2022-03-01	\N	172636.58	AE-06-0022
23	4	7	7	1	2022-03-14	\N	107129.70	AE-07-0023
24	15	8	8	1	2019-08-22	\N	62166.45	AE-08-0024
25	26	1	1	1	2015-07-03	\N	121917.52	AE-01-0025
26	37	2	2	1	2020-06-25	\N	187094.00	AE-02-0026
27	48	3	3	1	2016-07-19	\N	135600.95	AE-03-0027
28	59	4	4	1	2018-09-15	\N	151741.89	AE-04-0028
29	70	5	5	1	2021-03-10	\N	89670.31	AE-05-0029
30	81	6	6	1	2023-06-27	\N	77957.53	AE-06-0030
31	92	7	7	1	2021-09-29	\N	105123.65	AE-07-0031
32	103	8	8	1	2016-10-02	\N	107875.40	AE-08-0032
33	114	1	1	1	2020-07-22	\N	105056.65	AE-01-0033
34	125	2	2	1	2015-06-22	\N	64194.81	AE-02-0034
35	136	3	3	4	2020-06-03	\N	47426.41	AE-03-0035
\.


--
-- TOC entry 3684 (class 0 OID 24622)
-- Dependencies: 227
-- Data for Name: association_farms; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.association_farms (farm_id, association_id, join_date, status, notes) FROM stdin;
1	1	2022-10-15	active	\N
2	2	2022-01-29	active	\N
3	3	2023-04-26	active	\N
4	4	2022-01-26	active	\N
5	5	2018-04-08	inactive	Ферма 5 включена в программу развития
6	6	2019-06-19	active	\N
7	7	2021-11-15	inactive	\N
8	8	2020-10-27	suspended	\N
9	1	2024-08-20	active	\N
10	2	2022-05-02	inactive	Ферма 10 включена в программу развития
11	3	2019-07-24	active	\N
12	4	2021-12-23	suspended	\N
13	5	2017-04-29	active	\N
14	6	2025-05-31	inactive	\N
15	7	2024-02-26	active	Ферма 15 включена в программу развития
16	8	2019-07-29	inactive	\N
17	1	2020-08-22	suspended	\N
18	2	2019-05-27	active	\N
19	3	2025-03-19	inactive	\N
20	4	2024-04-12	active	Ферма 20 включена в программу развития
21	5	2024-09-26	active	\N
22	6	2017-06-22	active	\N
23	7	2024-07-19	suspended	\N
24	8	2022-12-02	active	\N
25	1	2022-05-11	active	Ферма 25 включена в программу развития
26	2	2022-09-08	active	\N
27	3	2023-09-16	active	\N
28	4	2016-11-21	active	\N
29	5	2021-05-06	active	\N
30	6	2025-06-27	active	Ферма 30 включена в программу развития
31	7	2023-12-26	active	\N
32	8	2018-01-23	inactive	\N
33	1	2021-07-05	active	\N
34	2	2017-11-01	active	\N
35	3	2016-05-12	active	Ферма 35 включена в программу развития
36	4	2016-03-02	inactive	\N
37	5	2020-08-18	active	\N
38	6	2023-07-12	suspended	\N
39	7	2024-06-30	active	\N
40	8	2017-12-26	inactive	Ферма 40 включена в программу развития
41	1	2020-12-04	inactive	\N
42	2	2021-06-29	active	\N
43	3	2025-09-20	active	\N
44	4	2018-10-07	active	\N
45	5	2024-09-14	active	Ферма 45 включена в программу развития
46	6	2024-03-30	active	\N
47	7	2024-01-28	active	\N
48	8	2022-11-09	suspended	\N
49	1	2022-05-31	active	\N
50	2	2021-11-21	active	Ферма 50 включена в программу развития
51	3	2023-11-24	active	\N
52	4	2021-11-02	active	\N
53	5	2021-02-14	active	\N
54	6	2016-06-22	inactive	\N
55	7	2018-10-03	active	Ферма 55 включена в программу развития
56	8	2020-06-08	suspended	\N
57	1	2016-06-09	active	\N
58	2	2021-10-15	active	\N
59	3	2016-07-20	active	\N
60	4	2020-12-05	active	Ферма 60 включена в программу развития
\.


--
-- TOC entry 3685 (class 0 OID 24627)
-- Dependencies: 228
-- Data for Name: association_member; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.association_member (id, association_id, person_id, membership_number, joined_date, status, notes) FROM stdin;
1	1	54	MEM-00001	2025-10-10	active	\N
2	2	116	MEM-00002	2023-09-13	active	\N
3	3	144	MEM-00003	2019-05-04	active	\N
4	4	53	MEM-00004	2021-03-02	active	Заметка по членству #4
5	5	164	MEM-00005	2015-12-28	inactive	\N
6	6	167	MEM-00006	2021-07-13	active	\N
7	7	178	MEM-00007	2019-12-13	active	\N
8	8	101	MEM-00008	2015-07-10	active	Заметка по членству #8
9	1	14	MEM-00009	2016-06-04	active	\N
10	2	27	MEM-00010	2015-01-30	active	\N
11	3	122	MEM-00011	2023-05-31	expelled	\N
12	4	78	MEM-00012	2023-04-15	active	Заметка по членству #12
13	5	15	MEM-00013	2019-10-12	active	\N
14	6	66	MEM-00014	2022-06-02	active	\N
15	7	80	MEM-00015	2024-10-02	active	\N
16	8	174	MEM-00016	2019-01-22	active	Заметка по членству #16
17	1	159	MEM-00017	2018-01-13	active	\N
18	2	47	MEM-00018	2014-10-01	active	\N
19	3	49	MEM-00019	2021-02-23	active	\N
20	4	176	MEM-00020	2016-05-07	active	Заметка по членству #20
21	5	158	MEM-00021	2017-10-25	inactive	\N
22	6	82	MEM-00022	2017-08-11	active	\N
23	7	152	MEM-00023	2016-02-02	active	\N
24	8	156	MEM-00024	2024-10-29	active	Заметка по членству #24
25	1	59	MEM-00025	2023-09-04	expelled	\N
26	2	42	MEM-00026	2017-09-13	active	\N
27	3	130	MEM-00027	2023-09-27	active	\N
28	4	155	MEM-00028	2016-04-09	active	Заметка по членству #28
29	5	18	MEM-00029	2018-04-06	active	\N
30	6	87	MEM-00030	2019-12-14	active	\N
31	7	179	MEM-00031	2019-11-28	active	\N
32	8	37	MEM-00032	2019-06-28	active	Заметка по членству #32
33	1	105	MEM-00033	2025-01-04	active	\N
34	2	28	MEM-00034	2021-09-05	active	\N
35	3	170	MEM-00035	2015-12-19	active	\N
36	4	67	MEM-00036	2024-10-09	active	Заметка по членству #36
37	5	57	MEM-00037	2018-12-15	active	\N
38	6	180	MEM-00038	2020-07-18	deceased	\N
39	7	113	MEM-00039	2024-08-15	inactive	\N
40	8	7	MEM-00040	2020-12-01	active	Заметка по членству #40
41	1	177	MEM-00041	2024-07-24	active	\N
42	2	79	MEM-00042	2019-12-11	active	\N
43	3	65	MEM-00043	2022-09-10	active	\N
44	4	117	MEM-00044	2024-07-26	active	Заметка по членству #44
45	5	96	MEM-00045	2023-12-31	active	\N
46	6	131	MEM-00046	2023-11-12	active	\N
47	7	34	MEM-00047	2019-01-28	inactive	\N
48	8	142	MEM-00048	2015-05-20	active	Заметка по членству #48
49	1	58	MEM-00049	2016-03-03	active	\N
50	2	60	MEM-00050	2021-09-17	inactive	\N
51	3	38	MEM-00051	2023-04-15	deceased	\N
52	4	10	MEM-00052	2019-02-07	active	Заметка по членству #52
53	5	2	MEM-00053	2022-09-07	inactive	\N
54	6	86	MEM-00054	2023-05-23	expelled	\N
55	7	107	MEM-00055	2024-08-25	active	\N
56	8	56	MEM-00056	2021-02-01	active	Заметка по членству #56
57	1	157	MEM-00057	2015-10-09	active	\N
58	2	109	MEM-00058	2014-10-12	active	\N
59	3	81	MEM-00059	2025-08-04	inactive	\N
60	4	175	MEM-00060	2017-04-22	active	Заметка по членству #60
61	5	22	MEM-00061	2024-01-27	active	\N
62	6	151	MEM-00062	2017-10-01	deceased	\N
63	7	138	MEM-00063	2018-04-28	active	\N
64	8	26	MEM-00064	2023-07-01	active	Заметка по членству #64
65	1	43	MEM-00065	2025-11-26	active	\N
66	2	1	MEM-00066	2017-04-26	inactive	\N
67	3	62	MEM-00067	2019-10-14	active	\N
68	4	112	MEM-00068	2015-07-03	active	Заметка по членству #68
69	5	3	MEM-00069	2022-11-30	active	\N
70	6	97	MEM-00070	2020-03-12	active	\N
71	7	139	MEM-00071	2025-10-20	active	\N
72	8	160	MEM-00072	2025-09-20	active	Заметка по членству #72
73	1	12	MEM-00073	2016-07-10	active	\N
74	2	137	MEM-00074	2017-04-29	active	\N
75	3	150	MEM-00075	2016-03-31	expelled	\N
76	4	133	MEM-00076	2016-04-07	active	Заметка по членству #76
77	5	40	MEM-00077	2021-09-17	inactive	\N
78	6	70	MEM-00078	2019-12-04	active	\N
79	7	136	MEM-00079	2025-05-18	active	\N
80	8	25	MEM-00080	2024-08-11	active	Заметка по членству #80
81	1	148	MEM-00081	2020-03-06	active	\N
82	2	19	MEM-00082	2023-01-04	active	\N
83	3	84	MEM-00083	2021-06-08	active	\N
84	4	73	MEM-00084	2015-02-08	active	Заметка по членству #84
85	5	88	MEM-00085	2020-01-10	inactive	\N
86	6	120	MEM-00086	2014-06-15	active	\N
87	7	90	MEM-00087	2020-10-18	active	\N
88	8	94	MEM-00088	2025-01-10	inactive	Заметка по членству #88
89	1	132	MEM-00089	2016-03-15	deceased	\N
90	2	149	MEM-00090	2019-02-11	active	\N
91	3	103	MEM-00091	2023-12-09	active	\N
92	4	93	MEM-00092	2022-04-06	active	Заметка по членству #92
93	5	143	MEM-00093	2023-05-08	active	\N
94	6	165	MEM-00094	2016-01-02	active	\N
95	7	127	MEM-00095	2018-09-28	inactive	\N
96	8	74	MEM-00096	2023-02-18	active	Заметка по членству #96
97	1	121	MEM-00097	2025-08-02	inactive	\N
98	2	134	MEM-00098	2015-04-06	active	\N
99	3	169	MEM-00099	2017-07-26	active	\N
100	4	95	MEM-00100	2025-05-14	active	Заметка по членству #100
101	5	69	MEM-00101	2025-10-04	inactive	\N
102	6	126	MEM-00102	2014-08-26	active	\N
103	7	31	MEM-00103	2018-07-03	inactive	\N
104	8	172	MEM-00104	2019-08-06	active	Заметка по членству #104
105	1	171	MEM-00105	2018-08-24	inactive	\N
106	2	168	MEM-00106	2018-06-20	active	\N
107	3	21	MEM-00107	2024-05-30	active	\N
108	4	30	MEM-00108	2024-04-04	expelled	Заметка по членству #108
109	5	75	MEM-00109	2019-09-16	active	\N
110	6	146	MEM-00110	2024-12-18	active	\N
111	7	46	MEM-00111	2017-06-22	expelled	\N
112	8	68	MEM-00112	2024-08-23	active	Заметка по членству #112
113	1	61	MEM-00113	2021-11-05	active	\N
114	2	118	MEM-00114	2019-10-20	active	\N
115	3	162	MEM-00115	2016-01-11	active	\N
116	4	140	MEM-00116	2022-09-02	active	Заметка по членству #116
117	5	100	MEM-00117	2018-10-22	inactive	\N
118	6	104	MEM-00118	2019-01-19	active	\N
119	7	153	MEM-00119	2020-01-14	active	\N
120	8	124	MEM-00120	2025-07-06	active	Заметка по членству #120
121	1	29	MEM-00121	2014-03-14	active	\N
122	2	77	MEM-00122	2018-07-15	active	\N
123	3	41	MEM-00123	2014-03-11	active	\N
124	4	4	MEM-00124	2020-11-07	active	Заметка по членству #124
125	5	173	MEM-00125	2023-06-25	active	\N
126	6	35	MEM-00126	2023-07-04	inactive	\N
127	7	50	MEM-00127	2021-10-27	active	\N
128	8	83	MEM-00128	2018-11-23	active	Заметка по членству #128
129	1	129	MEM-00129	2025-09-05	inactive	\N
130	2	89	MEM-00130	2024-11-19	active	\N
131	3	125	MEM-00131	2014-03-02	active	\N
132	4	145	MEM-00132	2022-05-24	active	Заметка по членству #132
133	5	92	MEM-00133	2020-05-04	active	\N
134	6	161	MEM-00134	2019-08-16	active	\N
135	7	5	MEM-00135	2021-11-12	deceased	\N
136	8	64	MEM-00136	2021-04-17	inactive	Заметка по членству #136
137	1	135	MEM-00137	2019-11-10	active	\N
138	2	52	MEM-00138	2020-09-13	active	\N
139	3	108	MEM-00139	2025-08-08	active	\N
140	4	36	MEM-00140	2015-04-11	inactive	Заметка по членству #140
141	5	99	MEM-00141	2023-01-24	active	\N
142	6	154	MEM-00142	2017-02-20	active	\N
143	7	23	MEM-00143	2025-02-26	deceased	\N
144	8	16	MEM-00144	2019-10-27	inactive	Заметка по членству #144
145	1	123	MEM-00145	2023-01-28	active	\N
146	2	141	MEM-00146	2014-10-17	active	\N
147	3	106	MEM-00147	2018-04-05	active	\N
148	4	163	MEM-00148	2018-12-27	active	Заметка по членству #148
149	5	76	MEM-00149	2024-09-02	inactive	\N
150	6	63	MEM-00150	2022-01-15	active	\N
151	7	9	MEM-00151	2017-10-24	inactive	\N
152	8	39	MEM-00152	2018-01-23	active	Заметка по членству #152
153	1	115	MEM-00153	2024-07-23	active	\N
154	2	72	MEM-00154	2019-10-20	active	\N
155	3	71	MEM-00155	2018-02-08	expelled	\N
156	4	48	MEM-00156	2021-07-12	active	Заметка по членству #156
157	5	55	MEM-00157	2019-03-31	active	\N
158	6	91	MEM-00158	2020-11-20	active	\N
159	7	45	MEM-00159	2021-06-07	active	\N
160	8	110	MEM-00160	2020-08-05	active	Заметка по членству #160
161	1	98	MEM-00161	2016-08-22	active	\N
162	2	8	MEM-00162	2015-08-07	active	\N
163	3	128	MEM-00163	2018-05-13	active	\N
164	4	102	MEM-00164	2022-07-24	active	Заметка по членству #164
165	5	111	MEM-00165	2023-04-08	active	\N
166	6	147	MEM-00166	2017-05-20	active	\N
167	7	44	MEM-00167	2018-06-29	active	\N
168	8	13	MEM-00168	2024-04-06	active	Заметка по членству #168
169	1	119	MEM-00169	2018-01-17	active	\N
170	2	11	MEM-00170	2023-10-23	active	\N
171	3	32	MEM-00171	2017-05-09	active	\N
172	4	85	MEM-00172	2015-04-21	active	Заметка по членству #172
173	5	24	MEM-00173	2020-02-19	active	\N
174	6	33	MEM-00174	2017-10-13	active	\N
175	7	114	MEM-00175	2017-07-21	active	\N
176	8	166	MEM-00176	2024-08-08	active	Заметка по членству #176
177	1	51	MEM-00177	2016-11-26	active	\N
178	2	17	MEM-00178	2023-08-19	active	\N
179	3	6	MEM-00179	2024-09-15	active	\N
180	4	20	MEM-00180	2014-11-06	active	Заметка по членству #180
\.


--
-- TOC entry 3672 (class 0 OID 24578)
-- Dependencies: 215
-- Data for Name: association_role; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.association_role (id, code, name, description) FROM stdin;
1	chairman	Председатель	Руководитель ассоциации
2	vice_chairman	Заместитель председателя	Заместитель руководителя ассоциации
3	board_member	Член правления	Участник правления ассоциации
4	accountant	Бухгалтер	Ведет бухгалтерский учет
5	agronomist	Агроном	Курирует агротехнологии
6	procurement_mgr	Менеджер по закупкам	Организует закупки для членов ассоциации
7	logistics_mgr	Логист	Координирует поставки и склад
8	inspector	Инспектор	Проводит проверки и аудит процессов
\.


--
-- TOC entry 3686 (class 0 OID 24632)
-- Dependencies: 229
-- Data for Name: contract; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.contract (id, supplier_id, farm_id, association_id, contract_number, sign_date, start_date, end_date, status, description) FROM stdin;
1	6	49	1	CTR-01-00001	2022-08-29	2022-09-06	2025-07-23	expired	Договор #1 на поставку/сотрудничество
2	\N	1	1	CTR-01-00002	2023-12-25	2024-01-03	2025-08-07	expired	Договор #2 на поставку/сотрудничество
3	\N	33	1	CTR-01-00003	2023-11-16	2023-11-16	2025-08-27	active	Договор #3 на поставку/сотрудничество
4	10	41	1	CTR-01-00004	2022-12-02	2022-12-09	2025-06-01	expired	Договор #4 на поставку/сотрудничество
5	11	33	1	CTR-01-00005	2025-09-29	2025-10-29	2026-02-25	active	Договор #5 на поставку/сотрудничество
6	19	\N	1	CTR-01-00006	2021-01-31	2021-02-05	\N	active	Договор #6 на поставку/сотрудничество
7	25	17	1	CTR-01-00007	2024-06-24	2024-07-19	\N	active	Договор #7 на поставку/сотрудничество
8	8	\N	1	CTR-01-00008	2022-10-22	2022-11-11	2027-08-26	active	Договор #8 на поставку/сотрудничество
9	2	17	1	CTR-01-00009	2024-05-24	2024-06-06	2026-06-12	terminated	Договор #9 на поставку/сотрудничество
10	3	9	1	CTR-01-00010	2023-09-22	2023-10-01	2027-05-12	active	Договор #10 на поставку/сотрудничество
11	10	57	1	CTR-01-00011	2024-01-15	2024-02-13	2025-01-12	active	Договор #11 на поставку/сотрудничество
12	6	\N	1	CTR-01-00012	2023-02-23	2023-03-14	2023-04-23	expired	Договор #12 на поставку/сотрудничество
13	5	\N	1	CTR-01-00013	2025-03-20	2025-03-21	2025-10-21	expired	Договор #13 на поставку/сотрудничество
14	17	41	1	CTR-01-00014	2021-01-16	2021-01-21	\N	active	Договор #14 на поставку/сотрудничество
15	2	25	1	CTR-01-00015	2024-08-09	2024-08-16	2026-01-29	expired	Договор #15 на поставку/сотрудничество
16	3	1	1	CTR-01-00016	2021-04-30	2021-05-20	2025-02-05	expired	Договор #16 на поставку/сотрудничество
17	4	17	1	CTR-01-00017	2025-09-20	2025-09-30	2026-03-22	expired	Договор #17 на поставку/сотрудничество
18	3	41	1	CTR-01-00018	2022-12-26	2023-01-14	2026-05-14	expired	Договор #18 на поставку/сотрудничество
19	\N	\N	2	CTR-02-00019	2021-10-20	2021-10-27	2023-10-31	expired	Договор #19 на поставку/сотрудничество
20	7	26	2	CTR-02-00020	2021-06-19	2021-07-19	2024-03-28	expired	Договор #20 на поставку/сотрудничество
21	\N	10	2	CTR-02-00021	2022-08-07	2022-08-17	\N	active	Договор #21 на поставку/сотрудничество
22	8	10	2	CTR-02-00022	2024-06-13	2024-06-22	2026-09-12	active	Договор #22 на поставку/сотрудничество
23	4	\N	2	CTR-02-00023	2024-10-04	2024-10-18	2027-06-11	terminated	Договор #23 на поставку/сотрудничество
24	11	18	2	CTR-02-00024	2024-09-11	2024-09-26	2027-04-30	active	Договор #24 на поставку/сотрудничество
25	19	10	2	CTR-02-00025	2025-08-10	2025-09-01	2027-04-02	active	Договор #25 на поставку/сотрудничество
26	3	2	2	CTR-02-00026	2021-11-16	2021-11-20	2024-02-28	expired	Договор #26 на поставку/сотрудничество
27	18	58	2	CTR-02-00027	2021-02-19	2021-03-03	2026-10-10	expired	Договор #27 на поставку/сотрудничество
28	8	26	2	CTR-02-00028	2024-11-01	2024-11-16	\N	active	Договор #28 на поставку/сотрудничество
29	5	34	2	CTR-02-00029	2024-08-18	2024-09-03	\N	active	Договор #29 на поставку/сотрудничество
30	3	18	2	CTR-02-00030	2024-05-26	2024-06-04	\N	active	Договор #30 на поставку/сотрудничество
31	\N	42	2	CTR-02-00031	2025-05-02	2025-05-31	2025-11-29	expired	Договор #31 на поставку/сотрудничество
32	\N	10	2	CTR-02-00032	2022-03-01	2022-03-05	\N	active	Договор #32 на поставку/сотрудничество
33	17	\N	2	CTR-02-00033	2023-02-18	2023-03-09	2026-04-08	terminated	Договор #33 на поставку/сотрудничество
34	4	26	2	CTR-02-00034	2025-06-30	2025-07-21	2026-10-04	active	Договор #34 на поставку/сотрудничество
35	\N	26	2	CTR-02-00035	2023-12-09	2023-12-13	\N	active	Договор #35 на поставку/сотрудничество
36	1	\N	2	CTR-02-00036	2025-07-05	2025-07-16	\N	active	Договор #36 на поставку/сотрудничество
37	14	27	3	CTR-03-00037	2023-01-26	2023-01-28	2026-01-21	active	Договор #37 на поставку/сотрудничество
38	12	27	3	CTR-03-00038	2023-09-29	2023-10-29	2027-11-04	active	Договор #38 на поставку/сотрудничество
39	\N	59	3	CTR-03-00039	2023-04-22	2023-04-30	2026-06-10	active	Договор #39 на поставку/сотрудничество
40	6	35	3	CTR-03-00040	2024-12-21	2025-01-01	2025-04-23	active	Договор #40 на поставку/сотрудничество
41	11	51	3	CTR-03-00041	2024-03-30	2024-04-03	2026-11-06	active	Договор #41 на поставку/сотрудничество
42	11	\N	3	CTR-03-00042	2021-10-20	2021-11-15	2026-11-06	expired	Договор #42 на поставку/сотрудничество
43	14	19	3	CTR-03-00043	2024-05-08	2024-06-07	\N	active	Договор #43 на поставку/сотрудничество
44	15	27	3	CTR-03-00044	2022-06-12	2022-07-03	2027-08-10	active	Договор #44 на поставку/сотрудничество
45	4	43	3	CTR-03-00045	2021-01-09	2021-01-14	\N	active	Договор #45 на поставку/сотрудничество
46	\N	3	3	CTR-03-00046	2022-08-11	2022-09-09	\N	active	Договор #46 на поставку/сотрудничество
47	5	35	3	CTR-03-00047	2024-11-30	2024-12-20	\N	active	Договор #47 на поставку/сотрудничество
48	20	11	3	CTR-03-00048	2022-08-23	2022-09-06	2027-08-23	terminated	Договор #48 на поставку/сотрудничество
49	\N	27	3	CTR-03-00049	2022-01-31	2022-02-14	2024-02-13	expired	Договор #49 на поставку/сотрудничество
50	\N	35	3	CTR-03-00050	2022-10-11	2022-10-13	2025-03-29	active	Договор #50 на поставку/сотрудничество
51	15	27	3	CTR-03-00051	2023-04-14	2023-05-04	\N	active	Договор #51 на поставку/сотрудничество
52	\N	\N	3	CTR-03-00052	2023-12-02	2023-12-15	\N	active	Договор #52 на поставку/сотрудничество
53	\N	\N	3	CTR-03-00053	2025-08-18	2025-08-22	2025-10-16	terminated	Договор #53 на поставку/сотрудничество
54	\N	35	3	CTR-03-00054	2023-10-28	2023-11-26	\N	active	Договор #54 на поставку/сотрудничество
55	18	28	4	CTR-04-00055	2023-05-17	2023-06-13	2026-12-04	active	Договор #55 на поставку/сотрудничество
56	\N	52	4	CTR-04-00056	2023-10-19	2023-11-11	2024-08-02	expired	Договор #56 на поставку/сотрудничество
57	11	36	4	CTR-04-00057	2024-05-27	2024-06-25	\N	active	Договор #57 на поставку/сотрудничество
58	\N	44	4	CTR-04-00058	2023-05-09	2023-05-17	2023-11-29	active	Договор #58 на поставку/сотрудничество
59	9	12	4	CTR-04-00059	2024-02-29	2024-03-29	\N	active	Договор #59 на поставку/сотрудничество
60	1	44	4	CTR-04-00060	2022-08-21	2022-09-02	\N	active	Договор #60 на поставку/сотрудничество
61	\N	4	4	CTR-04-00061	2024-01-13	2024-02-10	2024-05-23	terminated	Договор #61 на поставку/сотрудничество
62	21	36	4	CTR-04-00062	2021-05-06	2021-05-14	\N	active	Договор #62 на поставку/сотрудничество
63	17	28	4	CTR-04-00063	2025-09-04	2025-09-14	2025-12-05	expired	Договор #63 на поставку/сотрудничество
64	21	36	4	CTR-04-00064	2023-07-28	2023-08-07	\N	active	Договор #64 на поставку/сотрудничество
65	21	52	4	CTR-04-00065	2023-11-24	2023-12-23	\N	active	Договор #65 на поставку/сотрудничество
66	15	4	4	CTR-04-00066	2021-07-06	2021-08-05	2022-04-04	active	Договор #66 на поставку/сотрудничество
67	22	28	4	CTR-04-00067	2023-09-18	2023-10-10	\N	active	Договор #67 на поставку/сотрудничество
68	13	52	4	CTR-04-00068	2021-08-20	2021-08-22	2027-02-27	expired	Договор #68 на поставку/сотрудничество
69	12	28	4	CTR-04-00069	2024-09-21	2024-09-27	\N	active	Договор #69 на поставку/сотрудничество
70	\N	4	4	CTR-04-00070	2022-11-13	2022-11-30	\N	active	Договор #70 на поставку/сотрудничество
71	18	44	4	CTR-04-00071	2023-05-15	2023-05-25	\N	active	Договор #71 на поставку/сотрудничество
72	12	\N	4	CTR-04-00072	2022-05-22	2022-06-09	2027-01-01	expired	Договор #72 на поставку/сотрудничество
73	6	45	5	CTR-05-00073	2021-10-18	2021-10-21	2027-06-16	active	Договор #73 на поставку/сотрудничество
74	22	53	5	CTR-05-00074	2022-07-03	2022-08-01	\N	active	Договор #74 на поставку/сотрудничество
75	7	53	5	CTR-05-00075	2025-05-10	2025-05-16	2026-05-30	expired	Договор #75 на поставку/сотрудничество
76	15	53	5	CTR-05-00076	2023-02-26	2023-03-11	2027-12-26	expired	Договор #76 на поставку/сотрудничество
77	3	53	5	CTR-05-00077	2025-02-21	2025-03-15	\N	active	Договор #77 на поставку/сотрудничество
78	\N	21	5	CTR-05-00078	2023-05-24	2023-05-27	2023-09-03	expired	Договор #78 на поставку/сотрудничество
79	7	\N	5	CTR-05-00079	2021-01-16	2021-02-02	2026-08-17	active	Договор #79 на поставку/сотрудничество
80	1	53	5	CTR-05-00080	2025-08-25	2025-09-01	2027-12-02	expired	Договор #80 на поставку/сотрудничество
81	\N	29	5	CTR-05-00081	2023-07-23	2023-08-21	2024-07-13	active	Договор #81 на поставку/сотрудничество
82	10	13	5	CTR-05-00082	2024-11-20	2024-12-13	2027-05-20	active	Договор #82 на поставку/сотрудничество
83	2	5	5	CTR-05-00083	2021-10-27	2021-10-29	\N	active	Договор #83 на поставку/сотрудничество
84	\N	21	5	CTR-05-00084	2024-10-14	2024-10-23	2025-05-07	active	Договор #84 на поставку/сотрудничество
85	23	29	5	CTR-05-00085	2021-07-12	2021-08-05	2026-08-18	terminated	Договор #85 на поставку/сотрудничество
86	19	5	5	CTR-05-00086	2023-02-10	2023-02-21	2025-10-27	active	Договор #86 на поставку/сотрудничество
87	25	21	5	CTR-05-00087	2021-03-25	2021-04-13	2025-10-05	active	Договор #87 на поставку/сотрудничество
88	10	5	5	CTR-05-00088	2021-02-05	2021-02-24	2023-02-07	active	Договор #88 на поставку/сотрудничество
89	10	\N	5	CTR-05-00089	2024-10-13	2024-10-25	2027-02-08	active	Договор #89 на поставку/сотрудничество
90	7	\N	6	CTR-06-00090	2022-01-09	2022-01-13	2023-02-18	expired	Договор #90 на поставку/сотрудничество
91	20	30	6	CTR-06-00091	2021-02-13	2021-02-24	2026-10-04	expired	Договор #91 на поставку/сотрудничество
92	25	30	6	CTR-06-00092	2025-06-04	2025-06-24	2025-12-15	terminated	Договор #92 на поставку/сотрудничество
93	24	6	6	CTR-06-00093	2021-05-22	2021-06-03	2023-05-14	active	Договор #93 на поставку/сотрудничество
94	2	22	6	CTR-06-00094	2024-02-22	2024-03-11	2025-11-26	expired	Договор #94 на поставку/сотрудничество
95	\N	54	6	CTR-06-00095	2024-02-14	2024-03-14	2025-09-06	active	Договор #95 на поставку/сотрудничество
96	23	30	6	CTR-06-00096	2021-07-02	2021-07-21	\N	active	Договор #96 на поставку/сотрудничество
97	19	22	6	CTR-06-00097	2023-02-27	2023-03-06	2027-05-29	active	Договор #97 на поставку/сотрудничество
98	21	22	6	CTR-06-00098	2024-10-22	2024-11-17	\N	active	Договор #98 на поставку/сотрудничество
99	\N	30	6	CTR-06-00099	2024-11-19	2024-11-23	2027-06-05	active	Договор #99 на поставку/сотрудничество
100	17	30	6	CTR-06-00100	2024-01-09	2024-01-19	2025-01-23	terminated	Договор #100 на поставку/сотрудничество
101	\N	14	6	CTR-06-00101	2022-08-18	2022-08-18	\N	active	Договор #101 на поставку/сотрудничество
102	\N	14	6	CTR-06-00102	2021-02-12	2021-03-01	\N	active	Договор #102 на поставку/сотрудничество
103	24	6	6	CTR-06-00103	2022-10-17	2022-10-19	2026-03-07	expired	Договор #103 на поставку/сотрудничество
104	5	6	6	CTR-06-00104	2023-04-01	2023-05-01	2027-09-01	active	Договор #104 на поставку/сотрудничество
105	24	14	6	CTR-06-00105	2022-10-18	2022-11-15	2024-03-03	expired	Договор #105 на поставку/сотрудничество
106	23	14	6	CTR-06-00106	2025-07-21	2025-07-21	\N	active	Договор #106 на поставку/сотрудничество
107	19	55	7	CTR-07-00107	2024-05-03	2024-05-21	\N	active	Договор #107 на поставку/сотрудничество
108	24	23	7	CTR-07-00108	2025-10-15	2025-11-10	2026-03-25	active	Договор #108 на поставку/сотрудничество
109	8	23	7	CTR-07-00109	2021-05-13	2021-05-23	2023-08-27	expired	Договор #109 на поставку/сотрудничество
110	24	23	7	CTR-07-00110	2024-12-17	2024-12-24	2026-05-25	active	Договор #110 на поставку/сотрудничество
111	14	39	7	CTR-07-00111	2024-08-09	2024-08-09	2025-04-24	active	Договор #111 на поставку/сотрудничество
112	21	7	7	CTR-07-00112	2022-12-16	2023-01-12	2027-07-18	active	Договор #112 на поставку/сотрудничество
113	23	\N	7	CTR-07-00113	2025-04-25	2025-05-09	2026-07-28	terminated	Договор #113 на поставку/сотрудничество
114	3	23	7	CTR-07-00114	2024-10-26	2024-11-12	2026-06-25	active	Договор #114 на поставку/сотрудничество
115	16	\N	7	CTR-07-00115	2024-05-23	2024-06-06	2026-04-21	terminated	Договор #115 на поставку/сотрудничество
116	19	\N	7	CTR-07-00116	2025-03-10	2025-03-19	\N	active	Договор #116 на поставку/сотрудничество
117	14	23	7	CTR-07-00117	2024-08-19	2024-08-19	2025-06-17	active	Договор #117 на поставку/сотрудничество
118	16	7	7	CTR-07-00118	2023-11-09	2023-11-24	2024-11-04	expired	Договор #118 на поставку/сотрудничество
119	7	15	7	CTR-07-00119	2023-06-05	2023-06-22	2027-06-12	expired	Договор #119 на поставку/сотрудничество
120	24	55	7	CTR-07-00120	2025-05-22	2025-06-11	2026-01-03	terminated	Договор #120 на поставку/сотрудничество
121	6	31	7	CTR-07-00121	2023-06-29	2023-07-15	2026-09-01	active	Договор #121 на поставку/сотрудничество
122	7	31	7	CTR-07-00122	2021-06-25	2021-07-25	2022-08-05	expired	Договор #122 на поставку/сотрудничество
123	21	47	7	CTR-07-00123	2024-01-07	2024-02-06	2026-12-11	active	Договор #123 на поставку/сотрудничество
124	11	8	8	CTR-08-00124	2021-10-05	2021-10-15	\N	active	Договор #124 на поставку/сотрудничество
125	14	40	8	CTR-08-00125	2025-09-09	2025-09-24	\N	active	Договор #125 на поставку/сотрудничество
126	18	48	8	CTR-08-00126	2023-12-26	2023-12-26	2025-09-11	terminated	Договор #126 на поставку/сотрудничество
127	15	56	8	CTR-08-00127	2023-03-05	2023-03-15	2026-09-18	active	Договор #127 на поставку/сотрудничество
128	5	56	8	CTR-08-00128	2022-12-23	2023-01-19	2026-02-16	expired	Договор #128 на поставку/сотрудничество
129	3	56	8	CTR-08-00129	2022-09-24	2022-10-10	\N	active	Договор #129 на поставку/сотрудничество
130	25	48	8	CTR-08-00130	2024-05-20	2024-05-25	\N	active	Договор #130 на поставку/сотрудничество
131	17	\N	8	CTR-08-00131	2024-02-24	2024-03-21	2026-06-04	expired	Договор #131 на поставку/сотрудничество
132	\N	48	8	CTR-08-00132	2021-02-04	2021-02-28	\N	active	Договор #132 на поставку/сотрудничество
133	24	\N	8	CTR-08-00133	2022-11-28	2022-12-18	2025-03-28	expired	Договор #133 на поставку/сотрудничество
134	2	16	8	CTR-08-00134	2024-03-16	2024-04-09	2026-10-08	expired	Договор #134 на поставку/сотрудничество
135	5	\N	8	CTR-08-00135	2021-11-28	2021-12-05	2026-10-01	expired	Договор #135 на поставку/сотрудничество
136	21	32	8	CTR-08-00136	2021-12-11	2022-01-08	2024-11-23	expired	Договор #136 на поставку/сотрудничество
137	1	\N	8	CTR-08-00137	2025-07-22	2025-07-30	2026-11-26	expired	Договор #137 на поставку/сотрудничество
138	9	40	8	CTR-08-00138	2023-07-14	2023-07-25	2025-04-05	terminated	Договор #138 на поставку/сотрудничество
139	\N	24	8	CTR-08-00139	2023-10-11	2023-11-08	2024-08-12	expired	Договор #139 на поставку/сотрудничество
140	10	48	8	CTR-08-00140	2024-05-08	2024-05-19	2027-05-29	active	Договор #140 на поставку/сотрудничество
\.


--
-- TOC entry 3699 (class 0 OID 24677)
-- Dependencies: 242
-- Data for Name: employee_plot_assignment; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.employee_plot_assignment (id, farm_employee_id, farm_plot_id, assignment_type, assigned_at, unassigned_at, notes) FROM stdin;
1	1	128	primary	2024-09-27	\N	\N
2	1	8	primary	2024-09-10	\N	\N
3	1	188	primary	2024-11-07	\N	\N
4	2	135	primary	2019-01-15	\N	\N
5	2	75	primary	2020-08-27	\N	\N
6	3	142	primary	2021-02-23	\N	\N
7	3	22	secondary	2022-02-05	\N	Назначение сотрудника 3 на участок 22
8	4	149	secondary	2024-03-02	\N	\N
9	4	89	primary	2024-01-28	\N	\N
10	5	216	primary	2021-01-24	\N	\N
11	5	96	primary	2021-01-29	\N	\N
12	6	103	temporary	2025-07-20	\N	\N
13	7	50	primary	2021-08-13	\N	\N
14	7	170	secondary	2020-07-26	\N	Назначение сотрудника 7 на участок 170
15	8	177	primary	2021-09-08	\N	\N
16	9	64	primary	2018-03-03	\N	\N
17	10	131	primary	2019-05-06	\N	\N
18	11	78	secondary	2020-05-16	\N	\N
19	12	85	primary	2017-03-10	\N	\N
20	12	205	temporary	2016-12-16	\N	\N
21	12	145	temporary	2018-07-19	\N	Назначение сотрудника 12 на участок 145
22	13	212	primary	2016-10-30	\N	\N
23	13	92	primary	2018-10-20	\N	\N
24	14	219	temporary	2025-12-26	\N	\N
25	14	99	secondary	2025-06-24	\N	\N
26	15	166	secondary	2019-03-22	\N	\N
27	15	106	temporary	2020-01-03	\N	\N
28	16	173	primary	2023-12-10	\N	Назначение сотрудника 16 на участок 173
29	17	60	primary	2025-05-24	\N	\N
30	18	67	primary	2025-07-23	\N	\N
31	19	134	primary	2015-09-14	\N	\N
32	19	14	secondary	2018-01-27	\N	\N
33	20	201	secondary	2024-07-05	\N	\N
34	20	21	temporary	2025-01-19	\N	\N
35	20	81	primary	2025-06-27	2025-10-07	Назначение сотрудника 20 на участок 81
36	21	208	primary	2019-03-11	\N	\N
37	21	88	primary	2018-08-15	\N	\N
38	22	215	primary	2017-10-06	\N	\N
39	23	162	primary	2016-10-03	\N	\N
40	23	102	secondary	2017-07-07	\N	\N
41	23	42	primary	2017-06-17	\N	\N
42	24	49	primary	2022-04-05	\N	Назначение сотрудника 24 на участок 49
43	24	169	temporary	2024-01-15	\N	\N
44	25	176	primary	2022-08-22	\N	\N
45	26	123	primary	2022-08-28	\N	\N
46	27	70	secondary	2022-10-13	\N	\N
47	27	10	primary	2023-05-13	\N	\N
48	28	137	primary	2025-05-30	\N	\N
49	29	204	secondary	2024-04-09	\N	Назначение сотрудника 29 на участок 204
50	29	24	primary	2024-08-17	\N	\N
51	30	211	primary	2023-11-12	\N	\N
52	30	31	primary	2022-11-24	\N	\N
53	30	91	temporary	2023-01-29	2025-11-13	\N
54	31	158	primary	2019-04-29	\N	\N
55	32	165	primary	2025-04-16	\N	\N
56	32	105	primary	2025-07-28	\N	Назначение сотрудника 32 на участок 105
57	32	45	primary	2025-02-16	\N	\N
58	33	112	temporary	2018-07-27	\N	\N
59	34	59	primary	2017-04-18	\N	\N
60	35	126	primary	2022-09-07	\N	\N
61	36	13	secondary	2024-05-15	\N	\N
62	36	133	secondary	2024-05-04	\N	\N
63	37	80	primary	2025-08-25	2025-10-01	Назначение сотрудника 37 на участок 80
64	37	140	secondary	2025-11-26	\N	\N
65	38	147	primary	2018-11-22	\N	\N
66	39	94	primary	2024-03-26	\N	\N
67	40	101	primary	2023-06-25	\N	\N
68	41	48	primary	2019-03-10	\N	\N
69	41	168	secondary	2017-06-03	\N	\N
70	41	108	secondary	2019-04-25	\N	Назначение сотрудника 41 на участок 108
71	42	175	primary	2023-04-01	\N	\N
72	42	55	primary	2021-05-03	\N	\N
73	43	2	primary	2024-08-03	\N	\N
74	44	69	primary	2025-02-05	\N	\N
75	44	189	primary	2024-11-20	\N	\N
76	45	76	secondary	2016-10-12	\N	\N
77	45	16	primary	2017-09-15	\N	Назначение сотрудника 45 на участок 16
78	46	23	secondary	2025-07-06	\N	\N
79	47	30	secondary	2024-07-16	\N	\N
80	47	210	primary	2022-01-24	\N	\N
81	48	37	primary	2024-01-29	\N	\N
82	49	44	secondary	2018-02-22	\N	\N
83	50	51	primary	2024-04-18	\N	\N
84	50	171	primary	2025-02-22	\N	Назначение сотрудника 50 на участок 171
85	51	178	temporary	2018-07-28	\N	\N
86	52	65	secondary	2021-01-23	\N	\N
87	53	12	primary	2015-05-07	\N	\N
88	53	192	temporary	2016-05-05	\N	\N
89	54	19	primary	2025-07-28	\N	\N
90	55	206	secondary	2023-06-16	\N	\N
91	56	33	secondary	2024-12-24	\N	Назначение сотрудника 56 на участок 33
92	57	220	primary	2023-03-17	\N	\N
93	57	40	primary	2022-09-07	\N	\N
94	57	160	primary	2021-07-16	\N	\N
95	58	107	primary	2015-12-27	\N	\N
96	58	167	primary	2018-07-12	\N	\N
97	59	174	secondary	2020-05-18	\N	\N
98	60	61	primary	2021-01-08	\N	Назначение сотрудника 60 на участок 61
99	60	121	primary	2021-02-19	\N	\N
100	61	188	primary	2023-12-20	\N	\N
101	62	195	primary	2019-05-25	\N	\N
102	62	135	primary	2017-08-26	\N	\N
103	62	15	primary	2019-05-22	\N	\N
104	63	22	primary	2023-06-27	\N	\N
105	63	142	primary	2023-08-28	\N	Назначение сотрудника 63 на участок 142
106	64	209	secondary	2016-09-13	\N	\N
107	64	29	primary	2018-03-13	\N	\N
108	65	156	primary	2024-04-11	\N	\N
109	65	216	temporary	2024-04-20	\N	\N
110	66	43	primary	2025-03-13	\N	\N
111	66	103	primary	2025-09-24	\N	\N
112	67	50	secondary	2020-03-13	\N	Назначение сотрудника 67 на участок 50
113	67	170	primary	2020-01-09	\N	\N
114	68	57	secondary	2022-12-27	\N	\N
115	69	124	secondary	2019-01-29	\N	\N
116	70	191	primary	2022-08-10	\N	\N
117	70	71	primary	2024-04-07	\N	\N
118	71	138	primary	2025-09-18	\N	\N
119	72	25	primary	2018-10-28	\N	Назначение сотрудника 72 на участок 25
120	72	85	primary	2018-10-13	\N	\N
121	72	205	primary	2016-11-15	\N	\N
122	73	152	primary	2020-02-16	\N	\N
123	73	212	primary	2019-12-17	\N	\N
124	74	39	secondary	2024-02-25	\N	\N
125	75	166	primary	2023-05-05	\N	\N
126	75	106	primary	2021-07-20	\N	Назначение сотрудника 75 на участок 106
127	76	113	secondary	2025-01-09	\N	\N
128	77	120	primary	2021-11-23	\N	\N
129	78	67	secondary	2019-08-15	\N	\N
130	78	187	secondary	2017-10-25	\N	\N
131	79	194	primary	2020-10-16	\N	\N
132	79	14	primary	2020-05-24	\N	\N
133	80	81	primary	2017-08-24	\N	Назначение сотрудника 80 на участок 81
134	81	28	temporary	2017-08-19	\N	\N
135	82	95	primary	2015-05-23	\N	\N
136	83	162	primary	2019-02-05	\N	\N
137	84	109	primary	2024-11-11	\N	\N
138	84	49	primary	2024-02-07	\N	\N
139	84	169	primary	2024-04-22	\N	\N
140	85	116	secondary	2019-03-25	\N	Назначение сотрудника 85 на участок 116
141	86	183	secondary	2025-08-17	\N	\N
142	87	70	primary	2018-03-02	\N	\N
143	87	130	primary	2018-12-06	\N	\N
144	88	17	primary	2020-07-18	\N	\N
145	88	137	primary	2019-08-12	\N	\N
146	89	84	secondary	2023-03-13	\N	\N
147	89	144	primary	2023-03-19	\N	Назначение сотрудника 89 на участок 144
148	90	211	primary	2025-09-15	\N	\N
149	91	158	primary	2021-12-01	\N	\N
150	91	218	primary	2021-12-28	\N	\N
151	92	165	primary	2025-01-13	\N	\N
152	93	52	temporary	2025-11-11	\N	\N
153	93	172	primary	2025-12-12	\N	\N
154	94	179	temporary	2018-11-08	\N	Назначение сотрудника 94 на участок 179
155	94	119	secondary	2020-05-04	\N	\N
156	95	186	primary	2021-01-11	\N	\N
157	95	126	primary	2020-09-11	\N	\N
158	95	6	secondary	2018-12-12	\N	\N
159	96	193	temporary	2024-04-23	\N	\N
160	96	73	secondary	2022-11-19	\N	\N
161	97	140	primary	2018-12-19	\N	Назначение сотрудника 97 на участок 140
162	97	80	primary	2019-05-23	\N	\N
163	98	87	secondary	2021-06-06	2022-02-22	\N
164	99	214	primary	2023-08-10	\N	\N
165	100	41	primary	2023-08-31	\N	\N
166	100	161	secondary	2024-06-15	\N	\N
167	100	101	temporary	2023-11-20	2025-09-19	\N
168	101	108	primary	2017-05-22	\N	Назначение сотрудника 101 на участок 108
169	101	48	temporary	2018-08-04	\N	\N
170	101	168	temporary	2019-03-15	\N	\N
171	102	55	primary	2019-08-02	\N	\N
172	102	115	temporary	2021-08-08	\N	\N
173	103	122	primary	2015-10-30	\N	\N
174	103	62	secondary	2016-01-04	\N	\N
175	104	189	temporary	2017-07-05	\N	Назначение сотрудника 104 на участок 189
176	105	16	primary	2020-05-17	\N	\N
177	106	203	temporary	2023-01-26	\N	\N
178	107	150	primary	2020-08-19	\N	\N
179	108	97	temporary	2024-12-11	\N	\N
180	109	104	primary	2024-12-08	\N	\N
181	109	164	primary	2025-10-09	\N	\N
182	109	44	secondary	2025-12-06	\N	Назначение сотрудника 109 на участок 44
183	110	171	primary	2024-02-23	2024-11-18	\N
184	111	178	temporary	2025-02-17	\N	\N
185	112	65	primary	2018-04-08	\N	\N
186	112	185	temporary	2018-04-17	\N	\N
187	113	132	primary	2024-08-19	\N	\N
188	113	12	temporary	2023-11-12	\N	\N
189	113	192	temporary	2024-09-28	\N	Назначение сотрудника 113 на участок 192
190	114	199	temporary	2019-01-17	\N	\N
191	114	19	primary	2020-04-06	\N	\N
192	115	146	temporary	2020-04-05	\N	\N
193	116	93	primary	2024-08-21	\N	\N
194	116	153	primary	2024-11-22	\N	\N
195	117	160	secondary	2024-04-29	\N	\N
196	117	100	secondary	2024-10-06	\N	Назначение сотрудника 117 на участок 100
197	118	47	primary	2022-07-29	\N	\N
198	118	167	primary	2022-01-15	\N	\N
199	119	54	primary	2025-03-08	\N	\N
200	120	61	primary	2023-07-27	\N	\N
201	120	121	primary	2022-07-11	\N	\N
202	120	181	primary	2023-08-12	2025-11-17	\N
203	121	128	primary	2020-07-26	\N	Назначение сотрудника 121 на участок 128
204	122	75	temporary	2018-10-08	\N	\N
205	123	82	primary	2019-07-02	\N	\N
206	123	22	primary	2019-05-02	\N	\N
207	123	202	secondary	2020-10-16	\N	\N
208	124	149	temporary	2020-08-03	\N	\N
209	125	216	secondary	2019-04-30	\N	\N
210	126	43	temporary	2019-11-21	\N	Назначение сотрудника 126 на участок 43
211	126	163	primary	2020-04-24	\N	\N
212	127	50	secondary	2020-12-28	\N	\N
213	127	110	secondary	2021-06-02	\N	\N
214	128	57	secondary	2024-12-21	\N	\N
215	129	124	secondary	2018-04-05	\N	\N
216	130	131	primary	2025-06-30	\N	\N
217	130	191	temporary	2025-07-13	\N	Назначение сотрудника 130 на участок 191
218	130	71	secondary	2025-12-03	2025-12-25	\N
219	131	198	primary	2017-09-19	\N	\N
220	131	138	primary	2016-08-11	\N	\N
221	132	205	primary	2021-04-02	\N	\N
222	132	85	primary	2023-05-12	\N	\N
223	132	25	secondary	2023-06-09	\N	\N
224	133	32	primary	2019-06-17	\N	Назначение сотрудника 133 на участок 32
225	134	159	primary	2016-12-16	\N	\N
226	134	99	primary	2019-03-12	\N	\N
227	135	106	temporary	2024-04-26	\N	\N
228	136	113	primary	2023-01-12	\N	\N
229	136	173	secondary	2022-02-05	\N	\N
230	137	120	temporary	2025-07-24	\N	\N
231	137	180	secondary	2025-05-23	\N	Назначение сотрудника 137 на участок 180
232	138	67	secondary	2025-12-04	\N	\N
233	138	127	primary	2025-04-25	\N	\N
234	139	74	primary	2022-10-08	\N	\N
235	140	201	primary	2024-05-07	\N	\N
236	141	208	temporary	2024-08-22	\N	\N
237	141	28	primary	2025-05-12	\N	\N
238	141	88	secondary	2023-04-10	\N	Назначение сотрудника 141 на участок 88
239	142	35	temporary	2024-06-23	\N	\N
240	142	95	secondary	2024-05-30	\N	\N
241	142	215	secondary	2025-05-01	\N	\N
242	143	162	temporary	2025-12-26	\N	\N
243	143	42	primary	2025-07-31	\N	\N
244	144	109	secondary	2025-08-10	\N	\N
245	144	49	temporary	2025-05-24	\N	Назначение сотрудника 144 на участок 49
246	145	176	primary	2015-10-10	\N	\N
247	146	3	primary	2024-12-07	\N	\N
248	146	63	secondary	2025-10-17	\N	\N
249	147	190	primary	2020-08-25	\N	\N
250	147	130	primary	2019-03-04	\N	\N
251	148	197	primary	2016-10-30	2018-04-22	\N
252	149	144	secondary	2018-01-15	2018-09-09	Назначение сотрудника 149 на участок 144
253	150	211	primary	2022-01-28	\N	\N
254	150	31	temporary	2021-03-09	\N	\N
255	151	38	primary	2019-06-25	\N	\N
256	151	218	primary	2020-10-28	\N	\N
257	151	158	primary	2019-06-07	\N	\N
258	152	105	primary	2020-08-12	\N	\N
259	152	165	secondary	2020-06-01	\N	Назначение сотрудника 152 на участок 165
260	153	112	primary	2022-07-02	\N	\N
261	153	172	primary	2022-09-12	\N	\N
262	154	119	primary	2024-12-23	\N	\N
263	155	6	primary	2019-08-10	\N	\N
264	156	73	temporary	2017-04-11	\N	\N
265	156	133	secondary	2019-05-13	\N	\N
266	157	140	primary	2017-08-17	\N	Назначение сотрудника 157 на участок 140
267	157	80	primary	2017-11-08	\N	\N
268	158	147	primary	2024-01-13	\N	\N
269	158	27	temporary	2022-10-21	\N	\N
270	158	87	primary	2025-04-03	\N	\N
271	159	34	secondary	2020-10-05	\N	\N
272	160	101	primary	2021-07-16	\N	\N
273	160	161	primary	2022-12-08	\N	Назначение сотрудника 160 на участок 161
274	161	108	secondary	2024-03-16	\N	\N
275	161	48	primary	2023-06-09	\N	\N
276	162	115	secondary	2022-04-12	\N	\N
277	162	175	primary	2021-03-29	\N	\N
278	162	55	primary	2020-10-21	\N	\N
279	163	122	primary	2019-07-31	\N	\N
280	164	189	primary	2015-09-26	\N	Назначение сотрудника 164 на участок 189
281	165	196	temporary	2019-12-04	\N	\N
282	165	136	primary	2020-05-19	\N	\N
283	166	143	secondary	2024-05-18	\N	\N
284	166	203	primary	2022-08-28	\N	\N
285	167	30	secondary	2015-07-07	\N	\N
286	168	37	primary	2017-06-10	\N	\N
287	169	44	secondary	2018-05-04	\N	Назначение сотрудника 169 на участок 44
288	170	171	secondary	2025-01-21	\N	\N
289	170	51	primary	2024-11-03	\N	\N
290	170	111	primary	2024-08-31	2024-11-08	\N
291	171	178	temporary	2024-07-15	\N	\N
292	172	185	primary	2023-03-14	\N	\N
293	172	125	primary	2024-01-14	\N	\N
294	173	192	secondary	2025-01-02	\N	Назначение сотрудника 173 на участок 192
295	174	139	primary	2025-05-27	\N	\N
296	174	19	primary	2025-07-19	\N	\N
297	175	206	primary	2018-11-04	\N	\N
298	175	26	primary	2020-11-13	\N	\N
299	176	153	temporary	2023-05-04	\N	\N
300	176	213	primary	2021-08-27	\N	\N
301	177	160	primary	2020-11-11	\N	Назначение сотрудника 177 на участок 160
302	177	100	primary	2021-10-28	\N	\N
303	177	220	primary	2023-03-16	\N	\N
304	178	167	temporary	2020-08-13	\N	\N
305	179	174	temporary	2025-09-23	2025-10-15	\N
306	179	114	secondary	2025-12-29	\N	\N
307	179	54	secondary	2025-10-10	2025-10-23	\N
308	180	61	secondary	2025-10-31	\N	Назначение сотрудника 180 на участок 61
309	180	181	primary	2025-10-25	\N	\N
310	181	68	primary	2025-07-02	\N	\N
311	182	195	primary	2021-08-15	\N	\N
312	182	75	temporary	2020-07-27	\N	\N
313	183	142	secondary	2024-05-12	\N	\N
314	184	29	primary	2022-05-11	\N	\N
315	185	96	primary	2017-02-05	\N	Назначение сотрудника 185 на участок 96
316	186	103	secondary	2018-05-18	\N	\N
317	186	163	temporary	2019-08-13	\N	\N
318	187	170	secondary	2020-07-16	\N	\N
319	187	50	temporary	2018-09-16	\N	\N
320	187	110	secondary	2018-08-28	\N	\N
321	188	57	secondary	2017-03-09	\N	\N
322	189	124	primary	2025-04-09	\N	Назначение сотрудника 189 на участок 124
323	189	4	primary	2024-09-14	\N	\N
324	189	64	primary	2025-09-22	\N	\N
325	190	131	secondary	2024-05-26	\N	\N
326	190	11	temporary	2025-04-15	\N	\N
327	191	18	primary	2020-04-21	\N	\N
328	191	198	primary	2020-06-24	\N	\N
329	192	205	secondary	2025-03-03	\N	Назначение сотрудника 192 на участок 205
330	193	212	secondary	2017-11-27	2018-09-15	\N
331	194	219	primary	2025-12-04	\N	\N
332	195	166	primary	2021-10-11	\N	\N
333	195	106	secondary	2019-05-09	\N	\N
334	195	46	temporary	2022-03-13	\N	\N
335	196	113	temporary	2024-05-26	\N	\N
336	197	60	secondary	2025-07-17	\N	Назначение сотрудника 197 на участок 60
337	198	67	temporary	2023-08-20	\N	\N
338	198	127	secondary	2025-06-21	\N	\N
339	199	134	secondary	2021-11-28	\N	\N
340	199	74	temporary	2021-12-17	\N	\N
341	199	194	secondary	2020-03-31	\N	\N
342	200	81	secondary	2024-08-13	\N	\N
343	200	141	primary	2025-11-08	\N	Назначение сотрудника 200 на участок 141
344	200	201	secondary	2025-07-30	2025-11-30	\N
345	201	208	primary	2019-04-04	\N	\N
346	201	28	primary	2020-08-29	\N	\N
347	201	88	primary	2019-06-29	\N	\N
348	202	95	primary	2025-09-11	\N	\N
349	202	35	primary	2025-09-16	\N	\N
350	203	42	primary	2021-04-28	\N	Назначение сотрудника 203 на участок 42
351	203	162	secondary	2021-07-30	\N	\N
352	204	49	secondary	2024-05-11	\N	\N
353	204	169	primary	2024-11-21	\N	\N
354	204	109	primary	2023-08-09	\N	\N
355	205	56	temporary	2022-11-05	\N	\N
356	205	116	secondary	2023-12-08	\N	\N
357	206	123	secondary	2023-03-19	\N	Назначение сотрудника 206 на участок 123
358	207	70	secondary	2021-12-16	2022-09-18	\N
359	207	130	primary	2020-06-06	2021-05-17	\N
360	208	137	primary	2018-08-25	\N	\N
361	209	204	primary	2020-09-07	\N	\N
362	209	24	primary	2020-11-24	\N	\N
363	210	151	primary	2025-12-10	\N	\N
364	211	218	secondary	2021-07-31	\N	Назначение сотрудника 211 на участок 218
365	211	98	secondary	2021-04-02	\N	\N
366	212	165	primary	2019-04-28	2022-03-08	\N
367	212	105	temporary	2021-01-29	2021-10-30	\N
368	212	45	primary	2021-05-23	2024-05-20	\N
369	213	112	secondary	2020-11-13	\N	\N
370	214	119	primary	2020-10-10	\N	\N
371	214	59	primary	2021-10-13	\N	Назначение сотрудника 214 на участок 59
372	214	179	primary	2021-01-06	\N	\N
373	215	126	primary	2024-10-31	\N	\N
374	216	73	secondary	2016-01-24	\N	\N
375	217	140	primary	2023-04-02	\N	\N
376	218	207	primary	2022-05-29	\N	\N
377	218	27	temporary	2022-02-06	\N	\N
378	219	154	temporary	2018-11-18	\N	Назначение сотрудника 219 на участок 154
379	220	101	primary	2019-12-28	\N	\N
380	220	41	secondary	2021-10-18	\N	\N
381	221	48	secondary	2021-11-05	\N	\N
382	221	108	primary	2023-08-23	\N	\N
383	222	115	secondary	2017-10-19	\N	\N
384	222	55	secondary	2017-03-04	\N	\N
385	222	175	primary	2017-12-22	\N	Назначение сотрудника 222 на участок 175
386	223	2	temporary	2021-04-25	\N	\N
387	224	9	primary	2023-01-04	\N	\N
388	224	129	primary	2022-10-02	\N	\N
389	225	76	primary	2019-11-22	\N	\N
390	226	203	secondary	2023-11-15	\N	\N
391	227	210	primary	2016-01-31	\N	\N
392	227	150	primary	2015-12-17	\N	Назначение сотрудника 227 на участок 150
393	228	37	primary	2022-04-26	\N	\N
394	229	44	primary	2024-05-23	\N	\N
395	230	51	secondary	2021-12-12	\N	\N
396	231	118	primary	2020-05-08	\N	\N
397	232	65	primary	2018-04-14	\N	\N
398	232	5	primary	2018-05-29	\N	\N
399	232	125	primary	2018-06-06	\N	Назначение сотрудника 232 на участок 125
400	233	12	secondary	2023-03-24	\N	\N
401	233	192	primary	2023-03-18	\N	\N
402	233	132	primary	2024-09-17	\N	\N
403	234	19	primary	2024-10-21	\N	\N
404	235	206	primary	2015-12-14	\N	\N
405	236	33	primary	2024-04-15	\N	\N
406	237	40	primary	2023-01-10	\N	Назначение сотрудника 237 на участок 40
407	237	220	primary	2024-08-08	\N	\N
408	238	167	primary	2023-04-02	2024-04-28	\N
409	238	47	primary	2023-05-25	2024-07-08	\N
410	239	174	secondary	2025-02-22	\N	\N
411	239	114	secondary	2025-07-20	\N	\N
412	239	54	primary	2025-05-18	\N	\N
413	240	181	temporary	2025-01-24	\N	Назначение сотрудника 240 на участок 181
414	240	61	primary	2024-02-16	\N	\N
415	240	121	primary	2025-10-18	2025-12-03	\N
416	241	8	primary	2020-05-23	\N	\N
417	241	68	secondary	2020-07-30	\N	\N
418	242	135	secondary	2024-09-12	\N	\N
419	242	75	temporary	2024-12-31	\N	\N
420	243	142	primary	2016-07-26	\N	Назначение сотрудника 243 на участок 142
421	243	202	primary	2016-11-02	\N	\N
422	243	82	temporary	2017-08-12	\N	\N
423	244	89	temporary	2025-07-22	\N	\N
424	245	36	primary	2022-01-13	\N	\N
425	245	216	secondary	2023-10-21	\N	\N
426	246	163	temporary	2015-10-17	\N	\N
427	246	43	secondary	2018-02-11	\N	Назначение сотрудника 246 на участок 43
428	247	110	primary	2018-01-09	2019-02-19	\N
429	248	177	primary	2016-08-07	2021-10-25	\N
430	248	57	primary	2016-12-25	2021-12-29	\N
431	249	124	primary	2025-09-28	\N	\N
432	249	184	secondary	2024-01-03	\N	\N
433	250	71	temporary	2022-01-28	\N	\N
434	250	11	primary	2019-07-07	\N	Назначение сотрудника 250 на участок 11
435	251	78	primary	2021-08-05	\N	\N
436	251	18	primary	2020-12-05	\N	\N
437	251	138	primary	2022-02-21	\N	\N
438	252	85	primary	2019-05-24	\N	\N
439	253	92	temporary	2021-03-09	\N	\N
440	253	212	primary	2021-06-12	\N	\N
441	254	159	primary	2019-09-16	\N	Назначение сотрудника 254 на участок 159
442	254	219	secondary	2019-07-23	\N	\N
443	255	46	primary	2023-01-18	2023-04-08	\N
444	255	106	primary	2023-09-03	\N	\N
445	255	166	secondary	2022-10-17	2023-07-18	\N
446	256	53	primary	2024-07-27	\N	\N
447	256	173	primary	2023-12-27	\N	\N
448	256	113	primary	2023-04-22	\N	Назначение сотрудника 256 на участок 113
449	257	60	temporary	2024-07-25	\N	\N
450	257	180	primary	2024-11-27	\N	\N
451	257	120	primary	2025-03-24	\N	\N
452	258	7	secondary	2017-08-09	\N	\N
453	258	67	secondary	2017-01-21	\N	\N
454	259	194	temporary	2024-06-03	\N	\N
455	260	141	temporary	2024-09-17	\N	Назначение сотрудника 260 на участок 141
456	260	201	secondary	2025-09-24	\N	\N
457	261	208	primary	2024-09-14	\N	\N
458	261	148	temporary	2024-07-11	\N	\N
459	262	35	primary	2025-12-02	\N	\N
460	263	102	primary	2022-11-26	\N	\N
461	263	42	primary	2020-10-17	\N	\N
462	263	162	secondary	2022-02-18	\N	Назначение сотрудника 263 на участок 162
463	264	169	primary	2025-11-14	\N	\N
464	265	176	primary	2025-05-07	\N	\N
465	265	56	temporary	2025-03-16	\N	\N
466	266	123	primary	2015-09-15	\N	\N
467	266	183	primary	2017-05-26	\N	\N
468	267	10	primary	2016-06-20	\N	\N
469	268	77	primary	2024-06-24	\N	Назначение сотрудника 268 на участок 77
470	268	137	primary	2023-12-27	\N	\N
471	269	84	secondary	2017-01-18	\N	\N
472	269	24	primary	2016-08-13	\N	\N
473	270	151	primary	2018-04-16	\N	\N
474	270	91	secondary	2019-03-16	\N	\N
475	271	38	primary	2021-09-07	\N	\N
476	272	45	secondary	2025-06-13	\N	Назначение сотрудника 272 на участок 45
477	272	165	primary	2022-11-29	2024-02-14	\N
478	273	172	secondary	2023-05-17	2023-08-31	\N
479	273	52	secondary	2024-01-30	2024-07-11	\N
480	273	112	primary	2023-05-05	2024-03-25	\N
481	274	59	temporary	2025-03-02	\N	\N
482	275	186	secondary	2018-03-12	\N	\N
483	275	6	secondary	2019-07-24	\N	Назначение сотрудника 275 на участок 6
484	275	126	primary	2018-04-14	\N	\N
485	276	133	primary	2018-09-09	\N	\N
486	276	193	secondary	2017-08-13	\N	\N
487	277	20	primary	2019-07-25	\N	\N
488	278	87	primary	2025-05-23	\N	\N
489	278	207	temporary	2025-10-19	\N	\N
490	279	34	primary	2025-01-21	\N	Назначение сотрудника 279 на участок 34
491	280	101	primary	2022-07-22	\N	\N
492	281	48	primary	2015-03-12	\N	\N
493	281	168	temporary	2016-07-16	\N	\N
494	282	175	primary	2023-09-05	\N	\N
495	283	2	primary	2020-11-04	\N	\N
496	283	62	primary	2021-01-04	\N	\N
497	284	69	temporary	2023-04-17	\N	Назначение сотрудника 284 на участок 69
498	285	196	primary	2022-11-02	\N	\N
499	285	136	secondary	2022-11-27	\N	\N
500	285	16	primary	2023-04-01	\N	\N
501	286	143	temporary	2020-02-02	\N	\N
502	286	203	temporary	2019-01-19	\N	\N
503	287	90	secondary	2021-01-01	\N	\N
504	287	150	primary	2021-03-11	\N	Назначение сотрудника 287 на участок 150
505	288	157	temporary	2024-09-16	\N	\N
506	288	37	primary	2024-08-08	\N	\N
507	289	104	secondary	2025-12-28	\N	\N
508	290	171	secondary	2023-06-06	\N	\N
509	291	178	primary	2023-09-28	\N	\N
510	291	118	primary	2021-12-12	\N	\N
511	292	185	temporary	2025-09-04	\N	Назначение сотрудника 292 на участок 185
512	293	192	secondary	2025-11-19	\N	\N
513	293	132	primary	2025-02-14	\N	\N
514	294	79	temporary	2019-11-13	\N	\N
515	295	146	temporary	2025-02-14	\N	\N
516	296	153	primary	2021-04-11	\N	\N
517	296	93	primary	2021-04-10	\N	\N
518	297	100	primary	2025-05-17	\N	Назначение сотрудника 297 на участок 100
519	297	160	primary	2025-10-13	\N	\N
520	298	107	secondary	2020-04-03	\N	\N
521	298	167	primary	2019-12-22	\N	\N
522	299	114	primary	2023-03-24	\N	\N
523	300	1	secondary	2017-02-19	\N	\N
524	301	8	primary	2022-04-21	\N	\N
525	301	68	primary	2021-06-05	\N	Назначение сотрудника 301 на участок 68
526	302	135	primary	2022-04-14	\N	\N
527	302	15	primary	2021-12-23	\N	\N
528	303	82	primary	2019-02-06	\N	\N
529	303	142	primary	2019-04-26	\N	\N
530	304	209	secondary	2021-03-14	\N	\N
531	305	36	temporary	2023-04-12	\N	\N
532	305	96	temporary	2021-03-29	\N	Назначение сотрудника 305 на участок 96
533	306	163	primary	2022-03-04	\N	\N
534	307	170	primary	2023-06-30	\N	\N
535	307	110	primary	2021-05-22	\N	\N
536	308	177	primary	2022-08-16	\N	\N
537	309	4	primary	2021-12-27	\N	\N
538	310	131	secondary	2022-11-26	\N	\N
539	310	191	primary	2022-02-02	\N	Назначение сотрудника 310 на участок 191
540	310	11	secondary	2022-06-14	2024-11-11	\N
541	311	138	primary	2021-12-16	\N	\N
542	312	25	temporary	2018-11-19	\N	\N
543	312	205	primary	2019-11-16	\N	\N
544	313	212	temporary	2023-11-28	\N	\N
545	313	92	primary	2023-02-13	\N	\N
546	314	39	primary	2018-02-02	\N	Назначение сотрудника 314 на участок 39
547	315	106	primary	2017-11-18	\N	\N
548	315	46	primary	2017-11-30	\N	\N
549	316	113	primary	2019-05-10	\N	\N
550	316	53	primary	2020-06-21	\N	\N
551	317	180	primary	2025-02-03	\N	\N
552	318	7	primary	2022-06-11	\N	\N
553	319	194	primary	2016-09-10	\N	Назначение сотрудника 319 на участок 194
554	319	14	secondary	2018-08-26	\N	\N
555	319	134	temporary	2018-02-24	\N	\N
556	320	81	secondary	2025-04-10	\N	\N
557	320	21	primary	2025-08-07	\N	\N
558	320	201	temporary	2025-07-07	2025-12-11	\N
\.


--
-- TOC entry 3673 (class 0 OID 24581)
-- Dependencies: 216
-- Data for Name: employment_status; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.employment_status (id, name) FROM stdin;
1	active
2	vacation
3	sick_leave
4	probation
5	terminated
\.


--
-- TOC entry 3674 (class 0 OID 24584)
-- Dependencies: 217
-- Data for Name: farm; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm (id, name, legal_name, legal_address, actual_address, inn, ogrn, status, farm_type) FROM stdin;
1	Ферма-001	Крестьянское хозяйство «Ферма-001»	г. Краснодар, ул. Молодёжная, д. 101	г. Краснодар, ул. Лесная, д. 201	7701000001	1037700000001	active	peasant_farm
2	Ферма-002	Крестьянское хозяйство «Ферма-002»	г. Ростов-на-Дону, ул. Школьная, д. 102	г. Ростов-на-Дону, ул. Центральная, д. 202	7701000002	1037700000002	inactive	llc
3	Ферма-003	Крестьянское хозяйство «Ферма-003»	г. Белгород, ул. Мира, д. 103	г. Белгород, ул. Лесная, д. 203	7701000003	1037700000003	active	individual_entrepreneur
4	Ферма-004	Крестьянское хозяйство «Ферма-004»	г. Воронеж, ул. Садовая, д. 104	г. Воронеж, ул. Центральная, д. 204	7701000004	1037700000004	inactive	personal_subsidiary
5	Ферма-005	Крестьянское хозяйство «Ферма-005»	г. Тамбов, ул. Лесная, д. 105	г. Тамбов, ул. Лесная, д. 205	7701000005	1037700000005	active	peasant_farm
6	Ферма-006	Крестьянское хозяйство «Ферма-006»	г. Липецк, ул. Луговая, д. 106	г. Липецк, ул. Центральная, д. 206	7701000006	1037700000006	active	llc
7	Ферма-007	Крестьянское хозяйство «Ферма-007»	г. Ставрополь, ул. Полевая, д. 107	г. Ставрополь, ул. Лесная, д. 207	7701000007	1037700000007	active	individual_entrepreneur
8	Ферма-008	Крестьянское хозяйство «Ферма-008»	г. Саратов, ул. Степная, д. 108	г. Саратов, ул. Центральная, д. 208	7701000008	1037700000008	suspended	personal_subsidiary
9	Ферма-009	Крестьянское хозяйство «Ферма-009»	г. Волгоград, ул. Заречная, д. 109	г. Волгоград, ул. Лесная, д. 209	7701000009	1037700000009	active	peasant_farm
10	Ферма-010	Крестьянское хозяйство «Ферма-010»	г. Курск, ул. Центральная, д. 110	г. Курск, ул. Центральная, д. 210	7701000010	1037700000010	suspended	llc
11	Ферма-011	Крестьянское хозяйство «Ферма-011»	г. Краснодар, ул. Молодёжная, д. 111	г. Краснодар, ул. Лесная, д. 211	7701000011	1037700000011	suspended	individual_entrepreneur
12	Ферма-012	Крестьянское хозяйство «Ферма-012»	г. Ростов-на-Дону, ул. Школьная, д. 112	г. Ростов-на-Дону, ул. Центральная, д. 212	7701000012	1037700000012	active	personal_subsidiary
13	Ферма-013	Крестьянское хозяйство «Ферма-013»	г. Белгород, ул. Мира, д. 113	г. Белгород, ул. Лесная, д. 213	7701000013	1037700000013	active	peasant_farm
14	Ферма-014	Крестьянское хозяйство «Ферма-014»	г. Воронеж, ул. Садовая, д. 114	г. Воронеж, ул. Центральная, д. 214	7701000014	1037700000014	active	llc
15	Ферма-015	Крестьянское хозяйство «Ферма-015»	г. Тамбов, ул. Лесная, д. 115	г. Тамбов, ул. Лесная, д. 215	7701000015	1037700000015	active	individual_entrepreneur
16	Ферма-016	Крестьянское хозяйство «Ферма-016»	г. Липецк, ул. Луговая, д. 116	г. Липецк, ул. Центральная, д. 216	7701000016	1037700000016	active	personal_subsidiary
17	Ферма-017	Крестьянское хозяйство «Ферма-017»	г. Ставрополь, ул. Полевая, д. 117	г. Ставрополь, ул. Лесная, д. 217	7701000017	1037700000017	active	peasant_farm
18	Ферма-018	Крестьянское хозяйство «Ферма-018»	г. Саратов, ул. Степная, д. 118	г. Саратов, ул. Центральная, д. 218	7701000018	1037700000018	active	llc
19	Ферма-019	Крестьянское хозяйство «Ферма-019»	г. Волгоград, ул. Заречная, д. 119	г. Волгоград, ул. Лесная, д. 219	7701000019	1037700000019	archived	individual_entrepreneur
20	Ферма-020	Крестьянское хозяйство «Ферма-020»	г. Курск, ул. Центральная, д. 120	г. Курск, ул. Центральная, д. 220	7701000020	1037700000020	active	personal_subsidiary
21	Ферма-021	Крестьянское хозяйство «Ферма-021»	г. Краснодар, ул. Молодёжная, д. 121	г. Краснодар, ул. Лесная, д. 221	7701000021	1037700000021	inactive	peasant_farm
22	Ферма-022	Крестьянское хозяйство «Ферма-022»	г. Ростов-на-Дону, ул. Школьная, д. 122	г. Ростов-на-Дону, ул. Центральная, д. 222	7701000022	1037700000022	active	llc
23	Ферма-023	Крестьянское хозяйство «Ферма-023»	г. Белгород, ул. Мира, д. 123	г. Белгород, ул. Лесная, д. 223	7701000023	1037700000023	suspended	individual_entrepreneur
24	Ферма-024	Крестьянское хозяйство «Ферма-024»	г. Воронеж, ул. Садовая, д. 124	г. Воронеж, ул. Центральная, д. 224	7701000024	1037700000024	active	personal_subsidiary
25	Ферма-025	Крестьянское хозяйство «Ферма-025»	г. Тамбов, ул. Лесная, д. 125	г. Тамбов, ул. Лесная, д. 225	7701000025	1037700000025	active	peasant_farm
26	Ферма-026	Крестьянское хозяйство «Ферма-026»	г. Липецк, ул. Луговая, д. 126	г. Липецк, ул. Центральная, д. 226	7701000026	1037700000026	active	llc
27	Ферма-027	Крестьянское хозяйство «Ферма-027»	г. Ставрополь, ул. Полевая, д. 127	г. Ставрополь, ул. Лесная, д. 227	7701000027	1037700000027	archived	individual_entrepreneur
28	Ферма-028	Крестьянское хозяйство «Ферма-028»	г. Саратов, ул. Степная, д. 128	г. Саратов, ул. Центральная, д. 228	7701000028	1037700000028	active	personal_subsidiary
29	Ферма-029	Крестьянское хозяйство «Ферма-029»	г. Волгоград, ул. Заречная, д. 129	г. Волгоград, ул. Лесная, д. 229	7701000029	1037700000029	active	peasant_farm
30	Ферма-030	Крестьянское хозяйство «Ферма-030»	г. Курск, ул. Центральная, д. 130	г. Курск, ул. Центральная, д. 230	7701000030	1037700000030	active	llc
31	Ферма-031	Крестьянское хозяйство «Ферма-031»	г. Краснодар, ул. Молодёжная, д. 131	г. Краснодар, ул. Лесная, д. 231	7701000031	1037700000031	inactive	individual_entrepreneur
32	Ферма-032	Крестьянское хозяйство «Ферма-032»	г. Ростов-на-Дону, ул. Школьная, д. 132	г. Ростов-на-Дону, ул. Центральная, д. 232	7701000032	1037700000032	active	personal_subsidiary
33	Ферма-033	Крестьянское хозяйство «Ферма-033»	г. Белгород, ул. Мира, д. 133	г. Белгород, ул. Лесная, д. 233	7701000033	1037700000033	archived	peasant_farm
34	Ферма-034	Крестьянское хозяйство «Ферма-034»	г. Воронеж, ул. Садовая, д. 134	г. Воронеж, ул. Центральная, д. 234	7701000034	1037700000034	active	llc
35	Ферма-035	Крестьянское хозяйство «Ферма-035»	г. Тамбов, ул. Лесная, д. 135	г. Тамбов, ул. Лесная, д. 235	7701000035	1037700000035	suspended	individual_entrepreneur
36	Ферма-036	Крестьянское хозяйство «Ферма-036»	г. Липецк, ул. Луговая, д. 136	г. Липецк, ул. Центральная, д. 236	7701000036	1037700000036	active	personal_subsidiary
37	Ферма-037	Крестьянское хозяйство «Ферма-037»	г. Ставрополь, ул. Полевая, д. 137	г. Ставрополь, ул. Лесная, д. 237	7701000037	1037700000037	active	peasant_farm
38	Ферма-038	Крестьянское хозяйство «Ферма-038»	г. Саратов, ул. Степная, д. 138	г. Саратов, ул. Центральная, д. 238	7701000038	1037700000038	active	llc
39	Ферма-039	Крестьянское хозяйство «Ферма-039»	г. Волгоград, ул. Заречная, д. 139	г. Волгоград, ул. Лесная, д. 239	7701000039	1037700000039	active	individual_entrepreneur
40	Ферма-040	Крестьянское хозяйство «Ферма-040»	г. Курск, ул. Центральная, д. 140	г. Курск, ул. Центральная, д. 240	7701000040	1037700000040	active	personal_subsidiary
41	Ферма-041	Крестьянское хозяйство «Ферма-041»	г. Краснодар, ул. Молодёжная, д. 141	г. Краснодар, ул. Лесная, д. 241	7701000041	1037700000041	active	peasant_farm
42	Ферма-042	Крестьянское хозяйство «Ферма-042»	г. Ростов-на-Дону, ул. Школьная, д. 142	г. Ростов-на-Дону, ул. Центральная, д. 242	7701000042	1037700000042	inactive	llc
43	Ферма-043	Крестьянское хозяйство «Ферма-043»	г. Белгород, ул. Мира, д. 143	г. Белгород, ул. Лесная, д. 243	7701000043	1037700000043	suspended	individual_entrepreneur
44	Ферма-044	Крестьянское хозяйство «Ферма-044»	г. Воронеж, ул. Садовая, д. 144	г. Воронеж, ул. Центральная, д. 244	7701000044	1037700000044	active	personal_subsidiary
45	Ферма-045	Крестьянское хозяйство «Ферма-045»	г. Тамбов, ул. Лесная, д. 145	г. Тамбов, ул. Лесная, д. 245	7701000045	1037700000045	active	peasant_farm
46	Ферма-046	Крестьянское хозяйство «Ферма-046»	г. Липецк, ул. Луговая, д. 146	г. Липецк, ул. Центральная, д. 246	7701000046	1037700000046	active	llc
47	Ферма-047	Крестьянское хозяйство «Ферма-047»	г. Ставрополь, ул. Полевая, д. 147	г. Ставрополь, ул. Лесная, д. 247	7701000047	1037700000047	active	individual_entrepreneur
48	Ферма-048	Крестьянское хозяйство «Ферма-048»	г. Саратов, ул. Степная, д. 148	г. Саратов, ул. Центральная, д. 248	7701000048	1037700000048	archived	personal_subsidiary
49	Ферма-049	Крестьянское хозяйство «Ферма-049»	г. Волгоград, ул. Заречная, д. 149	г. Волгоград, ул. Лесная, д. 249	7701000049	1037700000049	active	peasant_farm
50	Ферма-050	Крестьянское хозяйство «Ферма-050»	г. Курск, ул. Центральная, д. 150	г. Курск, ул. Центральная, д. 250	7701000050	1037700000050	active	llc
51	Ферма-051	Крестьянское хозяйство «Ферма-051»	г. Краснодар, ул. Молодёжная, д. 151	г. Краснодар, ул. Лесная, д. 251	7701000051	1037700000051	active	individual_entrepreneur
52	Ферма-052	Крестьянское хозяйство «Ферма-052»	г. Ростов-на-Дону, ул. Школьная, д. 152	г. Ростов-на-Дону, ул. Центральная, д. 252	7701000052	1037700000052	active	personal_subsidiary
53	Ферма-053	Крестьянское хозяйство «Ферма-053»	г. Белгород, ул. Мира, д. 153	г. Белгород, ул. Лесная, д. 253	7701000053	1037700000053	active	peasant_farm
54	Ферма-054	Крестьянское хозяйство «Ферма-054»	г. Воронеж, ул. Садовая, д. 154	г. Воронеж, ул. Центральная, д. 254	7701000054	1037700000054	active	llc
55	Ферма-055	Крестьянское хозяйство «Ферма-055»	г. Тамбов, ул. Лесная, д. 155	г. Тамбов, ул. Лесная, д. 255	7701000055	1037700000055	active	individual_entrepreneur
56	Ферма-056	Крестьянское хозяйство «Ферма-056»	г. Липецк, ул. Луговая, д. 156	г. Липецк, ул. Центральная, д. 256	7701000056	1037700000056	active	personal_subsidiary
57	Ферма-057	Крестьянское хозяйство «Ферма-057»	г. Ставрополь, ул. Полевая, д. 157	г. Ставрополь, ул. Лесная, д. 257	7701000057	1037700000057	active	peasant_farm
58	Ферма-058	Крестьянское хозяйство «Ферма-058»	г. Саратов, ул. Степная, д. 158	г. Саратов, ул. Центральная, д. 258	7701000058	1037700000058	active	llc
59	Ферма-059	Крестьянское хозяйство «Ферма-059»	г. Волгоград, ул. Заречная, д. 159	г. Волгоград, ул. Лесная, д. 259	7701000059	1037700000059	active	individual_entrepreneur
60	Ферма-060	Крестьянское хозяйство «Ферма-060»	г. Курск, ул. Центральная, д. 160	г. Курск, ул. Центральная, д. 260	7701000060	1037700000060	inactive	personal_subsidiary
\.


--
-- TOC entry 3675 (class 0 OID 24589)
-- Dependencies: 218
-- Data for Name: farm_association; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_association (id, inn, ogrn, name, legal_address, status) FROM stdin;
1	7700000001	1027700000001	Ассоциация фермеров «Агросоюз-01»	г. Краснодар, ул. Садовая, д. 11	active
2	7700000002	1027700000002	Ассоциация фермеров «Агросоюз-02»	г. Ростов-на-Дону, ул. Степная, д. 12	active
3	7700000003	1027700000003	Ассоциация фермеров «Агросоюз-03»	г. Белгород, ул. Школьная, д. 13	active
4	7700000004	1027700000004	Ассоциация фермеров «Агросоюз-04»	г. Воронеж, ул. Луговая, д. 14	active
5	7700000005	1027700000005	Ассоциация фермеров «Агросоюз-05»	г. Тамбов, ул. Центральная, д. 15	inactive
6	7700000006	1027700000006	Ассоциация фермеров «Агросоюз-06»	г. Липецк, ул. Садовая, д. 16	suspended
7	7700000007	1027700000007	Ассоциация фермеров «Агросоюз-07»	г. Ставрополь, ул. Степная, д. 17	active
8	7700000008	1027700000008	Ассоциация фермеров «Агросоюз-08»	г. Саратов, ул. Школьная, д. 18	active
\.


--
-- TOC entry 3687 (class 0 OID 24635)
-- Dependencies: 230
-- Data for Name: farm_employee; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_employee (id, person_id, farm_id, role_id, employment_status_id, hire_date, dismissal_date, salary, employment_contract_number, is_primary_workplace) FROM stdin;
1	1	8	1	1	2024-04-12	\N	61452.01	FE-008-00001	1
2	2	15	2	1	2018-04-20	\N	99736.71	FE-015-00002	1
3	3	22	3	1	2020-10-18	\N	133730.05	FE-022-00003	1
4	4	29	4	1	2022-03-31	\N	80848.68	FE-029-00004	1
5	5	36	5	1	2019-05-13	\N	49033.25	FE-036-00005	1
6	6	43	6	1	2025-01-11	\N	109445.35	FE-043-00006	1
7	7	50	7	1	2019-08-21	\N	65844.93	FE-050-00007	1
8	8	57	8	1	2020-02-02	\N	97066.91	FE-057-00008	1
9	9	4	9	1	2017-04-07	\N	131258.17	FE-004-00009	0
10	10	11	10	1	2018-08-20	\N	108930.94	FE-011-00010	1
11	11	18	1	4	2020-03-06	\N	112049.84	FE-018-00011	1
12	12	25	2	1	2016-04-12	\N	110628.53	FE-025-00012	1
13	13	32	3	1	2015-12-16	\N	89286.62	FE-032-00013	1
14	14	39	4	1	2023-11-15	\N	142097.40	FE-039-00014	1
15	15	46	5	1	2017-04-28	\N	72305.40	FE-046-00015	1
16	16	53	6	1	2023-08-13	\N	47993.54	FE-053-00016	1
17	17	60	7	1	2024-10-08	\N	105165.05	FE-060-00017	1
18	18	7	8	1	2024-09-28	\N	44919.06	FE-007-00018	0
19	19	14	9	2	2015-07-30	\N	77091.06	FE-014-00019	1
20	20	21	10	1	2023-08-15	\N	140433.04	FE-021-00020	1
21	21	28	1	1	2017-01-10	\N	41753.82	FE-028-00021	1
22	22	35	2	1	2017-04-10	\N	28413.35	FE-035-00022	1
23	23	42	3	2	2015-02-20	\N	81661.19	FE-042-00023	1
24	24	49	4	3	2021-02-04	\N	122472.18	FE-049-00024	1
25	25	56	5	1	2020-07-14	\N	55831.77	FE-056-00025	1
26	26	3	6	1	2020-05-02	\N	81087.40	FE-003-00026	1
27	27	10	7	1	2021-09-28	\N	98211.95	FE-010-00027	0
28	28	17	8	1	2023-10-09	\N	60539.21	FE-017-00028	1
29	29	24	9	1	2024-03-13	\N	124554.54	FE-024-00029	1
30	30	31	10	1	2021-07-11	\N	34442.48	FE-031-00030	1
31	31	38	1	1	2016-08-18	\N	127898.03	FE-038-00031	1
32	32	45	2	1	2024-10-07	\N	98584.40	FE-045-00032	1
33	33	52	3	1	2015-12-26	\N	33736.67	FE-052-00033	1
34	34	59	4	1	2016-12-31	\N	46876.22	FE-059-00034	1
35	35	6	5	1	2022-05-31	\N	108907.09	FE-006-00035	1
36	36	13	6	1	2023-10-30	\N	93774.73	FE-013-00036	0
37	37	20	7	5	2025-06-04	2025-10-04	136983.14	FE-020-00037	1
38	38	27	8	1	2018-05-10	\N	65770.16	FE-027-00038	1
39	39	34	9	4	2024-03-13	\N	35917.00	FE-034-00039	1
40	40	41	10	1	2021-03-04	\N	35596.33	FE-041-00040	1
41	41	48	1	1	2016-05-14	\N	109270.47	FE-048-00041	1
42	42	55	2	1	2020-11-28	\N	76409.68	FE-055-00042	1
43	43	2	3	1	2023-03-31	\N	99610.29	FE-002-00043	1
44	44	9	4	1	2023-03-08	\N	142583.94	FE-009-00044	1
45	45	16	5	1	2015-12-26	\N	84588.57	FE-016-00045	0
46	46	23	6	1	2024-04-27	\N	143394.58	FE-023-00046	1
47	47	30	7	1	2021-10-29	\N	127192.36	FE-030-00047	1
48	48	37	8	1	2021-06-13	\N	144417.85	FE-037-00048	1
49	49	44	9	1	2016-06-15	\N	99871.93	FE-044-00049	1
50	50	51	10	1	2023-03-06	\N	101927.34	FE-051-00050	1
51	51	58	1	1	2017-01-06	\N	131388.65	FE-058-00051	1
52	52	5	2	1	2018-04-04	\N	142573.15	FE-005-00052	1
53	53	12	3	1	2015-04-09	\N	82391.62	FE-012-00053	1
54	54	19	4	2	2023-07-17	\N	127224.17	FE-019-00054	0
55	55	26	5	2	2021-05-27	\N	76442.81	FE-026-00055	1
56	56	33	6	1	2024-09-23	\N	122307.09	FE-033-00056	1
57	57	40	7	1	2020-12-03	\N	108732.00	FE-040-00057	1
58	58	47	8	1	2015-10-05	\N	64009.39	FE-047-00058	1
59	59	54	9	1	2018-08-19	\N	117742.81	FE-054-00059	1
60	60	1	10	1	2018-09-27	\N	67939.31	FE-001-00060	1
61	61	8	1	1	2021-11-28	\N	140333.20	FE-008-00061	1
62	62	15	2	3	2017-03-05	\N	66670.24	FE-015-00062	1
63	63	22	3	1	2022-07-17	\N	84055.67	FE-022-00063	0
64	64	29	4	1	2015-05-26	\N	103541.31	FE-029-00064	1
65	65	36	5	1	2022-01-13	\N	81892.30	FE-036-00065	1
66	66	43	6	3	2025-01-17	\N	67207.83	FE-043-00066	1
67	67	50	7	2	2019-08-23	\N	136551.43	FE-050-00067	1
68	68	57	8	1	2020-11-04	\N	63023.22	FE-057-00068	1
69	69	4	9	1	2018-07-16	\N	92955.73	FE-004-00069	1
70	70	11	10	1	2021-06-23	\N	108202.72	FE-011-00070	1
71	71	18	1	2	2024-12-16	\N	121106.59	FE-018-00071	1
72	72	25	2	1	2016-09-03	\N	60813.82	FE-025-00072	0
73	73	32	3	1	2019-03-17	\N	101301.77	FE-032-00073	1
74	74	39	4	3	2021-05-27	\N	98865.62	FE-039-00074	1
75	75	46	5	1	2020-09-03	\N	129157.46	FE-046-00075	1
76	76	53	6	2	2022-11-28	\N	128364.88	FE-053-00076	1
77	77	60	7	1	2020-02-06	\N	76422.74	FE-060-00077	1
78	78	7	8	1	2017-08-31	\N	71475.28	FE-007-00078	1
79	79	14	9	3	2018-05-24	\N	53288.37	FE-014-00079	1
80	80	21	10	1	2016-04-04	\N	71783.24	FE-021-00080	1
81	81	28	1	1	2017-04-07	\N	134862.40	FE-028-00081	0
82	82	35	2	1	2015-04-13	\N	121485.96	FE-035-00082	1
83	83	42	3	4	2016-09-23	\N	50254.77	FE-042-00083	1
84	84	49	4	4	2023-03-20	\N	130243.85	FE-049-00084	1
85	85	56	5	1	2018-09-02	\N	44765.52	FE-056-00085	1
86	86	3	6	4	2024-03-21	\N	142926.00	FE-003-00086	1
87	87	10	7	1	2016-06-27	\N	52006.89	FE-010-00087	1
88	88	17	8	1	2019-05-07	\N	111500.25	FE-017-00088	1
89	89	24	9	1	2022-09-06	\N	64384.81	FE-024-00089	1
90	90	31	10	1	2024-07-05	\N	97531.65	FE-031-00090	0
91	91	38	1	1	2020-03-27	\N	132034.75	FE-038-00091	1
92	92	45	2	1	2023-08-05	\N	105101.16	FE-045-00092	1
93	93	52	3	1	2025-02-20	\N	116743.78	FE-052-00093	1
94	94	59	4	1	2017-05-09	\N	65543.00	FE-059-00094	1
95	95	6	5	1	2018-02-07	\N	90276.27	FE-006-00095	1
96	96	13	6	2	2022-07-11	\N	41765.15	FE-013-00096	1
97	97	20	7	1	2018-01-25	\N	41391.87	FE-020-00097	1
98	98	27	8	5	2021-04-11	2024-03-28	39977.91	FE-027-00098	1
99	99	34	9	1	2021-02-28	\N	65262.73	FE-034-00099	0
100	100	41	10	1	2022-11-25	\N	122068.33	FE-041-00100	1
101	101	48	1	1	2016-12-02	\N	36335.42	FE-048-00101	1
102	102	55	2	1	2018-10-19	\N	91247.52	FE-055-00102	1
103	103	2	3	1	2015-04-29	\N	83076.06	FE-002-00103	1
104	104	9	4	1	2016-03-09	\N	72881.16	FE-009-00104	1
105	105	16	5	1	2019-05-25	\N	48941.01	FE-016-00105	1
106	106	23	6	1	2021-08-24	\N	101648.07	FE-023-00106	1
107	107	30	7	1	2020-01-01	\N	100108.89	FE-030-00107	1
108	108	37	8	1	2024-03-02	\N	51183.22	FE-037-00108	0
109	109	44	9	2	2024-01-28	\N	90417.74	FE-044-00109	1
110	110	51	10	5	2023-06-23	2025-09-12	82486.36	FE-051-00110	1
111	111	58	1	1	2024-02-12	\N	121875.01	FE-058-00111	1
112	112	5	2	1	2016-02-03	\N	81303.35	FE-005-00112	1
113	113	12	3	1	2022-12-19	\N	117679.74	FE-012-00113	1
114	114	19	4	1	2017-07-05	\N	44815.44	FE-019-00114	1
115	115	26	5	4	2017-11-22	\N	134804.81	FE-026-00115	1
116	116	33	6	1	2023-11-28	\N	135319.94	FE-033-00116	1
117	117	40	7	1	2024-04-01	\N	81034.12	FE-040-00117	0
118	118	47	8	1	2020-04-30	\N	41207.79	FE-047-00118	1
119	119	54	9	1	2024-06-26	\N	47983.78	FE-054-00119	1
120	120	1	10	2	2021-10-11	\N	95237.36	FE-001-00120	1
121	121	8	1	1	2019-07-20	\N	123653.69	FE-008-00121	1
122	122	15	2	2	2018-08-27	\N	93069.53	FE-015-00122	1
123	123	22	3	1	2018-05-22	\N	90331.23	FE-022-00123	1
124	124	29	4	1	2019-08-21	\N	55913.31	FE-029-00124	1
125	125	36	5	1	2016-10-13	\N	141945.33	FE-036-00125	1
126	126	43	6	1	2018-10-27	\N	52363.86	FE-043-00126	0
127	127	50	7	1	2019-11-01	\N	119764.74	FE-050-00127	1
128	128	57	8	1	2023-10-30	\N	68037.50	FE-057-00128	1
129	129	4	9	1	2016-12-09	\N	105525.63	FE-004-00129	1
130	130	11	10	1	2025-06-09	\N	111293.33	FE-011-00130	1
131	131	18	1	3	2016-04-13	\N	97283.09	FE-018-00131	1
132	132	25	2	1	2021-03-23	\N	39425.51	FE-025-00132	1
133	133	32	3	1	2018-06-15	\N	139838.36	FE-032-00133	1
134	134	39	4	1	2016-03-21	\N	108204.52	FE-039-00134	1
135	135	46	5	1	2024-03-12	\N	72332.59	FE-046-00135	0
136	136	53	6	1	2020-08-15	\N	38592.36	FE-053-00136	1
137	137	60	7	2	2024-12-13	\N	95949.84	FE-060-00137	1
138	138	7	8	2	2024-01-31	\N	28021.57	FE-007-00138	1
139	139	14	9	1	2021-09-28	\N	80998.01	FE-014-00139	1
140	140	21	10	1	2021-11-26	\N	95617.98	FE-021-00140	1
141	141	28	1	1	2022-08-23	\N	130658.77	FE-028-00141	1
142	142	35	2	1	2023-11-28	\N	134844.90	FE-035-00142	1
143	143	42	3	1	2025-07-30	\N	137591.15	FE-042-00143	1
144	144	49	4	1	2025-04-06	\N	68278.96	FE-049-00144	0
145	145	56	5	1	2015-02-03	\N	99544.42	FE-056-00145	1
146	146	3	6	1	2024-07-03	\N	42924.02	FE-003-00146	1
147	147	10	7	1	2019-02-21	\N	133425.86	FE-010-00147	1
148	148	17	8	5	2015-01-04	2022-12-23	93523.54	FE-017-00148	1
149	149	24	9	5	2016-03-25	2025-12-20	61018.48	FE-024-00149	1
150	150	31	10	1	2019-11-04	\N	110159.96	FE-031-00150	1
151	151	38	1	1	2018-07-27	\N	111277.52	FE-038-00151	1
152	152	45	2	1	2019-11-18	\N	91778.90	FE-045-00152	1
153	153	52	3	1	2020-04-29	\N	44271.06	FE-052-00153	0
154	154	59	4	1	2024-06-16	\N	136164.40	FE-059-00154	1
155	155	6	5	3	2016-09-23	\N	61751.09	FE-006-00155	1
156	156	13	6	4	2016-05-29	\N	43710.46	FE-013-00156	1
157	157	20	7	1	2015-03-06	\N	44847.52	FE-020-00157	1
158	158	27	8	4	2022-05-18	\N	79098.01	FE-027-00158	1
159	159	34	9	5	2018-02-27	2019-10-12	103142.42	FE-034-00159	1
160	160	41	10	1	2020-02-11	\N	132549.15	FE-041-00160	1
161	161	48	1	1	2023-03-09	\N	100920.07	FE-048-00161	1
162	162	55	2	1	2019-10-04	\N	92589.85	FE-055-00162	0
163	163	2	3	1	2018-12-26	\N	54411.69	FE-002-00163	1
164	164	9	4	1	2015-01-31	\N	95618.80	FE-009-00164	1
165	165	16	5	3	2017-08-27	\N	111319.21	FE-016-00165	1
166	166	23	6	1	2022-03-02	\N	95433.23	FE-023-00166	1
167	167	30	7	1	2015-06-05	\N	94473.50	FE-030-00167	1
168	168	37	8	1	2016-04-08	\N	95891.45	FE-037-00168	1
169	169	44	9	5	2015-11-24	2017-07-27	113431.70	FE-044-00169	1
170	170	51	10	1	2024-08-20	\N	143089.20	FE-051-00170	1
171	171	58	1	2	2023-04-17	\N	88483.10	FE-058-00171	0
172	172	5	2	1	2021-11-03	\N	38348.22	FE-005-00172	1
173	173	12	3	1	2023-11-04	\N	105657.22	FE-012-00173	1
174	174	19	4	1	2022-09-02	\N	102141.46	FE-019-00174	1
175	175	26	5	2	2018-07-03	\N	99725.58	FE-026-00175	1
176	176	33	6	4	2020-12-31	\N	119087.67	FE-033-00176	1
177	177	40	7	1	2020-09-30	\N	134008.44	FE-040-00177	1
178	178	47	8	1	2018-09-19	\N	77258.88	FE-047-00178	1
179	179	54	9	5	2025-07-10	2025-10-23	61837.39	FE-054-00179	1
180	180	1	10	1	2025-05-08	\N	97923.47	FE-001-00180	0
181	181	8	1	1	2023-10-21	\N	93953.63	FE-008-00181	1
182	182	15	2	1	2020-04-22	\N	95642.51	FE-015-00182	1
183	183	22	3	1	2022-05-12	\N	143872.69	FE-022-00183	1
184	184	29	4	2	2022-01-01	\N	31193.88	FE-029-00184	1
185	185	36	5	1	2016-06-03	\N	87154.74	FE-036-00185	1
186	186	43	6	3	2018-02-12	\N	57489.96	FE-043-00186	1
187	187	50	7	1	2018-02-01	\N	128369.43	FE-050-00187	1
188	188	57	8	1	2016-04-01	\N	98647.12	FE-057-00188	1
189	189	4	9	4	2023-04-15	\N	45682.87	FE-004-00189	0
190	190	11	10	1	2024-01-22	\N	48521.73	FE-011-00190	1
191	191	18	1	2	2017-08-25	\N	84735.47	FE-018-00191	1
192	192	25	2	2	2024-08-31	\N	31871.59	FE-025-00192	1
193	193	32	3	5	2016-08-17	2023-05-22	137271.20	FE-032-00193	1
194	194	39	4	1	2023-05-30	\N	116073.07	FE-039-00194	1
195	195	46	5	1	2019-04-19	\N	55839.85	FE-046-00195	1
196	196	53	6	1	2022-02-24	\N	47572.14	FE-053-00196	1
197	197	60	7	1	2024-06-10	\N	68373.86	FE-060-00197	1
198	198	7	8	1	2023-06-22	\N	123885.13	FE-007-00198	0
199	199	14	9	1	2020-03-15	\N	74653.14	FE-014-00199	1
200	200	21	10	1	2023-05-15	\N	138945.46	FE-021-00200	1
201	201	28	1	4	2018-10-14	\N	72436.69	FE-028-00201	1
202	202	35	2	1	2024-12-24	\N	58727.14	FE-035-00202	1
203	203	42	3	1	2019-11-16	\N	29387.98	FE-042-00203	1
204	204	49	4	4	2022-11-27	\N	72694.48	FE-049-00204	1
205	205	56	5	1	2020-12-11	\N	107896.53	FE-056-00205	1
206	206	3	6	1	2021-08-14	\N	102578.63	FE-003-00206	1
207	207	10	7	5	2019-11-04	2022-12-24	67191.19	FE-010-00207	0
208	208	17	8	1	2016-02-07	\N	57875.24	FE-017-00208	1
209	209	24	9	1	2020-03-03	\N	118458.67	FE-024-00209	1
210	210	31	10	2	2025-07-29	\N	96880.74	FE-031-00210	1
211	211	38	1	1	2018-10-14	\N	107293.02	FE-038-00211	1
212	212	45	2	5	2018-07-07	2024-09-26	29237.62	FE-045-00212	1
213	213	52	3	1	2020-09-08	\N	29399.49	FE-052-00213	1
214	214	59	4	1	2020-02-28	\N	108203.45	FE-059-00214	1
215	215	6	5	1	2023-02-18	\N	61145.75	FE-006-00215	1
216	216	13	6	1	2015-04-08	\N	100469.48	FE-013-00216	0
217	217	20	7	1	2021-08-30	\N	104822.85	FE-020-00217	1
218	218	27	8	3	2021-09-21	\N	102655.70	FE-027-00218	1
219	219	34	9	1	2016-05-06	\N	38107.21	FE-034-00219	1
220	220	41	10	1	2019-03-20	\N	104798.17	FE-041-00220	1
221	221	48	1	4	2021-05-07	\N	121974.41	FE-048-00221	1
222	222	55	2	1	2016-03-30	\N	109391.48	FE-055-00222	1
223	223	2	3	1	2020-03-27	\N	105275.91	FE-002-00223	1
224	224	9	4	1	2020-02-01	\N	135955.36	FE-009-00224	1
225	225	16	5	1	2019-10-15	\N	108989.91	FE-016-00225	0
226	226	23	6	1	2023-08-21	\N	65838.28	FE-023-00226	1
227	227	30	7	1	2015-10-31	\N	60442.64	FE-030-00227	1
228	228	37	8	1	2019-05-11	\N	66920.60	FE-037-00228	1
229	229	44	9	1	2024-02-12	\N	58965.79	FE-044-00229	1
230	230	51	10	1	2020-07-07	\N	142592.37	FE-051-00230	1
231	231	58	1	1	2019-05-28	\N	48877.73	FE-058-00231	1
232	232	5	2	1	2016-08-12	\N	91253.96	FE-005-00232	1
233	233	12	3	1	2021-11-19	\N	103270.57	FE-012-00233	1
234	234	19	4	4	2023-01-09	\N	58302.56	FE-019-00234	0
235	235	26	5	1	2015-10-16	\N	94847.31	FE-026-00235	1
236	236	33	6	1	2023-07-15	\N	30195.48	FE-033-00236	1
237	237	40	7	1	2022-04-28	\N	102919.23	FE-040-00237	1
238	238	47	8	5	2022-06-15	2024-09-21	59173.07	FE-047-00238	1
239	239	54	9	1	2025-02-22	\N	73785.65	FE-054-00239	1
240	240	1	10	1	2022-12-08	\N	137969.60	FE-001-00240	1
241	241	8	1	1	2019-11-22	\N	75257.79	FE-008-00241	1
242	242	15	2	1	2024-02-10	\N	119889.66	FE-015-00242	1
243	243	22	3	1	2015-01-14	\N	72977.59	FE-022-00243	0
244	244	29	4	1	2023-09-30	\N	84401.76	FE-029-00244	1
245	245	36	5	1	2021-01-23	\N	67012.45	FE-036-00245	1
246	246	43	6	1	2015-08-15	\N	101975.89	FE-043-00246	1
247	247	50	7	5	2016-01-07	2020-03-12	90551.28	FE-050-00247	1
248	248	57	8	5	2015-11-26	2025-06-01	81777.80	FE-057-00248	1
249	249	4	9	2	2023-10-03	\N	51765.10	FE-004-00249	1
250	250	11	10	1	2019-04-23	\N	105305.89	FE-011-00250	1
251	1	18	1	1	2019-05-17	\N	132885.90	FE-018-00251	1
252	2	25	2	1	2019-01-11	\N	84920.91	FE-025-00252	0
253	3	32	3	4	2019-11-29	\N	114032.93	FE-032-00253	1
254	4	39	4	1	2017-03-01	\N	30967.66	FE-039-00254	1
255	5	46	5	5	2020-10-07	2023-08-04	73485.80	FE-046-00255	1
256	6	53	6	2	2022-03-20	\N	64004.51	FE-053-00256	1
257	7	60	7	1	2024-05-10	\N	122978.04	FE-060-00257	1
258	8	7	8	5	2015-03-09	2015-09-06	67696.36	FE-007-00258	1
259	9	14	9	1	2023-02-01	\N	33009.30	FE-014-00259	1
260	10	21	10	1	2024-01-19	\N	104500.44	FE-021-00260	1
261	11	28	1	1	2024-05-10	\N	45427.57	FE-028-00261	0
262	12	35	2	1	2025-05-16	\N	36427.92	FE-035-00262	1
263	13	42	3	1	2020-06-25	\N	104498.29	FE-042-00263	1
264	14	49	4	1	2025-04-18	\N	100349.53	FE-049-00264	1
265	15	56	5	1	2022-10-22	\N	43272.91	FE-056-00265	1
266	16	3	6	1	2015-07-24	\N	133845.50	FE-003-00266	1
267	17	10	7	3	2016-02-03	\N	81373.60	FE-010-00267	1
268	18	17	8	1	2021-08-17	\N	35483.04	FE-017-00268	1
269	19	24	9	2	2016-04-12	\N	86165.79	FE-024-00269	1
270	20	31	10	2	2017-06-22	\N	35383.88	FE-031-00270	0
271	21	38	1	1	2018-10-13	\N	122947.96	FE-038-00271	1
272	22	45	2	5	2022-08-10	2024-12-10	65935.49	FE-045-00272	1
273	23	52	3	5	2021-06-17	2024-08-07	44067.54	FE-052-00273	1
274	24	59	4	3	2023-11-23	\N	52948.12	FE-059-00274	1
275	25	6	5	1	2016-12-11	\N	67253.94	FE-006-00275	1
276	26	13	6	1	2016-08-01	\N	116633.36	FE-013-00276	1
277	27	20	7	1	2019-02-24	\N	92748.70	FE-020-00277	1
278	28	27	8	1	2024-02-28	\N	32761.77	FE-027-00278	1
279	29	34	9	1	2024-07-02	\N	101966.77	FE-034-00279	0
280	30	41	10	1	2021-07-31	\N	29828.75	FE-041-00280	1
281	31	48	1	1	2015-01-27	\N	28845.21	FE-048-00281	1
282	32	55	2	4	2023-04-18	\N	104255.43	FE-055-00282	1
283	33	2	3	4	2019-09-01	\N	109139.91	FE-002-00283	1
284	34	9	4	2	2020-05-03	\N	109005.92	FE-009-00284	1
285	35	16	5	1	2022-03-05	\N	59701.12	FE-016-00285	1
286	36	23	6	1	2017-09-06	\N	137756.48	FE-023-00286	1
287	37	30	7	1	2019-05-10	\N	97763.82	FE-030-00287	1
288	38	37	8	1	2021-11-11	\N	139184.87	FE-037-00288	0
289	39	44	9	1	2023-10-11	\N	63240.37	FE-044-00289	1
290	40	51	10	1	2021-04-03	\N	55569.76	FE-051-00290	1
291	41	58	1	1	2020-12-11	\N	142284.60	FE-058-00291	1
292	42	5	2	1	2024-09-03	\N	117052.00	FE-005-00292	1
293	43	12	3	1	2023-04-21	\N	82313.27	FE-012-00293	1
294	44	19	4	1	2017-05-02	\N	60341.91	FE-019-00294	1
295	45	26	5	1	2023-12-07	\N	45458.44	FE-026-00295	1
296	46	33	6	3	2020-10-20	\N	86209.24	FE-033-00296	1
297	47	40	7	1	2025-05-07	\N	33044.53	FE-040-00297	0
298	48	47	8	1	2018-03-13	\N	52522.00	FE-047-00298	1
299	49	54	9	1	2021-12-01	\N	122202.98	FE-054-00299	1
300	50	1	10	2	2015-06-27	\N	40217.27	FE-001-00300	1
301	51	8	1	1	2021-01-10	\N	48578.85	FE-008-00301	1
302	52	15	2	2	2020-06-09	\N	88243.73	FE-015-00302	1
303	53	22	3	1	2017-08-21	\N	112404.16	FE-022-00303	1
304	54	29	4	1	2018-12-05	\N	77030.75	FE-029-00304	1
305	55	36	5	2	2020-05-28	\N	54917.40	FE-036-00305	1
306	56	43	6	1	2020-12-30	\N	39473.50	FE-043-00306	0
307	57	50	7	1	2020-11-28	\N	71937.29	FE-050-00307	1
308	58	57	8	1	2020-04-13	\N	96455.79	FE-057-00308	1
309	59	4	9	1	2019-11-21	\N	44958.13	FE-004-00309	1
310	60	11	10	4	2020-02-07	\N	48833.11	FE-011-00310	1
311	61	18	1	1	2020-02-18	\N	112219.08	FE-018-00311	1
312	62	25	2	1	2018-01-09	\N	51658.67	FE-025-00312	1
313	63	32	3	1	2022-10-06	\N	94533.90	FE-032-00313	1
314	64	39	4	1	2016-03-31	\N	127643.21	FE-039-00314	1
315	65	46	5	1	2016-12-02	\N	116959.73	FE-046-00315	0
316	66	53	6	1	2018-04-23	\N	98625.41	FE-053-00316	1
317	67	60	7	2	2022-02-26	\N	139472.94	FE-060-00317	1
318	68	7	8	4	2021-06-25	\N	141505.89	FE-007-00318	1
319	69	14	9	3	2015-12-21	\N	76168.45	FE-014-00319	1
320	70	21	10	1	2024-09-01	\N	144492.73	FE-021-00320	1
\.


--
-- TOC entry 3688 (class 0 OID 24638)
-- Dependencies: 231
-- Data for Name: farm_owner; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_owner (id, person_id, status, rating) FROM stdin;
1	120	inactive	85.70
2	78	active	73.77
3	177	inactive	62.42
4	42	active	87.33
5	176	archived	92.57
6	122	active	95.55
7	44	active	80.32
8	28	active	82.20
9	43	inactive	82.21
10	164	inactive	94.16
11	93	active	94.68
12	137	active	76.72
13	134	inactive	74.61
14	82	active	84.15
15	71	active	93.64
16	144	active	88.09
17	59	active	64.97
18	69	archived	93.85
19	60	active	85.63
20	40	active	74.26
21	138	active	76.45
22	75	active	83.54
23	79	active	91.78
24	72	inactive	82.27
25	161	active	96.58
26	74	active	75.88
27	126	active	62.36
28	141	inactive	69.27
29	61	active	67.38
30	81	active	79.50
31	63	active	91.36
32	178	active	74.77
33	171	active	58.92
34	57	active	69.06
35	152	archived	88.16
36	151	active	72.95
37	109	active	79.43
38	142	inactive	90.23
39	116	archived	83.37
40	130	inactive	80.02
41	179	active	75.30
42	67	active	91.73
43	29	inactive	92.57
44	112	active	89.15
45	133	active	86.89
46	91	active	75.14
47	27	active	84.32
48	25	active	70.80
49	165	active	90.60
50	70	active	71.64
51	20	active	94.91
52	92	active	91.57
53	158	active	73.21
54	50	active	63.72
55	64	active	92.92
56	89	active	91.69
57	51	active	70.04
58	55	inactive	57.07
59	117	active	84.91
60	30	active	94.68
61	38	active	81.22
62	105	active	97.89
63	125	active	87.76
64	65	active	74.26
65	124	active	96.13
66	168	inactive	95.50
67	87	active	62.73
68	45	inactive	59.61
69	172	active	81.97
70	159	active	94.22
71	114	active	82.10
72	56	active	66.05
73	110	active	70.26
74	85	active	90.81
75	119	active	81.81
76	128	inactive	77.80
77	37	inactive	79.38
78	97	active	87.79
79	157	active	64.67
80	86	active	86.79
\.


--
-- TOC entry 3689 (class 0 OID 24641)
-- Dependencies: 232
-- Data for Name: farm_ownership; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_ownership (id, farm_id, farm_owner_id, ownership_percentage, started_at, ended_at) FROM stdin;
1	1	20	28.00	2015-01-23	\N
2	1	39	22.00	2015-01-23	\N
3	1	13	50.00	2015-01-23	\N
4	2	6	49.00	2013-07-12	\N
5	2	37	51.00	2013-07-12	\N
6	3	78	100.00	2021-10-25	\N
7	4	62	100.00	2015-08-14	\N
8	5	2	38.00	2015-12-22	\N
9	5	65	62.00	2015-12-22	\N
10	6	32	40.00	2013-09-27	\N
11	6	20	60.00	2013-09-27	\N
12	7	13	39.00	2021-11-18	\N
13	7	53	61.00	2021-11-18	\N
14	8	55	63.00	2023-12-09	\N
15	8	39	37.00	2023-12-09	\N
16	9	10	55.00	2014-04-22	\N
17	9	43	45.00	2014-04-22	\N
18	10	54	36.00	2012-02-08	\N
19	10	57	64.00	2012-02-08	\N
20	11	40	100.00	2016-07-10	\N
21	12	12	22.00	2021-06-16	\N
22	12	22	16.00	2021-06-16	\N
23	12	47	62.00	2021-06-16	\N
24	13	13	100.00	2019-07-24	\N
25	14	74	45.00	2015-10-07	\N
26	14	80	41.00	2015-10-07	\N
27	14	69	14.00	2015-10-07	\N
28	15	15	23.00	2019-05-03	\N
29	15	45	67.00	2019-05-03	\N
30	15	63	10.00	2019-05-03	\N
31	16	38	41.00	2020-11-10	\N
32	16	76	59.00	2020-11-10	\N
33	17	34	23.00	2019-04-03	\N
34	17	72	37.00	2019-04-03	\N
35	17	55	40.00	2019-04-03	\N
36	18	51	35.00	2020-09-23	\N
37	18	25	65.00	2020-09-23	\N
38	19	80	56.00	2018-03-31	\N
39	19	74	44.00	2018-03-31	\N
40	20	12	37.00	2016-11-25	\N
41	20	15	63.00	2016-11-25	\N
42	21	59	38.00	2015-11-05	\N
43	21	60	62.00	2015-11-05	\N
44	22	69	45.00	2020-09-18	\N
45	22	59	55.00	2020-09-18	\N
46	23	21	40.00	2019-02-17	\N
47	23	28	60.00	2019-02-17	\N
48	24	3	20.00	2022-01-25	\N
49	24	74	53.00	2022-01-25	\N
50	24	28	27.00	2022-01-25	\N
51	25	72	53.00	2012-04-18	\N
52	25	77	47.00	2012-04-18	\N
53	26	26	44.00	2017-04-20	\N
54	26	34	56.00	2017-04-20	\N
55	27	78	21.00	2020-05-26	\N
56	27	76	23.00	2020-05-26	\N
57	27	31	56.00	2020-05-26	\N
58	28	47	47.00	2016-08-01	\N
59	28	44	53.00	2016-08-01	\N
60	29	22	65.00	2021-12-14	\N
61	29	59	35.00	2021-12-14	\N
62	30	28	30.00	2012-11-20	\N
63	30	21	56.00	2012-11-20	\N
64	30	62	14.00	2012-11-20	\N
65	31	45	59.00	2012-07-13	\N
66	31	50	41.00	2012-07-13	\N
67	32	28	42.00	2016-08-13	\N
68	32	49	58.00	2016-08-13	\N
69	33	78	54.00	2020-07-22	\N
70	33	80	46.00	2020-07-22	\N
71	34	35	55.00	2012-02-19	\N
72	34	57	45.00	2012-02-19	\N
73	35	37	100.00	2013-07-27	\N
74	36	10	24.00	2015-11-23	\N
75	36	29	20.00	2015-11-23	\N
76	36	24	56.00	2015-11-23	\N
77	37	24	51.00	2013-10-02	\N
78	37	22	49.00	2013-10-02	\N
79	38	6	40.00	2016-06-06	\N
80	38	54	60.00	2016-06-06	\N
81	39	14	42.00	2018-10-19	\N
82	39	65	58.00	2018-10-19	\N
83	40	42	43.00	2022-10-16	\N
84	40	47	57.00	2022-10-16	\N
85	41	27	100.00	2020-12-06	\N
86	42	48	48.00	2023-05-02	\N
87	42	57	52.00	2023-05-02	\N
88	43	30	61.00	2022-10-23	\N
89	43	44	39.00	2022-10-23	\N
90	44	40	100.00	2021-08-02	\N
91	45	66	24.00	2015-12-10	\N
92	45	59	50.00	2015-12-10	\N
93	45	32	26.00	2015-12-10	\N
94	46	6	49.00	2018-05-09	\N
95	46	54	51.00	2018-05-09	\N
96	47	65	37.00	2019-06-22	\N
97	47	3	63.00	2019-06-22	\N
98	48	48	100.00	2016-05-22	\N
99	49	38	35.00	2023-03-25	\N
100	49	64	65.00	2023-03-25	\N
101	50	12	49.00	2020-11-06	\N
102	50	37	28.00	2020-11-06	\N
103	50	51	23.00	2020-11-06	\N
104	51	12	29.00	2013-08-06	\N
105	51	75	56.00	2013-08-06	\N
106	51	37	15.00	2013-08-06	\N
107	52	39	47.00	2015-08-23	\N
108	52	34	53.00	2015-08-23	\N
109	53	30	59.00	2019-10-26	\N
110	53	18	41.00	2019-10-26	\N
111	54	37	57.00	2013-04-17	\N
112	54	12	43.00	2013-04-17	\N
113	55	23	42.00	2022-09-18	\N
114	55	78	58.00	2022-09-18	\N
115	56	27	100.00	2012-08-19	\N
116	57	29	54.00	2022-03-20	\N
117	57	11	46.00	2022-03-20	\N
118	58	29	39.00	2012-04-11	\N
119	58	18	25.00	2012-04-11	\N
120	58	78	36.00	2012-04-11	\N
121	59	5	100.00	2020-08-14	\N
122	60	36	51.00	2016-03-26	\N
123	60	31	49.00	2016-03-26	\N
\.


--
-- TOC entry 3690 (class 0 OID 24644)
-- Dependencies: 233
-- Data for Name: farm_plot; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_plot (id, farm_plot_type_id, name, area, location, cadastral_number, status) FROM stdin;
1	3	Участок-001	210.24	г. Воронеж, район 2	95:63:9957402:0001	active
2	4	Участок-002	298.85	г. Ставрополь, район 3	11:76:2786278:0002	active
3	6	Участок-003	0.90	г. Курск, район 4	95:17:4353806:0003	active
4	7	Участок-004	3.97	г. Белгород, район 5	46:97:5203716:0004	active
5	9	Участок-005	7.48	г. Липецк, район 6	47:36:1542011:0005	active
6	10	Участок-006	23.10	г. Волгоград, район 7	88:96:4763253:0006	active
7	12	Участок-007	92.97	г. Ростов-на-Дону, район 8	40:75:5825775:0007	active
8	14	Участок-008	1.18	г. Тамбов, район 9	58:59:7988821:0008	archived
9	15	Участок-009	14.65	г. Саратов, район 10	50:80:1902651:0009	active
10	3	Участок-010	99.20	г. Краснодар, район 11	89:66:4784075:0010	suspended
11	4	Участок-011	251.05	г. Воронеж, район 12	92:57:4296204:0011	inactive
12	6	Участок-012	4.83	г. Ставрополь, район 13	47:67:3976712:0012	archived
13	7	Участок-013	0.56	г. Курск, район 14	32:75:3002424:0013	active
14	9	Участок-014	22.14	г. Белгород, район 15	81:43:3182154:0014	active
15	10	Участок-015	38.84	г. Липецк, район 16	10:52:8823741:0015	active
16	12	Участок-016	40.12	г. Волгоград, район 17	29:51:1853212:0016	inactive
17	14	Участок-017	9.57	г. Ростов-на-Дону, район 18	94:48:9230079:0017	inactive
18	15	Участок-018	8.48	г. Тамбов, район 1	19:50:9814666:0018	active
19	3	Участок-019	76.60	г. Саратов, район 2	18:74:3673793:0019	active
20	4	Участок-020	194.91	г. Краснодар, район 3	21:54:4711087:0020	suspended
21	6	Участок-021	3.38	г. Воронеж, район 4	52:56:5925360:0021	active
22	7	Участок-022	3.81	г. Ставрополь, район 5	77:70:1147701:0022	active
23	9	Участок-023	18.84	г. Курск, район 6	40:92:5090740:0023	archived
24	10	Участок-024	32.30	г. Белгород, район 7	51:58:2902100:0024	active
25	12	Участок-025	90.23	г. Липецк, район 8	46:13:9671603:0025	inactive
26	14	Участок-026	5.94	г. Волгоград, район 9	75:67:9167227:0026	active
27	15	Участок-027	13.53	г. Ростов-на-Дону, район 10	34:51:9621722:0027	active
28	3	Участок-028	47.15	г. Тамбов, район 11	78:78:1122986:0028	active
29	4	Участок-029	85.61	г. Саратов, район 12	90:37:8117053:0029	inactive
30	6	Участок-030	1.19	г. Краснодар, район 13	99:65:2209981:0030	active
31	7	Участок-031	0.94	г. Воронеж, район 14	68:52:1635626:0031	active
32	9	Участок-032	7.17	г. Ставрополь, район 15	42:81:2181822:0032	active
33	10	Участок-033	14.11	г. Курск, район 16	43:62:7510106:0033	active
34	12	Участок-034	72.58	г. Белгород, район 17	50:12:7534756:0034	active
35	14	Участок-035	8.26	г. Липецк, район 18	92:89:2060950:0035	inactive
36	15	Участок-036	8.99	г. Волгоград, район 1	99:19:7001212:0036	suspended
37	3	Участок-037	50.56	г. Ростов-на-Дону, район 2	97:48:2451316:0037	active
38	4	Участок-038	325.44	г. Тамбов, район 3	58:60:1517059:0038	active
39	6	Участок-039	2.76	г. Саратов, район 4	27:60:5948917:0039	archived
40	7	Участок-040	0.89	г. Краснодар, район 5	96:92:7260091:0040	archived
41	9	Участок-041	5.45	г. Воронеж, район 6	81:28:3067542:0041	active
42	10	Участок-042	27.91	г. Ставрополь, район 7	60:78:2428191:0042	active
43	12	Участок-043	117.28	г. Курск, район 8	54:37:7923193:0043	suspended
44	14	Участок-044	12.85	г. Белгород, район 9	28:30:4086885:0044	active
45	15	Участок-045	12.60	г. Липецк, район 10	34:24:4042682:0045	active
46	3	Участок-046	205.04	г. Волгоград, район 11	16:80:8738745:0046	active
47	4	Участок-047	39.50	г. Ростов-на-Дону, район 12	95:18:5205840:0047	active
48	6	Участок-048	3.68	г. Тамбов, район 13	72:52:6991787:0048	active
49	7	Участок-049	3.35	г. Саратов, район 14	22:46:2171198:0049	suspended
50	9	Участок-050	17.71	г. Краснодар, район 15	67:52:6128383:0050	active
51	10	Участок-051	11.10	г. Воронеж, район 16	82:56:6307189:0051	archived
52	12	Участок-052	41.92	г. Ставрополь, район 17	50:41:4716584:0052	active
53	14	Участок-053	9.79	г. Курск, район 18	45:56:3253277:0053	active
54	15	Участок-054	5.31	г. Белгород, район 1	71:69:8471866:0054	active
55	3	Участок-055	117.33	г. Липецк, район 2	80:23:3948933:0055	active
56	4	Участок-056	42.46	г. Волгоград, район 3	45:27:4295580:0056	active
57	6	Участок-057	4.45	г. Ростов-на-Дону, район 4	19:19:1312564:0057	inactive
58	7	Участок-058	3.23	г. Тамбов, район 5	84:84:7797028:0058	inactive
59	9	Участок-059	13.51	г. Саратов, район 6	14:80:5372416:0059	active
60	10	Участок-060	44.95	г. Краснодар, район 7	70:93:3493647:0060	active
61	12	Участок-061	49.03	г. Воронеж, район 8	85:47:3246154:0061	active
62	14	Участок-062	7.90	г. Ставрополь, район 9	60:56:9424968:0062	active
63	15	Участок-063	3.96	г. Курск, район 10	88:97:3782393:0063	inactive
64	3	Участок-064	62.02	г. Белгород, район 11	30:32:3452813:0064	active
65	4	Участок-065	176.97	г. Липецк, район 12	55:15:4671175:0065	inactive
66	6	Участок-066	1.31	г. Волгоград, район 13	43:57:4926181:0066	active
67	7	Участок-067	1.19	г. Ростов-на-Дону, район 14	56:59:8879825:0067	suspended
68	9	Участок-068	40.26	г. Тамбов, район 15	15:93:6437011:0068	active
69	10	Участок-069	43.74	г. Саратов, район 16	91:45:5417710:0069	active
70	12	Участок-070	94.12	г. Краснодар, район 17	58:93:7350486:0070	active
71	14	Участок-071	9.97	г. Воронеж, район 18	19:76:5220919:0071	active
72	15	Участок-072	13.66	г. Ставрополь, район 1	76:61:6920646:0072	inactive
73	3	Участок-073	130.59	г. Курск, район 2	71:87:1110690:0073	active
74	4	Участок-074	61.55	г. Белгород, район 3	31:39:2293325:0074	active
75	6	Участок-075	1.55	г. Липецк, район 4	29:33:3640018:0075	active
76	7	Участок-076	3.64	г. Волгоград, район 5	25:94:4864570:0076	active
77	9	Участок-077	18.21	г. Ростов-на-Дону, район 6	24:71:9033176:0077	active
78	10	Участок-078	28.63	г. Тамбов, район 7	92:62:1376582:0078	active
79	12	Участок-079	36.63	г. Саратов, район 8	85:30:3260294:0079	active
80	14	Участок-080	1.07	г. Краснодар, район 9	83:28:1938047:0080	active
81	15	Участок-081	13.88	г. Воронеж, район 10	76:61:1312392:0081	suspended
82	3	Участок-082	258.87	г. Ставрополь, район 11	46:20:4633834:0082	active
83	4	Участок-083	347.58	г. Курск, район 12	71:31:4680314:0083	inactive
84	6	Участок-084	3.08	г. Белгород, район 13	84:33:9569884:0084	active
85	7	Участок-085	3.68	г. Липецк, район 14	53:68:2154992:0085	active
86	9	Участок-086	20.42	г. Волгоград, район 15	23:12:4394845:0086	active
87	10	Участок-087	41.37	г. Ростов-на-Дону, район 16	97:63:3509220:0087	active
88	12	Участок-088	43.06	г. Тамбов, район 17	88:86:1654924:0088	suspended
89	14	Участок-089	8.21	г. Саратов, район 18	25:21:9090660:0089	active
90	15	Участок-090	13.58	г. Краснодар, район 1	79:10:8239315:0090	active
91	3	Участок-091	139.08	г. Воронеж, район 2	15:99:9888846:0091	active
92	4	Участок-092	329.37	г. Ставрополь, район 3	71:58:6880713:0092	active
93	6	Участок-093	3.76	г. Курск, район 4	99:83:2639417:0093	active
94	7	Участок-094	1.42	г. Белгород, район 5	39:20:5752426:0094	active
95	9	Участок-095	26.75	г. Липецк, район 6	42:34:1091762:0095	active
96	10	Участок-096	36.58	г. Волгоград, район 7	83:76:2976835:0096	active
97	12	Участок-097	68.75	г. Ростов-на-Дону, район 8	36:98:6299951:0097	active
98	14	Участок-098	2.05	г. Тамбов, район 9	64:93:5255251:0098	active
99	15	Участок-099	8.27	г. Саратов, район 10	82:56:5623138:0099	archived
100	3	Участок-100	148.15	г. Краснодар, район 11	36:62:9094465:0100	active
101	4	Участок-101	202.30	г. Воронеж, район 12	71:42:6144785:0101	active
102	6	Участок-102	3.92	г. Ставрополь, район 13	60:27:2997927:0102	active
103	7	Участок-103	2.34	г. Курск, район 14	99:30:2682396:0103	active
104	9	Участок-104	21.02	г. Белгород, район 15	53:80:1873301:0104	active
105	10	Участок-105	43.58	г. Липецк, район 16	10:21:3438206:0105	active
106	12	Участок-106	98.03	г. Волгоград, район 17	21:18:5413951:0106	active
107	14	Участок-107	1.66	г. Ростов-на-Дону, район 18	14:59:7721705:0107	active
108	15	Участок-108	8.05	г. Тамбов, район 1	86:17:3940561:0108	active
109	3	Участок-109	162.06	г. Саратов, район 2	35:94:9693169:0109	active
110	4	Участок-110	26.26	г. Краснодар, район 3	86:75:9653916:0110	active
111	6	Участок-111	3.15	г. Воронеж, район 4	96:14:7035435:0111	active
112	7	Участок-112	0.20	г. Ставрополь, район 5	30:21:5629452:0112	inactive
113	9	Участок-113	19.13	г. Курск, район 6	60:61:4360650:0113	active
114	10	Участок-114	34.48	г. Белгород, район 7	18:21:5773013:0114	active
115	12	Участок-115	92.68	г. Липецк, район 8	84:54:6829664:0115	archived
116	14	Участок-116	4.51	г. Волгоград, район 9	87:34:8629590:0116	suspended
117	15	Участок-117	8.84	г. Ростов-на-Дону, район 10	43:32:7830585:0117	active
118	3	Участок-118	202.14	г. Тамбов, район 11	74:47:6703681:0118	suspended
119	4	Участок-119	283.99	г. Саратов, район 12	42:78:5196677:0119	active
120	6	Участок-120	4.84	г. Краснодар, район 13	82:71:5081447:0120	inactive
121	7	Участок-121	4.81	г. Воронеж, район 14	68:97:8729606:0121	suspended
122	9	Участок-122	39.48	г. Ставрополь, район 15	71:24:5325497:0122	active
123	10	Участок-123	30.35	г. Курск, район 16	38:96:4857587:0123	inactive
124	12	Участок-124	73.62	г. Белгород, район 17	55:76:9986747:0124	active
125	14	Участок-125	12.57	г. Липецк, район 18	41:46:2560439:0125	active
126	15	Участок-126	11.36	г. Волгоград, район 1	53:19:3994493:0126	active
127	3	Участок-127	133.44	г. Ростов-на-Дону, район 2	83:44:4066614:0127	suspended
128	4	Участок-128	243.84	г. Тамбов, район 3	34:73:4349848:0128	active
129	6	Участок-129	1.36	г. Саратов, район 4	43:36:4845464:0129	active
130	7	Участок-130	1.40	г. Краснодар, район 5	89:55:3617171:0130	inactive
131	9	Участок-131	9.80	г. Воронеж, район 6	20:96:9265544:0131	active
132	10	Участок-132	35.57	г. Ставрополь, район 7	79:15:9091518:0132	active
133	12	Участок-133	61.00	г. Курск, район 8	83:45:9234347:0133	active
134	14	Участок-134	11.58	г. Белгород, район 9	75:45:7675310:0134	active
135	15	Участок-135	8.86	г. Липецк, район 10	16:77:8099712:0135	active
136	3	Участок-136	196.16	г. Волгоград, район 11	74:54:7605473:0136	inactive
137	4	Участок-137	274.93	г. Ростов-на-Дону, район 12	68:25:9694039:0137	active
138	6	Участок-138	3.51	г. Тамбов, район 13	11:96:4535838:0138	active
139	7	Участок-139	0.68	г. Саратов, район 14	56:88:5357055:0139	active
140	9	Участок-140	15.51	г. Краснодар, район 15	44:70:6756203:0140	active
141	10	Участок-141	24.95	г. Воронеж, район 16	36:63:7246319:0141	active
142	12	Участок-142	72.08	г. Ставрополь, район 17	67:33:3200150:0142	archived
143	14	Участок-143	10.45	г. Курск, район 18	22:44:4956154:0143	active
144	15	Участок-144	12.92	г. Белгород, район 1	35:28:3509000:0144	active
145	3	Участок-145	18.26	г. Липецк, район 2	63:73:3256060:0145	archived
146	4	Участок-146	299.55	г. Волгоград, район 3	76:78:6297024:0146	inactive
147	6	Участок-147	1.64	г. Ростов-на-Дону, район 4	81:82:1800429:0147	active
148	7	Участок-148	3.51	г. Тамбов, район 5	17:37:5266272:0148	active
149	9	Участок-149	30.19	г. Саратов, район 6	67:52:5358202:0149	active
150	10	Участок-150	27.41	г. Краснодар, район 7	42:54:1332781:0150	active
151	12	Участок-151	65.64	г. Воронеж, район 8	96:87:4337963:0151	active
152	14	Участок-152	9.92	г. Ставрополь, район 9	78:32:5774942:0152	active
153	15	Участок-153	10.26	г. Курск, район 10	72:67:2315459:0153	active
154	3	Участок-154	110.99	г. Белгород, район 11	90:28:3811450:0154	active
155	4	Участок-155	326.07	г. Липецк, район 12	63:90:7790959:0155	inactive
156	6	Участок-156	0.54	г. Волгоград, район 13	30:53:4828861:0156	active
157	7	Участок-157	3.33	г. Ростов-на-Дону, район 14	44:87:7634639:0157	active
158	9	Участок-158	22.80	г. Тамбов, район 15	56:85:9739850:0158	archived
159	10	Участок-159	21.56	г. Саратов, район 16	33:12:3097821:0159	active
160	12	Участок-160	106.06	г. Краснодар, район 17	99:20:4532803:0160	active
161	14	Участок-161	1.99	г. Воронеж, район 18	18:70:1903041:0161	active
162	15	Участок-162	6.26	г. Ставрополь, район 1	20:23:5403218:0162	active
163	3	Участок-163	43.24	г. Курск, район 2	59:49:4847954:0163	active
164	4	Участок-164	110.20	г. Белгород, район 3	90:26:3202464:0164	active
165	6	Участок-165	1.02	г. Липецк, район 4	14:94:6986802:0165	inactive
166	7	Участок-166	1.76	г. Волгоград, район 5	70:69:5570355:0166	active
167	9	Участок-167	10.40	г. Ростов-на-Дону, район 6	99:79:3970181:0167	active
168	10	Участок-168	23.26	г. Тамбов, район 7	91:82:8297709:0168	active
169	12	Участок-169	103.14	г. Саратов, район 8	49:54:9080774:0169	active
170	14	Участок-170	12.17	г. Краснодар, район 9	66:49:7136825:0170	active
171	15	Участок-171	2.76	г. Воронеж, район 10	11:10:9779789:0171	inactive
172	3	Участок-172	299.06	г. Ставрополь, район 11	54:46:8871835:0172	active
173	4	Участок-173	245.40	г. Курск, район 12	72:71:6026630:0173	active
174	6	Участок-174	2.27	г. Белгород, район 13	47:86:4879150:0174	inactive
175	7	Участок-175	2.83	г. Липецк, район 14	34:62:8333427:0175	active
176	9	Участок-176	11.53	г. Волгоград, район 15	92:59:7347794:0176	active
177	10	Участок-177	8.17	г. Ростов-на-Дону, район 16	90:40:9667278:0177	inactive
178	12	Участок-178	138.29	г. Тамбов, район 17	98:97:5561139:0178	active
179	14	Участок-179	11.27	г. Саратов, район 18	44:62:4154127:0179	inactive
180	15	Участок-180	3.14	г. Краснодар, район 1	47:24:8639388:0180	active
181	3	Участок-181	106.16	г. Воронеж, район 2	89:72:4150584:0181	active
182	4	Участок-182	65.97	г. Ставрополь, район 3	78:12:4119515:0182	archived
183	6	Участок-183	1.55	г. Курск, район 4	26:66:1137641:0183	active
184	7	Участок-184	0.92	г. Белгород, район 5	73:37:6736345:0184	active
185	9	Участок-185	36.43	г. Липецк, район 6	73:57:8779334:0185	active
186	10	Участок-186	6.27	г. Волгоград, район 7	20:28:1940167:0186	active
187	12	Участок-187	71.01	г. Ростов-на-Дону, район 8	79:70:4046805:0187	active
188	14	Участок-188	12.70	г. Тамбов, район 9	58:11:3046175:0188	archived
189	15	Участок-189	10.86	г. Саратов, район 10	59:66:5146693:0189	active
190	3	Участок-190	26.23	г. Краснодар, район 11	68:22:9743263:0190	active
191	4	Участок-191	84.83	г. Воронеж, район 12	59:97:6236326:0191	suspended
192	6	Участок-192	0.96	г. Ставрополь, район 13	15:80:9739692:0192	active
193	7	Участок-193	2.51	г. Курск, район 14	48:35:7046955:0193	suspended
194	9	Участок-194	39.55	г. Белгород, район 15	37:24:4958849:0194	active
195	10	Участок-195	17.98	г. Липецк, район 16	31:45:3007387:0195	active
196	12	Участок-196	51.60	г. Волгоград, район 17	15:84:8973718:0196	active
197	14	Участок-197	2.75	г. Ростов-на-Дону, район 18	21:41:3645741:0197	active
198	15	Участок-198	10.45	г. Тамбов, район 1	53:90:2007511:0198	active
199	3	Участок-199	230.93	г. Саратов, район 2	99:78:2801733:0199	active
200	4	Участок-200	92.95	г. Краснодар, район 3	81:92:6827024:0200	inactive
201	6	Участок-201	2.31	г. Воронеж, район 4	34:12:7337911:0201	active
202	7	Участок-202	0.85	г. Ставрополь, район 5	15:72:8340032:0202	active
203	9	Участок-203	26.71	г. Курск, район 6	72:34:4131076:0203	active
204	10	Участок-204	17.95	г. Белгород, район 7	69:45:3690116:0204	active
205	12	Участок-205	40.15	г. Липецк, район 8	47:43:7159129:0205	active
206	14	Участок-206	11.75	г. Волгоград, район 9	74:58:4112748:0206	suspended
207	15	Участок-207	9.49	г. Ростов-на-Дону, район 10	56:89:5006173:0207	active
208	3	Участок-208	36.22	г. Тамбов, район 11	78:59:4185480:0208	active
209	4	Участок-209	181.68	г. Саратов, район 12	80:29:5750117:0209	active
210	6	Участок-210	3.28	г. Краснодар, район 13	24:22:9777848:0210	active
211	7	Участок-211	0.41	г. Воронеж, район 14	26:32:1524686:0211	active
212	9	Участок-212	24.41	г. Ставрополь, район 15	22:14:7519606:0212	active
213	10	Участок-213	26.03	г. Курск, район 16	99:85:5293936:0213	active
214	12	Участок-214	48.69	г. Белгород, район 17	19:11:4502044:0214	active
215	14	Участок-215	13.96	г. Липецк, район 18	20:69:2018380:0215	active
216	15	Участок-216	11.28	г. Волгоград, район 1	63:57:1978112:0216	inactive
217	3	Участок-217	104.05	г. Ростов-на-Дону, район 2	15:19:6098672:0217	active
218	4	Участок-218	87.33	г. Тамбов, район 3	76:22:2698275:0218	active
219	6	Участок-219	2.75	г. Саратов, район 4	34:34:9828037:0219	active
220	7	Участок-220	2.94	г. Краснодар, район 5	32:81:7965641:0220	active
\.


--
-- TOC entry 3691 (class 0 OID 24647)
-- Dependencies: 234
-- Data for Name: farm_plot_assignment; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_plot_assignment (farm_id, farm_plot_id, status, notes) FROM stdin;
1	1	active	\N
1	61	active	\N
1	121	active	\N
1	181	inactive	\N
2	2	active	\N
2	62	inactive	\N
2	122	active	\N
2	182	active	\N
3	3	active	\N
3	63	active	\N
3	123	active	\N
3	183	active	\N
4	4	active	\N
4	64	inactive	\N
4	124	active	\N
4	184	active	\N
5	5	active	\N
5	65	active	\N
5	125	active	\N
5	185	active	\N
6	6	active	Привязка участка 6 к ферме 6
6	66	active	Привязка участка 66 к ферме 6
6	126	active	Привязка участка 126 к ферме 6
6	186	suspended	Привязка участка 186 к ферме 6
7	7	active	\N
7	67	active	\N
7	127	active	\N
7	187	active	\N
8	8	active	\N
8	68	inactive	\N
8	128	active	\N
8	188	active	\N
9	9	active	\N
9	69	active	\N
9	129	inactive	\N
9	189	active	\N
10	10	active	\N
10	70	active	\N
10	130	inactive	\N
10	190	suspended	\N
11	11	active	\N
11	71	suspended	\N
11	131	active	\N
11	191	active	\N
12	12	active	Привязка участка 12 к ферме 12
12	72	active	Привязка участка 72 к ферме 12
12	132	active	Привязка участка 132 к ферме 12
12	192	active	Привязка участка 192 к ферме 12
13	13	suspended	\N
13	73	inactive	\N
13	133	suspended	\N
13	193	active	\N
14	14	inactive	\N
14	74	active	\N
14	134	active	\N
14	194	active	\N
15	15	active	\N
15	75	suspended	\N
15	135	active	\N
15	195	active	\N
16	16	suspended	\N
16	76	active	\N
16	136	active	\N
16	196	active	\N
17	17	inactive	\N
17	77	active	\N
17	137	active	\N
17	197	active	\N
18	18	active	Привязка участка 18 к ферме 18
18	78	active	Привязка участка 78 к ферме 18
18	138	active	Привязка участка 138 к ферме 18
18	198	suspended	Привязка участка 198 к ферме 18
19	19	active	\N
19	79	inactive	\N
19	139	active	\N
19	199	suspended	\N
20	20	inactive	\N
20	80	suspended	\N
20	140	active	\N
20	200	active	\N
21	21	active	\N
21	81	active	\N
21	141	active	\N
21	201	active	\N
22	22	active	\N
22	82	active	\N
22	142	active	\N
22	202	active	\N
23	23	active	\N
23	83	active	\N
23	143	inactive	\N
23	203	active	\N
24	24	active	Привязка участка 24 к ферме 24
24	84	active	Привязка участка 84 к ферме 24
24	144	active	Привязка участка 144 к ферме 24
24	204	suspended	Привязка участка 204 к ферме 24
25	25	active	\N
25	85	active	\N
25	145	active	\N
25	205	suspended	\N
26	26	active	\N
26	86	active	\N
26	146	inactive	\N
26	206	suspended	\N
27	27	active	\N
27	87	active	\N
27	147	active	\N
27	207	suspended	\N
28	28	active	\N
28	88	active	\N
28	148	active	\N
28	208	active	\N
29	29	suspended	\N
29	89	active	\N
29	149	active	\N
29	209	active	\N
30	30	active	Привязка участка 30 к ферме 30
30	90	active	Привязка участка 90 к ферме 30
30	150	active	Привязка участка 150 к ферме 30
30	210	active	Привязка участка 210 к ферме 30
31	31	active	\N
31	91	suspended	\N
31	151	active	\N
31	211	active	\N
32	32	active	\N
32	92	suspended	\N
32	152	active	\N
32	212	active	\N
33	33	active	\N
33	93	active	\N
33	153	active	\N
33	213	active	\N
34	34	active	\N
34	94	active	\N
34	154	active	\N
34	214	active	\N
35	35	inactive	\N
35	95	active	\N
35	155	active	\N
35	215	inactive	\N
36	36	active	Привязка участка 36 к ферме 36
36	96	active	Привязка участка 96 к ферме 36
36	156	suspended	Привязка участка 156 к ферме 36
36	216	active	Привязка участка 216 к ферме 36
37	37	suspended	\N
37	97	active	\N
37	157	active	\N
37	217	active	\N
38	38	active	\N
38	98	active	\N
38	158	active	\N
38	218	active	\N
39	39	active	\N
39	99	active	\N
39	159	active	\N
39	219	active	\N
40	40	active	\N
40	100	active	\N
40	160	inactive	\N
40	220	active	\N
41	41	suspended	\N
41	101	active	\N
41	161	active	\N
42	42	inactive	Привязка участка 42 к ферме 42
42	102	active	Привязка участка 102 к ферме 42
42	162	active	Привязка участка 162 к ферме 42
43	43	inactive	\N
43	103	active	\N
43	163	active	\N
44	44	active	\N
44	104	active	\N
44	164	active	\N
45	45	active	\N
45	105	active	\N
45	165	active	\N
46	46	active	\N
46	106	active	\N
46	166	active	\N
47	47	active	\N
47	107	active	\N
47	167	active	\N
48	48	active	Привязка участка 48 к ферме 48
48	108	active	Привязка участка 108 к ферме 48
48	168	suspended	Привязка участка 168 к ферме 48
49	49	active	\N
49	109	active	\N
49	169	active	\N
50	50	active	\N
50	110	active	\N
50	170	active	\N
51	51	inactive	\N
51	111	active	\N
51	171	active	\N
52	52	active	\N
52	112	active	\N
52	172	inactive	\N
53	53	active	\N
53	113	suspended	\N
53	173	active	\N
54	54	active	Привязка участка 54 к ферме 54
54	114	active	Привязка участка 114 к ферме 54
54	174	active	Привязка участка 174 к ферме 54
55	55	active	\N
55	115	active	\N
55	175	active	\N
56	56	active	\N
56	116	active	\N
56	176	inactive	\N
57	57	active	\N
57	117	active	\N
57	177	active	\N
58	58	active	\N
58	118	active	\N
58	178	suspended	\N
59	59	active	\N
59	119	active	\N
59	179	active	\N
60	60	active	Привязка участка 60 к ферме 60
60	120	active	Привязка участка 120 к ферме 60
60	180	active	Привязка участка 180 к ферме 60
\.


--
-- TOC entry 3700 (class 0 OID 24682)
-- Dependencies: 243
-- Data for Name: farm_plot_consumption_product; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_plot_consumption_product (product_id, farm_plot_id, quantity, consumption_now) FROM stdin;
1	16	207	1
1	20	112	1
1	37	95	0
1	103	431	0
1	118	881	0
1	151	872	0
1	174	1071	1
1	193	779	1
1	195	12	0
1	197	1032	0
1	214	885	1
2	109	597	1
2	216	865	1
3	26	967	0
3	49	487	1
3	57	821	1
3	121	287	1
3	123	827	1
3	152	568	1
4	12	913	0
4	44	1128	1
4	50	1178	1
4	58	1125	0
4	80	792	0
4	88	792	0
4	127	200	0
4	146	810	0
4	158	678	1
4	161	510	1
4	167	335	0
4	211	1148	1
5	60	275	1
5	77	683	1
5	159	219	0
5	178	1027	1
5	200	242	1
5	209	1142	0
6	54	952	1
6	116	660	1
6	128	430	1
7	29	1100	1
7	71	624	0
7	111	127	1
7	120	383	0
7	167	84	1
8	165	82	1
8	220	455	0
9	10	162	0
9	24	972	0
9	31	997	0
9	79	525	1
9	98	627	0
9	138	1004	0
10	5	595	1
10	70	927	0
10	84	471	0
10	91	646	0
10	119	372	0
10	128	633	1
10	148	1029	1
10	176	898	1
11	112	644	1
11	157	852	0
11	167	643	0
11	218	822	1
12	47	1089	0
12	58	1166	0
12	102	58	1
12	165	1015	0
12	166	584	1
12	172	281	0
12	175	276	0
12	200	1140	1
12	219	1152	1
13	4	1127	1
13	39	1159	0
13	59	1005	1
13	65	210	1
13	84	987	1
13	116	835	0
13	122	60	1
13	164	1158	1
13	171	1118	1
13	172	1019	1
14	5	700	1
14	68	110	0
14	69	665	0
14	76	1047	1
14	144	861	0
14	206	679	1
15	33	558	1
15	82	335	0
15	89	1147	0
15	128	1184	1
15	130	329	1
15	157	789	0
15	161	562	0
15	201	509	1
16	168	153	0
16	169	415	1
16	175	617	1
16	204	1144	0
17	9	1123	0
17	87	596	0
17	126	101	1
17	143	419	0
17	209	179	0
18	7	627	0
18	38	579	0
18	77	628	0
18	93	267	1
18	99	112	1
18	125	574	1
18	152	246	1
18	156	28	1
18	190	58	1
18	198	375	1
18	220	1053	1
19	9	927	1
19	48	853	0
19	61	798	1
19	71	551	1
19	118	410	1
19	123	200	0
19	189	923	0
19	197	839	1
19	203	1091	1
20	28	97	0
20	48	786	0
20	49	975	1
20	50	882	1
20	57	492	1
20	69	411	1
20	96	570	0
20	107	737	1
20	122	650	1
20	144	698	1
21	7	1092	0
21	8	949	0
21	42	269	0
21	198	297	1
22	37	189	1
22	56	544	1
22	65	701	0
22	97	437	1
22	151	200	1
22	154	800	0
22	160	721	0
22	180	315	1
22	203	20	0
23	81	585	0
23	103	560	0
23	129	749	1
23	184	643	0
23	209	462	1
24	6	927	1
24	157	1166	1
24	183	523	0
24	189	181	1
24	194	87	0
24	214	863	0
25	7	619	1
25	13	915	1
25	111	770	1
25	133	1199	0
25	162	468	1
26	25	1088	1
26	42	660	0
26	53	1120	1
26	149	506	1
26	153	910	1
27	9	123	1
27	33	616	1
27	54	661	1
27	68	636	0
27	77	1161	0
27	124	1068	1
28	47	193	0
28	65	656	1
28	74	732	1
28	130	525	1
28	163	863	0
28	176	1049	1
29	36	43	0
29	62	567	0
29	64	1002	1
29	67	578	1
29	94	86	1
29	141	833	0
29	145	440	0
30	69	914	1
30	71	1054	1
30	78	363	1
30	105	140	1
30	114	990	1
30	126	382	0
30	136	833	0
30	198	322	1
30	211	241	0
31	148	855	1
31	181	173	0
31	205	154	1
31	212	684	1
32	110	896	1
32	135	1178	1
32	139	129	0
32	141	764	1
32	146	850	0
32	180	586	0
33	19	968	1
33	23	223	1
33	36	1128	1
33	81	255	1
33	85	1156	1
33	104	293	1
33	212	33	1
34	102	616	0
34	119	73	1
34	165	150	0
34	170	157	1
34	194	1053	1
35	28	847	1
35	30	1189	1
35	32	694	0
35	33	747	1
35	82	11	1
35	94	432	0
35	138	803	0
35	140	973	0
35	142	894	0
35	145	677	1
36	1	650	0
36	43	455	1
36	92	361	0
36	106	176	1
36	135	743	1
36	186	740	1
36	199	323	1
37	2	430	1
37	40	237	0
37	84	619	0
37	86	268	1
37	110	907	1
37	131	263	1
37	185	138	0
37	206	163	0
37	208	1143	1
38	3	460	0
38	10	1074	1
38	45	913	1
38	48	701	1
38	72	479	0
38	122	988	1
38	147	1163	1
38	175	945	0
38	177	389	0
38	192	567	1
38	204	741	0
39	28	439	1
39	35	34	1
39	55	761	1
39	63	652	1
39	73	451	0
39	96	251	1
39	132	881	0
39	144	325	0
39	153	667	0
39	156	1177	1
39	170	434	1
39	215	740	0
40	173	38	0
41	17	1042	1
41	53	1079	1
41	74	1026	1
41	202	402	1
42	18	726	1
42	23	530	0
42	52	916	1
42	103	690	1
42	113	193	0
42	145	591	1
42	199	613	1
43	5	38	1
43	38	40	1
43	58	1124	1
43	101	1000	0
43	117	935	0
43	121	673	0
43	131	385	1
43	170	324	0
43	182	110	1
44	8	1011	1
44	21	854	0
44	143	800	1
44	188	717	0
45	88	1182	0
45	155	425	0
45	210	1024	0
46	22	151	1
46	66	265	0
46	95	931	0
46	101	734	1
46	136	193	1
46	220	985	0
47	44	578	1
47	75	333	0
48	11	631	1
48	47	189	1
48	79	1131	1
48	119	406	1
48	187	222	1
48	213	47	1
49	4	272	1
49	90	946	1
49	99	790	1
49	100	451	1
49	101	328	0
49	106	992	1
49	179	255	0
50	10	414	0
50	13	499	0
50	46	830	1
50	56	1131	1
50	59	59	0
50	60	247	1
50	80	619	1
50	81	1177	1
50	137	832	1
50	152	255	0
50	183	695	1
51	8	1056	0
51	12	669	1
51	43	222	1
51	70	81	1
51	142	482	1
52	11	1182	1
52	18	739	0
52	56	912	1
52	87	63	0
52	104	1102	1
52	155	396	1
52	161	74	0
52	168	86	1
52	190	745	1
53	27	402	1
53	64	396	1
53	143	516	1
53	158	848	0
53	196	125	0
54	15	176	1
54	49	300	1
54	168	1119	0
54	219	461	1
55	26	482	0
55	73	49	1
55	133	116	0
55	177	494	1
55	216	104	1
56	3	448	1
56	37	518	1
56	63	99	1
56	80	960	0
56	91	8	1
56	114	354	0
56	133	553	1
56	134	934	0
56	183	141	1
56	188	828	1
57	132	359	1
57	212	829	1
58	17	573	1
58	51	1135	1
58	52	536	1
58	54	727	1
58	75	306	0
58	86	57	0
58	90	778	0
58	158	1132	1
58	188	750	0
58	217	564	1
59	40	1095	1
59	63	1010	1
59	98	743	1
59	108	1063	1
60	34	424	1
60	174	335	0
60	205	64	1
60	211	204	1
61	31	1184	0
61	40	747	0
61	72	55	0
61	150	272	1
61	163	425	0
61	191	51	0
61	197	254	1
61	199	738	0
61	208	935	1
62	45	1062	1
62	57	227	1
62	67	565	0
62	74	431	0
62	104	6	1
62	171	209	1
63	1	163	1
63	35	1048	0
63	82	874	1
63	95	38	1
63	115	1128	1
63	171	168	1
63	185	960	1
64	184	9	1
64	213	1176	1
65	50	355	1
66	24	15	1
66	26	135	1
66	41	876	0
66	43	330	1
66	83	197	1
66	139	858	1
66	149	950	1
66	207	200	0
67	18	676	0
67	21	276	1
67	38	644	1
67	42	12	1
67	59	1051	0
67	64	918	1
67	67	642	1
67	186	583	1
68	108	653	1
68	111	1102	1
68	186	457	0
68	205	1111	1
69	68	161	1
69	70	901	0
69	87	499	0
69	164	581	1
70	1	1041	1
70	13	813	0
70	14	126	1
70	29	574	0
70	51	118	0
70	94	872	1
70	148	464	0
70	166	217	1
70	196	210	1
70	216	777	1
\.


--
-- TOC entry 3701 (class 0 OID 24685)
-- Dependencies: 244
-- Data for Name: farm_plot_production_product; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_plot_production_product (product_id, farm_plot_id, quantity, production_now) FROM stdin;
1	3	2872	1
1	11	2880	1
1	39	1307	1
1	42	2147	0
1	58	2208	1
1	64	946	1
1	76	1337	1
1	85	2120	1
1	99	1598	1
1	103	1849	1
1	139	169	1
1	147	2717	1
1	177	1266	1
1	192	2889	1
1	195	1955	1
1	205	22	1
1	217	515	1
2	7	1109	0
2	63	1702	1
2	78	1500	1
2	112	730	1
2	124	377	1
2	129	1559	1
2	137	2980	1
2	138	2780	1
2	140	1277	1
2	148	2145	0
2	168	1367	1
2	197	2739	0
3	28	1402	1
3	38	2021	1
3	144	444	1
3	172	2378	0
4	18	1168	1
4	148	2477	1
4	153	2618	1
4	160	114	1
4	161	538	1
4	176	1342	0
5	16	2776	0
5	25	2840	1
5	62	2541	1
5	118	1122	1
5	121	1916	1
5	148	648	0
5	159	2002	0
6	42	2028	1
6	59	710	1
6	76	233	1
6	95	1116	1
6	98	2250	1
6	101	749	1
6	119	54	0
6	123	2520	1
7	69	1799	1
7	119	1176	0
7	150	2626	1
7	155	2589	1
7	175	2872	0
7	176	717	1
7	177	1368	1
7	213	1670	1
8	7	49	1
8	22	1609	0
8	26	2955	1
8	97	2590	1
8	107	1498	0
8	133	1959	1
8	181	905	1
8	214	2681	1
9	3	1785	1
9	37	2221	1
9	125	385	1
9	169	1181	1
9	190	433	1
9	193	1772	1
9	217	646	1
10	29	2872	0
10	56	366	1
10	60	1572	1
10	103	1331	1
10	110	598	1
10	116	2490	0
10	158	1304	1
10	170	831	1
10	194	59	1
11	6	141	1
11	56	696	1
11	88	254	1
11	95	455	1
11	96	1987	0
11	134	354	1
11	155	474	1
11	214	1126	0
12	2	878	0
12	19	2480	1
12	34	1147	1
12	55	1300	0
12	56	63	0
12	79	2598	1
12	104	2044	1
12	106	1961	0
12	144	2544	0
12	152	2551	1
12	162	1225	1
12	180	1012	1
12	206	350	1
12	212	1013	1
13	3	1727	1
13	8	2159	0
13	15	2506	1
13	30	2397	0
13	36	2660	1
13	50	2889	1
13	77	2032	1
13	80	2555	1
13	94	68	1
13	120	635	1
13	124	1287	1
13	139	2481	1
13	145	2754	1
13	151	2383	0
13	157	1714	1
13	174	1145	1
13	191	862	1
13	192	1270	0
13	196	1074	1
13	208	2465	1
13	215	269	1
14	2	1650	1
14	6	371	1
14	32	2803	0
14	65	1797	1
14	79	512	1
14	90	1831	1
14	98	2115	0
14	110	143	1
14	117	1286	1
14	129	933	1
14	130	1645	0
14	176	62	1
14	182	2125	0
14	200	1860	0
14	218	2091	1
15	47	384	1
15	75	1169	0
15	100	1174	1
15	132	1944	1
15	142	2456	1
15	161	1361	1
15	185	87	0
15	189	491	1
15	198	1700	0
16	47	2821	1
16	49	2176	1
16	61	2605	1
16	67	2812	1
16	72	1766	1
16	82	2165	0
16	159	2477	1
16	168	2839	1
16	179	1977	1
16	219	1677	1
16	220	2712	1
17	16	1932	1
17	21	2383	1
17	30	241	0
17	44	688	1
17	52	537	1
17	68	1429	1
17	72	837	1
17	78	2432	1
17	79	2700	0
17	83	2339	1
17	95	2589	1
17	114	1626	1
17	120	2082	1
17	127	1550	1
17	139	2256	0
17	150	1323	1
17	151	2589	0
17	173	2180	1
17	213	2141	1
18	4	471	1
18	31	879	1
18	57	2792	0
18	87	880	1
18	94	470	0
18	96	1279	1
18	105	2226	1
18	111	1246	1
18	178	626	0
18	212	1548	1
18	216	2965	0
19	24	2067	1
19	32	250	1
19	48	1564	0
19	87	231	1
19	99	118	1
19	103	483	1
19	104	1597	1
19	109	127	1
19	122	1418	1
19	124	1882	1
19	134	2937	1
19	138	636	0
19	155	2460	1
19	196	1456	0
19	203	2162	0
20	26	235	0
20	33	1259	1
20	64	1553	1
20	72	1663	1
20	81	2315	0
20	101	1391	1
20	102	1130	0
20	103	1979	0
20	129	1655	0
20	183	916	0
20	206	825	1
20	216	2574	1
21	18	1075	1
21	31	1212	0
21	40	163	1
21	65	796	1
21	110	1725	1
21	117	287	1
21	122	557	1
21	125	2381	1
21	152	402	1
21	204	284	0
22	1	2853	1
22	6	1153	1
22	51	1615	1
22	100	1496	0
22	136	1736	1
22	164	1755	1
22	171	1588	1
22	178	537	1
22	193	1884	1
22	196	1394	1
23	49	1275	1
23	60	228	1
23	89	2221	1
23	92	758	1
23	134	303	1
23	163	1629	1
24	4	2012	1
24	5	1082	0
24	36	1242	1
24	39	2292	1
24	63	435	1
24	70	491	0
24	85	2445	1
24	92	2353	1
24	154	1236	1
25	5	1694	1
25	13	1353	1
25	22	2802	0
25	46	648	1
25	58	2945	1
25	84	2123	0
25	92	106	1
25	93	1766	1
25	140	86	1
25	163	830	1
25	182	1414	1
25	184	623	1
25	188	2189	1
26	4	890	1
26	71	2077	1
26	81	1732	1
26	96	1839	0
26	136	2213	1
26	146	529	1
26	162	2920	1
26	165	283	1
26	184	692	1
26	209	575	1
27	40	379	1
27	43	901	0
27	85	675	1
27	90	1639	1
27	140	1536	0
27	152	2450	1
27	158	1866	0
27	164	1132	0
27	218	1960	1
28	8	1558	1
28	44	2568	1
28	114	2310	1
28	122	2513	1
28	135	398	0
28	161	361	1
29	19	2958	1
29	20	790	1
29	23	2224	0
29	24	617	1
29	48	930	1
29	60	566	1
29	86	995	1
29	99	2704	1
29	106	90	1
29	147	37	0
29	154	1568	0
29	176	2986	1
29	180	1465	1
29	189	166	1
29	191	853	1
29	216	46	0
30	20	1413	1
30	72	807	0
30	122	2074	1
30	151	2085	1
31	17	2722	1
31	56	2289	0
31	69	2465	1
31	118	2355	1
31	149	809	1
31	154	1024	1
31	185	786	1
31	193	702	1
32	17	452	1
32	42	1221	1
32	53	2914	1
32	61	1245	1
32	71	2913	0
32	95	58	0
32	97	2246	1
32	105	2201	1
32	108	248	1
32	167	1542	1
32	195	1210	0
33	1	1587	1
33	45	2864	1
33	64	1774	0
33	90	2599	0
33	126	527	0
33	163	2958	1
33	209	837	1
34	52	2344	1
34	86	2281	1
34	88	62	0
34	113	1755	0
34	128	1022	1
34	135	1775	1
34	156	2044	0
34	180	863	1
34	194	719	1
34	208	2329	1
34	220	619	1
35	46	2497	1
35	48	1140	1
35	65	592	1
35	67	2023	1
35	75	2934	1
35	87	25	1
35	178	2409	1
35	198	2720	0
36	12	1038	1
36	36	1592	1
36	44	1583	1
36	77	777	1
36	83	1206	1
36	87	1039	0
36	89	1068	0
36	104	542	1
36	125	2930	0
36	132	894	1
36	155	2393	0
36	170	1321	0
36	200	2178	1
36	201	1426	0
37	102	1107	0
37	112	64	1
37	143	2561	0
37	156	759	1
37	172	2656	0
37	173	171	0
37	192	778	1
37	204	2449	1
38	6	1977	1
38	15	2362	0
38	24	623	1
38	61	910	0
38	121	2583	1
38	129	1913	1
38	166	489	1
38	188	2144	1
38	215	2414	1
39	41	722	1
39	42	1842	1
39	86	937	1
39	102	2730	1
39	174	2299	0
39	175	517	0
39	204	83	1
39	208	1340	0
39	210	2255	1
39	212	2245	0
39	219	773	0
40	25	2604	0
40	55	1271	1
40	164	1795	0
40	205	1462	1
41	25	2913	1
41	51	2796	0
41	115	1541	1
41	119	2068	1
41	138	1316	1
41	168	2344	0
41	193	72	1
41	211	1930	1
42	18	2766	1
42	30	2926	1
42	37	2943	0
42	88	2524	1
42	104	2860	0
42	138	624	1
42	152	67	1
43	19	1425	1
43	26	941	1
43	78	1542	1
43	105	1734	1
43	131	2495	1
43	142	1548	1
43	215	859	1
44	9	1425	0
44	33	1023	1
44	69	1175	1
44	74	1982	1
44	112	475	1
44	159	668	1
45	2	46	0
45	50	2508	1
45	107	590	1
45	136	678	0
45	159	1062	0
45	162	2050	1
46	43	2837	1
46	44	2272	1
46	54	1194	1
46	63	2126	0
46	120	1377	1
46	137	2317	0
46	165	158	0
46	167	317	1
46	186	1787	1
46	207	2629	1
46	211	2309	1
47	10	1570	1
47	60	483	1
47	75	1330	0
47	78	1150	1
47	116	1394	1
47	133	886	0
47	197	1464	1
47	199	813	0
48	21	1303	0
48	46	1796	1
48	53	2797	1
48	88	2977	1
48	91	403	1
48	147	1718	1
48	177	1083	0
49	20	1081	1
49	32	1960	1
49	34	511	1
49	73	2170	1
49	90	1120	1
49	109	1759	1
49	114	2555	1
49	143	2617	1
49	153	554	0
49	158	751	1
50	15	606	0
50	18	576	0
50	68	106	1
50	100	471	1
50	105	962	1
50	123	644	1
50	137	2465	1
50	169	2179	1
51	27	2720	0
51	28	1959	0
51	29	2996	1
51	38	2415	1
51	66	1376	0
51	84	697	1
51	93	304	1
51	147	2098	1
51	157	2031	0
51	175	2040	0
51	191	2853	1
51	201	1286	1
51	213	1915	0
52	10	1478	0
52	14	1446	1
52	33	726	1
52	52	58	1
52	63	829	1
52	94	2996	0
52	114	336	0
52	115	817	1
52	127	1825	1
52	131	284	1
52	173	2661	1
52	184	2384	1
52	188	2810	1
53	2	604	1
53	13	1495	0
53	73	2614	0
53	127	2578	0
53	166	1462	1
54	66	676	1
54	67	1350	0
54	106	983	1
54	108	883	0
54	158	756	1
54	170	2043	1
54	174	1436	1
54	201	2393	1
55	1	2203	1
55	32	234	1
55	45	854	1
55	69	1776	1
55	71	442	1
55	74	1769	1
55	102	1446	0
55	150	731	1
55	165	1551	0
55	188	743	1
56	20	379	1
56	33	1006	1
56	77	1266	1
56	82	2083	0
56	91	627	0
56	123	999	1
56	133	2820	1
56	143	426	0
56	149	2296	0
56	170	1144	1
56	187	1136	1
56	199	865	1
56	202	2341	0
56	205	584	1
56	220	2115	1
57	31	821	1
57	65	2924	1
57	112	2651	1
57	126	2019	1
57	145	1742	1
57	211	1938	1
57	220	776	1
58	80	326	0
58	82	297	1
58	128	2657	1
58	139	788	1
58	171	2483	1
58	179	2181	1
58	186	2274	1
59	11	588	1
59	113	1273	1
59	132	148	0
59	141	1462	0
59	183	452	0
59	197	1386	0
59	202	1441	1
60	34	1843	1
60	54	1772	1
60	62	1242	1
60	130	371	1
60	210	1809	1
61	9	1538	1
61	13	32	0
61	71	792	0
61	81	199	1
61	111	2180	1
61	126	2683	0
61	132	176	1
61	146	1920	1
61	207	1038	0
62	31	57	1
62	52	2941	1
62	57	2878	1
62	110	335	1
62	115	1578	1
62	119	401	0
62	121	680	1
62	180	1233	1
62	198	2640	1
62	215	2956	1
63	9	2495	1
63	14	2529	0
63	19	2379	0
63	27	1245	0
63	35	941	1
63	62	41	1
63	100	2541	1
63	164	2571	1
63	182	2438	0
63	195	91	1
63	204	910	1
64	16	1847	0
64	84	2106	1
64	141	1715	1
64	181	890	1
64	203	1930	1
64	216	1252	1
65	16	376	1
65	25	2047	1
65	41	1509	0
65	54	2962	0
65	68	469	0
65	113	1133	0
65	128	2815	1
65	145	1882	1
65	183	1599	0
65	187	1386	1
65	196	429	1
65	206	2941	1
65	211	1700	1
66	13	1339	1
66	35	2366	0
66	43	790	1
66	46	2492	1
66	57	2059	1
66	62	255	1
66	70	1041	1
66	184	301	1
66	187	221	1
67	23	456	1
67	142	740	1
67	179	1169	1
67	198	1483	1
68	12	2884	0
68	29	2891	0
68	38	869	0
68	50	1175	1
68	59	2507	1
68	86	1233	1
68	94	1567	1
68	106	852	0
68	135	773	0
68	166	1341	1
68	186	1107	1
69	22	1329	0
69	38	2689	1
69	43	203	1
69	47	290	1
69	81	1054	0
69	84	1618	1
69	130	463	0
69	160	644	1
69	177	196	1
69	190	2560	1
70	17	886	1
70	61	1104	1
70	117	64	0
70	145	2400	0
70	200	2801	1
\.


--
-- TOC entry 3676 (class 0 OID 24594)
-- Dependencies: 219
-- Data for Name: farm_plot_type; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_plot_type (id, name, description, farm_plot_level, parent_id) FROM stdin;
1	Сельхозземля	Корневой тип земель сельхозназначения	1	\N
2	Полевой участок	Открытый полевой участок	2	1
3	Орошаемый участок	Полевой участок с орошением	3	2
4	Богарный участок	Полевой участок без орошения	3	2
5	Тепличный комплекс	Участок защищенного грунта	2	1
6	Стеклянная теплица	Капитальная стеклянная теплица	3	5
7	Пленочная теплица	Легкая пленочная теплица	3	5
8	Сад	Плодовый участок	2	1
9	Яблоневый сад	Сад яблонь	3	8
10	Грушевый сад	Сад груш	3	8
11	Пастбище	Участок для выпаса животных	2	1
12	Культурное пастбище	Улучшенное пастбище	3	11
13	Животноводческая зона	Корневой тип для производственных зон	1	\N
14	Коровник	Здание/зона для содержания КРС	2	13
15	Птичник	Здание/зона для птицы	2	13
\.


--
-- TOC entry 3677 (class 0 OID 24597)
-- Dependencies: 220
-- Data for Name: farm_role; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.farm_role (id, name, description) FROM stdin;
1	Управляющий фермы	Отвечает за общую деятельность хозяйства
2	Агроном	Организует полевые работы
3	Ветеринар	Контролирует здоровье животных
4	Тракторист	Работает на тракторной технике
5	Комбайнер	Управляет зерноуборочной техникой
6	Кладовщик	Ведет складской учет
7	Бухгалтер	Ведет финансовый учет
8	Рабочий	Универсальный сотрудник
9	Специалист по орошению	Контролирует ирригационные системы
10	Механик	Обслуживает технику и оборудование
\.


--
-- TOC entry 3678 (class 0 OID 24600)
-- Dependencies: 221
-- Data for Name: identity_document_type; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.identity_document_type (id, code, name, description) FROM stdin;
1	passport_rf	Паспорт РФ	Основной документ гражданина Российской Федерации
2	foreign_passport	Заграничный паспорт	Документ для выезда за пределы РФ
3	military_id	Военный билет	Документ воинского учета
4	residence_permit	Вид на жительство	Документ для иностранных граждан
5	birth_certificate	Свидетельство о рождении	Документ для несовершеннолетних
\.


--
-- TOC entry 3679 (class 0 OID 24603)
-- Dependencies: 222
-- Data for Name: person; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.person (id, first_name, last_name, middle_name, birth_date, phone, email, address) FROM stdin;
1	Антон	Петров	Иванович	1976-09-03	+79413282723	person001@farm-association.test	г. Волгоград, ул. Полевая, д. 152, кв. 109
2	Иван	Иванов	Петрович	1971-08-13	+79396178713	person002@farm-association.test	г. Волгоград, ул. Молодёжная, д. 167, кв. 180
3	Юлия	Васильева	Алексеевна	1992-04-19	+79853841030	person003@farm-association.test	г. Ставрополь, ул. Лесная, д. 72, кв. 40
4	Алексей	Зайцев	Николаевич	1961-03-03	+79214892255	person004@farm-association.test	г. Липецк, ул. Мира, д. 68, кв. 207
5	Иван	Соловьёв	Евгеньевич	2000-02-09	+79254872080	person005@farm-association.test	г. Тамбов, ул. Мира, д. 93, кв. 148
6	Ольга	Соловьёва	Петровна	1956-02-10	+79943334720	person006@farm-association.test	г. Воронеж, ул. Полевая, д. 98, кв. 72
7	Евгений	Волков	Николаевич	1966-08-04	+79574633695	person007@farm-association.test	г. Тамбов, ул. Полевая, д. 156, кв. 163
8	Сергей	Фёдоров	Алексеевич	1966-08-29	+79694884491	person008@farm-association.test	г. Волгоград, ул. Молодёжная, д. 176, кв. 84
9	Полина	Зайцева	Ивановна	1972-07-19	+79149245061	person009@farm-association.test	г. Тамбов, ул. Полевая, д. 55, кв. 234
10	Владимир	Лебедев	Николаевич	1971-01-28	+79936116092	person010@farm-association.test	г. Саратов, ул. Садовая, д. 68, кв. 36
11	Алексей	Соловьёв	Михайлович	2000-05-09	+79438648464	person011@farm-association.test	г. Курск, ул. Школьная, д. 93, кв. 57
12	Елена	Фёдорова	Евгеньевна	1960-02-26	+79169812429	person012@farm-association.test	г. Белгород, ул. Школьная, д. 153, кв. 17
13	Андрей	Васильев	Владимирович	1993-12-28	+79773578011	person013@farm-association.test	г. Ростов-на-Дону, ул. Луговая, д. 69, кв. 197
14	Антон	Попов	Петрович	1978-05-01	+79652616810	person014@farm-association.test	г. Тамбов, ул. Луговая, д. 46, кв. 130
15	Дарья	Петрова	Дмитриевна	1997-07-18	+79873032957	person015@farm-association.test	г. Белгород, ул. Луговая, д. 136, кв. 236
16	Иван	Морозов	Николаевич	1995-11-01	+79122145649	person016@farm-association.test	г. Воронеж, ул. Центральная, д. 62, кв. 225
17	Владимир	Петров	Петрович	1995-08-07	+79188787826	person017@farm-association.test	г. Белгород, ул. Заречная, д. 141, кв. 43
18	Татьяна	Фёдорова	Владимировна	1989-12-17	+79376529835	person018@farm-association.test	г. Тамбов, ул. Школьная, д. 172, кв. 167
19	Николай	Новиков	Михайлович	1992-07-03	+79253533818	person019@farm-association.test	г. Липецк, ул. Центральная, д. 151, кв. 142
20	Алексей	Морозов	Алексеевич	1952-08-23	+79198249017	person020@farm-association.test	г. Воронеж, ул. Полевая, д. 9, кв. 221
21	Наталья	Петрова	Михайловна	1973-05-08	+79457857237	person021@farm-association.test	г. Волгоград, ул. Садовая, д. 147, кв. 148
22	Евгений	Кузнецов	Евгеньевич	1988-07-08	+79341962294	person022@farm-association.test	г. Ставрополь, ул. Лесная, д. 109, кв. 106
23	Евгений	Павлов	Иванович	1960-10-29	+79175125323	person023@farm-association.test	г. Воронеж, ул. Молодёжная, д. 49, кв. 138
24	Светлана	Сидорова	Андреевна	1968-06-17	+79455734119	person024@farm-association.test	г. Саратов, ул. Луговая, д. 26, кв. 13
25	Антон	Фёдоров	Иванович	1960-05-14	+79402706272	person025@farm-association.test	г. Саратов, ул. Молодёжная, д. 103, кв. 232
26	Иван	Сидоров	Андреевич	1952-03-11	+79593716846	person026@farm-association.test	г. Ставрополь, ул. Луговая, д. 170, кв. 184
27	Светлана	Сидорова	Алексеевна	1978-08-14	+79371598479	person027@farm-association.test	г. Краснодар, ул. Лесная, д. 15, кв. 13
28	Владимир	Новиков	Михайлович	1999-08-25	+79301587520	person028@farm-association.test	г. Белгород, ул. Полевая, д. 153, кв. 18
29	Антон	Павлов	Алексеевич	1988-03-22	+79256834184	person029@farm-association.test	г. Курск, ул. Центральная, д. 159, кв. 21
30	Ирина	Волкова	Владимировна	2002-09-16	+79764234336	person030@farm-association.test	г. Липецк, ул. Молодёжная, д. 68, кв. 102
31	Сергей	Волков	Дмитриевич	1993-01-07	+79508691911	person031@farm-association.test	г. Саратов, ул. Мира, д. 145, кв. 26
32	Пётр	Фёдоров	Алексеевич	1997-05-20	+79432355418	person032@farm-association.test	г. Воронеж, ул. Лесная, д. 73, кв. 41
33	Светлана	Павлова	Михайловна	1979-02-20	+79889269377	person033@farm-association.test	г. Краснодар, ул. Луговая, д. 77, кв. 239
34	Антон	Петров	Сергеевич	1975-09-23	+79242098029	person034@farm-association.test	г. Тамбов, ул. Степная, д. 155, кв. 54
35	Роман	Попов	Алексеевич	1975-09-07	+79746004216	person035@farm-association.test	г. Ростов-на-Дону, ул. Школьная, д. 71, кв. 12
36	Анна	Попова	Сергеевна	1975-07-02	+79308596680	person036@farm-association.test	г. Ставрополь, ул. Луговая, д. 3, кв. 29
37	Пётр	Лебедев	Сергеевич	2000-12-11	+79149545784	person037@farm-association.test	г. Волгоград, ул. Садовая, д. 111, кв. 33
38	Иван	Смирнов	Николаевич	1955-07-31	+79553159741	person038@farm-association.test	г. Ростов-на-Дону, ул. Лесная, д. 144, кв. 227
39	Полина	Васильева	Владимировна	1965-11-12	+79409853032	person039@farm-association.test	г. Ставрополь, ул. Центральная, д. 46, кв. 189
40	Фёдор	Попов	Андреевич	1974-04-06	+79442639923	person040@farm-association.test	г. Ставрополь, ул. Центральная, д. 121, кв. 57
41	Алексей	Павлов	Евгеньевич	1983-05-15	+79499403938	person041@farm-association.test	г. Краснодар, ул. Молодёжная, д. 103, кв. 85
42	Татьяна	Павлова	Петровна	1977-01-15	+79547567561	person042@farm-association.test	г. Волгоград, ул. Лесная, д. 8, кв. 30
43	Фёдор	Смирнов	Сергеевич	2004-02-01	+79431392386	person043@farm-association.test	г. Ставрополь, ул. Лесная, д. 81, кв. 112
44	Владимир	Фёдоров	Петрович	1986-07-23	+79832944215	person044@farm-association.test	г. Ставрополь, ул. Центральная, д. 134, кв. 237
45	Алина	Фёдорова	Алексеевна	1984-09-03	+79651719552	person045@farm-association.test	г. Курск, ул. Лесная, д. 170, кв. 218
46	Пётр	Соловьёв	Дмитриевич	1997-06-30	+79497826251	person046@farm-association.test	г. Ставрополь, ул. Степная, д. 142, кв. 33
47	Алексей	Васильев	Андреевич	1967-08-12	+79886824861	person047@farm-association.test	г. Волгоград, ул. Центральная, д. 78, кв. 74
48	Ольга	Васильева	Владимировна	1980-11-28	+79695526696	person048@farm-association.test	г. Воронеж, ул. Луговая, д. 122, кв. 204
49	Фёдор	Зайцев	Сергеевич	1959-08-10	+79466279491	person049@farm-association.test	г. Курск, ул. Лесная, д. 24, кв. 210
50	Григорий	Кузнецов	Дмитриевич	1972-02-26	+79352501315	person050@farm-association.test	г. Воронеж, ул. Заречная, д. 157, кв. 218
51	Алина	Петрова	Евгеньевна	1989-03-07	+79906893499	person051@farm-association.test	г. Ставрополь, ул. Заречная, д. 103, кв. 63
52	Сергей	Волков	Иванович	1961-07-24	+79643243299	person052@farm-association.test	г. Волгоград, ул. Заречная, д. 13, кв. 143
53	Алексей	Лебедев	Петрович	1992-12-13	+79279206995	person053@farm-association.test	г. Волгоград, ул. Луговая, д. 153, кв. 82
54	Алина	Лебедева	Евгеньевна	1997-04-14	+79649508067	person054@farm-association.test	г. Белгород, ул. Заречная, д. 116, кв. 67
55	Григорий	Кузнецов	Дмитриевич	1998-10-07	+79727414045	person055@farm-association.test	г. Саратов, ул. Полевая, д. 74, кв. 61
56	Дмитрий	Попов	Николаевич	2000-06-16	+79202412939	person056@farm-association.test	г. Ставрополь, ул. Садовая, д. 55, кв. 17
57	Ирина	Васильева	Николаевна	2000-09-05	+79695251736	person057@farm-association.test	г. Ставрополь, ул. Школьная, д. 150, кв. 243
58	Роман	Иванов	Владимирович	1986-02-15	+79711065548	person058@farm-association.test	г. Ставрополь, ул. Школьная, д. 138, кв. 192
59	Роман	Фёдоров	Владимирович	1971-10-14	+79723244465	person059@farm-association.test	г. Саратов, ул. Центральная, д. 100, кв. 87
60	Вера	Волкова	Андреевна	1966-10-22	+79692308978	person060@farm-association.test	г. Краснодар, ул. Школьная, д. 152, кв. 145
61	Антон	Иванов	Петрович	1990-06-14	+79279876933	person061@farm-association.test	г. Краснодар, ул. Степная, д. 98, кв. 84
62	Алексей	Новиков	Николаевич	1982-04-12	+79583846342	person062@farm-association.test	г. Ростов-на-Дону, ул. Заречная, д. 5, кв. 192
63	Юлия	Иванова	Николаевна	1972-02-12	+79931709315	person063@farm-association.test	г. Краснодар, ул. Молодёжная, д. 52, кв. 215
64	Иван	Морозов	Сергеевич	1973-05-26	+79265849524	person064@farm-association.test	г. Курск, ул. Молодёжная, д. 120, кв. 180
65	Дмитрий	Зайцев	Николаевич	1967-01-20	+79877212430	person065@farm-association.test	г. Тамбов, ул. Полевая, д. 149, кв. 7
66	Дарья	Смирнова	Владимировна	1985-09-02	+79608323519	person066@farm-association.test	г. Курск, ул. Молодёжная, д. 27, кв. 179
67	Григорий	Смирнов	Владимирович	1962-11-10	+79829011554	person067@farm-association.test	г. Волгоград, ул. Школьная, д. 170, кв. 95
68	Пётр	Фёдоров	Николаевич	1953-02-18	+79639427223	person068@farm-association.test	г. Ставрополь, ул. Лесная, д. 163, кв. 229
69	Полина	Новикова	Сергеевна	1991-01-26	+79328517693	person069@farm-association.test	г. Тамбов, ул. Мира, д. 138, кв. 199
70	Евгений	Новиков	Андреевич	1976-01-29	+79519724121	person070@farm-association.test	г. Тамбов, ул. Заречная, д. 63, кв. 193
71	Евгений	Морозов	Владимирович	1985-12-31	+79531297351	person071@farm-association.test	г. Белгород, ул. Заречная, д. 55, кв. 91
72	Алина	Смирнова	Николаевна	1977-02-01	+79868184581	person072@farm-association.test	г. Краснодар, ул. Луговая, д. 49, кв. 22
73	Алексей	Соловьёв	Андреевич	1995-10-31	+79818764098	person073@farm-association.test	г. Саратов, ул. Заречная, д. 115, кв. 203
74	Иван	Петров	Дмитриевич	1971-11-18	+79618084149	person074@farm-association.test	г. Курск, ул. Лесная, д. 122, кв. 142
75	Юлия	Попова	Андреевна	2001-05-18	+79524609968	person075@farm-association.test	г. Тамбов, ул. Степная, д. 65, кв. 60
76	Пётр	Соловьёв	Алексеевич	1980-04-22	+79258607898	person076@farm-association.test	г. Белгород, ул. Молодёжная, д. 56, кв. 190
77	Евгений	Смирнов	Владимирович	1999-01-25	+79863892234	person077@farm-association.test	г. Тамбов, ул. Молодёжная, д. 93, кв. 46
78	Татьяна	Иванова	Михайловна	1963-05-10	+79451461680	person078@farm-association.test	г. Тамбов, ул. Садовая, д. 164, кв. 223
79	Григорий	Новиков	Петрович	1953-02-05	+79833917071	person079@farm-association.test	г. Саратов, ул. Лесная, д. 48, кв. 248
80	Иван	Смирнов	Евгеньевич	1962-03-27	+79185107219	person080@farm-association.test	г. Курск, ул. Центральная, д. 39, кв. 39
81	Алина	Морозова	Дмитриевна	1959-08-23	+79412218163	person081@farm-association.test	г. Курск, ул. Мира, д. 159, кв. 58
82	Григорий	Фёдоров	Андреевич	1992-06-01	+79664048564	person082@farm-association.test	г. Тамбов, ул. Мира, д. 159, кв. 16
83	Владимир	Соловьёв	Петрович	1970-08-22	+79903164394	person083@farm-association.test	г. Ростов-на-Дону, ул. Садовая, д. 62, кв. 45
84	Юлия	Петрова	Сергеевна	1952-03-28	+79625619886	person084@farm-association.test	г. Саратов, ул. Степная, д. 9, кв. 60
85	Дмитрий	Соловьёв	Дмитриевич	1992-09-24	+79198033943	person085@farm-association.test	г. Курск, ул. Молодёжная, д. 109, кв. 30
86	Михаил	Кузнецов	Сергеевич	1975-10-31	+79281731731	person086@farm-association.test	г. Тамбов, ул. Мира, д. 146, кв. 236
87	Татьяна	Новикова	Петровна	1994-01-18	+79984119961	person087@farm-association.test	г. Тамбов, ул. Луговая, д. 139, кв. 127
88	Евгений	Петров	Владимирович	1955-07-30	+79658525187	person088@farm-association.test	г. Тамбов, ул. Центральная, д. 24, кв. 59
89	Антон	Павлов	Владимирович	2004-09-02	+79128839644	person089@farm-association.test	г. Курск, ул. Центральная, д. 45, кв. 121
90	Юлия	Волкова	Евгеньевна	1976-12-14	+79336996591	person090@farm-association.test	г. Саратов, ул. Полевая, д. 121, кв. 90
91	Андрей	Попов	Николаевич	1961-05-20	+79304376298	person091@farm-association.test	г. Саратов, ул. Степная, д. 170, кв. 242
92	Андрей	Павлов	Михайлович	1955-04-17	+79681905042	person092@farm-association.test	г. Липецк, ул. Полевая, д. 104, кв. 222
93	Юлия	Павлова	Ивановна	2000-09-04	+79695231634	person093@farm-association.test	г. Волгоград, ул. Лесная, д. 160, кв. 194
94	Евгений	Волков	Евгеньевич	1956-08-18	+79363738026	person094@farm-association.test	г. Тамбов, ул. Заречная, д. 179, кв. 125
95	Пётр	Иванов	Владимирович	1973-06-20	+79304188011	person095@farm-association.test	г. Волгоград, ул. Школьная, д. 24, кв. 58
96	Полина	Лебедева	Петровна	1993-05-25	+79257632973	person096@farm-association.test	г. Тамбов, ул. Луговая, д. 70, кв. 107
97	Степан	Новиков	Евгеньевич	1973-11-12	+79686642859	person097@farm-association.test	г. Воронеж, ул. Мира, д. 131, кв. 192
98	Фёдор	Сидоров	Петрович	1976-10-13	+79634487444	person098@farm-association.test	г. Краснодар, ул. Степная, д. 77, кв. 215
99	Екатерина	Морозова	Евгеньевна	1965-05-01	+79676517154	person099@farm-association.test	г. Липецк, ул. Луговая, д. 140, кв. 97
100	Евгений	Лебедев	Николаевич	1968-12-03	+79993448359	person100@farm-association.test	г. Воронеж, ул. Школьная, д. 12, кв. 82
101	Роман	Новиков	Андреевич	1986-08-21	+79949119329	person101@farm-association.test	г. Саратов, ул. Центральная, д. 33, кв. 129
102	Екатерина	Попова	Петровна	1991-07-02	+79226386811	person102@farm-association.test	г. Белгород, ул. Школьная, д. 168, кв. 249
103	Сергей	Петров	Евгеньевич	1975-10-11	+79537389860	person103@farm-association.test	г. Ростов-на-Дону, ул. Лесная, д. 173, кв. 220
104	Михаил	Васильев	Николаевич	1995-10-11	+79791368918	person104@farm-association.test	г. Воронеж, ул. Степная, д. 59, кв. 192
105	Мария	Васильева	Петровна	1961-01-06	+79662709848	person105@farm-association.test	г. Краснодар, ул. Центральная, д. 84, кв. 204
106	Иван	Смирнов	Николаевич	1985-08-17	+79652494177	person106@farm-association.test	г. Ставрополь, ул. Мира, д. 175, кв. 203
107	Сергей	Сидоров	Сергеевич	1959-01-31	+79889915889	person107@farm-association.test	г. Воронеж, ул. Заречная, д. 150, кв. 37
108	Ольга	Новикова	Дмитриевна	1993-03-25	+79427821169	person108@farm-association.test	г. Тамбов, ул. Луговая, д. 41, кв. 19
109	Евгений	Попов	Владимирович	1978-11-02	+79915349842	person109@farm-association.test	г. Саратов, ул. Степная, д. 51, кв. 99
110	Степан	Новиков	Петрович	1973-04-12	+79586855583	person110@farm-association.test	г. Тамбов, ул. Степная, д. 6, кв. 213
111	Вера	Васильева	Дмитриевна	1952-09-22	+79829859716	person111@farm-association.test	г. Курск, ул. Заречная, д. 74, кв. 199
112	Григорий	Кузнецов	Владимирович	1983-08-10	+79387513489	person112@farm-association.test	г. Тамбов, ул. Садовая, д. 161, кв. 25
113	Фёдор	Волков	Иванович	1979-09-19	+79661348456	person113@farm-association.test	г. Белгород, ул. Полевая, д. 76, кв. 84
114	Людмила	Васильева	Сергеевна	1970-01-05	+79269057956	person114@farm-association.test	г. Волгоград, ул. Луговая, д. 70, кв. 213
115	Сергей	Смирнов	Евгеньевич	1978-06-24	+79539232469	person115@farm-association.test	г. Ростов-на-Дону, ул. Садовая, д. 58, кв. 221
116	Антон	Соловьёв	Андреевич	2001-12-31	+79561926011	person116@farm-association.test	г. Тамбов, ул. Луговая, д. 32, кв. 117
117	Наталья	Волкова	Дмитриевна	2004-06-10	+79589429157	person117@farm-association.test	г. Ростов-на-Дону, ул. Молодёжная, д. 121, кв. 7
118	Владимир	Лебедев	Михайлович	1981-06-05	+79883269218	person118@farm-association.test	г. Саратов, ул. Степная, д. 167, кв. 105
119	Пётр	Сидоров	Иванович	1955-05-04	+79486042422	person119@farm-association.test	г. Воронеж, ул. Луговая, д. 35, кв. 100
120	Светлана	Попова	Михайловна	1989-08-07	+79858602963	person120@farm-association.test	г. Ростов-на-Дону, ул. Заречная, д. 158, кв. 105
121	Дмитрий	Иванов	Николаевич	1971-06-29	+79665554056	person121@farm-association.test	г. Ростов-на-Дону, ул. Лесная, д. 140, кв. 231
122	Антон	Попов	Иванович	1987-09-20	+79452942568	person122@farm-association.test	г. Ростов-на-Дону, ул. Молодёжная, д. 165, кв. 164
123	Екатерина	Иванова	Ивановна	1981-12-02	+79412288236	person123@farm-association.test	г. Ростов-на-Дону, ул. Луговая, д. 54, кв. 151
124	Алексей	Павлов	Алексеевич	1981-06-23	+79289078610	person124@farm-association.test	г. Тамбов, ул. Садовая, д. 34, кв. 139
125	Дмитрий	Зайцев	Сергеевич	1961-11-11	+79949871326	person125@farm-association.test	г. Краснодар, ул. Лесная, д. 61, кв. 151
126	Наталья	Иванова	Сергеевна	1975-10-22	+79162296377	person126@farm-association.test	г. Ростов-на-Дону, ул. Полевая, д. 122, кв. 115
127	Григорий	Попов	Михайлович	1961-10-13	+79676153888	person127@farm-association.test	г. Краснодар, ул. Луговая, д. 78, кв. 118
128	Антон	Иванов	Иванович	1994-12-21	+79615369723	person128@farm-association.test	г. Саратов, ул. Заречная, д. 19, кв. 231
129	Мария	Попова	Владимировна	1965-04-22	+79182294589	person129@farm-association.test	г. Курск, ул. Луговая, д. 84, кв. 98
130	Владимир	Фёдоров	Дмитриевич	1992-09-14	+79747196522	person130@farm-association.test	г. Ростов-на-Дону, ул. Луговая, д. 56, кв. 111
131	Евгений	Лебедев	Алексеевич	1989-02-14	+79539476861	person131@farm-association.test	г. Ставрополь, ул. Полевая, д. 81, кв. 110
132	Наталья	Волкова	Дмитриевна	1985-07-31	+79298037018	person132@farm-association.test	г. Ростов-на-Дону, ул. Полевая, д. 24, кв. 111
133	Пётр	Соловьёв	Николаевич	1963-09-03	+79811618581	person133@farm-association.test	г. Волгоград, ул. Лесная, д. 172, кв. 32
134	Андрей	Попов	Андреевич	1956-08-12	+79467144955	person134@farm-association.test	г. Ростов-на-Дону, ул. Мира, д. 130, кв. 55
135	Елена	Волкова	Евгеньевна	1972-02-13	+79234588157	person135@farm-association.test	г. Ростов-на-Дону, ул. Степная, д. 147, кв. 58
136	Григорий	Васильев	Михайлович	2001-12-12	+79137239498	person136@farm-association.test	г. Тамбов, ул. Центральная, д. 47, кв. 70
137	Роман	Зайцев	Дмитриевич	1982-06-27	+79541063328	person137@farm-association.test	г. Курск, ул. Школьная, д. 18, кв. 37
138	Людмила	Волкова	Ивановна	1960-03-25	+79773205863	person138@farm-association.test	г. Саратов, ул. Лесная, д. 41, кв. 95
139	Дмитрий	Соловьёв	Николаевич	2002-12-02	+79861861629	person139@farm-association.test	г. Белгород, ул. Мира, д. 13, кв. 173
140	Пётр	Смирнов	Евгеньевич	1990-01-15	+79727216663	person140@farm-association.test	г. Тамбов, ул. Молодёжная, д. 132, кв. 30
141	Наталья	Васильева	Петровна	1977-05-30	+79967948572	person141@farm-association.test	г. Волгоград, ул. Степная, д. 12, кв. 57
142	Андрей	Морозов	Иванович	1952-09-08	+79364083727	person142@farm-association.test	г. Тамбов, ул. Степная, д. 84, кв. 31
143	Иван	Новиков	Андреевич	1967-10-06	+79264897839	person143@farm-association.test	г. Волгоград, ул. Луговая, д. 172, кв. 207
144	Наталья	Петрова	Андреевна	1955-10-18	+79651196819	person144@farm-association.test	г. Липецк, ул. Мира, д. 110, кв. 147
145	Андрей	Соловьёв	Андреевич	1977-12-21	+79245141251	person145@farm-association.test	г. Белгород, ул. Мира, д. 118, кв. 213
146	Роман	Лебедев	Николаевич	1959-11-27	+79659642341	person146@farm-association.test	г. Ставрополь, ул. Мира, д. 103, кв. 135
147	Мария	Васильева	Дмитриевна	1982-06-17	+79384413119	person147@farm-association.test	г. Волгоград, ул. Полевая, д. 136, кв. 131
148	Алексей	Лебедев	Николаевич	1983-07-01	+79929342840	person148@farm-association.test	г. Ростов-на-Дону, ул. Садовая, д. 66, кв. 51
149	Сергей	Морозов	Сергеевич	1958-10-05	+79328919073	person149@farm-association.test	г. Саратов, ул. Мира, д. 149, кв. 115
150	Вера	Лебедева	Владимировна	1980-12-29	+79904232966	person150@farm-association.test	г. Ростов-на-Дону, ул. Заречная, д. 114, кв. 162
151	Дмитрий	Зайцев	Дмитриевич	1957-01-14	+79556191949	person151@farm-association.test	г. Саратов, ул. Заречная, д. 10, кв. 15
152	Николай	Павлов	Дмитриевич	1958-11-19	+79929842188	person152@farm-association.test	г. Курск, ул. Луговая, д. 99, кв. 119
153	Екатерина	Фёдорова	Ивановна	1992-05-06	+79837673451	person153@farm-association.test	г. Курск, ул. Заречная, д. 129, кв. 39
154	Иван	Новиков	Петрович	1982-10-23	+79206169232	person154@farm-association.test	г. Краснодар, ул. Молодёжная, д. 113, кв. 113
155	Михаил	Фёдоров	Владимирович	1966-03-29	+79564814659	person155@farm-association.test	г. Ставрополь, ул. Лесная, д. 174, кв. 154
156	Анна	Зайцева	Николаевна	1957-11-29	+79521968196	person156@farm-association.test	г. Ставрополь, ул. Степная, д. 65, кв. 186
157	Степан	Волков	Владимирович	1965-06-28	+79521838494	person157@farm-association.test	г. Белгород, ул. Лесная, д. 80, кв. 248
158	Антон	Соловьёв	Андреевич	1963-07-27	+79868252049	person158@farm-association.test	г. Волгоград, ул. Школьная, д. 165, кв. 203
159	Наталья	Павлова	Сергеевна	1999-03-23	+79217619564	person159@farm-association.test	г. Волгоград, ул. Лесная, д. 5, кв. 93
160	Дмитрий	Сидоров	Алексеевич	1982-08-28	+79722963827	person160@farm-association.test	г. Белгород, ул. Полевая, д. 76, кв. 217
161	Григорий	Петров	Михайлович	2000-06-04	+79771389453	person161@farm-association.test	г. Курск, ул. Садовая, д. 153, кв. 97
162	Елена	Сидорова	Сергеевна	1966-11-07	+79661446256	person162@farm-association.test	г. Воронеж, ул. Заречная, д. 157, кв. 73
163	Григорий	Соловьёв	Евгеньевич	1972-12-28	+79783444970	person163@farm-association.test	г. Воронеж, ул. Лесная, д. 174, кв. 243
164	Владимир	Новиков	Евгеньевич	1977-04-12	+79586147763	person164@farm-association.test	г. Белгород, ул. Молодёжная, д. 155, кв. 36
165	Полина	Смирнова	Ивановна	1995-02-11	+79576672376	person165@farm-association.test	г. Ростов-на-Дону, ул. Степная, д. 22, кв. 196
166	Сергей	Смирнов	Евгеньевич	1998-01-20	+79289506521	person166@farm-association.test	г. Воронеж, ул. Заречная, д. 90, кв. 239
167	Иван	Васильев	Иванович	1987-07-27	+79744824059	person167@farm-association.test	г. Ростов-на-Дону, ул. Лесная, д. 58, кв. 8
168	Наталья	Лебедева	Петровна	1982-01-29	+79282401446	person168@farm-association.test	г. Саратов, ул. Садовая, д. 121, кв. 115
169	Владимир	Иванов	Петрович	1953-09-14	+79423202980	person169@farm-association.test	г. Курск, ул. Луговая, д. 109, кв. 29
170	Григорий	Смирнов	Алексеевич	1979-01-07	+79251484063	person170@farm-association.test	г. Курск, ул. Заречная, д. 17, кв. 29
171	Полина	Лебедева	Евгеньевна	2000-02-03	+79127477583	person171@farm-association.test	г. Воронеж, ул. Садовая, д. 75, кв. 110
172	Иван	Морозов	Николаевич	1973-07-31	+79835263395	person172@farm-association.test	г. Ростов-на-Дону, ул. Луговая, д. 93, кв. 18
173	Михаил	Фёдоров	Михайлович	1997-07-16	+79801205970	person173@farm-association.test	г. Краснодар, ул. Школьная, д. 96, кв. 65
174	Людмила	Иванова	Николаевна	1958-01-22	+79543469490	person174@farm-association.test	г. Ростов-на-Дону, ул. Мира, д. 86, кв. 35
175	Иван	Попов	Михайлович	1982-05-16	+79922799769	person175@farm-association.test	г. Саратов, ул. Садовая, д. 35, кв. 17
176	Роман	Зайцев	Евгеньевич	1955-04-26	+79473061535	person176@farm-association.test	г. Краснодар, ул. Лесная, д. 80, кв. 132
177	Ирина	Павлова	Михайловна	1994-06-21	+79421379234	person177@farm-association.test	г. Тамбов, ул. Лесная, д. 13, кв. 222
178	Антон	Попов	Дмитриевич	1963-03-02	+79575476166	person178@farm-association.test	г. Ставрополь, ул. Лесная, д. 48, кв. 128
179	Роман	Новиков	Николаевич	1998-08-02	+79449202064	person179@farm-association.test	г. Ростов-на-Дону, ул. Школьная, д. 155, кв. 211
180	Елена	Фёдорова	Дмитриевна	1980-10-25	+79231815194	person180@farm-association.test	г. Тамбов, ул. Степная, д. 115, кв. 155
181	Роман	Васильев	Сергеевич	1991-10-28	+79545571555	person181@farm-association.test	г. Курск, ул. Школьная, д. 71, кв. 164
182	Григорий	Иванов	Петрович	1988-06-10	+79566259630	person182@farm-association.test	г. Краснодар, ул. Садовая, д. 156, кв. 174
183	Алина	Новикова	Ивановна	1963-04-29	+79183419256	person183@farm-association.test	г. Липецк, ул. Школьная, д. 146, кв. 9
184	Владимир	Сидоров	Евгеньевич	1985-04-12	+79575541983	person184@farm-association.test	г. Белгород, ул. Луговая, д. 94, кв. 102
185	Николай	Волков	Дмитриевич	1974-05-24	+79241263373	person185@farm-association.test	г. Волгоград, ул. Школьная, д. 144, кв. 31
186	Татьяна	Зайцева	Дмитриевна	1992-01-14	+79377264698	person186@farm-association.test	г. Саратов, ул. Молодёжная, д. 32, кв. 35
187	Степан	Петров	Евгеньевич	1967-06-28	+79661899750	person187@farm-association.test	г. Липецк, ул. Полевая, д. 141, кв. 139
188	Дмитрий	Лебедев	Дмитриевич	1966-02-17	+79997533256	person188@farm-association.test	г. Волгоград, ул. Молодёжная, д. 32, кв. 52
189	Алина	Сидорова	Алексеевна	1996-04-27	+79134698083	person189@farm-association.test	г. Липецк, ул. Заречная, д. 142, кв. 34
190	Владимир	Лебедев	Петрович	1957-11-24	+79495077177	person190@farm-association.test	г. Ставрополь, ул. Школьная, д. 148, кв. 19
191	Сергей	Попов	Петрович	1992-05-14	+79697967654	person191@farm-association.test	г. Белгород, ул. Луговая, д. 164, кв. 151
192	Елена	Зайцева	Сергеевна	1990-10-24	+79749891725	person192@farm-association.test	г. Волгоград, ул. Садовая, д. 78, кв. 43
193	Сергей	Попов	Алексеевич	1983-01-16	+79763902042	person193@farm-association.test	г. Воронеж, ул. Луговая, д. 71, кв. 33
194	Антон	Смирнов	Владимирович	1999-11-11	+79216149231	person194@farm-association.test	г. Курск, ул. Мира, д. 40, кв. 44
195	Вера	Морозова	Владимировна	1982-04-16	+79821421320	person195@farm-association.test	г. Краснодар, ул. Мира, д. 68, кв. 167
196	Алексей	Зайцев	Владимирович	1989-05-16	+79897541373	person196@farm-association.test	г. Волгоград, ул. Степная, д. 165, кв. 245
197	Дмитрий	Новиков	Алексеевич	1988-06-06	+79485641998	person197@farm-association.test	г. Краснодар, ул. Садовая, д. 113, кв. 107
198	Светлана	Новикова	Алексеевна	1982-07-09	+79872475050	person198@farm-association.test	г. Липецк, ул. Школьная, д. 34, кв. 195
199	Николай	Фёдоров	Михайлович	1961-07-11	+79503476925	person199@farm-association.test	г. Тамбов, ул. Заречная, д. 64, кв. 37
200	Пётр	Иванов	Дмитриевич	1986-06-18	+79885284130	person200@farm-association.test	г. Липецк, ул. Мира, д. 81, кв. 49
201	Алина	Сидорова	Евгеньевна	1998-03-06	+79696104973	person201@farm-association.test	г. Краснодар, ул. Полевая, д. 101, кв. 130
202	Евгений	Кузнецов	Алексеевич	2004-05-02	+79551491646	person202@farm-association.test	г. Саратов, ул. Мира, д. 168, кв. 173
203	Евгений	Смирнов	Михайлович	1952-09-24	+79235412743	person203@farm-association.test	г. Липецк, ул. Школьная, д. 94, кв. 12
204	Ирина	Иванова	Владимировна	2002-05-27	+79344718046	person204@farm-association.test	г. Ростов-на-Дону, ул. Школьная, д. 130, кв. 116
205	Григорий	Фёдоров	Дмитриевич	1962-08-29	+79261996057	person205@farm-association.test	г. Липецк, ул. Луговая, д. 94, кв. 194
206	Сергей	Кузнецов	Владимирович	1997-09-03	+79616121515	person206@farm-association.test	г. Краснодар, ул. Садовая, д. 86, кв. 206
207	Светлана	Фёдорова	Евгеньевна	1965-05-12	+79876272751	person207@farm-association.test	г. Курск, ул. Лесная, д. 42, кв. 101
208	Владимир	Соловьёв	Дмитриевич	1982-03-06	+79749487578	person208@farm-association.test	г. Саратов, ул. Мира, д. 77, кв. 122
209	Степан	Иванов	Николаевич	1981-09-19	+79962126384	person209@farm-association.test	г. Тамбов, ул. Центральная, д. 153, кв. 122
210	Татьяна	Волкова	Владимировна	2003-10-27	+79398381684	person210@farm-association.test	г. Саратов, ул. Садовая, д. 135, кв. 162
211	Роман	Морозов	Андреевич	1965-04-04	+79973481483	person211@farm-association.test	г. Ростов-на-Дону, ул. Молодёжная, д. 5, кв. 113
212	Николай	Васильев	Сергеевич	1989-01-17	+79983086274	person212@farm-association.test	г. Курск, ул. Заречная, д. 16, кв. 181
213	Елена	Фёдорова	Алексеевна	2002-04-27	+79517787177	person213@farm-association.test	г. Ставрополь, ул. Лесная, д. 45, кв. 118
214	Фёдор	Фёдоров	Николаевич	2001-01-03	+79557929792	person214@farm-association.test	г. Тамбов, ул. Мира, д. 124, кв. 50
215	Алексей	Смирнов	Михайлович	1978-10-12	+79384044636	person215@farm-association.test	г. Саратов, ул. Лесная, д. 123, кв. 90
216	Юлия	Лебедева	Дмитриевна	1977-10-26	+79256879679	person216@farm-association.test	г. Ставрополь, ул. Школьная, д. 89, кв. 245
217	Григорий	Зайцев	Сергеевич	1978-01-21	+79153942054	person217@farm-association.test	г. Саратов, ул. Степная, д. 123, кв. 55
218	Алексей	Павлов	Михайлович	1976-04-28	+79818124427	person218@farm-association.test	г. Ростов-на-Дону, ул. Мира, д. 151, кв. 62
219	Ольга	Иванова	Михайловна	1972-03-30	+79913381622	person219@farm-association.test	г. Ставрополь, ул. Лесная, д. 121, кв. 26
220	Антон	Зайцев	Сергеевич	1952-06-18	+79802616293	person220@farm-association.test	г. Саратов, ул. Заречная, д. 167, кв. 52
221	Григорий	Смирнов	Николаевич	1977-08-09	+79921602193	person221@farm-association.test	г. Курск, ул. Молодёжная, д. 137, кв. 190
222	Людмила	Павлова	Ивановна	1967-09-15	+79639573214	person222@farm-association.test	г. Ставрополь, ул. Заречная, д. 48, кв. 240
223	Роман	Павлов	Дмитриевич	1955-05-12	+79114058287	person223@farm-association.test	г. Ростов-на-Дону, ул. Лесная, д. 73, кв. 117
224	Антон	Фёдоров	Михайлович	1996-04-23	+79279717469	person224@farm-association.test	г. Тамбов, ул. Молодёжная, д. 29, кв. 85
225	Елена	Соловьёва	Евгеньевна	1975-01-30	+79331145347	person225@farm-association.test	г. Курск, ул. Молодёжная, д. 45, кв. 157
226	Степан	Волков	Андреевич	1990-05-15	+79754352161	person226@farm-association.test	г. Ростов-на-Дону, ул. Садовая, д. 36, кв. 123
227	Николай	Лебедев	Алексеевич	1952-08-11	+79434924067	person227@farm-association.test	г. Тамбов, ул. Лесная, д. 78, кв. 150
228	Людмила	Морозова	Ивановна	1975-06-19	+79934689840	person228@farm-association.test	г. Краснодар, ул. Полевая, д. 120, кв. 79
229	Сергей	Васильев	Михайлович	1979-11-25	+79982209147	person229@farm-association.test	г. Липецк, ул. Мира, д. 57, кв. 57
230	Сергей	Новиков	Сергеевич	1992-10-25	+79874826399	person230@farm-association.test	г. Волгоград, ул. Заречная, д. 138, кв. 206
231	Вера	Павлова	Алексеевна	1974-03-14	+79978728620	person231@farm-association.test	г. Волгоград, ул. Заречная, д. 136, кв. 181
232	Николай	Петров	Владимирович	1962-01-24	+79179488074	person232@farm-association.test	г. Воронеж, ул. Мира, д. 138, кв. 39
233	Сергей	Попов	Михайлович	1991-08-22	+79247963684	person233@farm-association.test	г. Саратов, ул. Полевая, д. 131, кв. 115
234	Алина	Иванова	Евгеньевна	1963-11-02	+79755256882	person234@farm-association.test	г. Краснодар, ул. Луговая, д. 119, кв. 173
235	Григорий	Смирнов	Иванович	1987-07-10	+79429361037	person235@farm-association.test	г. Курск, ул. Полевая, д. 12, кв. 109
236	Николай	Соловьёв	Петрович	2000-07-19	+79171707014	person236@farm-association.test	г. Тамбов, ул. Школьная, д. 47, кв. 197
237	Елена	Зайцева	Андреевна	1985-08-05	+79585595858	person237@farm-association.test	г. Ростов-на-Дону, ул. Луговая, д. 35, кв. 168
238	Степан	Попов	Петрович	1968-01-05	+79785027726	person238@farm-association.test	г. Воронеж, ул. Центральная, д. 6, кв. 77
239	Евгений	Волков	Михайлович	1990-01-18	+79784883941	person239@farm-association.test	г. Саратов, ул. Лесная, д. 40, кв. 71
240	Ольга	Лебедева	Петровна	1954-11-21	+79945298812	person240@farm-association.test	г. Воронеж, ул. Молодёжная, д. 18, кв. 26
241	Владимир	Иванов	Евгеньевич	1956-05-14	+79418581561	person241@farm-association.test	г. Саратов, ул. Молодёжная, д. 139, кв. 56
242	Григорий	Павлов	Иванович	1964-07-27	+79743963983	person242@farm-association.test	г. Липецк, ул. Мира, д. 154, кв. 198
243	Вера	Павлова	Николаевна	1973-04-02	+79489972894	person243@farm-association.test	г. Волгоград, ул. Молодёжная, д. 106, кв. 77
244	Дмитрий	Иванов	Михайлович	1967-09-17	+79907956481	person244@farm-association.test	г. Саратов, ул. Центральная, д. 89, кв. 242
245	Антон	Волков	Андреевич	1999-01-19	+79508136362	person245@farm-association.test	г. Белгород, ул. Степная, д. 97, кв. 48
246	Алина	Фёдорова	Евгеньевна	1973-08-14	+79384072869	person246@farm-association.test	г. Краснодар, ул. Луговая, д. 106, кв. 248
247	Андрей	Фёдоров	Михайлович	1964-01-11	+79593484236	person247@farm-association.test	г. Липецк, ул. Полевая, д. 116, кв. 217
248	Николай	Петров	Михайлович	1969-02-01	+79163745896	person248@farm-association.test	г. Курск, ул. Мира, д. 11, кв. 224
249	Мария	Кузнецова	Владимировна	2002-02-19	+79375903652	person249@farm-association.test	г. Тамбов, ул. Центральная, д. 55, кв. 240
250	Алексей	Соловьёв	Петрович	1994-12-19	+79418128736	person250@farm-association.test	г. Ставрополь, ул. Молодёжная, д. 142, кв. 83
\.


--
-- TOC entry 3692 (class 0 OID 24652)
-- Dependencies: 235
-- Data for Name: person_document; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.person_document (id, person_id, document_type_id, document_number, issued_by, issued_date, expiration_date, is_primary) FROM stdin;
1	1	1	45 78 400614	ОВД г. Ростов-на-Дону	2005-12-08	\N	1
2	2	1	74 99 806774	ОВД г. Белгород	2000-03-11	\N	1
3	3	1	32 23 200992	ОВД г. Воронеж	2011-02-12	\N	1
4	4	1	29 40 301255	ОВД г. Тамбов	2005-11-17	\N	1
5	5	1	77 42 954814	ОВД г. Липецк	2021-01-17	\N	1
6	5	3	DOC-005-3	Орган выдачи 6	2021-02-02	\N	0
7	6	1	60 20 491172	ОВД г. Ставрополь	2008-11-30	\N	1
8	7	1	68 82 355021	ОВД г. Саратов	2004-07-21	\N	1
9	8	1	38 48 817269	ОВД г. Волгоград	2007-10-26	\N	1
10	9	1	20 93 999908	ОВД г. Курск	2009-04-14	\N	1
11	10	1	14 21 524965	ОВД г. Краснодар	2007-04-15	\N	1
12	10	3	DOC-010-3	Орган выдачи 11	2020-06-16	\N	0
13	11	1	70 17 767678	ОВД г. Ростов-на-Дону	2020-07-16	\N	1
14	12	1	99 31 186702	ОВД г. Белгород	2000-02-08	\N	1
15	13	1	65 92 922478	ОВД г. Воронеж	2013-08-02	\N	1
16	14	1	82 22 653815	ОВД г. Тамбов	2003-09-18	\N	1
17	15	1	39 37 825916	ОВД г. Липецк	2012-01-05	\N	1
18	15	4	DOC-015-4	Орган выдачи 16	2020-11-11	2030-11-09	0
19	16	1	15 19 817675	ОВД г. Ставрополь	2012-11-11	\N	1
20	17	1	79 82 790077	ОВД г. Саратов	2012-09-24	\N	1
21	18	1	32 50 116418	ОВД г. Волгоград	2004-04-26	\N	1
22	19	1	85 28 888795	ОВД г. Курск	2008-10-25	\N	1
23	20	1	60 19 413814	ОВД г. Краснодар	2009-03-15	\N	1
24	20	2	DOC-020-2	Орган выдачи 1	2024-08-24	2034-08-22	0
25	21	1	82 59 809922	ОВД г. Ростов-на-Дону	2002-09-11	\N	1
26	22	1	52 59 892313	ОВД г. Белгород	2008-07-27	\N	1
27	23	1	27 98 859340	ОВД г. Воронеж	2008-04-17	\N	1
28	24	1	74 54 156304	ОВД г. Тамбов	2000-11-17	\N	1
29	25	1	65 39 979414	ОВД г. Липецк	2001-02-04	\N	1
30	25	2	DOC-025-2	Орган выдачи 6	2019-08-22	2029-08-19	0
31	26	1	88 86 515973	ОВД г. Ставрополь	2006-10-14	\N	1
32	27	1	51 13 764926	ОВД г. Саратов	2008-09-03	\N	1
33	28	1	67 72 339220	ОВД г. Волгоград	2016-09-13	\N	1
34	29	1	80 58 552825	ОВД г. Курск	2006-03-15	\N	1
35	30	1	97 84 792764	ОВД г. Краснодар	2018-10-12	\N	1
36	30	3	DOC-030-3	Орган выдачи 11	2021-03-05	\N	0
37	31	1	89 47 936821	ОВД г. Ростов-на-Дону	2015-09-01	\N	1
38	32	1	19 20 380989	ОВД г. Белгород	2014-02-19	\N	1
39	33	1	58 91 261771	ОВД г. Воронеж	2001-09-23	\N	1
40	34	1	59 50 478221	ОВД г. Тамбов	2008-04-17	\N	1
41	35	1	21 10 423241	ОВД г. Липецк	2001-03-12	\N	1
42	35	3	DOC-035-3	Орган выдачи 16	2020-01-27	\N	0
43	36	1	44 23 238943	ОВД г. Ставрополь	2008-07-15	\N	1
44	37	1	33 65 570646	ОВД г. Саратов	2015-11-30	\N	1
45	38	1	81 75 527865	ОВД г. Волгоград	2006-03-31	\N	1
46	39	1	13 21 471106	ОВД г. Курск	2001-02-27	\N	1
47	40	1	21 86 727094	ОВД г. Краснодар	2006-03-20	\N	1
48	40	3	DOC-040-3	Орган выдачи 1	2020-08-21	\N	0
49	41	1	47 62 506637	ОВД г. Ростов-на-Дону	2000-02-20	\N	1
50	42	1	20 81 354533	ОВД г. Белгород	2008-09-26	\N	1
51	43	1	76 31 818605	ОВД г. Воронеж	2024-06-27	\N	1
52	44	1	31 27 381981	ОВД г. Тамбов	2004-10-25	\N	1
53	45	1	44 73 253023	ОВД г. Липецк	2003-05-18	\N	1
54	45	2	DOC-045-2	Орган выдачи 6	2015-10-01	2025-09-28	0
55	46	1	45 63 414313	ОВД г. Ставрополь	2016-05-11	\N	1
56	47	1	19 56 363926	ОВД г. Саратов	2005-06-06	\N	1
57	48	1	90 73 722651	ОВД г. Волгоград	2002-10-07	\N	1
58	49	1	35 68 212168	ОВД г. Курск	2006-11-30	\N	1
59	50	1	48 10 514913	ОВД г. Краснодар	2001-07-08	\N	1
60	50	3	DOC-050-3	Орган выдачи 11	2020-07-09	\N	0
61	51	1	52 66 450996	ОВД г. Ростов-на-Дону	2012-03-09	\N	1
62	52	1	93 86 242736	ОВД г. Белгород	2004-10-29	\N	1
63	53	1	51 87 829066	ОВД г. Воронеж	2010-04-23	\N	1
64	54	1	71 50 286266	ОВД г. Тамбов	2013-07-11	\N	1
65	55	1	50 47 870571	ОВД г. Липецк	2017-03-22	\N	1
66	55	4	DOC-055-4	Орган выдачи 16	2023-11-07	2033-11-04	0
67	56	1	83 41 441716	ОВД г. Ставрополь	2019-12-15	\N	1
68	57	1	45 60 482746	ОВД г. Саратов	2018-11-20	\N	1
69	58	1	82 35 720995	ОВД г. Волгоград	2001-05-23	\N	1
70	59	1	33 97 904905	ОВД г. Курск	2006-02-12	\N	1
71	60	1	13 69 842776	ОВД г. Краснодар	2006-03-03	\N	1
72	60	2	DOC-060-2	Орган выдачи 1	2021-10-31	2031-10-29	0
73	61	1	47 98 172518	ОВД г. Ростов-на-Дону	2013-05-21	\N	1
74	62	1	62 96 623557	ОВД г. Белгород	2009-03-08	\N	1
75	63	1	91 48 353777	ОВД г. Воронеж	2001-07-24	\N	1
76	64	1	94 29 849979	ОВД г. Тамбов	2002-10-31	\N	1
77	65	1	58 19 570880	ОВД г. Липецк	2004-09-29	\N	1
78	65	4	DOC-065-4	Орган выдачи 6	2022-09-30	2032-09-27	0
79	66	1	61 78 631221	ОВД г. Ставрополь	2006-07-12	\N	1
80	67	1	98 63 670053	ОВД г. Саратов	2009-10-02	\N	1
81	68	1	56 99 951755	ОВД г. Волгоград	2000-05-30	\N	1
82	69	1	86 91 189110	ОВД г. Курск	2011-01-30	\N	1
83	70	1	41 94 796982	ОВД г. Краснодар	2001-03-23	\N	1
84	70	3	DOC-070-3	Орган выдачи 11	2015-09-24	\N	0
85	71	1	88 15 690579	ОВД г. Ростов-на-Дону	2007-04-04	\N	1
86	72	1	96 92 519029	ОВД г. Белгород	2007-03-27	\N	1
87	73	1	52 65 211090	ОВД г. Воронеж	2018-04-05	\N	1
88	74	1	22 43 332785	ОВД г. Тамбов	2000-02-13	\N	1
89	75	1	76 81 708717	ОВД г. Липецк	2021-02-03	\N	1
90	75	4	DOC-075-4	Орган выдачи 16	2021-11-04	2031-11-02	0
91	76	1	57 60 585556	ОВД г. Ставрополь	2004-12-30	\N	1
92	77	1	96 85 823975	ОВД г. Саратов	2021-09-19	\N	1
93	78	1	29 54 125755	ОВД г. Волгоград	2005-08-11	\N	1
94	79	1	23 47 536181	ОВД г. Курск	2005-05-31	\N	1
95	80	1	24 28 465249	ОВД г. Краснодар	2000-12-17	\N	1
96	80	3	DOC-080-3	Орган выдачи 1	2019-09-11	\N	0
97	81	1	36 76 606384	ОВД г. Ростов-на-Дону	2005-02-08	\N	1
98	82	1	70 22 559450	ОВД г. Белгород	2010-04-24	\N	1
99	83	1	99 67 434400	ОВД г. Воронеж	2008-02-05	\N	1
100	84	1	48 15 949541	ОВД г. Тамбов	2000-10-02	\N	1
101	85	1	24 12 990429	ОВД г. Липецк	2014-09-10	\N	1
102	85	3	DOC-085-3	Орган выдачи 6	2014-06-14	\N	0
103	86	1	31 41 641058	ОВД г. Ставрополь	2007-08-05	\N	1
104	87	1	80 30 447067	ОВД г. Саратов	2009-12-29	\N	1
105	88	1	64 69 343118	ОВД г. Волгоград	2006-04-12	\N	1
106	89	1	90 33 295440	ОВД г. Курск	2023-03-16	\N	1
107	90	1	94 65 514798	ОВД г. Краснодар	2007-03-08	\N	1
108	90	2	DOC-090-2	Орган выдачи 11	2016-06-04	2026-06-02	0
109	91	1	85 64 507723	ОВД г. Ростов-на-Дону	2005-01-15	\N	1
110	92	1	37 36 391865	ОВД г. Белгород	2000-01-19	\N	1
111	93	1	18 83 206694	ОВД г. Воронеж	2023-02-07	\N	1
112	94	1	78 33 483561	ОВД г. Тамбов	2008-12-18	\N	1
113	95	1	35 68 219490	ОВД г. Липецк	2003-08-28	\N	1
114	95	3	DOC-095-3	Орган выдачи 16	2022-12-23	\N	0
115	96	1	91 50 725969	ОВД г. Ставрополь	2013-04-21	\N	1
116	97	1	88 60 715519	ОВД г. Саратов	2004-05-10	\N	1
117	98	1	54 55 986589	ОВД г. Волгоград	2001-04-12	\N	1
118	99	1	89 32 953266	ОВД г. Курск	2005-02-18	\N	1
119	100	1	48 88 718836	ОВД г. Краснодар	2007-07-22	\N	1
120	100	2	DOC-100-2	Орган выдачи 1	2015-01-02	2024-12-30	0
121	101	1	25 40 420214	ОВД г. Ростов-на-Дону	2004-02-20	\N	1
122	102	1	33 57 827120	ОВД г. Белгород	2006-10-20	\N	1
123	103	1	75 59 538555	ОВД г. Воронеж	2001-08-02	\N	1
124	104	1	27 83 502349	ОВД г. Тамбов	2016-06-14	\N	1
125	105	1	33 72 766371	ОВД г. Липецк	2004-09-28	\N	1
126	105	4	DOC-105-4	Орган выдачи 6	2015-11-20	2025-11-17	0
127	106	1	31 72 402865	ОВД г. Ставрополь	2006-03-22	\N	1
128	107	1	33 50 982885	ОВД г. Саратов	2001-07-28	\N	1
129	108	1	89 16 476479	ОВД г. Волгоград	2012-04-11	\N	1
130	109	1	72 27 304607	ОВД г. Курск	2000-02-09	\N	1
131	110	1	59 81 629990	ОВД г. Краснодар	2009-02-21	\N	1
132	110	4	DOC-110-4	Орган выдачи 11	2023-02-06	2033-02-03	0
133	111	1	97 72 535820	ОВД г. Ростов-на-Дону	2004-08-02	\N	1
134	112	1	66 72 275377	ОВД г. Белгород	2007-12-18	\N	1
135	113	1	82 13 934541	ОВД г. Воронеж	2000-12-04	\N	1
136	114	1	38 47 134283	ОВД г. Тамбов	2008-07-14	\N	1
137	115	1	38 78 402692	ОВД г. Липецк	2003-01-30	\N	1
138	115	2	DOC-115-2	Орган выдачи 16	2022-04-04	2032-04-01	0
139	116	1	73 80 635618	ОВД г. Ставрополь	2022-05-01	\N	1
140	117	1	83 24 380901	ОВД г. Саратов	2019-09-13	\N	1
141	118	1	79 56 668373	ОВД г. Волгоград	2008-09-09	\N	1
142	119	1	15 66 672892	ОВД г. Курск	2009-03-23	\N	1
143	120	1	64 23 871740	ОВД г. Краснодар	2006-01-15	\N	1
144	120	4	DOC-120-4	Орган выдачи 1	2017-07-20	2027-07-18	0
145	121	1	14 67 375581	ОВД г. Ростов-на-Дону	2003-05-04	\N	1
146	122	1	21 66 223088	ОВД г. Белгород	2005-08-13	\N	1
147	123	1	40 36 950083	ОВД г. Воронеж	2008-09-11	\N	1
148	124	1	85 98 467663	ОВД г. Тамбов	2008-04-09	\N	1
149	125	1	88 90 548965	ОВД г. Липецк	2009-08-27	\N	1
150	125	2	DOC-125-2	Орган выдачи 6	2015-02-14	2025-02-11	0
151	126	1	36 36 944565	ОВД г. Ставрополь	2008-10-19	\N	1
152	127	1	82 54 658136	ОВД г. Саратов	2000-08-31	\N	1
153	128	1	86 78 276556	ОВД г. Волгоград	2012-02-11	\N	1
154	129	1	47 47 700556	ОВД г. Курск	2003-08-18	\N	1
155	130	1	75 96 954779	ОВД г. Краснодар	2009-09-10	\N	1
156	130	2	DOC-130-2	Орган выдачи 11	2015-01-15	2025-01-12	0
157	131	1	62 17 390794	ОВД г. Ростов-на-Дону	2011-07-11	\N	1
158	132	1	93 26 835499	ОВД г. Белгород	2009-09-21	\N	1
159	133	1	26 41 253347	ОВД г. Воронеж	2009-09-17	\N	1
160	134	1	51 41 898462	ОВД г. Тамбов	2007-12-23	\N	1
161	135	1	96 60 613234	ОВД г. Липецк	2009-06-17	\N	1
162	135	2	DOC-135-2	Орган выдачи 16	2024-12-13	2034-12-11	0
163	136	1	44 90 534346	ОВД г. Ставрополь	2023-01-11	\N	1
164	137	1	67 19 764406	ОВД г. Саратов	2004-03-16	\N	1
165	138	1	21 61 640659	ОВД г. Волгоград	2008-11-28	\N	1
166	139	1	45 98 486999	ОВД г. Курск	2025-04-26	\N	1
167	140	1	72 51 710224	ОВД г. Краснодар	2009-02-16	\N	1
168	140	2	DOC-140-2	Орган выдачи 1	2014-02-02	2024-01-31	0
169	141	1	68 91 796405	ОВД г. Ростов-на-Дону	2008-03-12	\N	1
170	142	1	55 18 934328	ОВД г. Белгород	2007-10-27	\N	1
171	143	1	60 37 550513	ОВД г. Воронеж	2005-12-29	\N	1
172	144	1	37 73 381581	ОВД г. Тамбов	2009-04-01	\N	1
173	145	1	46 53 673896	ОВД г. Липецк	2003-08-15	\N	1
174	145	4	DOC-145-4	Орган выдачи 6	2014-12-05	2024-12-02	0
175	146	1	72 53 815707	ОВД г. Ставрополь	2006-05-07	\N	1
176	147	1	16 15 204411	ОВД г. Саратов	2008-07-19	\N	1
177	148	1	68 12 230272	ОВД г. Волгоград	2007-01-08	\N	1
178	149	1	30 66 578693	ОВД г. Курск	2009-08-13	\N	1
179	150	1	64 35 822040	ОВД г. Краснодар	2000-01-03	\N	1
180	150	2	DOC-150-2	Орган выдачи 11	2018-10-08	2028-10-05	0
181	151	1	45 21 786385	ОВД г. Ростов-на-Дону	2001-10-13	\N	1
182	152	1	42 20 489906	ОВД г. Белгород	2004-01-16	\N	1
183	153	1	93 31 154302	ОВД г. Воронеж	2013-10-14	\N	1
184	154	1	90 49 861253	ОВД г. Тамбов	2004-06-11	\N	1
185	155	1	39 64 787326	ОВД г. Липецк	2007-11-04	\N	1
186	155	2	DOC-155-2	Орган выдачи 16	2014-02-18	2024-02-16	0
187	156	1	37 71 181724	ОВД г. Ставрополь	2000-01-05	\N	1
188	157	1	85 38 644676	ОВД г. Саратов	2001-06-27	\N	1
189	158	1	66 11 108454	ОВД г. Волгоград	2007-08-17	\N	1
190	159	1	53 25 542828	ОВД г. Курск	2021-01-05	\N	1
191	160	1	26 71 174268	ОВД г. Краснодар	2007-10-13	\N	1
192	160	2	DOC-160-2	Орган выдачи 1	2020-08-04	2030-08-02	0
193	161	1	23 23 428572	ОВД г. Ростов-на-Дону	2015-06-01	\N	1
194	162	1	48 27 501312	ОВД г. Белгород	2004-02-14	\N	1
195	163	1	27 92 810029	ОВД г. Воронеж	2008-09-03	\N	1
196	164	1	18 77 690653	ОВД г. Тамбов	2001-08-08	\N	1
197	165	1	88 93 272088	ОВД г. Липецк	2009-03-13	\N	1
198	165	3	DOC-165-3	Орган выдачи 6	2020-12-22	\N	0
199	166	1	37 90 884616	ОВД г. Ставрополь	2020-02-23	\N	1
200	167	1	62 88 819916	ОВД г. Саратов	2003-04-04	\N	1
201	168	1	37 21 205345	ОВД г. Волгоград	2004-12-17	\N	1
202	169	1	25 85 865470	ОВД г. Курск	2001-07-27	\N	1
203	170	1	54 64 429780	ОВД г. Краснодар	2004-04-21	\N	1
204	170	2	DOC-170-2	Орган выдачи 11	2017-07-22	2027-07-20	0
205	171	1	92 20 360221	ОВД г. Ростов-на-Дону	2017-03-09	\N	1
206	172	1	86 86 856211	ОВД г. Белгород	2006-03-16	\N	1
207	173	1	46 98 128127	ОВД г. Воронеж	2018-05-05	\N	1
208	174	1	94 48 315425	ОВД г. Тамбов	2009-07-04	\N	1
209	175	1	87 75 297810	ОВД г. Липецк	2005-10-21	\N	1
210	175	4	DOC-175-4	Орган выдачи 16	2020-10-14	2030-10-12	0
211	176	1	92 16 921875	ОВД г. Ставрополь	2003-04-25	\N	1
212	177	1	40 73 504630	ОВД г. Саратов	2017-05-12	\N	1
213	178	1	40 73 769077	ОВД г. Волгоград	2001-04-06	\N	1
214	179	1	19 77 112653	ОВД г. Курск	2019-03-27	\N	1
215	180	1	50 26 505629	ОВД г. Краснодар	2004-01-19	\N	1
216	180	4	DOC-180-4	Орган выдачи 1	2021-06-09	2031-06-07	0
217	181	1	79 97 281162	ОВД г. Ростов-на-Дону	2009-11-28	\N	1
218	182	1	70 19 117500	ОВД г. Белгород	2010-12-26	\N	1
219	183	1	18 11 374921	ОВД г. Воронеж	2006-08-02	\N	1
220	184	1	15 17 927630	ОВД г. Тамбов	2002-06-02	\N	1
221	185	1	74 46 759104	ОВД г. Липецк	2004-06-18	\N	1
222	185	4	DOC-185-4	Орган выдачи 6	2023-03-28	2033-03-25	0
223	186	1	63 64 835901	ОВД г. Ставрополь	2014-08-18	\N	1
224	187	1	20 91 662628	ОВД г. Саратов	2004-07-05	\N	1
225	188	1	88 29 390946	ОВД г. Волгоград	2006-01-11	\N	1
226	189	1	49 20 636141	ОВД г. Курск	2011-04-01	\N	1
227	190	1	29 78 442622	ОВД г. Краснодар	2002-04-12	\N	1
228	190	3	DOC-190-3	Орган выдачи 11	2025-02-25	\N	0
229	191	1	93 97 774396	ОВД г. Ростов-на-Дону	2013-06-21	\N	1
230	192	1	49 99 558737	ОВД г. Белгород	2005-07-17	\N	1
231	193	1	40 17 356355	ОВД г. Воронеж	2008-03-08	\N	1
232	194	1	65 24 575394	ОВД г. Тамбов	2014-10-26	\N	1
233	195	1	87 17 424314	ОВД г. Липецк	2006-11-12	\N	1
234	195	4	DOC-195-4	Орган выдачи 16	2015-12-09	2025-12-06	0
235	196	1	11 27 835575	ОВД г. Ставрополь	2004-09-13	\N	1
236	197	1	30 73 463546	ОВД г. Саратов	2002-07-19	\N	1
237	198	1	76 43 275073	ОВД г. Волгоград	2005-11-12	\N	1
238	199	1	26 44 868538	ОВД г. Курск	2004-03-03	\N	1
239	200	1	25 13 451284	ОВД г. Краснодар	2010-05-23	\N	1
240	200	3	DOC-200-3	Орган выдачи 1	2018-02-06	\N	0
241	201	1	18 43 846925	ОВД г. Ростов-на-Дону	2018-01-15	\N	1
242	202	1	90 19 620248	ОВД г. Белгород	2024-10-15	\N	1
243	203	1	75 56 158132	ОВД г. Воронеж	2005-02-07	\N	1
244	204	1	82 31 485548	ОВД г. Тамбов	2021-12-29	\N	1
245	205	1	42 23 701790	ОВД г. Липецк	2001-10-06	\N	1
246	205	4	DOC-205-4	Орган выдачи 6	2014-08-02	2024-07-30	0
247	206	1	75 10 146177	ОВД г. Ставрополь	2014-03-23	\N	1
248	207	1	11 41 148215	ОВД г. Саратов	2009-08-18	\N	1
249	208	1	56 59 257375	ОВД г. Волгоград	2005-04-16	\N	1
250	209	1	14 80 942127	ОВД г. Курск	2002-01-03	\N	1
251	210	1	93 63 335421	ОВД г. Краснодар	2025-10-25	\N	1
252	210	3	DOC-210-3	Орган выдачи 11	2023-03-14	\N	0
253	211	1	50 44 966120	ОВД г. Ростов-на-Дону	2004-08-31	\N	1
254	212	1	83 57 225200	ОВД г. Белгород	2003-11-29	\N	1
255	213	1	96 16 288056	ОВД г. Воронеж	2021-12-09	\N	1
256	214	1	76 15 520400	ОВД г. Тамбов	2017-07-02	\N	1
257	215	1	69 46 911102	ОВД г. Липецк	2000-10-09	\N	1
258	215	3	DOC-215-3	Орган выдачи 16	2019-05-05	\N	0
259	216	1	80 68 108356	ОВД г. Ставрополь	2000-12-19	\N	1
260	217	1	35 47 691194	ОВД г. Саратов	2004-02-20	\N	1
261	218	1	48 89 354367	ОВД г. Волгоград	2008-10-18	\N	1
262	219	1	57 85 617326	ОВД г. Курск	2005-03-15	\N	1
263	220	1	35 79 897423	ОВД г. Краснодар	2008-08-30	\N	1
264	220	2	DOC-220-2	Орган выдачи 1	2015-05-18	2025-05-15	0
265	221	1	62 13 344315	ОВД г. Ростов-на-Дону	2000-01-27	\N	1
266	222	1	54 91 980357	ОВД г. Белгород	2006-01-27	\N	1
267	223	1	11 52 101533	ОВД г. Воронеж	2007-10-11	\N	1
268	224	1	94 58 990247	ОВД г. Тамбов	2018-10-23	\N	1
269	225	1	49 23 315161	ОВД г. Липецк	2008-04-11	\N	1
270	225	4	DOC-225-4	Орган выдачи 6	2017-05-05	2027-05-03	0
271	226	1	72 17 249289	ОВД г. Ставрополь	2009-01-24	\N	1
272	227	1	45 21 146032	ОВД г. Саратов	2007-12-24	\N	1
273	228	1	76 62 834891	ОВД г. Волгоград	2002-08-06	\N	1
274	229	1	68 20 708982	ОВД г. Курск	2004-03-05	\N	1
275	230	1	74 27 768169	ОВД г. Краснодар	2007-11-17	\N	1
276	230	3	DOC-230-3	Орган выдачи 11	2013-09-05	\N	0
277	231	1	82 17 556112	ОВД г. Ростов-на-Дону	2006-08-21	\N	1
278	232	1	26 40 405844	ОВД г. Белгород	2007-06-06	\N	1
279	233	1	50 60 834764	ОВД г. Воронеж	2008-07-22	\N	1
280	234	1	51 50 574751	ОВД г. Тамбов	2009-04-30	\N	1
281	235	1	39 19 312559	ОВД г. Липецк	2004-07-23	\N	1
282	235	2	DOC-235-2	Орган выдачи 16	2025-02-10	2035-02-08	0
283	236	1	29 23 272359	ОВД г. Ставрополь	2015-10-05	\N	1
284	237	1	69 50 526259	ОВД г. Саратов	2005-01-15	\N	1
285	238	1	78 55 907756	ОВД г. Волгоград	2001-05-09	\N	1
286	239	1	67 49 585854	ОВД г. Курск	2006-05-14	\N	1
287	240	1	25 21 265271	ОВД г. Краснодар	2002-12-21	\N	1
288	240	4	DOC-240-4	Орган выдачи 1	2018-10-28	2028-10-25	0
289	241	1	99 87 142288	ОВД г. Ростов-на-Дону	2009-04-07	\N	1
290	242	1	51 28 195643	ОВД г. Белгород	2002-05-30	\N	1
291	243	1	41 55 515492	ОВД г. Воронеж	2007-12-22	\N	1
292	244	1	16 96 413357	ОВД г. Тамбов	2005-10-04	\N	1
293	245	1	32 13 524035	ОВД г. Липецк	2015-12-16	\N	1
294	245	3	DOC-245-3	Орган выдачи 6	2023-04-10	\N	0
295	246	1	80 41 199326	ОВД г. Ставрополь	2008-05-22	\N	1
296	247	1	23 27 227352	ОВД г. Саратов	2005-03-07	\N	1
297	248	1	17 38 237064	ОВД г. Волгоград	2000-02-18	\N	1
298	249	1	61 57 819622	ОВД г. Курск	2018-05-07	\N	1
299	250	1	92 20 710610	ОВД г. Краснодар	2016-01-09	\N	1
300	250	4	DOC-250-4	Орган выдачи 11	2018-10-04	2028-10-01	0
\.


--
-- TOC entry 3693 (class 0 OID 24655)
-- Dependencies: 236
-- Data for Name: product; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.product (id, type_id, name, unit_id) FROM stdin;
1	3	Пшеница озимая серия 01	1
2	3	Пшеница озимая серия 02	1
3	3	Пшеница озимая серия 03	1
4	3	Пшеница озимая серия 04	1
5	4	Кукуруза зерновая серия 01	1
6	4	Кукуруза зерновая серия 02	1
7	4	Кукуруза зерновая серия 03	1
8	4	Кукуруза зерновая серия 04	1
9	5	Ячмень кормовой серия 01	1
10	5	Ячмень кормовой серия 02	1
11	5	Ячмень кормовой серия 03	1
12	5	Ячмень кормовой серия 04	1
13	7	Картофель столовый серия 01	1
14	7	Картофель столовый серия 02	1
15	7	Картофель столовый серия 03	1
16	7	Картофель столовый серия 04	1
17	8	Морковь столовая серия 01	1
18	8	Морковь столовая серия 02	1
19	8	Морковь столовая серия 03	1
20	8	Морковь столовая серия 04	1
21	9	Капуста белокочанная серия 01	1
22	9	Капуста белокочанная серия 02	1
23	9	Капуста белокочанная серия 03	1
24	9	Капуста белокочанная серия 04	1
25	11	Яблоко свежее серия 01	1
26	11	Яблоко свежее серия 02	1
27	11	Яблоко свежее серия 03	1
28	11	Яблоко свежее серия 04	1
29	12	Груша десертная серия 01	1
30	12	Груша десертная серия 02	1
31	12	Груша десертная серия 03	1
32	12	Груша десертная серия 04	1
33	15	Молочное стадо серия 01	10
34	15	Молочное стадо серия 02	10
35	16	Мясное стадо серия 01	10
36	16	Мясное стадо серия 02	10
37	18	Яйцо куриное серия 01	4
38	18	Яйцо куриное серия 02	4
39	19	Мясо бройлера серия 01	1
40	19	Мясо бройлера серия 02	1
41	22	Молоко пастеризованное серия 01	3
42	22	Молоко пастеризованное серия 02	3
43	23	Сыр полутвердый серия 01	1
44	23	Сыр полутвердый серия 02	1
45	24	Масло сливочное серия 01	1
46	24	Масло сливочное серия 02	1
47	27	Комбикорм универсальный серия 01	6
48	27	Комбикорм универсальный серия 02	6
49	28	Сено луговое серия 01	6
50	28	Сено луговое серия 02	6
51	30	Удобрение NPK серия 01	6
52	30	Удобрение NPK серия 02	6
53	31	Удобрение органическое серия 01	6
54	31	Удобрение органическое серия 02	6
55	33	Семена пшеницы серия 01	6
56	33	Семена пшеницы серия 02	6
57	34	Семена кукурузы серия 01	6
58	34	Семена кукурузы серия 02	6
59	19	Мясо бройлера дополнительная партия 59	1
60	8	Морковь столовая дополнительная партия 60	1
61	34	Семена кукурузы дополнительная партия 61	6
62	9	Капуста белокочанная дополнительная партия 62	1
63	28	Сено луговое дополнительная партия 63	6
64	30	Удобрение NPK дополнительная партия 64	6
65	34	Семена кукурузы дополнительная партия 65	6
66	18	Яйцо куриное дополнительная партия 66	4
67	7	Картофель столовый дополнительная партия 67	1
68	19	Мясо бройлера дополнительная партия 68	1
69	30	Удобрение NPK дополнительная партия 69	6
70	27	Комбикорм универсальный дополнительная партия 70	6
\.


--
-- TOC entry 3680 (class 0 OID 24608)
-- Dependencies: 223
-- Data for Name: product_type; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.product_type (id, parent_id, sku, product_level, name, description) FROM stdin;
1	\N	PT-001	1	Растениеводство	Корневая категория продукции растениеводства
2	1	PT-002	2	Зерновые	Категория зерновых культур
3	2	PT-003	3	Пшеница	Зерновая культура
4	2	PT-004	3	Кукуруза	Зерновая культура
5	2	PT-005	3	Ячмень	Зерновая культура
6	1	PT-006	2	Овощи	Категория овощей
7	6	PT-007	3	Картофель	Корнеплод
8	6	PT-008	3	Морковь	Корнеплод
9	6	PT-009	3	Капуста	Овощная культура
10	1	PT-010	2	Фрукты	Категория фруктов
11	10	PT-011	3	Яблоко	Плодовая культура
12	10	PT-012	3	Груша	Плодовая культура
13	\N	PT-013	1	Животноводство	Корневая категория продукции животноводства
14	13	PT-014	2	Крупный рогатый скот	Категория КРС
15	14	PT-015	3	Молочное стадо	Животноводческая категория
16	14	PT-016	3	Мясное стадо	Животноводческая категория
17	13	PT-017	2	Птицеводство	Категория птицеводства
18	17	PT-018	3	Куры-несушки	Категория яичного направления
19	17	PT-019	3	Бройлеры	Категория мясного направления
20	\N	PT-020	1	Переработка	Корневая категория переработанной продукции
21	20	PT-021	2	Молочная продукция	Группа молочной продукции
22	21	PT-022	3	Молоко	Готовая молочная продукция
23	21	PT-023	3	Сыр	Готовая молочная продукция
24	21	PT-024	3	Масло сливочное	Готовая молочная продукция
25	\N	PT-025	1	Материалы	Материалы и ресурсы для хозяйств
26	25	PT-026	2	Корма	Категория кормов
27	26	PT-027	3	Комбикорм	Корм для животных
28	26	PT-028	3	Сено	Корм грубый
29	25	PT-029	2	Удобрения	Категория удобрений
30	29	PT-030	3	Минеральные удобрения	Удобрения химического происхождения
31	29	PT-031	3	Органические удобрения	Удобрения органического происхождения
32	25	PT-032	2	Семена	Категория семенного материала
33	32	PT-033	3	Семена пшеницы	Посевной материал для пшеницы
34	32	PT-034	3	Семена кукурузы	Посевной материал для кукурузы
\.


--
-- TOC entry 3694 (class 0 OID 24658)
-- Dependencies: 237
-- Data for Name: purchase_order; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.purchase_order (id, association_id, supplier_id, delivery_address, order_date, expected_delivery_date, status, total_amount, received_at, created_by) FROM stdin;
1	2	24	г. Ростов-на-Дону, ул. Степная, д. 12	2026-01-10	2026-01-29	received	238912.90	2026-01-25	14
2	7	18	г. Ставрополь, ул. Степная, д. 17	2025-02-06	2025-02-07	draft	352062.19	\N	27
3	2	16	г. Ростов-на-Дону, ул. Степная, д. 12	2025-03-23	2025-04-17	sent	184034.09	\N	40
4	5	5	г. Тамбов, ул. Центральная, д. 15	2024-04-08	2024-05-22	received	449047.63	2024-04-12	53
5	6	8	г. Липецк, ул. Садовая, д. 16	2024-06-04	2024-06-06	received	131412.43	2024-06-16	66
6	5	16	г. Тамбов, ул. Центральная, д. 15	2024-05-07	2024-05-14	sent	386000.42	\N	79
7	7	15	г. Ставрополь, ул. Степная, д. 17	2024-08-06	2024-08-21	draft	500674.34	\N	92
8	5	7	г. Тамбов, ул. Центральная, д. 15	2025-11-20	2025-12-22	received	323113.13	2025-12-25	105
9	7	6	г. Ставрополь, ул. Степная, д. 17	2025-02-17	2025-03-03	sent	503573.97	\N	118
10	2	15	г. Ростов-на-Дону, ул. Степная, д. 12	2025-12-31	2026-01-02	received	180941.73	2026-01-13	131
11	8	6	г. Саратов, ул. Школьная, д. 18	2024-06-21	2024-07-06	archived	272539.96	\N	144
12	2	17	г. Ростов-на-Дону, ул. Степная, д. 12	2024-09-18	2024-10-17	sent	145076.23	\N	157
13	1	19	г. Краснодар, ул. Садовая, д. 11	2025-11-01	2025-11-13	sent	311000.66	\N	170
14	2	25	г. Ростов-на-Дону, ул. Степная, д. 12	2025-04-18	2025-05-24	cancelled	372795.18	\N	183
15	1	16	г. Краснодар, ул. Садовая, д. 11	2024-06-08	2024-06-17	sent	431090.14	\N	196
16	1	10	г. Краснодар, ул. Садовая, д. 11	2025-04-16	2025-05-20	cancelled	601653.26	\N	209
17	3	17	г. Белгород, ул. Школьная, д. 13	2025-05-26	2025-06-04	cancelled	419024.56	\N	222
18	6	1	г. Липецк, ул. Садовая, д. 16	2024-11-18	2024-12-20	sent	368173.79	\N	235
19	3	21	г. Белгород, ул. Школьная, д. 13	2024-11-05	2024-12-01	sent	458982.12	\N	248
20	3	14	г. Белгород, ул. Школьная, д. 13	2024-02-22	2024-03-09	archived	385874.27	\N	11
21	8	3	г. Саратов, ул. Школьная, д. 18	2024-12-26	2025-01-17	sent	196679.42	\N	24
22	8	12	г. Саратов, ул. Школьная, д. 18	2025-09-28	2025-10-19	sent	273302.83	\N	37
23	2	5	г. Ростов-на-Дону, ул. Степная, д. 12	2025-01-21	2025-02-12	received	280415.98	2025-02-20	50
24	7	5	г. Ставрополь, ул. Степная, д. 17	2025-03-13	2025-04-18	received	217869.59	2025-03-28	63
25	8	4	г. Саратов, ул. Школьная, д. 18	2024-03-22	2024-05-04	received	396949.72	2024-04-14	76
26	5	20	г. Тамбов, ул. Центральная, д. 15	2024-09-08	2024-09-23	cancelled	601303.71	\N	89
27	2	5	г. Ростов-на-Дону, ул. Степная, д. 12	2025-07-22	2025-08-06	cancelled	412539.88	\N	102
28	7	19	г. Ставрополь, ул. Степная, д. 17	2024-12-05	2024-12-25	draft	182818.54	\N	115
29	3	14	г. Белгород, ул. Школьная, д. 13	2025-04-29	2025-05-01	sent	423803.88	\N	128
30	5	8	г. Тамбов, ул. Центральная, д. 15	2025-09-17	2025-09-28	sent	361762.39	\N	141
31	1	16	г. Краснодар, ул. Садовая, д. 11	2024-06-30	2024-07-03	sent	172436.17	\N	154
32	1	7	г. Краснодар, ул. Садовая, д. 11	2024-06-23	2024-07-19	cancelled	233639.68	\N	167
33	3	21	г. Белгород, ул. Школьная, д. 13	2025-12-17	2026-01-19	draft	514759.42	\N	180
34	7	14	г. Ставрополь, ул. Степная, д. 17	2024-08-28	2024-09-17	received	154383.60	2024-09-12	193
35	3	5	г. Белгород, ул. Школьная, д. 13	2026-01-10	2026-01-15	sent	515243.03	\N	206
36	4	16	г. Воронеж, ул. Луговая, д. 14	2025-11-02	2025-11-05	sent	253347.03	\N	219
37	8	5	г. Саратов, ул. Школьная, д. 18	2024-11-30	2024-12-28	received	292532.45	2025-01-07	232
38	3	14	г. Белгород, ул. Школьная, д. 13	2024-02-09	2024-02-10	received	264514.11	2024-02-22	245
39	5	17	г. Тамбов, ул. Центральная, д. 15	2025-02-04	2025-03-10	archived	550106.43	\N	8
40	1	14	г. Краснодар, ул. Садовая, д. 11	2025-08-26	2025-10-06	archived	590956.17	\N	21
41	8	9	г. Саратов, ул. Школьная, д. 18	2024-11-03	2024-12-15	archived	293431.86	\N	34
42	4	13	г. Воронеж, ул. Луговая, д. 14	2024-01-03	2024-01-31	draft	369597.07	\N	47
43	5	25	г. Тамбов, ул. Центральная, д. 15	2025-09-06	2025-10-19	draft	297388.83	\N	60
44	4	16	г. Воронеж, ул. Луговая, д. 14	2024-01-22	2024-01-29	sent	263310.03	\N	73
45	2	3	г. Ростов-на-Дону, ул. Степная, д. 12	2024-06-08	2024-06-16	archived	272312.34	\N	86
46	8	17	г. Саратов, ул. Школьная, д. 18	2024-05-05	2024-06-05	received	478789.20	2024-05-24	99
47	1	20	г. Краснодар, ул. Садовая, д. 11	2024-03-29	2024-04-03	sent	515336.35	\N	112
48	2	17	г. Ростов-на-Дону, ул. Степная, д. 12	2025-02-16	2025-03-02	sent	428955.13	\N	125
49	6	7	г. Липецк, ул. Садовая, д. 16	2024-02-12	2024-02-15	received	459006.69	2024-02-14	138
50	3	16	г. Белгород, ул. Школьная, д. 13	2025-05-21	2025-06-26	archived	304978.66	\N	151
51	3	23	г. Белгород, ул. Школьная, д. 13	2024-09-21	2024-10-16	archived	242605.43	\N	164
52	6	6	г. Липецк, ул. Садовая, д. 16	2025-12-29	2026-01-04	archived	393813.64	\N	177
53	1	9	г. Краснодар, ул. Садовая, д. 11	2024-11-14	2024-12-20	cancelled	328698.22	\N	190
54	5	14	г. Тамбов, ул. Центральная, д. 15	2024-04-05	2024-05-18	sent	318072.61	\N	203
55	2	11	г. Ростов-на-Дону, ул. Степная, д. 12	2024-06-21	2024-07-03	received	391036.61	2024-07-11	216
56	8	18	г. Саратов, ул. Школьная, д. 18	2024-08-31	2024-09-04	received	126916.49	2024-09-14	229
57	2	23	г. Ростов-на-Дону, ул. Степная, д. 12	2025-03-06	2025-03-23	received	193298.89	2025-03-23	242
58	2	22	г. Ростов-на-Дону, ул. Степная, д. 12	2024-09-11	2024-09-20	received	166232.24	2024-09-20	5
59	4	23	г. Воронеж, ул. Луговая, д. 14	2024-04-23	2024-06-07	draft	265273.25	\N	18
60	7	13	г. Ставрополь, ул. Степная, д. 17	2025-11-06	2025-12-10	received	425043.75	2025-12-06	31
61	7	15	г. Ставрополь, ул. Степная, д. 17	2024-06-11	2024-06-15	cancelled	126277.93	\N	44
62	3	14	г. Белгород, ул. Школьная, д. 13	2024-01-15	2024-02-15	cancelled	490616.51	\N	57
63	1	25	г. Краснодар, ул. Садовая, д. 11	2024-08-28	2024-10-09	received	545069.19	2024-09-13	70
64	2	8	г. Ростов-на-Дону, ул. Степная, д. 12	2024-10-16	2024-11-02	cancelled	339131.72	\N	83
65	6	1	г. Липецк, ул. Садовая, д. 16	2025-05-07	2025-05-20	sent	398437.10	\N	96
66	4	8	г. Воронеж, ул. Луговая, д. 14	2024-11-28	2024-12-01	archived	167926.61	\N	109
67	4	14	г. Воронеж, ул. Луговая, д. 14	2024-02-14	2024-02-28	received	362178.28	2024-03-04	122
68	8	7	г. Саратов, ул. Школьная, д. 18	2024-04-05	2024-04-27	archived	324906.63	\N	135
69	4	14	г. Воронеж, ул. Луговая, д. 14	2024-12-04	2024-12-11	draft	576898.47	\N	148
70	5	7	г. Тамбов, ул. Центральная, д. 15	2024-06-25	2024-07-06	sent	247702.59	\N	161
71	2	24	г. Ростов-на-Дону, ул. Степная, д. 12	2026-01-11	2026-01-25	sent	363724.49	\N	174
72	2	3	г. Ростов-на-Дону, ул. Степная, д. 12	2024-06-10	2024-06-24	draft	426719.11	\N	187
73	6	15	г. Липецк, ул. Садовая, д. 16	2024-06-10	2024-07-19	received	314923.08	2024-07-18	200
74	1	7	г. Краснодар, ул. Садовая, д. 11	2025-02-19	2025-03-10	received	166257.80	2025-03-22	213
75	8	13	г. Саратов, ул. Школьная, д. 18	2025-10-27	2025-12-04	received	328644.81	2025-11-06	226
76	2	8	г. Ростов-на-Дону, ул. Степная, д. 12	2025-07-05	2025-08-10	draft	483343.55	\N	239
77	8	3	г. Саратов, ул. Школьная, д. 18	2025-03-08	2025-04-18	received	451975.89	2025-04-26	2
78	1	3	г. Краснодар, ул. Садовая, д. 11	2024-11-26	2024-12-07	cancelled	656667.84	\N	15
79	3	12	г. Белгород, ул. Школьная, д. 13	2024-11-16	2024-12-11	received	422519.99	2024-12-20	28
80	1	11	г. Краснодар, ул. Садовая, д. 11	2025-01-23	2025-02-23	draft	191333.98	\N	41
81	5	17	г. Тамбов, ул. Центральная, д. 15	2024-06-02	2024-06-28	received	449762.66	2024-06-11	54
82	5	6	г. Тамбов, ул. Центральная, д. 15	2025-08-13	2025-09-07	received	568865.68	2025-09-19	67
83	3	5	г. Белгород, ул. Школьная, д. 13	2024-03-07	2024-03-11	sent	292046.29	\N	80
84	5	21	г. Тамбов, ул. Центральная, д. 15	2024-06-30	2024-08-05	archived	338515.23	\N	93
85	3	23	г. Белгород, ул. Школьная, д. 13	2025-02-13	2025-03-25	received	102153.57	2025-04-02	106
86	7	25	г. Ставрополь, ул. Степная, д. 17	2026-01-04	2026-01-13	sent	555195.14	\N	119
87	6	25	г. Липецк, ул. Садовая, д. 16	2024-10-24	2024-10-27	sent	125641.09	\N	132
88	7	9	г. Ставрополь, ул. Степная, д. 17	2024-10-24	2024-11-26	received	303501.49	2024-11-28	145
89	6	2	г. Липецк, ул. Садовая, д. 16	2024-10-23	2024-12-06	received	394146.23	2024-12-18	158
90	3	4	г. Белгород, ул. Школьная, д. 13	2024-04-02	2024-04-09	received	493762.62	2024-04-18	171
91	7	2	г. Ставрополь, ул. Степная, д. 17	2026-01-10	2026-01-15	archived	487669.38	\N	184
92	1	1	г. Краснодар, ул. Садовая, д. 11	2025-08-23	2025-08-30	sent	247933.72	\N	197
93	5	20	г. Тамбов, ул. Центральная, д. 15	2024-05-02	2024-05-29	archived	540537.72	\N	210
94	5	4	г. Тамбов, ул. Центральная, д. 15	2025-07-06	2025-07-25	sent	250310.21	\N	223
95	3	3	г. Белгород, ул. Школьная, д. 13	2024-09-02	2024-10-01	sent	226657.09	\N	236
96	1	25	г. Краснодар, ул. Садовая, д. 11	2024-12-11	2024-12-27	received	407529.72	2024-12-25	249
97	4	12	г. Воронеж, ул. Луговая, д. 14	2025-02-03	2025-03-01	archived	92426.37	\N	12
98	4	9	г. Воронеж, ул. Луговая, д. 14	2024-12-14	2025-01-06	draft	365532.05	\N	25
99	4	17	г. Воронеж, ул. Луговая, д. 14	2024-09-27	2024-10-16	received	160637.23	2024-10-01	38
100	8	22	г. Саратов, ул. Школьная, д. 18	2025-07-17	2025-08-20	sent	179091.53	\N	51
101	1	20	г. Краснодар, ул. Садовая, д. 11	2024-02-18	2024-03-22	sent	556761.56	\N	64
102	7	25	г. Ставрополь, ул. Степная, д. 17	2025-10-01	2025-11-07	cancelled	491661.05	\N	77
103	2	10	г. Ростов-на-Дону, ул. Степная, д. 12	2024-01-22	2024-02-24	archived	487209.31	\N	90
104	7	20	г. Ставрополь, ул. Степная, д. 17	2025-12-17	2025-12-19	archived	389303.46	\N	103
105	4	17	г. Воронеж, ул. Луговая, д. 14	2025-09-13	2025-09-28	sent	499441.38	\N	116
106	7	11	г. Ставрополь, ул. Степная, д. 17	2024-01-31	2024-03-07	cancelled	235056.29	\N	129
107	7	19	г. Ставрополь, ул. Степная, д. 17	2025-01-22	2025-02-14	received	370693.78	2025-01-28	142
108	6	19	г. Липецк, ул. Садовая, д. 16	2024-12-10	2024-12-18	sent	334242.94	\N	155
109	1	1	г. Краснодар, ул. Садовая, д. 11	2024-06-17	2024-07-18	cancelled	305836.90	\N	168
110	2	25	г. Ростов-на-Дону, ул. Степная, д. 12	2025-01-31	2025-03-05	sent	189764.58	\N	181
111	2	10	г. Ростов-на-Дону, ул. Степная, д. 12	2025-10-06	2025-11-14	draft	352103.64	\N	194
112	4	21	г. Воронеж, ул. Луговая, д. 14	2024-10-13	2024-10-31	draft	338149.37	\N	207
113	3	12	г. Белгород, ул. Школьная, д. 13	2024-01-20	2024-02-25	cancelled	476769.38	\N	220
114	2	7	г. Ростов-на-Дону, ул. Степная, д. 12	2024-07-22	2024-08-04	received	100847.48	2024-07-26	233
115	2	15	г. Ростов-на-Дону, ул. Степная, д. 12	2024-01-11	2024-01-25	cancelled	271521.22	\N	246
116	6	5	г. Липецк, ул. Садовая, д. 16	2025-04-23	2025-05-25	received	436897.16	2025-05-17	9
117	6	2	г. Липецк, ул. Садовая, д. 16	2024-05-24	2024-06-19	sent	167293.96	\N	22
118	7	16	г. Ставрополь, ул. Степная, д. 17	2025-07-01	2025-07-08	archived	191364.93	\N	35
119	3	4	г. Белгород, ул. Школьная, д. 13	2025-10-05	2025-10-18	sent	470112.60	\N	48
120	4	4	г. Воронеж, ул. Луговая, д. 14	2025-06-11	2025-06-23	received	350748.66	2025-06-23	61
121	2	15	г. Ростов-на-Дону, ул. Степная, д. 12	2025-05-04	2025-06-18	draft	319024.79	\N	74
122	3	8	г. Белгород, ул. Школьная, д. 13	2025-10-06	2025-10-29	sent	290926.38	\N	87
123	2	22	г. Ростов-на-Дону, ул. Степная, д. 12	2024-09-28	2024-10-24	received	359883.06	2024-10-04	100
124	5	18	г. Тамбов, ул. Центральная, д. 15	2024-01-17	2024-01-28	sent	385201.29	\N	113
125	5	17	г. Тамбов, ул. Центральная, д. 15	2024-10-25	2024-12-06	sent	369732.66	\N	126
126	2	12	г. Ростов-на-Дону, ул. Степная, д. 12	2024-07-02	2024-07-18	archived	327057.86	\N	139
127	2	24	г. Ростов-на-Дону, ул. Степная, д. 12	2025-10-04	2025-11-16	archived	619019.23	\N	152
128	6	17	г. Липецк, ул. Садовая, д. 16	2025-08-13	2025-08-26	received	115457.82	2025-09-07	165
129	4	12	г. Воронеж, ул. Луговая, д. 14	2024-02-23	2024-04-08	sent	338870.41	\N	178
130	1	1	г. Краснодар, ул. Садовая, д. 11	2025-11-20	2025-11-28	sent	380171.21	\N	191
131	5	6	г. Тамбов, ул. Центральная, д. 15	2025-10-15	2025-10-31	received	314460.10	2025-10-21	204
132	6	6	г. Липецк, ул. Садовая, д. 16	2025-06-29	2025-07-21	received	316552.45	2025-07-31	217
133	4	11	г. Воронеж, ул. Луговая, д. 14	2025-06-26	2025-08-04	cancelled	394377.96	\N	230
134	6	1	г. Липецк, ул. Садовая, д. 16	2025-09-16	2025-10-07	cancelled	467028.03	\N	243
135	8	6	г. Саратов, ул. Школьная, д. 18	2024-02-18	2024-03-02	received	77804.40	2024-03-13	6
136	6	7	г. Липецк, ул. Садовая, д. 16	2024-04-19	2024-05-03	received	305679.35	2024-05-02	19
137	2	10	г. Ростов-на-Дону, ул. Степная, д. 12	2024-03-30	2024-04-02	cancelled	424071.67	\N	32
138	3	25	г. Белгород, ул. Школьная, д. 13	2025-05-04	2025-05-05	sent	314032.60	\N	45
139	7	14	г. Ставрополь, ул. Степная, д. 17	2024-02-29	2024-03-21	archived	424023.24	\N	58
140	6	24	г. Липецк, ул. Садовая, д. 16	2025-08-25	2025-09-22	archived	584344.12	\N	71
141	3	1	г. Белгород, ул. Школьная, д. 13	2024-02-10	2024-02-13	sent	171821.15	\N	84
142	2	7	г. Ростов-на-Дону, ул. Степная, д. 12	2024-01-06	2024-01-27	sent	265128.51	\N	97
143	5	17	г. Тамбов, ул. Центральная, д. 15	2025-06-21	2025-07-31	received	505712.45	2025-07-30	110
144	3	12	г. Белгород, ул. Школьная, д. 13	2024-10-21	2024-11-04	archived	374670.60	\N	123
145	2	5	г. Ростов-на-Дону, ул. Степная, д. 12	2024-07-23	2024-08-31	sent	134058.85	\N	136
146	8	12	г. Саратов, ул. Школьная, д. 18	2024-01-25	2024-02-29	sent	449281.59	\N	149
147	5	16	г. Тамбов, ул. Центральная, д. 15	2024-05-18	2024-06-12	sent	495416.31	\N	162
148	3	23	г. Белгород, ул. Школьная, д. 13	2025-03-26	2025-04-03	received	339219.43	2025-04-02	175
149	1	11	г. Краснодар, ул. Садовая, д. 11	2024-07-05	2024-07-17	sent	345392.25	\N	188
150	6	21	г. Липецк, ул. Садовая, д. 16	2025-01-07	2025-02-21	archived	459893.05	\N	201
151	6	9	г. Липецк, ул. Садовая, д. 16	2024-09-16	2024-10-23	cancelled	81274.41	\N	214
152	8	25	г. Саратов, ул. Школьная, д. 18	2025-06-23	2025-07-25	received	293709.65	2025-07-08	227
153	1	18	г. Краснодар, ул. Садовая, д. 11	2024-07-11	2024-07-23	draft	362161.89	\N	240
154	8	12	г. Саратов, ул. Школьная, д. 18	2025-12-01	2025-12-15	received	469386.84	2025-12-26	3
155	2	19	г. Ростов-на-Дону, ул. Степная, д. 12	2025-08-10	2025-08-31	draft	208131.27	\N	16
156	3	25	г. Белгород, ул. Школьная, д. 13	2026-01-17	2026-02-24	received	300835.87	2026-03-02	29
157	4	1	г. Воронеж, ул. Луговая, д. 14	2025-08-13	2025-09-09	archived	375404.71	\N	42
158	3	24	г. Белгород, ул. Школьная, д. 13	2024-06-04	2024-06-12	sent	195934.97	\N	55
159	2	4	г. Ростов-на-Дону, ул. Степная, д. 12	2024-04-25	2024-05-20	received	302558.62	2024-05-05	68
160	4	7	г. Воронеж, ул. Луговая, д. 14	2025-09-29	2025-10-13	draft	234811.19	\N	81
161	5	1	г. Тамбов, ул. Центральная, д. 15	2025-07-25	2025-09-07	received	123295.80	2025-08-29	94
162	4	23	г. Воронеж, ул. Луговая, д. 14	2025-01-10	2025-02-02	received	550987.29	2025-01-16	107
163	8	4	г. Саратов, ул. Школьная, д. 18	2024-05-25	2024-06-12	sent	326439.19	\N	120
164	1	5	г. Краснодар, ул. Садовая, д. 11	2025-05-08	2025-05-12	received	378454.68	2025-05-21	133
165	3	5	г. Белгород, ул. Школьная, д. 13	2024-10-15	2024-11-18	received	292031.82	2024-10-23	146
166	1	20	г. Краснодар, ул. Садовая, д. 11	2025-01-06	2025-02-01	archived	476996.98	\N	159
167	3	8	г. Белгород, ул. Школьная, д. 13	2024-09-14	2024-10-15	cancelled	70477.47	\N	172
168	2	2	г. Ростов-на-Дону, ул. Степная, д. 12	2024-07-31	2024-08-02	cancelled	371608.05	\N	185
169	6	15	г. Липецк, ул. Садовая, д. 16	2024-04-19	2024-04-29	received	284030.87	2024-05-14	198
170	1	15	г. Краснодар, ул. Садовая, д. 11	2025-12-30	2026-01-14	sent	296297.39	\N	211
171	4	1	г. Воронеж, ул. Луговая, д. 14	2024-01-18	2024-02-24	received	364399.89	2024-02-11	224
172	7	6	г. Ставрополь, ул. Степная, д. 17	2025-03-26	2025-04-12	cancelled	273466.61	\N	237
173	5	13	г. Тамбов, ул. Центральная, д. 15	2025-02-28	2025-03-28	draft	169586.55	\N	250
174	1	1	г. Краснодар, ул. Садовая, д. 11	2026-02-05	2026-03-19	sent	363309.59	\N	13
175	1	11	г. Краснодар, ул. Садовая, д. 11	2024-09-12	2024-10-25	archived	149085.19	\N	26
176	8	13	г. Саратов, ул. Школьная, д. 18	2025-09-12	2025-10-16	archived	393576.65	\N	39
177	2	5	г. Ростов-на-Дону, ул. Степная, д. 12	2024-08-09	2024-09-10	sent	140545.20	\N	52
178	5	19	г. Тамбов, ул. Центральная, д. 15	2025-08-05	2025-08-13	archived	250709.50	\N	65
179	8	18	г. Саратов, ул. Школьная, д. 18	2024-06-24	2024-07-01	draft	337930.07	\N	78
180	2	8	г. Ростов-на-Дону, ул. Степная, д. 12	2025-01-13	2025-02-26	received	465601.95	2025-02-23	91
181	1	3	г. Краснодар, ул. Садовая, д. 11	2025-07-13	2025-08-08	archived	374797.31	\N	104
182	6	15	г. Липецк, ул. Садовая, д. 16	2024-01-12	2024-02-16	sent	319248.58	\N	117
183	4	13	г. Воронеж, ул. Луговая, д. 14	2025-07-30	2025-08-05	sent	258084.30	\N	130
184	7	18	г. Ставрополь, ул. Степная, д. 17	2025-05-31	2025-06-10	received	270371.54	2025-06-14	143
185	1	16	г. Краснодар, ул. Садовая, д. 11	2025-07-19	2025-08-30	sent	232856.72	\N	156
186	7	6	г. Ставрополь, ул. Степная, д. 17	2024-10-26	2024-12-06	draft	320210.25	\N	169
187	6	4	г. Липецк, ул. Садовая, д. 16	2025-01-30	2025-02-09	sent	368819.65	\N	182
188	4	20	г. Воронеж, ул. Луговая, д. 14	2024-02-12	2024-03-11	sent	160794.72	\N	195
189	4	4	г. Воронеж, ул. Луговая, д. 14	2025-08-23	2025-09-08	sent	242645.61	\N	208
190	2	13	г. Ростов-на-Дону, ул. Степная, д. 12	2024-12-06	2025-01-20	draft	96532.62	\N	221
191	1	7	г. Краснодар, ул. Садовая, д. 11	2025-12-14	2026-01-02	sent	369828.50	\N	234
192	1	13	г. Краснодар, ул. Садовая, д. 11	2025-04-17	2025-05-23	sent	364855.02	\N	247
193	7	20	г. Ставрополь, ул. Степная, д. 17	2025-11-10	2025-12-15	received	327447.34	2025-11-27	10
194	1	4	г. Краснодар, ул. Садовая, д. 11	2024-05-07	2024-06-16	sent	565676.53	\N	23
195	4	20	г. Воронеж, ул. Луговая, д. 14	2025-07-04	2025-08-09	cancelled	243108.55	\N	36
196	6	12	г. Липецк, ул. Садовая, д. 16	2025-09-05	2025-09-09	received	229525.13	2025-09-07	49
197	5	6	г. Тамбов, ул. Центральная, д. 15	2024-02-18	2024-03-08	archived	176246.05	\N	62
198	6	14	г. Липецк, ул. Садовая, д. 16	2025-01-07	2025-02-08	cancelled	514205.98	\N	75
199	7	5	г. Ставрополь, ул. Степная, д. 17	2024-09-05	2024-09-29	sent	283254.48	\N	88
200	6	8	г. Липецк, ул. Садовая, д. 16	2024-02-24	2024-02-25	received	238268.23	2024-03-07	101
201	6	20	г. Липецк, ул. Садовая, д. 16	2024-03-09	2024-04-03	draft	380090.39	\N	114
202	7	2	г. Ставрополь, ул. Степная, д. 17	2024-01-07	2024-02-11	draft	299870.41	\N	127
203	8	1	г. Саратов, ул. Школьная, д. 18	2024-10-06	2024-11-04	sent	402397.64	\N	140
204	6	8	г. Липецк, ул. Садовая, д. 16	2025-03-16	2025-04-08	received	398475.53	2025-04-11	153
205	6	9	г. Липецк, ул. Садовая, д. 16	2025-01-05	2025-01-23	draft	481213.96	\N	166
206	8	10	г. Саратов, ул. Школьная, д. 18	2025-07-04	2025-08-13	cancelled	432513.10	\N	179
207	7	22	г. Ставрополь, ул. Степная, д. 17	2024-06-22	2024-06-27	archived	499489.11	\N	192
208	4	2	г. Воронеж, ул. Луговая, д. 14	2025-01-30	2025-03-10	archived	146185.23	\N	205
209	5	1	г. Тамбов, ул. Центральная, д. 15	2024-02-06	2024-03-22	cancelled	410024.10	\N	218
210	6	9	г. Липецк, ул. Садовая, д. 16	2024-09-25	2024-10-23	draft	503355.26	\N	231
211	2	5	г. Ростов-на-Дону, ул. Степная, д. 12	2025-07-03	2025-08-02	archived	426204.08	\N	244
212	6	16	г. Липецк, ул. Садовая, д. 16	2025-08-15	2025-09-29	sent	212230.45	\N	7
213	5	6	г. Тамбов, ул. Центральная, д. 15	2024-10-01	2024-10-10	archived	338300.64	\N	20
214	7	6	г. Ставрополь, ул. Степная, д. 17	2025-08-03	2025-08-17	archived	71031.28	\N	33
215	7	15	г. Ставрополь, ул. Степная, д. 17	2025-03-20	2025-04-14	draft	291784.09	\N	46
216	2	1	г. Ростов-на-Дону, ул. Степная, д. 12	2025-07-14	2025-07-20	cancelled	280560.70	\N	59
217	6	21	г. Липецк, ул. Садовая, д. 16	2024-09-06	2024-10-18	archived	355189.36	\N	72
218	2	2	г. Ростов-на-Дону, ул. Степная, д. 12	2024-01-04	2024-01-17	received	387796.68	2024-01-06	85
219	1	23	г. Краснодар, ул. Садовая, д. 11	2024-02-17	2024-03-16	received	396808.62	2024-03-14	98
220	5	1	г. Тамбов, ул. Центральная, д. 15	2025-04-02	2025-05-07	sent	305301.88	\N	111
\.


--
-- TOC entry 3695 (class 0 OID 24661)
-- Dependencies: 238
-- Data for Name: purchase_order_item; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.purchase_order_item (id, purchase_order_id, product_id, quantity, unit_price, vat_rate, currency) FROM stdin;
1	1	43	201.899	395.25	20.00	RUB
2	1	51	142.663	385.64	20.00	RUB
3	1	46	234.949	242.43	10.00	RUB
4	1	13	81.707	418.81	20.00	RUB
5	1	18	45.305	285.12	0.00	RUB
6	2	48	412.368	195.10	20.00	RUB
7	2	35	288.052	228.73	0.00	RUB
8	2	15	140.194	312.43	10.00	RUB
9	2	69	317.775	509.55	10.00	RUB
10	3	44	382.461	367.22	0.00	RUB
11	3	36	30.107	276.60	0.00	RUB
12	3	52	145.904	241.66	20.00	RUB
13	4	62	488.650	205.40	20.00	RUB
14	4	44	145.815	329.27	10.00	RUB
15	4	27	127.933	428.31	0.00	RUB
16	4	35	312.973	325.67	20.00	RUB
17	4	3	378.346	380.46	20.00	RUB
18	5	53	197.803	394.87	20.00	RUB
19	5	49	311.985	135.92	0.00	RUB
20	5	5	54.677	199.37	20.00	RUB
21	6	58	447.024	501.69	0.00	RUB
22	6	50	146.941	177.45	0.00	RUB
23	6	53	126.998	422.67	10.00	RUB
24	6	44	223.245	367.22	20.00	RUB
25	7	32	226.839	180.43	20.00	RUB
26	7	58	445.178	537.62	20.00	RUB
27	7	14	377.840	583.34	0.00	RUB
28	8	60	311.663	132.63	10.00	RUB
29	8	69	307.211	457.17	10.00	RUB
30	8	33	80.813	122.66	0.00	RUB
31	8	53	304.467	431.63	20.00	RUB
32	9	14	450.242	411.51	10.00	RUB
33	9	36	304.213	191.53	10.00	RUB
34	9	31	480.721	132.69	20.00	RUB
35	9	44	468.280	419.07	0.00	RUB
36	10	2	364.847	189.13	10.00	RUB
37	10	21	53.836	308.89	10.00	RUB
38	10	66	180.633	338.22	10.00	RUB
39	10	32	189.631	180.43	10.00	RUB
40	11	54	489.789	322.60	10.00	RUB
41	11	16	33.509	255.38	0.00	RUB
42	11	49	54.087	346.78	0.00	RUB
43	11	13	55.265	536.51	20.00	RUB
44	11	48	280.884	204.96	10.00	RUB
45	12	50	122.253	395.79	10.00	RUB
46	12	8	91.648	356.15	0.00	RUB
47	12	42	123.590	518.24	0.00	RUB
48	13	23	405.787	365.19	0.00	RUB
49	13	61	305.291	361.02	0.00	RUB
50	13	7	43.017	384.50	0.00	RUB
51	13	32	141.832	254.21	0.00	RUB
52	14	20	14.286	395.19	0.00	RUB
53	14	9	385.511	443.02	20.00	RUB
54	14	60	408.520	94.42	20.00	RUB
55	14	50	257.210	323.07	0.00	RUB
56	14	47	202.965	368.00	0.00	RUB
57	15	52	232.923	241.66	20.00	RUB
58	15	7	63.839	255.41	0.00	RUB
59	15	12	89.423	497.48	20.00	RUB
60	15	50	379.712	177.45	0.00	RUB
61	15	58	491.600	501.69	0.00	RUB
62	16	51	472.210	370.31	10.00	RUB
63	16	62	395.329	350.71	20.00	RUB
64	16	42	361.994	291.81	20.00	RUB
65	16	70	208.547	401.46	10.00	RUB
66	16	59	266.501	370.68	10.00	RUB
67	17	57	422.554	479.68	0.00	RUB
68	17	29	306.154	555.94	10.00	RUB
69	17	61	197.460	233.62	10.00	RUB
70	18	53	322.842	421.15	0.00	RUB
71	18	55	230.471	436.24	20.00	RUB
72	18	20	397.357	331.36	20.00	RUB
73	19	59	248.520	327.31	10.00	RUB
74	19	46	359.316	366.21	10.00	RUB
75	19	14	442.368	556.22	10.00	RUB
76	20	4	237.894	266.87	10.00	RUB
77	20	21	444.653	276.96	10.00	RUB
78	20	61	48.281	313.94	10.00	RUB
79	20	24	183.839	489.70	20.00	RUB
80	20	28	280.287	335.56	0.00	RUB
81	21	31	358.441	125.73	10.00	RUB
82	21	61	39.345	197.95	20.00	RUB
83	21	65	117.160	328.76	20.00	RUB
84	21	16	191.921	230.44	10.00	RUB
85	21	18	207.672	294.12	20.00	RUB
86	22	5	70.416	185.90	10.00	RUB
87	22	12	381.661	502.10	10.00	RUB
88	22	1	384.183	178.51	0.00	RUB
89	23	21	312.268	186.60	10.00	RUB
90	23	30	203.256	129.49	0.00	RUB
91	23	12	31.567	364.95	0.00	RUB
92	23	34	477.244	300.07	0.00	RUB
93	23	65	215.004	191.16	0.00	RUB
94	24	65	140.099	191.16	10.00	RUB
95	24	70	395.636	283.69	0.00	RUB
96	24	44	239.470	329.27	0.00	RUB
97	25	13	278.483	336.79	20.00	RUB
98	25	12	171.994	494.55	20.00	RUB
99	25	3	170.001	197.38	0.00	RUB
100	25	18	337.198	188.71	20.00	RUB
101	25	41	298.992	404.40	20.00	RUB
102	26	34	157.279	241.15	20.00	RUB
103	26	36	495.743	352.96	0.00	RUB
104	26	69	476.529	421.89	10.00	RUB
105	26	56	402.820	465.11	10.00	RUB
106	27	14	463.765	420.12	20.00	RUB
107	27	41	275.683	508.78	10.00	RUB
108	27	12	212.196	364.95	10.00	RUB
109	28	30	42.497	109.86	10.00	RUB
110	28	45	463.801	262.09	0.00	RUB
111	28	37	223.455	253.26	10.00	RUB
112	29	4	440.762	266.87	10.00	RUB
113	29	21	464.639	276.96	10.00	RUB
114	29	62	298.327	244.50	10.00	RUB
115	29	68	220.752	473.61	0.00	RUB
116	30	43	50.226	416.53	20.00	RUB
117	30	21	14.160	320.27	20.00	RUB
118	30	18	219.986	232.23	0.00	RUB
119	30	37	467.843	441.71	20.00	RUB
120	30	35	327.505	239.90	0.00	RUB
121	31	49	52.885	190.85	0.00	RUB
122	31	70	447.511	302.18	10.00	RUB
123	31	52	58.986	241.66	20.00	RUB
124	31	7	50.349	255.41	0.00	RUB
125	32	23	311.563	350.76	20.00	RUB
126	32	1	45.125	122.28	20.00	RUB
127	32	70	149.378	383.77	0.00	RUB
128	32	22	236.545	260.04	10.00	RUB
129	33	59	427.557	327.31	20.00	RUB
130	33	41	89.775	312.76	10.00	RUB
131	33	70	407.033	326.19	10.00	RUB
132	33	56	476.335	339.61	10.00	RUB
133	33	58	153.890	339.20	20.00	RUB
134	34	14	28.031	506.83	0.00	RUB
135	34	6	297.965	390.02	0.00	RUB
136	34	18	82.363	290.96	10.00	RUB
137	35	3	223.314	380.46	0.00	RUB
138	35	62	465.638	205.40	20.00	RUB
139	35	14	471.864	420.12	20.00	RUB
140	35	66	367.396	371.26	10.00	RUB
141	36	65	25.376	395.00	10.00	RUB
142	36	52	200.884	241.66	10.00	RUB
143	36	16	237.117	247.38	10.00	RUB
144	36	57	394.025	345.46	10.00	RUB
145	37	70	248.578	283.69	10.00	RUB
146	37	27	82.602	428.31	0.00	RUB
147	37	30	127.611	129.49	20.00	RUB
148	37	43	252.725	329.26	10.00	RUB
149	37	65	454.580	191.16	20.00	RUB
150	38	18	61.253	290.96	20.00	RUB
151	38	61	108.839	313.94	0.00	RUB
152	38	56	172.934	522.85	20.00	RUB
153	38	40	430.309	283.76	20.00	RUB
154	39	29	402.137	555.94	10.00	RUB
155	39	2	482.430	230.46	20.00	RUB
156	39	50	278.090	395.79	20.00	RUB
157	39	30	498.427	179.11	10.00	RUB
158	39	40	31.812	503.68	10.00	RUB
159	40	68	457.912	473.61	20.00	RUB
160	40	43	290.230	557.13	10.00	RUB
161	40	14	419.053	506.83	0.00	RUB
162	41	61	140.029	206.51	20.00	RUB
163	41	40	122.318	376.06	0.00	RUB
164	41	39	476.484	458.60	10.00	RUB
165	42	16	409.068	237.90	20.00	RUB
166	42	67	470.559	306.46	0.00	RUB
167	42	54	255.858	500.56	20.00	RUB
168	43	31	125.068	306.67	10.00	RUB
169	43	40	284.770	331.51	10.00	RUB
170	43	56	96.846	480.68	20.00	RUB
171	43	48	108.126	348.67	20.00	RUB
172	43	59	220.455	364.60	10.00	RUB
173	44	61	339.187	360.92	10.00	RUB
174	44	12	89.312	497.48	10.00	RUB
175	44	44	110.006	367.22	0.00	RUB
176	44	49	187.101	190.85	10.00	RUB
177	44	17	161.651	125.92	20.00	RUB
178	45	25	482.920	314.86	0.00	RUB
179	45	39	384.801	261.45	10.00	RUB
180	45	50	69.505	282.77	20.00	RUB
181	46	61	183.088	233.62	10.00	RUB
182	46	10	405.235	516.04	20.00	RUB
183	46	57	473.021	479.68	20.00	RUB
184	47	3	272.057	394.69	10.00	RUB
185	47	56	495.646	465.11	20.00	RUB
186	47	12	182.177	296.69	10.00	RUB
187	47	37	428.218	288.12	10.00	RUB
188	48	60	351.704	285.21	20.00	RUB
189	48	39	121.108	263.14	20.00	RUB
190	48	30	467.560	179.11	10.00	RUB
191	48	43	283.154	397.01	10.00	RUB
192	48	40	199.765	503.68	20.00	RUB
193	49	46	213.612	93.68	0.00	RUB
194	49	69	485.096	457.17	10.00	RUB
195	49	4	51.760	322.94	0.00	RUB
196	49	28	301.378	495.70	20.00	RUB
197	49	60	385.401	132.63	20.00	RUB
198	50	5	395.212	274.40	20.00	RUB
199	50	22	165.363	261.37	10.00	RUB
200	50	36	273.170	276.60	10.00	RUB
201	50	53	153.791	422.67	0.00	RUB
202	50	57	36.907	345.46	10.00	RUB
203	51	56	487.813	286.21	10.00	RUB
204	51	31	46.805	199.94	10.00	RUB
205	51	51	371.928	232.92	10.00	RUB
206	51	68	21.139	331.18	10.00	RUB
207	52	16	56.627	255.38	10.00	RUB
208	52	49	386.319	346.78	10.00	RUB
209	52	62	445.320	193.39	0.00	RUB
210	52	65	292.442	412.59	20.00	RUB
211	52	46	402.182	95.99	0.00	RUB
212	53	21	454.097	367.57	20.00	RUB
213	53	61	155.798	206.51	10.00	RUB
214	53	62	378.628	342.32	0.00	RUB
215	54	24	62.346	489.70	10.00	RUB
216	54	6	30.837	390.02	10.00	RUB
217	54	13	208.934	501.58	0.00	RUB
218	54	4	139.498	266.87	20.00	RUB
219	54	40	470.432	283.76	0.00	RUB
220	55	20	92.991	279.32	20.00	RUB
221	55	40	259.745	250.53	10.00	RUB
222	55	48	12.590	204.99	20.00	RUB
223	55	26	438.786	455.78	20.00	RUB
224	55	36	399.384	243.92	0.00	RUB
225	56	66	76.743	265.62	20.00	RUB
226	56	63	211.656	169.58	20.00	RUB
227	56	22	150.955	467.95	0.00	RUB
228	57	27	103.671	552.88	10.00	RUB
229	57	43	348.737	335.24	20.00	RUB
230	57	49	100.335	190.07	20.00	RUB
231	58	45	203.412	238.46	0.00	RUB
232	58	60	145.509	276.31	0.00	RUB
233	58	65	291.542	265.90	20.00	RUB
234	59	56	80.748	286.21	20.00	RUB
235	59	25	327.502	370.02	0.00	RUB
236	59	40	435.713	277.66	10.00	RUB
237	60	61	183.057	285.12	20.00	RUB
238	60	58	430.476	508.07	20.00	RUB
239	60	66	128.957	196.58	10.00	RUB
240	60	37	475.080	244.14	10.00	RUB
241	60	50	33.977	376.79	0.00	RUB
242	61	48	151.646	401.49	10.00	RUB
243	61	14	64.104	583.34	10.00	RUB
244	61	26	78.942	354.68	10.00	RUB
245	62	13	325.683	501.58	10.00	RUB
246	62	28	108.453	335.56	10.00	RUB
247	62	14	421.339	506.83	10.00	RUB
248	62	61	142.123	313.94	10.00	RUB
249	62	21	118.077	276.96	0.00	RUB
250	63	22	191.709	324.88	10.00	RUB
251	63	9	348.055	443.02	10.00	RUB
252	63	48	432.381	348.67	20.00	RUB
253	63	6	401.736	361.00	20.00	RUB
254	63	1	207.820	157.86	0.00	RUB
255	64	41	461.695	303.12	20.00	RUB
256	64	7	253.548	433.63	20.00	RUB
257	64	21	11.652	320.27	0.00	RUB
258	64	49	170.415	135.92	20.00	RUB
259	64	62	387.146	161.03	10.00	RUB
260	65	47	351.053	270.91	10.00	RUB
261	65	19	297.234	313.97	20.00	RUB
262	65	20	206.957	331.36	20.00	RUB
263	65	55	286.956	436.24	10.00	RUB
264	65	36	57.084	284.70	10.00	RUB
265	66	39	173.380	480.93	0.00	RUB
266	66	70	221.103	280.17	20.00	RUB
267	66	2	18.051	234.80	0.00	RUB
268	66	41	60.564	303.12	0.00	RUB
269	67	13	118.159	501.58	0.00	RUB
270	67	4	117.051	266.87	0.00	RUB
271	67	11	27.942	418.26	0.00	RUB
272	67	14	459.920	506.83	10.00	RUB
273	67	6	68.936	390.02	10.00	RUB
274	68	23	375.745	350.76	0.00	RUB
275	68	15	31.644	158.30	20.00	RUB
276	68	1	145.770	122.28	0.00	RUB
277	68	30	332.076	78.93	20.00	RUB
278	68	40	367.046	392.50	10.00	RUB
279	69	62	351.226	244.50	10.00	RUB
280	69	28	379.130	335.56	20.00	RUB
281	69	5	351.010	275.16	0.00	RUB
282	69	40	372.061	283.76	20.00	RUB
283	69	43	290.135	557.13	20.00	RUB
284	70	50	154.550	325.40	0.00	RUB
285	70	11	334.214	262.57	0.00	RUB
286	70	55	313.137	350.19	0.00	RUB
287	71	21	242.128	304.98	0.00	RUB
288	71	39	93.974	493.10	0.00	RUB
289	71	23	271.185	296.50	20.00	RUB
290	71	51	423.025	385.64	10.00	RUB
291	72	65	408.174	328.76	10.00	RUB
292	72	20	359.712	274.70	20.00	RUB
293	72	25	281.949	314.86	20.00	RUB
294	72	41	270.124	388.49	20.00	RUB
295	73	14	309.365	583.34	10.00	RUB
296	73	32	146.796	180.43	10.00	RUB
297	73	24	307.078	351.61	20.00	RUB
298	74	70	72.122	383.77	0.00	RUB
299	74	23	122.572	350.76	20.00	RUB
300	74	51	271.436	352.15	0.00	RUB
301	75	10	439.142	402.33	0.00	RUB
302	75	15	127.837	174.11	0.00	RUB
303	75	54	259.124	500.56	20.00	RUB
304	76	41	415.774	303.12	0.00	RUB
305	76	37	219.583	441.71	0.00	RUB
306	76	51	78.459	374.70	0.00	RUB
307	76	43	352.400	416.53	0.00	RUB
308	76	1	263.798	318.95	20.00	RUB
309	77	54	496.448	314.12	10.00	RUB
310	77	31	24.680	125.73	0.00	RUB
311	77	11	392.223	495.57	20.00	RUB
312	77	50	348.533	282.77	20.00	RUB
313	78	6	477.013	397.42	0.00	RUB
314	78	39	225.100	261.45	20.00	RUB
315	78	41	327.298	388.49	0.00	RUB
316	78	51	395.858	260.77	20.00	RUB
317	78	34	484.635	367.00	0.00	RUB
318	79	36	277.430	391.91	0.00	RUB
319	79	54	304.171	497.41	20.00	RUB
320	79	64	274.607	283.03	10.00	RUB
321	79	32	250.658	264.03	0.00	RUB
322	79	37	83.783	221.90	10.00	RUB
323	80	54	11.370	373.65	10.00	RUB
324	80	40	444.038	250.53	10.00	RUB
325	80	27	179.223	293.98	10.00	RUB
326	80	56	53.457	433.11	20.00	RUB
327	81	50	142.011	395.79	0.00	RUB
328	81	36	495.427	446.43	10.00	RUB
329	81	10	334.049	516.04	10.00	RUB
330	82	48	127.791	204.96	20.00	RUB
331	82	25	429.453	278.24	20.00	RUB
332	82	49	235.412	346.78	10.00	RUB
333	82	63	395.396	336.71	0.00	RUB
334	82	13	388.460	536.51	20.00	RUB
335	83	35	419.100	325.67	0.00	RUB
336	83	64	446.200	240.71	20.00	RUB
337	83	30	371.868	129.49	10.00	RUB
338	84	59	361.909	327.31	0.00	RUB
339	84	5	268.877	380.77	0.00	RUB
340	84	6	205.857	430.14	0.00	RUB
341	84	58	85.882	339.20	0.00	RUB
342	85	30	338.053	137.68	20.00	RUB
343	85	49	160.673	190.07	10.00	RUB
344	85	67	87.711	285.84	0.00	RUB
345	86	22	480.779	324.88	10.00	RUB
346	86	59	50.262	364.60	0.00	RUB
347	86	31	412.798	306.67	10.00	RUB
348	86	48	473.272	348.67	20.00	RUB
349	86	9	201.042	443.02	10.00	RUB
350	87	1	153.978	157.86	20.00	RUB
351	87	5	21.399	299.19	0.00	RUB
352	87	20	240.218	395.19	10.00	RUB
353	88	50	282.484	325.51	20.00	RUB
354	88	9	264.186	343.50	10.00	RUB
355	88	53	172.460	403.34	0.00	RUB
356	88	6	238.185	179.39	20.00	RUB
357	88	2	40.150	212.06	20.00	RUB
358	89	28	183.981	475.65	20.00	RUB
359	89	14	136.281	386.80	10.00	RUB
360	89	32	442.905	347.63	0.00	RUB
361	89	41	203.413	491.39	0.00	RUB
362	90	27	128.137	388.66	0.00	RUB
363	90	19	319.128	329.63	0.00	RUB
364	90	25	251.365	387.25	0.00	RUB
365	90	18	208.447	188.71	10.00	RUB
366	90	41	499.727	404.40	0.00	RUB
367	91	32	56.701	347.63	20.00	RUB
368	91	48	430.605	193.08	20.00	RUB
369	91	34	362.940	351.71	20.00	RUB
370	91	25	446.994	279.61	0.00	RUB
371	91	49	456.640	289.47	0.00	RUB
372	92	21	266.696	178.82	0.00	RUB
373	92	7	129.749	398.16	10.00	RUB
374	92	48	173.587	311.73	20.00	RUB
375	92	26	354.391	266.57	10.00	RUB
376	93	2	398.120	207.42	0.00	RUB
377	93	8	444.083	316.99	0.00	RUB
378	93	7	162.502	255.79	0.00	RUB
379	93	24	406.172	338.57	20.00	RUB
380	93	25	323.251	427.24	20.00	RUB
381	94	41	22.924	404.40	0.00	RUB
382	94	12	380.695	494.55	10.00	RUB
383	94	39	105.406	276.14	20.00	RUB
384	94	33	87.262	271.14	0.00	RUB
385	95	20	350.754	274.70	20.00	RUB
386	95	39	154.035	261.45	20.00	RUB
387	95	50	272.691	282.77	20.00	RUB
388	95	42	30.998	416.92	10.00	RUB
389	96	59	67.091	364.60	0.00	RUB
390	96	10	218.993	353.50	0.00	RUB
391	96	22	470.034	324.88	0.00	RUB
392	96	48	438.666	348.67	0.00	RUB
393	97	54	85.456	497.41	10.00	RUB
394	97	62	45.613	133.01	20.00	RUB
395	97	41	114.298	383.67	0.00	RUB
396	98	2	290.300	212.06	10.00	RUB
397	98	21	316.925	367.57	10.00	RUB
398	98	4	217.992	217.07	20.00	RUB
399	98	37	325.634	430.42	0.00	RUB
400	99	41	12.430	499.14	20.00	RUB
401	99	34	492.722	217.59	0.00	RUB
402	99	15	177.879	265.47	0.00	RUB
403	100	49	193.408	379.77	20.00	RUB
404	100	10	70.827	319.74	0.00	RUB
405	100	2	336.843	246.39	10.00	RUB
406	101	21	150.025	224.26	0.00	RUB
407	101	37	468.131	288.12	20.00	RUB
408	101	58	497.728	463.85	10.00	RUB
409	101	5	295.621	319.27	0.00	RUB
410	101	4	207.399	303.69	20.00	RUB
411	102	56	265.884	480.68	0.00	RUB
412	102	40	272.280	331.51	10.00	RUB
413	102	67	293.847	444.87	20.00	RUB
414	102	23	476.448	243.95	0.00	RUB
415	102	22	81.997	324.88	20.00	RUB
416	103	53	421.201	362.10	0.00	RUB
417	103	47	154.272	300.30	0.00	RUB
418	103	42	390.372	291.81	10.00	RUB
419	103	15	208.064	233.47	20.00	RUB
420	103	34	480.708	261.85	0.00	RUB
421	104	33	234.019	146.66	10.00	RUB
422	104	7	486.448	255.79	0.00	RUB
423	104	20	324.549	192.66	0.00	RUB
424	104	13	377.146	445.52	10.00	RUB
425	105	29	77.876	555.94	20.00	RUB
426	105	61	380.936	233.62	10.00	RUB
427	105	60	415.727	285.21	10.00	RUB
428	105	41	480.410	499.14	20.00	RUB
429	105	31	67.725	129.81	0.00	RUB
430	106	41	276.805	432.82	0.00	RUB
431	106	56	222.731	433.11	10.00	RUB
432	106	59	55.814	336.52	20.00	RUB
433	107	32	406.284	254.21	0.00	RUB
434	107	36	339.286	446.48	20.00	RUB
435	107	45	442.321	262.09	0.00	RUB
436	108	52	82.986	388.03	20.00	RUB
437	108	47	400.179	279.05	10.00	RUB
438	108	36	426.384	446.48	10.00	RUB
439	109	51	222.782	347.72	10.00	RUB
440	109	58	454.596	488.54	10.00	RUB
441	109	34	40.676	154.46	0.00	RUB
442	110	31	33.479	306.67	0.00	RUB
443	110	5	193.844	299.19	0.00	RUB
444	110	32	309.913	392.05	10.00	RUB
445	111	59	96.823	370.68	20.00	RUB
446	111	19	265.686	219.21	20.00	RUB
447	111	70	475.218	401.46	20.00	RUB
448	111	31	211.456	257.08	0.00	RUB
449	111	51	34.647	370.31	10.00	RUB
450	112	46	89.533	366.21	20.00	RUB
451	112	32	472.036	150.57	0.00	RUB
452	112	70	365.750	326.19	10.00	RUB
453	112	58	338.983	339.20	20.00	RUB
454	113	49	491.171	362.48	20.00	RUB
455	113	11	352.647	449.15	20.00	RUB
456	113	60	42.729	194.03	10.00	RUB
457	113	41	21.140	383.67	20.00	RUB
458	113	66	340.570	363.91	10.00	RUB
459	114	60	465.999	132.63	0.00	RUB
460	114	46	178.923	93.68	10.00	RUB
461	114	51	63.270	352.15	20.00	RUB
462	115	70	305.434	379.71	0.00	RUB
463	115	68	196.116	328.97	0.00	RUB
464	115	23	377.665	241.03	0.00	RUB
465	116	35	446.786	325.67	20.00	RUB
466	116	34	155.397	300.07	10.00	RUB
467	116	41	300.352	508.78	20.00	RUB
468	116	65	481.007	191.16	20.00	RUB
469	117	41	144.942	491.39	0.00	RUB
470	117	62	125.889	137.64	0.00	RUB
471	117	1	344.204	228.77	0.00	RUB
472	118	61	93.456	360.92	0.00	RUB
473	118	67	46.837	445.76	0.00	RUB
474	118	49	323.141	190.85	10.00	RUB
475	118	30	448.190	167.53	20.00	RUB
476	119	11	368.302	307.82	20.00	RUB
477	119	39	224.586	276.14	10.00	RUB
478	119	12	434.626	494.55	0.00	RUB
479	119	45	438.619	181.89	20.00	RUB
480	120	9	377.633	368.15	20.00	RUB
481	120	39	21.230	276.14	20.00	RUB
482	120	29	298.420	486.24	20.00	RUB
483	120	18	321.959	188.71	10.00	RUB
484	121	57	431.450	462.00	0.00	RUB
485	121	2	188.892	189.13	0.00	RUB
486	121	26	194.297	354.68	0.00	RUB
487	121	20	51.333	293.31	0.00	RUB
488	122	37	437.991	441.71	10.00	RUB
489	122	18	17.268	232.23	10.00	RUB
490	122	50	274.147	340.88	10.00	RUB
491	123	2	170.422	246.39	10.00	RUB
492	123	51	484.769	191.62	20.00	RUB
493	123	60	477.897	276.31	20.00	RUB
494	123	47	299.410	156.38	20.00	RUB
495	123	8	141.574	325.85	10.00	RUB
496	124	6	348.979	446.92	0.00	RUB
497	124	21	334.534	419.39	20.00	RUB
498	124	20	222.244	400.17	20.00	RUB
499	125	70	318.463	278.31	20.00	RUB
500	125	40	51.167	503.68	10.00	RUB
501	125	29	231.413	555.94	20.00	RUB
502	125	31	258.692	129.81	0.00	RUB
503	125	15	350.687	265.47	10.00	RUB
504	126	8	274.242	224.21	20.00	RUB
505	126	41	101.513	383.67	20.00	RUB
506	126	49	231.483	362.48	10.00	RUB
507	126	44	392.321	363.77	20.00	RUB
508	127	54	429.942	480.91	20.00	RUB
509	127	46	458.506	242.43	20.00	RUB
510	127	41	253.641	370.30	0.00	RUB
511	127	39	420.152	493.10	0.00	RUB
512	128	10	165.944	516.04	20.00	RUB
513	128	60	10.321	285.21	20.00	RUB
514	128	15	101.256	265.47	20.00	RUB
515	129	31	497.292	156.86	10.00	RUB
516	129	20	366.567	242.21	10.00	RUB
517	129	54	345.950	497.41	20.00	RUB
518	130	43	141.478	319.26	0.00	RUB
519	130	20	467.773	331.36	10.00	RUB
520	130	19	235.736	313.97	10.00	RUB
521	130	16	106.093	191.62	10.00	RUB
522	130	1	414.970	206.42	20.00	RUB
523	131	30	434.483	243.80	0.00	RUB
524	131	46	277.629	95.99	20.00	RUB
525	131	36	467.923	191.53	10.00	RUB
526	131	14	208.402	411.51	10.00	RUB
527	131	31	49.007	132.69	20.00	RUB
528	132	26	484.628	436.70	0.00	RUB
529	132	14	207.789	411.51	20.00	RUB
530	132	45	118.610	163.63	20.00	RUB
531	133	20	284.367	279.32	10.00	RUB
532	133	23	190.923	375.01	0.00	RUB
533	133	53	476.298	444.31	10.00	RUB
534	133	15	264.278	120.05	10.00	RUB
535	134	47	160.243	270.91	20.00	RUB
536	134	36	68.789	284.70	20.00	RUB
537	134	14	447.995	435.40	20.00	RUB
538	134	51	46.510	347.72	0.00	RUB
539	134	53	457.801	421.15	20.00	RUB
540	135	46	52.145	95.99	10.00	RUB
541	135	31	112.991	132.69	10.00	RUB
542	135	36	53.222	191.53	10.00	RUB
543	135	48	232.302	204.96	10.00	RUB
544	136	11	126.693	262.57	10.00	RUB
545	136	17	229.190	369.07	10.00	RUB
546	136	33	32.438	122.66	20.00	RUB
547	136	53	140.220	431.63	0.00	RUB
548	136	23	351.592	350.76	20.00	RUB
549	137	70	474.508	401.46	0.00	RUB
550	137	51	351.079	370.31	20.00	RUB
551	137	7	241.715	428.47	0.00	RUB
552	138	50	283.324	323.07	10.00	RUB
553	138	22	207.075	324.88	0.00	RUB
554	138	40	468.235	331.51	20.00	RUB
555	139	20	146.974	249.90	0.00	RUB
556	139	43	77.727	557.13	20.00	RUB
557	139	14	175.566	506.83	10.00	RUB
558	139	11	491.745	418.26	20.00	RUB
559	139	28	147.011	335.56	20.00	RUB
560	140	65	159.225	445.02	20.00	RUB
561	140	51	456.139	385.64	0.00	RUB
562	140	61	408.085	231.72	10.00	RUB
563	140	39	492.839	493.10	0.00	RUB
564	141	1	327.197	206.42	0.00	RUB
565	141	32	45.647	220.72	0.00	RUB
566	141	14	74.253	435.40	0.00	RUB
567	141	33	401.663	154.05	10.00	RUB
568	142	53	320.173	431.63	20.00	RUB
569	142	11	371.696	262.57	20.00	RUB
570	142	28	59.181	495.70	10.00	RUB
571	143	35	230.012	338.89	20.00	RUB
572	143	39	396.526	263.14	20.00	RUB
573	143	36	475.234	446.43	20.00	RUB
574	143	41	212.507	499.14	20.00	RUB
575	143	50	13.119	395.79	10.00	RUB
576	144	12	408.648	502.10	20.00	RUB
577	144	44	276.249	363.77	0.00	RUB
578	144	5	371.153	185.90	0.00	RUB
579	145	27	38.902	428.31	20.00	RUB
580	145	21	13.976	186.60	0.00	RUB
581	145	17	264.599	229.20	10.00	RUB
582	145	38	138.571	239.06	0.00	RUB
583	145	65	109.939	191.16	20.00	RUB
584	146	41	335.927	383.67	0.00	RUB
585	146	12	395.592	502.10	10.00	RUB
586	146	53	245.239	372.06	10.00	RUB
587	146	37	137.567	221.90	10.00	RUB
588	147	57	260.355	345.46	20.00	RUB
589	147	22	373.532	261.37	10.00	RUB
590	147	66	450.829	451.69	20.00	RUB
591	147	53	128.134	422.67	0.00	RUB
592	147	16	202.323	247.38	20.00	RUB
593	148	40	107.177	277.66	20.00	RUB
594	148	55	366.869	509.53	20.00	RUB
595	148	25	331.144	370.02	0.00	RUB
596	149	34	369.075	347.00	10.00	RUB
597	149	26	332.051	455.78	20.00	RUB
598	149	1	46.928	210.02	10.00	RUB
599	149	16	125.773	267.47	20.00	RUB
600	149	20	80.498	279.32	0.00	RUB
601	150	69	214.759	500.81	10.00	RUB
602	150	6	365.145	430.14	10.00	RUB
603	150	30	371.186	195.25	10.00	RUB
604	150	70	117.244	326.19	10.00	RUB
605	150	14	152.023	556.22	0.00	RUB
606	151	53	11.317	403.34	20.00	RUB
607	151	4	95.422	217.07	0.00	RUB
608	151	47	89.424	316.06	10.00	RUB
609	151	2	130.780	212.06	20.00	RUB
610	152	5	74.027	299.19	0.00	RUB
611	152	9	408.037	443.02	0.00	RUB
612	152	22	76.011	324.88	10.00	RUB
613	152	50	204.595	323.07	20.00	RUB
614	153	32	249.583	312.87	20.00	RUB
615	153	26	443.564	459.63	20.00	RUB
616	153	15	256.696	312.43	20.00	RUB
617	154	20	322.339	242.21	10.00	RUB
618	154	31	462.908	156.86	0.00	RUB
619	154	12	108.978	502.10	10.00	RUB
620	154	32	489.215	264.03	0.00	RUB
621	154	18	462.047	291.78	20.00	RUB
622	155	15	437.283	85.36	20.00	RUB
623	155	61	450.284	361.02	0.00	RUB
624	155	41	21.192	388.98	0.00	RUB
625	156	23	244.205	243.95	20.00	RUB
626	156	56	435.250	480.68	20.00	RUB
627	156	31	104.497	306.67	20.00	RUB
628	157	53	254.934	421.15	20.00	RUB
629	157	58	484.033	488.54	0.00	RUB
630	157	33	204.932	154.05	20.00	RUB
631	158	19	378.803	254.22	0.00	RUB
632	158	21	36.217	304.98	20.00	RUB
633	158	6	387.975	228.34	20.00	RUB
634	159	25	228.781	387.25	20.00	RUB
635	159	19	178.040	329.63	20.00	RUB
636	159	48	463.510	335.00	10.00	RUB
637	160	11	16.570	262.57	0.00	RUB
638	160	17	233.118	369.07	20.00	RUB
639	160	38	407.517	241.86	20.00	RUB
640	160	60	60.778	132.63	10.00	RUB
641	160	40	96.307	392.50	0.00	RUB
642	161	16	335.813	191.62	0.00	RUB
643	161	51	83.563	347.72	10.00	RUB
644	161	33	194.033	154.05	0.00	RUB
645	162	20	360.680	221.38	20.00	RUB
646	162	67	302.139	285.84	0.00	RUB
647	162	52	391.817	332.89	20.00	RUB
648	162	26	253.313	433.78	0.00	RUB
649	162	25	390.418	370.02	0.00	RUB
650	163	48	160.403	335.00	0.00	RUB
651	163	39	113.886	276.14	0.00	RUB
652	163	13	137.085	336.79	0.00	RUB
653	163	23	445.170	438.23	0.00	RUB
654	164	70	39.551	283.69	20.00	RUB
655	164	66	313.518	371.26	10.00	RUB
656	164	14	362.232	420.12	0.00	RUB
657	164	9	221.299	433.54	20.00	RUB
658	164	30	20.966	129.49	0.00	RUB
659	165	3	319.550	380.46	0.00	RUB
660	165	30	118.700	129.49	20.00	RUB
661	165	70	434.563	283.69	0.00	RUB
662	165	24	132.352	240.30	0.00	RUB
663	166	59	447.591	347.53	10.00	RUB
664	166	20	315.224	192.66	10.00	RUB
665	166	1	226.978	335.14	20.00	RUB
666	166	24	391.406	338.57	10.00	RUB
667	166	70	192.265	271.12	10.00	RUB
668	167	5	236.255	199.37	0.00	RUB
669	167	49	64.175	135.92	20.00	RUB
670	167	51	39.105	374.70	0.00	RUB
671	168	52	230.170	307.75	10.00	RUB
672	168	1	384.088	228.77	0.00	RUB
673	168	8	249.602	342.78	10.00	RUB
674	168	28	262.977	475.65	20.00	RUB
675	168	62	16.433	137.64	0.00	RUB
676	169	51	237.093	239.81	0.00	RUB
677	169	58	259.098	537.62	10.00	RUB
678	169	54	231.952	378.86	0.00	RUB
679	170	8	477.405	442.05	10.00	RUB
680	170	11	157.116	326.53	10.00	RUB
681	170	26	95.741	354.68	20.00	RUB
682	171	34	354.670	154.46	20.00	RUB
683	171	58	134.343	488.54	0.00	RUB
684	171	1	110.235	206.42	20.00	RUB
685	171	2	12.679	249.42	0.00	RUB
686	171	55	499.882	436.24	0.00	RUB
687	172	63	296.722	336.71	20.00	RUB
688	172	21	127.868	409.54	0.00	RUB
689	172	1	352.744	320.99	10.00	RUB
690	172	65	19.300	412.59	0.00	RUB
691	173	66	43.228	196.58	0.00	RUB
692	173	38	34.783	435.38	20.00	RUB
693	173	44	247.970	465.59	10.00	RUB
694	173	29	70.918	429.97	10.00	RUB
695	174	7	412.068	398.16	20.00	RUB
696	174	20	381.071	331.36	20.00	RUB
697	174	38	319.227	228.58	10.00	RUB
698	175	37	135.206	257.58	20.00	RUB
699	175	16	255.900	267.47	20.00	RUB
700	175	27	155.838	293.98	20.00	RUB
701	176	9	299.255	483.99	20.00	RUB
702	176	66	68.936	196.58	10.00	RUB
703	176	48	197.288	321.14	20.00	RUB
704	176	61	377.454	285.12	0.00	RUB
705	176	27	211.335	303.84	10.00	RUB
706	177	66	131.638	371.26	0.00	RUB
707	177	62	383.476	205.40	10.00	RUB
708	177	30	99.678	129.49	20.00	RUB
709	178	32	90.920	254.21	10.00	RUB
710	178	17	425.580	271.22	20.00	RUB
711	178	4	78.388	248.08	0.00	RUB
712	178	52	238.962	388.03	20.00	RUB
713	179	45	319.104	290.11	20.00	RUB
714	179	47	317.607	335.12	0.00	RUB
715	179	34	426.313	325.86	20.00	RUB
716	180	62	451.408	161.03	0.00	RUB
717	180	7	50.915	433.63	10.00	RUB
718	180	70	481.634	280.17	20.00	RUB
719	180	21	419.777	320.27	20.00	RUB
720	180	30	316.721	320.32	10.00	RUB
721	181	55	231.133	404.81	10.00	RUB
722	181	16	265.754	230.44	20.00	RUB
723	181	31	464.280	125.73	20.00	RUB
724	181	65	491.599	328.76	20.00	RUB
725	182	60	215.206	274.49	10.00	RUB
726	182	41	241.905	423.83	0.00	RUB
727	182	54	416.117	378.86	0.00	RUB
728	183	48	51.176	321.14	0.00	RUB
729	183	29	260.340	429.97	10.00	RUB
730	183	66	340.427	196.58	20.00	RUB
731	183	13	142.504	440.62	0.00	RUB
732	184	47	31.136	335.12	10.00	RUB
733	184	48	122.375	195.10	0.00	RUB
734	184	7	136.739	359.25	0.00	RUB
735	184	21	397.157	419.39	10.00	RUB
736	184	32	65.122	312.87	10.00	RUB
737	185	17	283.512	125.92	10.00	RUB
738	185	49	364.204	190.85	10.00	RUB
739	185	70	62.434	302.18	10.00	RUB
740	185	43	294.707	369.12	20.00	RUB
741	186	63	362.974	336.71	20.00	RUB
742	186	44	220.361	419.07	10.00	RUB
743	186	14	231.999	411.51	10.00	RUB
744	186	46	84.328	95.99	10.00	RUB
745	186	31	15.691	132.69	20.00	RUB
746	187	29	277.889	486.24	0.00	RUB
747	187	33	256.241	271.14	20.00	RUB
748	187	3	480.476	197.38	0.00	RUB
749	187	12	140.300	494.55	10.00	RUB
750	188	7	268.333	255.79	20.00	RUB
751	188	34	292.118	241.15	10.00	RUB
752	188	33	20.807	146.66	0.00	RUB
753	188	59	53.699	347.53	0.00	RUB
754	189	11	114.538	307.82	20.00	RUB
755	189	23	209.740	438.23	0.00	RUB
756	189	33	177.447	271.14	0.00	RUB
757	189	25	31.458	387.25	10.00	RUB
758	189	19	167.397	329.63	20.00	RUB
759	190	21	158.023	185.32	0.00	RUB
760	190	67	138.470	306.46	20.00	RUB
761	190	29	57.707	429.97	10.00	RUB
762	191	17	72.121	369.07	10.00	RUB
763	191	38	301.328	241.86	20.00	RUB
764	191	15	378.974	158.30	20.00	RUB
765	191	33	30.497	122.66	20.00	RUB
766	191	53	478.649	431.63	20.00	RUB
767	192	48	263.316	321.14	0.00	RUB
768	192	27	395.766	303.84	10.00	RUB
769	192	67	315.048	306.46	10.00	RUB
770	192	47	317.029	200.28	10.00	RUB
771	193	21	225.770	224.26	20.00	RUB
772	193	7	196.770	255.79	10.00	RUB
773	193	37	347.458	288.12	0.00	RUB
774	193	25	170.390	427.24	20.00	RUB
775	193	54	137.997	388.25	10.00	RUB
776	194	11	351.526	307.82	10.00	RUB
777	194	25	445.685	387.25	0.00	RUB
778	194	12	182.118	494.55	0.00	RUB
779	194	48	242.496	335.00	10.00	RUB
780	194	19	344.555	329.63	0.00	RUB
781	195	70	211.287	271.12	20.00	RUB
782	195	69	327.333	421.89	0.00	RUB
783	195	63	262.187	182.03	10.00	RUB
784	196	62	329.150	133.01	10.00	RUB
785	196	21	405.427	328.99	10.00	RUB
786	196	60	269.873	194.03	20.00	RUB
787	197	36	253.777	191.53	0.00	RUB
788	197	30	170.168	243.80	10.00	RUB
789	197	63	138.444	336.71	0.00	RUB
790	197	46	411.894	95.99	10.00	RUB
791	198	11	393.433	418.26	10.00	RUB
792	198	40	386.323	283.76	0.00	RUB
793	198	28	104.221	335.56	20.00	RUB
794	198	14	404.580	506.83	0.00	RUB
795	199	64	387.508	240.71	10.00	RUB
796	199	12	353.293	364.95	0.00	RUB
797	199	21	63.115	186.60	20.00	RUB
798	199	70	173.661	283.69	20.00	RUB
799	200	53	98.712	394.87	10.00	RUB
800	200	35	219.809	239.90	10.00	RUB
801	200	1	350.035	318.95	10.00	RUB
802	200	64	162.202	215.25	0.00	RUB
803	201	3	491.912	394.69	0.00	RUB
804	201	36	192.530	352.96	20.00	RUB
805	201	37	409.490	288.12	10.00	RUB
806	202	56	358.374	286.92	20.00	RUB
807	202	48	72.648	193.08	0.00	RUB
808	202	49	94.716	289.47	20.00	RUB
809	202	34	442.414	351.71	10.00	RUB
810	203	55	407.875	436.24	0.00	RUB
811	203	56	82.165	380.95	0.00	RUB
812	203	36	460.566	284.70	0.00	RUB
813	203	43	109.907	319.26	10.00	RUB
814	203	32	122.116	220.72	10.00	RUB
815	204	24	321.423	383.69	10.00	RUB
816	204	30	491.867	320.32	20.00	RUB
817	204	41	95.666	303.12	0.00	RUB
818	204	9	70.469	265.85	20.00	RUB
819	204	5	350.411	199.37	0.00	RUB
820	205	37	329.581	430.42	10.00	RUB
821	205	40	372.486	376.06	0.00	RUB
822	205	62	424.300	342.32	0.00	RUB
823	205	6	64.588	179.39	0.00	RUB
824	205	25	85.540	496.21	20.00	RUB
825	206	17	389.041	206.95	0.00	RUB
826	206	34	129.019	261.85	10.00	RUB
827	206	70	326.082	401.46	20.00	RUB
828	206	7	373.708	428.47	10.00	RUB
829	206	42	93.163	291.81	0.00	RUB
830	207	23	402.586	369.35	0.00	RUB
831	207	2	307.303	246.39	10.00	RUB
832	207	43	481.535	504.44	0.00	RUB
833	207	45	134.916	238.46	20.00	RUB
834	208	25	256.490	279.61	0.00	RUB
835	208	63	140.715	109.73	0.00	RUB
836	208	8	172.202	342.78	0.00	RUB
837	209	21	225.865	178.82	20.00	RUB
838	209	34	150.437	154.46	0.00	RUB
839	209	38	422.854	228.58	10.00	RUB
840	209	11	494.560	447.14	10.00	RUB
841	209	3	134.510	212.66	20.00	RUB
842	210	29	305.356	433.32	0.00	RUB
843	210	21	230.665	367.57	20.00	RUB
844	210	6	62.505	179.39	20.00	RUB
845	210	69	346.258	458.95	20.00	RUB
846	210	9	338.064	343.50	0.00	RUB
847	211	24	288.270	240.30	10.00	RUB
848	211	30	494.204	129.49	20.00	RUB
849	211	9	262.101	433.54	0.00	RUB
850	211	64	443.552	240.71	20.00	RUB
851	211	38	303.437	239.06	10.00	RUB
852	212	36	77.390	276.60	10.00	RUB
853	212	12	123.950	497.48	0.00	RUB
854	212	50	432.305	177.45	20.00	RUB
855	212	49	274.819	190.85	10.00	RUB
856	213	49	265.517	346.78	0.00	RUB
857	213	31	288.326	132.69	0.00	RUB
858	213	46	83.964	95.99	0.00	RUB
859	213	43	492.812	312.89	20.00	RUB
860	213	45	279.356	163.63	0.00	RUB
861	214	54	92.214	322.60	0.00	RUB
862	214	48	31.943	204.96	0.00	RUB
863	214	46	17.302	95.99	0.00	RUB
864	214	30	96.107	243.80	0.00	RUB
865	214	13	17.976	536.51	20.00	RUB
866	215	26	339.022	354.68	20.00	RUB
867	215	40	460.580	292.02	20.00	RUB
868	215	17	245.615	150.81	10.00	RUB
869	216	1	55.541	206.42	0.00	RUB
870	216	3	459.462	212.66	0.00	RUB
871	216	56	449.893	380.95	10.00	RUB
872	217	5	145.418	380.77	20.00	RUB
873	217	1	324.342	115.53	0.00	RUB
874	217	56	164.282	339.61	10.00	RUB
875	217	3	434.514	373.86	20.00	RUB
876	217	21	165.032	267.27	20.00	RUB
877	218	41	11.719	491.39	10.00	RUB
878	218	32	472.295	347.63	20.00	RUB
879	218	49	298.280	289.47	10.00	RUB
880	218	11	418.705	314.09	10.00	RUB
881	219	13	202.564	520.22	10.00	RUB
882	219	68	156.555	331.18	0.00	RUB
883	219	52	357.390	332.89	20.00	RUB
884	219	67	421.954	285.84	0.00	RUB
885	220	26	313.121	266.57	0.00	RUB
886	220	33	266.168	154.05	0.00	RUB
887	220	36	189.969	284.70	20.00	RUB
888	220	20	382.502	331.36	10.00	RUB
\.


--
-- TOC entry 3696 (class 0 OID 24664)
-- Dependencies: 239
-- Data for Name: purchase_requisition; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.purchase_requisition (id, farm_id, product_id, quantity, max_price_per_unit, offer_date, required_date, priority, valid_until, status, notes) FROM stdin;
1	6	54	4501	153.29	2025-07-21	2025-08-08	4	2025-09-11	completed	\N
2	11	52	1315	257.51	2025-04-15	2025-04-16	5	2025-04-30	approved	\N
3	16	67	4644	106.26	2025-06-25	2025-09-20	4	2025-10-29	draft	\N
4	21	4	3049	489.60	2025-03-15	2025-04-07	3	2025-05-06	completed	Заявка на закупку #4
5	26	59	3098	53.09	2025-08-31	2025-09-30	2	2025-10-01	cancelled	\N
6	31	17	3545	533.46	2026-02-21	2026-04-07	4	2026-05-17	approved	\N
7	36	24	3124	48.23	2024-03-21	2024-05-30	1	2024-07-05	approved	\N
8	41	18	606	479.02	2025-01-04	2025-02-15	5	2025-03-07	cancelled	Заявка на закупку #8
9	46	16	3782	\N	2025-05-13	2025-07-10	3	2025-07-15	completed	\N
10	51	18	4009	206.21	2025-09-08	2025-10-01	1	2025-10-07	approved	\N
11	56	19	2306	91.47	2025-12-30	2026-03-15	4	2026-03-23	approved	\N
12	1	23	1342	118.91	2025-05-03	2025-06-14	5	2025-07-21	draft	Заявка на закупку #12
13	6	7	2175	443.76	2025-01-10	2025-02-13	2	2025-02-19	completed	\N
14	11	45	2415	248.80	2024-02-16	2024-03-27	4	2024-04-28	draft	\N
15	16	19	1044	512.96	2025-01-10	2025-03-14	1	2025-04-19	draft	\N
16	21	7	529	393.99	2024-08-12	2024-11-10	3	2024-12-06	delivering	Заявка на закупку #16
17	26	7	3773	136.14	2024-05-19	2024-06-23	4	2024-07-04	approved	\N
18	31	42	2303	\N	2024-01-06	2024-01-13	1	2024-02-13	approved	\N
19	36	67	1507	130.11	2024-09-25	2024-11-21	2	2024-11-26	draft	\N
20	41	54	603	258.11	2025-07-09	2025-08-25	1	2025-09-24	draft	Заявка на закупку #20
21	46	45	4617	399.23	2026-02-10	2026-02-17	1	2026-02-24	approved	\N
22	51	1	1468	512.55	2024-04-21	2024-07-14	1	2024-08-03	delivering	\N
23	56	11	367	456.57	2024-08-20	2024-10-15	1	2024-11-20	draft	\N
24	1	6	278	298.93	2025-03-16	2025-05-30	3	2025-05-30	draft	Заявка на закупку #24
25	6	62	2110	547.39	2025-10-31	2025-11-11	5	2025-12-17	completed	\N
26	11	42	2410	167.12	2025-10-23	2025-11-10	5	2025-12-13	approved	\N
27	16	28	3386	\N	2024-12-18	2025-01-03	5	2025-01-19	delivering	\N
28	21	61	956	271.51	2026-01-20	2026-01-31	5	2026-03-04	cancelled	Заявка на закупку #28
29	26	61	1666	490.67	2025-07-05	2025-07-15	2	2025-07-19	draft	\N
30	31	43	381	126.09	2025-07-28	2025-10-20	5	2025-11-14	delivering	\N
31	36	24	572	302.23	2024-10-30	2024-11-26	3	2024-12-24	delivering	\N
32	41	31	2773	527.96	2024-06-04	2024-07-27	5	2024-07-30	approved	Заявка на закупку #32
33	46	35	4480	283.84	2026-02-02	2026-03-28	4	2026-04-09	approved	\N
34	51	29	2069	538.96	2025-07-08	2025-09-10	2	2025-10-15	delivering	\N
35	56	23	2614	488.11	2025-01-26	2025-04-16	3	2025-05-11	cancelled	\N
36	1	70	398	\N	2024-12-04	2025-01-15	1	2025-02-12	completed	Заявка на закупку #36
37	6	36	420	260.35	2025-04-04	2025-06-22	3	2025-07-14	draft	\N
38	11	51	1316	155.35	2024-07-06	2024-07-07	5	2024-07-09	approved	\N
39	16	43	2668	100.00	2025-05-20	2025-08-08	3	2025-08-09	completed	\N
40	21	14	1923	95.85	2024-03-26	2024-04-07	4	2024-05-09	cancelled	Заявка на закупку #40
41	26	13	4933	592.32	2024-01-20	2024-02-07	3	2024-02-25	completed	\N
42	31	57	3197	70.23	2025-12-20	2026-01-11	2	2026-02-20	draft	\N
43	36	48	1925	216.47	2024-10-01	2024-12-29	1	2025-01-09	cancelled	\N
44	41	11	1117	142.05	2024-11-17	2025-01-28	3	2025-01-31	draft	Заявка на закупку #44
45	46	54	425	\N	2024-03-21	2024-04-30	1	2024-05-21	delivering	\N
46	51	24	2852	581.30	2025-11-01	2026-01-27	5	2026-02-12	approved	\N
47	56	44	272	285.31	2024-02-14	2024-04-17	1	2024-05-15	completed	\N
48	1	17	3808	373.18	2024-02-20	2024-04-05	5	2024-04-28	completed	Заявка на закупку #48
49	6	30	548	66.96	2025-02-06	2025-03-14	5	2025-03-27	cancelled	\N
50	11	2	1675	426.42	2025-08-15	2025-08-30	1	2025-09-16	approved	\N
51	16	5	1767	279.91	2025-06-02	2025-08-19	3	2025-08-23	draft	\N
52	21	39	778	575.03	2025-10-31	2025-12-13	5	2026-01-21	completed	Заявка на закупку #52
53	26	70	2952	233.82	2026-02-07	2026-03-10	3	2026-04-15	approved	\N
54	31	6	2970	\N	2025-02-01	2025-02-10	1	2025-02-13	approved	\N
55	36	63	4674	566.72	2024-01-14	2024-04-05	1	2024-04-30	approved	\N
56	41	8	613	346.17	2026-01-04	2026-03-01	4	2026-03-17	draft	Заявка на закупку #56
57	46	31	3933	320.53	2025-12-12	2026-03-09	2	2026-03-22	completed	\N
58	51	37	2522	365.39	2025-01-18	2025-02-15	1	2025-02-15	approved	\N
59	56	59	3098	464.91	2024-06-09	2024-07-21	3	2024-07-21	delivering	\N
60	1	45	100	80.81	2024-08-05	2024-09-22	3	2024-10-08	draft	Заявка на закупку #60
61	6	51	1514	200.09	2025-04-15	2025-06-22	3	2025-07-03	approved	\N
62	11	24	4701	433.38	2024-06-02	2024-06-30	2	2024-07-19	delivering	\N
63	16	13	581	\N	2024-01-16	2024-01-20	5	2024-01-22	completed	\N
64	21	54	390	239.45	2024-01-06	2024-03-27	5	2024-05-01	cancelled	Заявка на закупку #64
65	26	66	2084	176.07	2024-06-04	2024-06-18	1	2024-07-12	approved	\N
66	31	29	1124	385.11	2024-01-07	2024-02-12	3	2024-03-12	delivering	\N
67	36	70	3705	221.04	2025-08-02	2025-08-25	1	2025-09-04	approved	\N
68	41	55	2563	133.79	2024-02-17	2024-03-17	2	2024-04-12	completed	Заявка на закупку #68
69	46	23	79	535.46	2024-05-24	2024-07-21	3	2024-08-12	approved	\N
70	51	10	2105	298.69	2025-04-19	2025-04-22	5	2025-04-30	draft	\N
71	56	57	948	90.87	2024-12-18	2025-01-07	2	2025-02-11	draft	\N
72	1	25	3544	\N	2025-12-18	2026-01-08	1	2026-02-15	completed	Заявка на закупку #72
73	6	22	563	237.44	2025-06-21	2025-07-06	5	2025-08-11	cancelled	\N
74	11	1	3008	356.70	2025-03-20	2025-04-02	1	2025-05-04	approved	\N
75	16	69	3052	226.10	2025-07-17	2025-07-27	5	2025-08-16	approved	\N
76	21	60	3863	133.79	2025-12-08	2025-12-20	4	2026-01-24	approved	Заявка на закупку #76
77	26	7	2792	553.61	2025-05-05	2025-07-12	5	2025-07-25	draft	\N
78	31	49	4996	567.86	2025-06-04	2025-08-17	4	2025-08-28	completed	\N
79	36	36	2727	578.41	2024-11-14	2024-11-20	1	2024-12-22	draft	\N
80	41	62	1341	40.68	2024-06-30	2024-09-08	2	2024-10-17	delivering	Заявка на закупку #80
81	46	17	1883	\N	2024-05-18	2024-08-10	2	2024-08-13	delivering	\N
82	51	36	2293	312.87	2024-10-12	2025-01-03	2	2025-01-11	cancelled	\N
83	56	68	908	76.98	2025-01-03	2025-03-17	5	2025-03-29	completed	\N
84	1	17	116	111.45	2025-10-11	2025-11-06	1	2025-11-23	cancelled	Заявка на закупку #84
85	6	20	1804	517.33	2024-05-13	2024-07-08	4	2024-07-30	completed	\N
86	11	17	855	176.47	2026-01-05	2026-01-29	1	2026-02-06	approved	\N
87	16	54	2758	536.35	2025-02-20	2025-03-24	4	2025-04-05	draft	\N
88	21	8	2831	440.54	2025-09-01	2025-09-30	4	2025-10-29	approved	Заявка на закупку #88
89	26	51	4798	93.73	2025-07-04	2025-08-01	2	2025-08-05	completed	\N
90	31	21	4910	\N	2024-04-01	2024-05-24	1	2024-06-23	delivering	\N
91	36	6	1313	365.94	2025-06-27	2025-07-24	1	2025-07-25	delivering	\N
92	41	70	2270	77.45	2025-01-31	2025-03-22	2	2025-03-31	approved	Заявка на закупку #92
93	46	26	980	275.09	2024-01-15	2024-03-19	1	2024-04-11	approved	\N
94	51	48	3431	511.20	2025-02-06	2025-04-05	1	2025-04-29	delivering	\N
95	56	51	1263	87.10	2025-06-08	2025-07-28	3	2025-08-20	draft	\N
96	1	37	2096	208.28	2024-02-07	2024-04-03	4	2024-04-28	cancelled	Заявка на закупку #96
97	6	40	4963	471.32	2026-01-04	2026-02-01	5	2026-02-27	approved	\N
98	11	39	4962	251.08	2025-04-09	2025-05-03	2	2025-05-13	approved	\N
99	16	56	972	\N	2024-03-31	2024-05-07	2	2024-05-23	delivering	\N
100	21	53	1135	555.00	2026-01-26	2026-03-10	5	2026-03-30	delivering	Заявка на закупку #100
101	26	20	840	420.34	2025-06-09	2025-08-29	1	2025-10-02	completed	\N
102	31	13	3348	122.61	2024-09-02	2024-10-11	1	2024-11-18	approved	\N
103	36	49	2169	163.54	2025-07-27	2025-09-08	2	2025-09-16	completed	\N
104	41	37	3785	585.50	2024-07-10	2024-08-25	1	2024-09-17	approved	Заявка на закупку #104
105	46	29	2021	186.02	2024-07-26	2024-07-30	2	2024-08-03	draft	\N
106	51	47	4343	319.72	2024-01-29	2024-03-16	3	2024-04-08	completed	\N
107	56	50	4627	418.08	2025-05-26	2025-07-07	4	2025-08-01	approved	\N
108	1	35	2758	\N	2024-07-23	2024-09-07	2	2024-10-04	approved	Заявка на закупку #108
109	6	61	2228	260.10	2026-01-15	2026-04-01	5	2026-04-25	cancelled	\N
110	11	9	3539	156.83	2024-10-14	2024-10-30	1	2024-11-10	approved	\N
111	16	48	191	339.87	2024-01-09	2024-03-22	3	2024-04-30	cancelled	\N
112	21	40	4171	89.02	2024-12-15	2025-01-07	4	2025-02-14	draft	Заявка на закупку #112
113	26	48	4726	253.52	2024-11-05	2025-01-01	2	2025-01-08	completed	\N
114	31	31	120	293.79	2024-10-09	2024-10-26	2	2024-11-18	completed	\N
115	36	16	2186	141.69	2024-05-05	2024-06-10	1	2024-06-26	completed	\N
116	41	26	2040	483.88	2024-06-10	2024-08-02	1	2024-08-21	completed	Заявка на закупку #116
117	46	40	1758	\N	2026-01-05	2026-01-12	4	2026-02-17	approved	\N
118	51	59	1861	98.38	2025-11-28	2025-11-30	1	2026-01-03	completed	\N
119	56	16	4070	566.48	2024-12-30	2025-01-20	2	2025-01-27	draft	\N
120	1	58	766	553.04	2025-07-24	2025-08-28	3	2025-09-07	delivering	Заявка на закупку #120
121	6	36	1453	282.45	2025-06-01	2025-07-29	3	2025-08-15	completed	\N
122	11	64	408	102.96	2024-07-17	2024-10-07	2	2024-10-08	completed	\N
123	16	53	2807	530.17	2024-05-31	2024-06-28	4	2024-07-27	approved	\N
124	21	56	3289	421.90	2025-02-20	2025-03-16	2	2025-04-14	draft	Заявка на закупку #124
125	26	36	517	401.60	2024-02-17	2024-05-07	3	2024-05-16	completed	\N
126	31	69	525	\N	2024-01-26	2024-03-13	3	2024-04-15	completed	\N
127	36	6	307	54.18	2024-06-04	2024-08-04	2	2024-08-12	draft	\N
128	41	24	2629	299.34	2024-07-18	2024-08-02	4	2024-08-03	approved	Заявка на закупку #128
129	46	30	4709	427.69	2024-05-06	2024-06-02	4	2024-07-05	completed	\N
130	51	12	4059	596.33	2024-02-05	2024-03-01	1	2024-03-13	approved	\N
131	56	45	2885	437.14	2025-06-14	2025-08-03	2	2025-08-04	approved	\N
132	1	24	4725	446.70	2025-05-11	2025-06-11	3	2025-06-19	completed	Заявка на закупку #132
133	6	46	1756	549.34	2024-05-31	2024-06-21	2	2024-07-31	completed	\N
134	11	57	1868	431.61	2024-07-13	2024-09-17	5	2024-10-25	draft	\N
135	16	37	2988	\N	2025-04-30	2025-07-27	2	2025-08-18	delivering	\N
136	21	30	3477	279.02	2025-06-03	2025-07-17	1	2025-08-16	approved	Заявка на закупку #136
137	26	7	4525	586.63	2024-02-16	2024-04-21	2	2024-05-15	approved	\N
138	31	16	4152	491.88	2024-07-23	2024-09-09	1	2024-09-15	draft	\N
139	36	16	1287	262.00	2024-12-28	2025-03-20	5	2025-04-03	completed	\N
140	41	24	744	579.65	2025-05-19	2025-08-12	4	2025-08-28	approved	Заявка на закупку #140
141	46	34	3307	307.95	2024-11-28	2025-01-23	3	2025-01-31	draft	\N
142	51	28	1261	119.32	2024-06-14	2024-08-18	1	2024-09-27	draft	\N
143	56	38	2088	598.40	2024-12-20	2025-03-01	5	2025-03-10	completed	\N
144	1	24	1473	\N	2026-01-21	2026-03-07	4	2026-03-10	approved	Заявка на закупку #144
145	6	44	1739	248.09	2024-02-20	2024-04-09	2	2024-05-19	completed	\N
146	11	62	2014	77.33	2025-10-25	2025-11-08	5	2025-11-11	delivering	\N
147	16	63	2269	98.54	2024-08-29	2024-08-29	4	2024-09-18	delivering	\N
148	21	40	1009	405.79	2024-11-27	2024-12-16	1	2024-12-20	delivering	Заявка на закупку #148
149	26	50	1439	264.72	2024-02-28	2024-05-03	3	2024-05-28	delivering	\N
150	31	61	3003	256.23	2024-11-17	2024-12-22	4	2024-12-24	approved	\N
151	36	33	4668	80.72	2024-06-27	2024-09-15	1	2024-10-08	draft	\N
152	41	35	599	83.40	2026-02-23	2026-05-20	3	2026-06-10	approved	Заявка на закупку #152
153	46	3	4675	\N	2025-11-17	2025-11-28	5	2025-12-22	approved	\N
154	51	28	4850	219.78	2025-07-14	2025-09-15	3	2025-09-26	draft	\N
155	56	1	1133	50.17	2024-06-21	2024-08-15	2	2024-08-15	draft	\N
156	1	64	797	116.13	2025-11-08	2025-11-19	2	2025-12-08	draft	Заявка на закупку #156
157	6	26	4692	176.44	2026-01-03	2026-02-27	5	2026-03-24	approved	\N
158	11	59	3335	60.35	2025-06-19	2025-08-18	2	2025-08-22	draft	\N
159	16	63	3973	169.54	2025-09-08	2025-10-01	2	2025-10-29	completed	\N
160	21	4	3781	577.12	2025-12-13	2026-02-24	5	2026-03-30	approved	Заявка на закупку #160
161	26	70	3453	237.35	2025-10-14	2025-11-09	5	2025-11-28	delivering	\N
162	31	12	3141	\N	2024-12-02	2025-01-10	2	2025-01-11	cancelled	\N
163	36	43	1498	170.58	2024-05-28	2024-06-02	2	2024-06-09	approved	\N
164	41	46	202	473.77	2025-05-12	2025-06-29	3	2025-07-17	approved	Заявка на закупку #164
165	46	59	2337	308.77	2025-12-29	2026-01-04	1	2026-01-12	approved	\N
166	51	60	3251	211.46	2025-07-11	2025-10-04	4	2025-11-04	approved	\N
167	56	58	4868	292.08	2024-11-12	2024-11-23	4	2024-12-05	approved	\N
168	1	45	3504	426.75	2024-02-04	2024-03-07	3	2024-03-09	draft	Заявка на закупку #168
169	6	32	2901	216.25	2024-08-13	2024-10-12	2	2024-10-14	approved	\N
170	11	50	2241	356.43	2025-09-23	2025-10-16	4	2025-11-13	draft	\N
171	16	46	1135	\N	2025-08-05	2025-11-02	4	2025-11-16	delivering	\N
172	21	7	3048	372.40	2026-01-06	2026-01-07	2	2026-01-30	approved	Заявка на закупку #172
173	26	21	4343	130.22	2025-02-27	2025-05-24	2	2025-06-20	delivering	\N
174	31	11	2582	383.89	2025-06-21	2025-09-08	5	2025-10-14	completed	\N
175	36	51	2866	556.66	2025-02-27	2025-05-22	4	2025-06-06	delivering	\N
176	41	43	2154	529.81	2024-10-26	2024-11-23	3	2024-12-09	completed	Заявка на закупку #176
177	46	40	1003	261.49	2025-09-09	2025-11-23	2	2025-12-31	delivering	\N
178	51	34	2269	363.86	2024-10-18	2024-11-15	5	2024-12-02	completed	\N
179	56	34	150	334.46	2025-11-23	2026-02-15	5	2026-02-25	draft	\N
180	1	7	4870	\N	2024-12-02	2025-02-04	4	2025-02-12	approved	Заявка на закупку #180
181	6	62	1800	593.46	2025-11-16	2026-01-23	5	2026-03-01	completed	\N
182	11	35	827	179.54	2024-04-16	2024-04-17	5	2024-05-15	draft	\N
183	16	23	953	521.84	2026-02-18	2026-02-23	4	2026-03-15	delivering	\N
184	21	67	2858	147.92	2025-10-07	2026-01-05	4	2026-01-28	delivering	Заявка на закупку #184
185	26	17	1039	60.88	2024-01-11	2024-01-16	3	2024-02-02	approved	\N
186	31	39	3135	560.18	2024-05-31	2024-08-17	2	2024-09-22	draft	\N
187	36	4	1168	542.45	2024-12-30	2025-01-12	2	2025-01-19	delivering	\N
188	41	60	2740	520.74	2024-12-30	2025-03-07	5	2025-04-01	delivering	Заявка на закупку #188
189	46	44	3871	\N	2024-01-03	2024-03-14	4	2024-03-19	cancelled	\N
190	51	28	573	186.21	2025-01-04	2025-03-15	3	2025-03-27	delivering	\N
191	56	42	3258	257.22	2024-03-27	2024-04-05	2	2024-05-09	approved	\N
192	1	7	1686	206.11	2024-07-06	2024-07-30	4	2024-08-09	cancelled	Заявка на закупку #192
193	6	18	284	520.47	2024-07-18	2024-08-26	4	2024-09-13	delivering	\N
194	11	53	1713	407.37	2024-01-20	2024-02-27	2	2024-03-26	completed	\N
195	16	54	4142	455.21	2025-11-21	2026-02-13	4	2026-03-19	completed	\N
196	21	5	4533	483.22	2025-10-28	2025-12-07	2	2026-01-12	completed	Заявка на закупку #196
197	26	49	4782	577.19	2024-06-05	2024-08-27	1	2024-09-27	approved	\N
198	31	37	537	\N	2025-04-21	2025-05-24	2	2025-05-26	delivering	\N
199	36	41	488	126.89	2025-07-03	2025-08-18	1	2025-08-28	draft	\N
200	41	36	3468	258.87	2024-07-04	2024-08-30	3	2024-09-09	delivering	Заявка на закупку #200
201	46	18	4272	596.65	2025-09-30	2025-10-29	1	2025-11-06	draft	\N
202	51	43	3297	361.23	2024-04-20	2024-05-17	2	2024-05-17	delivering	\N
203	56	29	1626	182.04	2025-04-24	2025-06-19	1	2025-06-26	completed	\N
204	1	55	3747	244.51	2024-01-11	2024-03-07	4	2024-03-16	completed	Заявка на закупку #204
205	6	45	2592	179.51	2026-02-05	2026-02-12	2	2026-03-01	completed	\N
206	11	27	469	83.82	2024-06-26	2024-07-25	5	2024-08-29	completed	\N
207	16	63	1407	\N	2024-08-28	2024-08-31	2	2024-09-17	completed	\N
208	21	17	392	111.15	2024-10-30	2024-12-27	1	2025-02-02	completed	Заявка на закупку #208
209	26	3	754	294.36	2024-06-15	2024-07-05	2	2024-07-23	draft	\N
210	31	48	489	286.03	2026-01-09	2026-03-02	3	2026-04-05	approved	\N
211	36	14	3911	399.63	2025-12-08	2026-02-09	3	2026-02-25	approved	\N
212	41	24	2170	68.03	2025-01-03	2025-03-25	5	2025-04-08	approved	Заявка на закупку #212
213	46	57	1272	481.63	2025-04-25	2025-07-14	2	2025-08-04	approved	\N
214	51	9	1483	127.11	2025-09-05	2025-09-10	5	2025-10-01	approved	\N
215	56	69	2318	439.06	2025-06-03	2025-06-21	2	2025-07-22	delivering	\N
216	1	25	3707	\N	2025-08-10	2025-08-19	2	2025-08-25	draft	Заявка на закупку #216
217	6	12	396	106.77	2025-04-13	2025-06-17	1	2025-07-02	completed	\N
218	11	54	419	414.98	2024-03-16	2024-05-13	1	2024-06-13	delivering	\N
219	16	5	1857	375.03	2024-12-26	2025-01-08	5	2025-02-16	approved	\N
220	21	46	1469	374.98	2026-01-22	2026-02-05	4	2026-02-11	draft	Заявка на закупку #220
221	26	19	4042	569.39	2026-02-05	2026-03-09	2	2026-03-23	approved	\N
222	31	24	1349	52.02	2024-10-23	2024-11-11	2	2024-11-18	approved	\N
223	36	49	2635	423.66	2024-09-23	2024-12-07	4	2024-12-19	cancelled	\N
224	41	1	4430	497.21	2024-07-26	2024-09-01	3	2024-09-20	approved	Заявка на закупку #224
225	46	3	2506	\N	2025-05-29	2025-08-03	4	2025-09-04	approved	\N
226	51	43	106	191.74	2024-01-22	2024-02-12	1	2024-03-06	approved	\N
227	56	49	4167	423.24	2024-03-14	2024-05-31	3	2024-06-12	completed	\N
228	1	58	4080	184.13	2025-12-19	2026-01-11	4	2026-02-07	approved	Заявка на закупку #228
229	6	27	1320	570.67	2025-09-04	2025-10-14	4	2025-10-28	delivering	\N
230	11	58	4089	305.53	2024-06-23	2024-07-13	3	2024-08-18	draft	\N
231	16	54	669	536.06	2024-12-23	2025-03-02	1	2025-03-11	completed	\N
232	21	18	237	406.31	2025-08-09	2025-08-29	2	2025-10-05	delivering	Заявка на закупку #232
233	26	10	2652	557.47	2026-02-26	2026-04-19	2	2026-04-21	draft	\N
234	31	41	2597	\N	2026-02-14	2026-05-10	5	2026-06-04	delivering	\N
235	36	32	3805	414.29	2024-06-06	2024-08-27	4	2024-09-07	completed	\N
236	41	38	1483	63.94	2025-01-21	2025-01-30	3	2025-02-15	approved	Заявка на закупку #236
237	46	4	4525	230.20	2024-07-18	2024-08-13	1	2024-08-27	completed	\N
238	51	6	367	192.66	2025-07-01	2025-07-29	2	2025-08-02	completed	\N
239	56	11	1458	516.28	2024-04-06	2024-04-28	1	2024-05-21	completed	\N
240	1	66	932	438.29	2025-07-05	2025-07-24	4	2025-08-27	approved	Заявка на закупку #240
241	6	64	3298	206.85	2024-01-09	2024-01-22	5	2024-01-29	approved	\N
242	11	34	4566	340.85	2025-02-02	2025-03-13	3	2025-03-21	completed	\N
243	16	1	358	\N	2025-02-01	2025-02-23	5	2025-03-27	cancelled	\N
244	21	64	3632	397.44	2025-05-06	2025-06-06	3	2025-07-09	delivering	Заявка на закупку #244
245	26	50	3300	197.79	2025-02-12	2025-03-23	1	2025-04-30	approved	\N
246	31	17	1793	205.87	2024-07-03	2024-07-17	5	2024-07-26	completed	\N
247	36	56	1196	140.27	2024-07-16	2024-08-08	1	2024-08-27	completed	\N
248	41	6	2339	131.33	2024-09-16	2024-11-23	2	2024-11-30	delivering	Заявка на закупку #248
249	46	63	4196	553.54	2025-08-04	2025-11-01	1	2025-11-06	approved	\N
250	51	58	1266	502.21	2025-02-20	2025-05-06	5	2025-06-13	cancelled	\N
251	56	36	4723	426.77	2024-04-08	2024-06-30	2	2024-08-01	completed	\N
252	1	15	4511	\N	2024-01-21	2024-01-28	2	2024-02-24	approved	Заявка на закупку #252
253	6	3	1387	435.49	2024-03-04	2024-03-04	3	2024-03-16	completed	\N
254	11	70	2767	148.30	2025-02-09	2025-03-05	1	2025-03-05	delivering	\N
255	16	6	2685	493.38	2024-10-03	2024-11-03	5	2024-11-18	approved	\N
256	21	1	2920	349.76	2024-06-14	2024-08-06	5	2024-08-16	completed	Заявка на закупку #256
257	26	8	498	292.62	2025-06-30	2025-09-27	4	2025-10-07	completed	\N
258	31	56	2009	520.36	2024-06-30	2024-09-09	2	2024-10-11	approved	\N
259	36	50	1679	160.11	2025-05-10	2025-07-01	1	2025-08-02	delivering	\N
260	41	15	4452	551.31	2026-02-07	2026-03-30	2	2026-04-24	approved	Заявка на закупку #260
261	46	32	3565	\N	2025-07-19	2025-08-27	1	2025-08-27	cancelled	\N
262	51	25	1280	411.74	2024-06-09	2024-07-04	2	2024-08-11	draft	\N
263	56	70	4172	449.41	2024-08-17	2024-09-30	2	2024-10-29	completed	\N
264	1	9	4338	307.05	2025-12-26	2026-03-25	1	2026-04-19	cancelled	Заявка на закупку #264
265	6	62	4048	255.96	2025-10-28	2025-11-02	4	2025-12-12	completed	\N
266	11	22	61	395.49	2025-07-05	2025-09-09	1	2025-09-26	delivering	\N
267	16	10	1053	301.57	2025-03-31	2025-05-22	1	2025-06-20	cancelled	\N
268	21	15	2896	583.16	2025-03-05	2025-05-18	5	2025-06-02	approved	Заявка на закупку #268
269	26	39	2876	410.02	2024-07-01	2024-09-25	1	2024-10-04	completed	\N
270	31	49	3029	\N	2025-12-13	2026-02-02	5	2026-02-26	completed	\N
271	36	46	4718	504.48	2024-05-21	2024-07-01	4	2024-07-14	delivering	\N
272	41	65	4869	356.68	2024-06-15	2024-07-28	3	2024-08-11	approved	Заявка на закупку #272
273	46	56	83	467.84	2026-01-25	2026-03-04	2	2026-04-10	completed	\N
274	51	3	3655	190.15	2025-05-01	2025-05-19	5	2025-06-14	approved	\N
275	56	39	525	468.10	2025-08-10	2025-10-28	4	2025-11-28	draft	\N
276	1	1	4542	477.83	2025-04-13	2025-05-22	4	2025-06-07	approved	Заявка на закупку #276
277	6	37	3332	194.45	2025-06-05	2025-06-29	5	2025-07-31	completed	\N
278	11	15	3376	298.74	2024-10-22	2024-12-19	5	2025-01-28	approved	\N
279	16	52	4773	\N	2025-07-03	2025-09-09	1	2025-10-03	delivering	\N
280	21	69	239	188.52	2024-11-19	2024-11-20	5	2024-11-30	completed	Заявка на закупку #280
281	26	66	3241	530.47	2024-11-30	2024-12-21	1	2024-12-23	draft	\N
282	31	18	3767	61.89	2025-02-13	2025-03-22	5	2025-04-29	cancelled	\N
283	36	3	4469	140.32	2025-07-01	2025-08-02	3	2025-09-04	draft	\N
284	41	29	14	351.52	2025-07-06	2025-09-17	5	2025-09-29	completed	Заявка на закупку #284
285	46	6	2712	310.18	2025-11-13	2026-01-21	5	2026-02-05	approved	\N
286	51	61	2155	506.71	2026-01-31	2026-02-13	5	2026-03-07	approved	\N
287	56	31	129	123.65	2025-04-02	2025-05-19	1	2025-05-22	draft	\N
288	1	6	2835	\N	2026-01-07	2026-02-25	4	2026-03-03	approved	Заявка на закупку #288
289	6	46	1675	49.68	2025-01-01	2025-02-19	3	2025-02-25	approved	\N
290	11	25	3600	588.10	2025-11-11	2025-12-18	4	2026-01-15	approved	\N
291	16	65	3940	551.83	2024-01-20	2024-04-12	1	2024-05-15	approved	\N
292	21	54	296	57.49	2025-10-08	2025-11-03	4	2025-11-26	completed	Заявка на закупку #292
293	26	16	399	169.32	2024-10-29	2024-12-06	3	2024-12-14	approved	\N
294	31	6	3232	311.01	2025-11-12	2025-12-25	4	2026-01-29	cancelled	\N
295	36	52	3075	132.31	2024-02-09	2024-03-28	2	2024-04-05	draft	\N
296	41	45	4903	284.27	2025-05-04	2025-05-21	3	2025-06-17	completed	Заявка на закупку #296
297	46	24	1491	\N	2024-05-07	2024-07-23	1	2024-08-13	delivering	\N
298	51	38	3611	484.15	2024-09-21	2024-11-23	3	2024-12-23	cancelled	\N
299	56	39	4443	201.60	2024-11-29	2025-01-15	5	2025-01-29	approved	\N
300	1	42	1408	93.98	2024-08-25	2024-11-01	2	2024-11-21	draft	Заявка на закупку #300
301	6	11	1133	200.98	2026-01-16	2026-03-01	2	2026-03-07	draft	\N
302	11	8	37	454.97	2025-02-07	2025-02-23	2	2025-02-28	cancelled	\N
303	16	9	3879	172.62	2025-10-17	2025-11-04	4	2025-12-05	draft	\N
304	21	35	4401	399.82	2026-01-19	2026-02-19	5	2026-03-13	approved	Заявка на закупку #304
305	26	42	4549	221.85	2024-07-04	2024-09-16	3	2024-09-26	completed	\N
306	31	45	1156	\N	2025-12-30	2026-02-08	1	2026-03-05	draft	\N
307	36	65	2694	313.98	2025-09-21	2025-10-01	4	2025-10-05	draft	\N
308	41	7	4344	516.94	2024-02-18	2024-04-04	3	2024-05-12	draft	Заявка на закупку #308
309	46	58	3730	212.06	2025-04-27	2025-05-26	3	2025-07-05	delivering	\N
310	51	19	2447	285.48	2025-06-25	2025-08-01	1	2025-08-30	draft	\N
311	56	39	4688	297.30	2024-09-23	2024-10-30	5	2024-11-12	draft	\N
312	1	30	4010	325.91	2024-02-01	2024-02-29	2	2024-03-07	approved	Заявка на закупку #312
313	6	54	691	529.35	2024-03-11	2024-06-07	2	2024-06-22	completed	\N
314	11	67	4932	589.03	2025-03-13	2025-03-17	5	2025-04-16	approved	\N
315	16	23	4571	\N	2024-03-10	2024-04-24	2	2024-05-24	approved	\N
316	21	26	88	199.73	2025-07-22	2025-09-15	2	2025-10-05	completed	Заявка на закупку #316
317	26	41	1497	108.45	2025-12-09	2026-01-17	4	2026-02-06	completed	\N
318	31	28	2692	448.21	2025-05-09	2025-07-13	1	2025-07-19	approved	\N
319	36	14	2928	277.92	2025-05-04	2025-05-28	3	2025-06-14	completed	\N
320	41	64	727	451.72	2025-03-29	2025-05-16	2	2025-06-22	completed	Заявка на закупку #320
321	46	9	2552	165.71	2025-02-09	2025-02-26	5	2025-04-05	approved	\N
322	51	49	3532	186.72	2024-12-15	2024-12-21	5	2025-01-27	approved	\N
323	56	4	312	102.81	2025-06-29	2025-09-13	5	2025-10-04	approved	\N
324	1	3	4362	\N	2024-08-27	2024-11-22	3	2024-12-02	draft	Заявка на закупку #324
325	6	11	4436	372.89	2025-11-10	2026-01-15	3	2026-02-02	draft	\N
326	11	38	3203	195.08	2025-06-10	2025-08-15	3	2025-09-23	completed	\N
327	16	59	4784	247.60	2024-06-23	2024-09-02	3	2024-10-03	delivering	\N
328	21	45	4269	110.79	2025-06-14	2025-06-28	5	2025-07-01	draft	Заявка на закупку #328
329	26	10	3623	457.31	2025-04-18	2025-06-25	5	2025-08-04	completed	\N
330	31	18	2461	336.14	2024-07-11	2024-09-30	5	2024-10-28	completed	\N
331	36	60	127	558.69	2026-02-07	2026-04-22	5	2026-05-11	cancelled	\N
332	41	34	2702	494.10	2024-09-29	2024-10-25	4	2024-11-25	completed	Заявка на закупку #332
333	46	21	4295	\N	2025-02-04	2025-04-11	1	2025-04-16	delivering	\N
334	51	28	253	157.56	2025-05-29	2025-07-11	4	2025-08-14	draft	\N
335	56	11	4745	177.63	2025-05-18	2025-07-25	1	2025-08-11	cancelled	\N
336	1	44	4732	46.36	2024-01-24	2024-04-12	5	2024-05-22	completed	Заявка на закупку #336
337	6	14	3657	302.09	2024-02-18	2024-04-05	3	2024-04-14	approved	\N
338	11	55	3474	367.71	2024-08-14	2024-09-30	1	2024-11-09	completed	\N
339	16	41	4143	341.54	2024-01-22	2024-03-21	1	2024-04-10	delivering	\N
340	21	51	173	457.83	2024-05-04	2024-05-19	5	2024-06-03	completed	Заявка на закупку #340
341	26	11	95	179.23	2025-06-29	2025-07-15	5	2025-08-14	delivering	\N
342	31	41	2873	\N	2024-05-04	2024-06-04	4	2024-06-06	approved	\N
343	36	68	2697	502.76	2024-04-13	2024-06-16	1	2024-06-18	delivering	\N
344	41	8	1121	207.63	2026-01-01	2026-03-01	2	2026-03-26	draft	Заявка на закупку #344
345	46	44	4359	317.87	2025-01-23	2025-03-15	2	2025-04-12	draft	\N
346	51	45	4185	443.55	2025-02-14	2025-04-27	2	2025-05-10	approved	\N
347	56	8	68	390.91	2025-07-11	2025-08-21	4	2025-08-26	approved	\N
348	1	38	3699	412.77	2024-03-29	2024-06-20	3	2024-07-14	completed	Заявка на закупку #348
349	6	31	4800	312.91	2024-07-09	2024-08-23	3	2024-08-30	draft	\N
350	11	38	2026	260.02	2025-11-26	2026-02-12	1	2026-02-20	completed	\N
351	16	31	3872	\N	2025-05-12	2025-06-04	4	2025-06-05	completed	\N
352	21	70	4880	276.22	2026-02-23	2026-05-17	4	2026-06-12	draft	Заявка на закупку #352
353	26	32	2426	139.17	2024-03-25	2024-05-12	3	2024-05-28	draft	\N
354	31	53	637	59.58	2024-04-17	2024-07-05	4	2024-07-25	approved	\N
355	36	27	2752	339.83	2025-10-12	2025-11-19	4	2025-12-05	draft	\N
356	41	48	1029	60.99	2025-11-16	2026-01-26	5	2026-01-26	draft	Заявка на закупку #356
357	46	28	77	187.87	2025-05-13	2025-07-05	2	2025-08-01	draft	\N
358	51	66	849	187.45	2024-09-22	2024-12-17	3	2025-01-15	draft	\N
359	56	30	4859	217.51	2024-08-04	2024-08-06	5	2024-09-01	approved	\N
360	1	6	3437	\N	2025-05-22	2025-07-12	3	2025-08-19	approved	Заявка на закупку #360
361	6	1	4676	137.27	2025-06-01	2025-08-08	3	2025-09-16	approved	\N
362	11	35	2630	462.46	2024-12-25	2025-01-08	2	2025-01-29	approved	\N
363	16	59	1122	513.68	2024-05-06	2024-07-03	3	2024-07-15	draft	\N
364	21	55	4538	577.52	2024-05-02	2024-05-15	4	2024-06-01	completed	Заявка на закупку #364
365	26	5	1673	210.84	2024-08-01	2024-10-15	5	2024-11-17	cancelled	\N
366	31	65	4052	155.11	2025-04-26	2025-06-21	3	2025-07-23	delivering	\N
367	36	30	1766	266.63	2026-02-18	2026-04-19	4	2026-04-19	completed	\N
368	41	13	1915	441.78	2025-09-11	2025-09-12	5	2025-09-19	cancelled	Заявка на закупку #368
369	46	45	1591	\N	2025-06-08	2025-08-28	4	2025-08-29	delivering	\N
370	51	17	3253	114.61	2024-09-27	2024-10-18	1	2024-11-03	draft	\N
371	56	22	1915	540.89	2025-09-17	2025-09-21	3	2025-10-19	approved	\N
372	1	1	2209	464.46	2024-03-08	2024-03-10	1	2024-04-10	completed	Заявка на закупку #372
373	6	35	826	356.41	2024-06-12	2024-07-16	3	2024-07-31	completed	\N
374	11	10	497	387.98	2026-01-01	2026-03-20	3	2026-03-26	cancelled	\N
375	16	19	1191	460.37	2025-07-11	2025-09-08	1	2025-10-15	completed	\N
376	21	61	1247	456.87	2024-09-18	2024-11-30	4	2024-12-13	delivering	Заявка на закупку #376
377	26	48	650	595.44	2024-11-01	2024-11-19	2	2024-12-07	draft	\N
378	31	28	1422	\N	2025-03-05	2025-03-17	1	2025-04-20	completed	\N
379	36	7	300	147.75	2025-01-26	2025-04-03	4	2025-04-09	completed	\N
380	41	28	421	305.94	2024-06-01	2024-08-09	4	2024-09-06	completed	Заявка на закупку #380
381	46	22	469	563.18	2024-06-04	2024-06-08	1	2024-07-16	delivering	\N
382	51	31	3212	540.23	2025-02-28	2025-03-17	1	2025-04-22	approved	\N
383	56	53	4464	366.70	2024-01-11	2024-02-23	4	2024-03-24	approved	\N
384	1	4	500	93.64	2024-09-06	2024-09-23	3	2024-10-29	cancelled	Заявка на закупку #384
385	6	4	3789	161.07	2025-04-17	2025-04-28	2	2025-04-30	completed	\N
386	11	22	1322	92.37	2024-07-28	2024-10-05	4	2024-11-10	draft	\N
387	16	66	3881	\N	2024-03-18	2024-06-09	4	2024-07-10	draft	\N
388	21	16	214	490.39	2024-09-16	2024-10-26	3	2024-11-21	completed	Заявка на закупку #388
389	26	64	918	358.91	2025-07-14	2025-07-22	4	2025-07-22	approved	\N
390	31	40	2016	267.95	2024-05-28	2024-06-25	3	2024-07-13	completed	\N
391	36	17	1514	241.91	2024-04-05	2024-06-26	4	2024-06-26	draft	\N
392	41	24	1768	550.37	2024-10-03	2024-10-13	2	2024-10-14	approved	Заявка на закупку #392
393	46	40	3664	486.79	2025-04-30	2025-05-23	4	2025-07-01	draft	\N
394	51	24	3456	399.27	2024-12-27	2025-02-05	1	2025-02-19	cancelled	\N
395	56	61	3360	84.19	2025-04-21	2025-07-07	3	2025-08-11	draft	\N
396	1	18	2224	\N	2024-12-19	2025-03-17	3	2025-04-23	approved	Заявка на закупку #396
397	6	15	3141	60.75	2024-04-18	2024-05-01	5	2024-05-08	delivering	\N
398	11	69	4773	556.42	2024-04-03	2024-04-24	4	2024-05-28	completed	\N
399	16	37	4979	489.31	2025-02-27	2025-03-31	4	2025-05-05	approved	\N
400	21	66	3461	111.06	2025-04-19	2025-06-06	1	2025-06-06	approved	Заявка на закупку #400
401	26	35	2565	295.46	2024-01-25	2024-03-30	2	2024-04-27	completed	\N
402	31	12	4634	101.34	2024-02-03	2024-04-05	3	2024-05-14	completed	\N
403	36	17	712	284.82	2024-02-14	2024-02-20	4	2024-03-27	completed	\N
404	41	68	1661	260.10	2026-01-17	2026-04-07	2	2026-04-19	completed	Заявка на закупку #404
405	46	30	2292	\N	2025-07-07	2025-08-10	5	2025-09-06	delivering	\N
406	51	6	4140	128.32	2024-08-17	2024-10-29	5	2024-11-24	delivering	\N
407	56	56	1545	122.90	2024-01-10	2024-01-31	1	2024-02-10	delivering	\N
408	1	63	1225	525.08	2025-01-02	2025-02-13	1	2025-02-26	completed	Заявка на закупку #408
409	6	1	509	51.55	2025-07-15	2025-09-19	4	2025-10-04	completed	\N
410	11	52	3724	173.99	2024-04-23	2024-07-10	1	2024-07-10	approved	\N
411	16	12	1003	181.19	2025-02-19	2025-02-19	1	2025-03-28	cancelled	\N
412	21	12	3159	357.92	2026-01-29	2026-04-19	2	2026-05-04	draft	Заявка на закупку #412
413	26	64	1661	562.94	2026-01-29	2026-04-13	5	2026-05-19	completed	\N
414	31	6	3190	\N	2024-01-10	2024-03-19	3	2024-04-07	approved	\N
415	36	11	4496	278.14	2024-09-21	2024-11-09	2	2024-11-11	draft	\N
416	41	66	2281	423.21	2024-09-12	2024-11-06	1	2024-11-12	completed	Заявка на закупку #416
417	46	32	3660	86.19	2024-04-08	2024-06-13	1	2024-07-03	draft	\N
418	51	48	2132	224.65	2024-05-24	2024-08-05	3	2024-08-14	cancelled	\N
419	56	25	2138	490.76	2025-11-11	2025-11-12	1	2025-12-05	delivering	\N
420	1	31	2285	271.21	2024-01-31	2024-03-28	3	2024-04-21	cancelled	Заявка на закупку #420
421	6	33	2331	365.92	2025-12-21	2026-01-24	4	2026-02-20	approved	\N
422	11	7	792	72.79	2024-05-21	2024-06-24	1	2024-07-02	completed	\N
423	16	70	760	\N	2025-12-06	2026-02-08	2	2026-02-16	completed	\N
424	21	66	3669	374.02	2026-02-24	2026-04-01	1	2026-04-10	cancelled	Заявка на закупку #424
425	26	51	1162	58.86	2024-12-20	2025-02-22	1	2025-03-25	delivering	\N
426	31	42	1123	301.84	2025-01-02	2025-03-24	2	2025-04-19	delivering	\N
427	36	29	2831	597.62	2024-01-18	2024-01-19	3	2024-01-24	draft	\N
428	41	13	4597	572.23	2025-07-18	2025-10-15	3	2025-10-21	approved	Заявка на закупку #428
429	46	54	578	391.87	2025-10-08	2025-11-18	3	2025-12-24	draft	\N
430	51	51	947	534.69	2024-03-03	2024-05-01	5	2024-05-12	delivering	\N
431	56	34	3503	268.70	2024-02-25	2024-03-06	2	2024-04-10	delivering	\N
432	1	19	3764	\N	2024-03-29	2024-04-01	1	2024-04-12	delivering	Заявка на закупку #432
433	6	17	4661	451.60	2025-10-04	2025-11-27	1	2025-12-19	draft	\N
434	11	4	2176	570.04	2026-01-02	2026-02-18	1	2026-03-21	completed	\N
435	16	56	2673	165.66	2026-03-01	2026-04-03	3	2026-04-27	completed	\N
436	21	40	1465	540.55	2025-03-26	2025-05-18	2	2025-06-27	delivering	Заявка на закупку #436
437	26	4	887	388.34	2024-12-06	2025-01-05	1	2025-01-28	approved	\N
438	31	27	2542	315.42	2024-06-01	2024-07-04	2	2024-07-26	approved	\N
439	36	54	690	315.05	2025-02-23	2025-05-02	2	2025-05-12	cancelled	\N
440	41	13	1043	502.59	2025-11-15	2025-12-08	4	2026-01-05	cancelled	Заявка на закупку #440
441	46	66	3221	\N	2025-03-04	2025-06-02	4	2025-06-24	cancelled	\N
442	51	52	1260	519.32	2024-10-09	2024-10-18	5	2024-11-15	completed	\N
443	56	53	4481	287.52	2024-10-03	2024-12-21	3	2025-01-20	approved	\N
444	1	11	1317	363.78	2025-10-16	2025-11-11	3	2025-11-11	draft	Заявка на закупку #444
445	6	14	4190	209.85	2025-08-02	2025-10-02	3	2025-10-11	draft	\N
446	11	41	2237	494.55	2024-11-10	2024-11-16	3	2024-12-12	completed	\N
447	16	49	4496	125.32	2024-04-23	2024-05-12	1	2024-06-02	completed	\N
448	21	56	2080	378.58	2026-02-08	2026-03-25	2	2026-04-16	delivering	Заявка на закупку #448
449	26	23	150	583.14	2025-11-06	2026-02-02	3	2026-02-20	completed	\N
450	31	12	1023	\N	2024-04-13	2024-07-07	5	2024-08-07	completed	\N
\.


--
-- TOC entry 3697 (class 0 OID 24669)
-- Dependencies: 240
-- Data for Name: sales_requisition; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.sales_requisition (id, farm_id, product_id, quantity, price_per_unit, offer_date, valid_until, status, notes) FROM stdin;
1	4	70	2264	122.86	2024-05-30	2024-07-03	approved	\N
2	7	55	966	314.18	2024-12-11	2025-01-28	delivering	\N
3	10	37	2059	793.89	2025-05-29	2025-07-24	approved	\N
4	13	9	2173	491.71	2025-08-23	2025-10-18	approved	\N
5	16	3	1669	397.35	2026-03-03	2026-03-09	approved	Коммерческое предложение на продажу #5
6	19	13	3354	140.14	2024-02-14	2024-03-01	draft	\N
7	22	48	1283	220.68	2026-02-06	2026-02-28	draft	\N
8	25	24	1022	784.40	2024-05-01	2024-06-20	approved	\N
9	28	40	2513	683.66	2025-06-24	2025-07-26	draft	\N
10	31	31	2044	175.22	2025-12-31	2026-02-20	completed	Коммерческое предложение на продажу #10
11	34	65	1587	99.68	2024-02-29	2024-03-17	approved	\N
12	37	26	440	557.50	2025-07-17	2025-09-12	delivering	\N
13	40	7	338	130.60	2024-10-09	2024-10-16	cancelled	\N
14	43	27	1229	831.54	2025-05-01	2025-06-27	draft	\N
15	46	14	3082	637.93	2026-02-04	2026-02-15	delivering	Коммерческое предложение на продажу #15
16	49	60	3130	606.14	2025-02-28	2025-03-07	completed	\N
17	\N	14	3633	587.64	2025-02-01	2025-02-15	completed	\N
18	55	46	2810	659.42	2026-02-08	2026-02-16	completed	\N
19	58	34	413	546.65	2024-10-18	2024-12-16	cancelled	\N
20	1	60	3374	497.68	2025-01-08	2025-01-13	completed	Коммерческое предложение на продажу #20
21	4	10	853	864.68	2024-12-14	2025-01-02	approved	\N
22	7	22	2902	155.88	2025-04-28	2025-05-13	delivering	\N
23	10	57	2150	684.25	2025-04-27	2025-05-10	completed	\N
24	13	8	2938	246.56	2024-04-24	2024-05-25	draft	\N
25	16	4	2273	638.27	2024-04-23	2024-06-16	completed	Коммерческое предложение на продажу #25
26	19	20	541	810.22	2025-11-24	2026-01-06	completed	\N
27	22	50	1039	461.55	2026-03-04	2026-03-16	delivering	\N
28	25	63	1397	762.69	2024-02-11	2024-03-15	completed	\N
29	28	14	3695	817.98	2024-07-05	2024-08-30	approved	\N
30	31	2	2769	665.76	2025-07-16	2025-08-13	completed	Коммерческое предложение на продажу #30
31	34	60	3561	874.89	2024-02-02	2024-02-07	approved	\N
32	37	33	2750	788.57	2024-07-17	2024-09-11	completed	\N
33	40	28	3599	389.00	2024-07-09	2024-07-24	cancelled	\N
34	\N	16	1610	549.39	2025-01-28	2025-03-12	completed	\N
35	46	49	2268	352.79	2025-04-11	2025-05-02	approved	Коммерческое предложение на продажу #35
36	49	61	91	383.65	2025-03-29	2025-05-16	completed	\N
37	52	9	1799	578.68	2024-05-01	2024-05-21	delivering	\N
38	55	47	2864	687.54	2024-05-25	2024-06-04	approved	\N
39	58	15	3445	398.55	2024-11-01	2024-12-28	draft	\N
40	1	10	28	460.89	2024-01-10	2024-01-28	approved	Коммерческое предложение на продажу #40
41	4	6	3864	861.97	2025-10-26	2025-12-03	completed	\N
42	7	21	1165	772.01	2025-12-20	2026-02-10	completed	\N
43	10	69	3555	242.70	2024-05-20	2024-06-12	delivering	\N
44	13	38	2071	110.73	2024-01-25	2024-01-30	approved	\N
45	16	14	3035	414.20	2024-03-17	2024-05-13	draft	Коммерческое предложение на продажу #45
46	19	30	201	179.52	2025-01-12	2025-01-21	approved	\N
47	22	47	3137	737.37	2025-07-17	2025-08-15	completed	\N
48	25	52	2930	726.09	2024-08-24	2024-10-10	completed	\N
49	28	43	963	316.83	2024-04-02	2024-04-12	draft	\N
50	31	3	3369	155.13	2026-02-18	2026-03-12	cancelled	Коммерческое предложение на продажу #50
51	\N	54	1073	704.56	2024-06-08	2024-07-14	delivering	\N
52	37	55	3514	865.49	2024-11-23	2024-12-28	approved	\N
53	40	44	1324	684.07	2024-04-23	2024-05-06	delivering	\N
54	43	10	3720	434.44	2025-05-19	2025-06-14	completed	\N
55	46	18	969	555.47	2026-02-22	2026-04-04	delivering	Коммерческое предложение на продажу #55
56	49	13	1408	361.13	2024-06-07	2024-07-06	completed	\N
57	52	6	3132	622.37	2025-09-04	2025-10-08	approved	\N
58	55	50	87	184.71	2024-10-26	2024-11-27	completed	\N
59	58	48	3170	291.76	2026-03-02	2026-04-05	delivering	\N
60	1	5	1476	109.64	2024-04-14	2024-04-20	delivering	Коммерческое предложение на продажу #60
61	4	8	3397	477.53	2024-05-05	2024-06-02	approved	\N
62	7	34	3101	606.23	2025-06-28	2025-08-13	completed	\N
63	10	62	2732	665.65	2024-08-09	2024-08-20	delivering	\N
64	13	40	944	502.91	2026-03-05	2026-03-21	cancelled	\N
65	16	38	3374	116.60	2024-11-04	2024-12-11	completed	Коммерческое предложение на продажу #65
66	19	59	2761	208.00	2024-02-21	2024-03-07	delivering	\N
67	22	43	1073	706.14	2025-02-07	2025-03-27	delivering	\N
68	\N	19	3669	435.98	2025-07-22	2025-08-29	completed	\N
69	28	6	1847	407.04	2024-09-13	2024-09-26	draft	\N
70	31	49	2808	229.81	2025-05-04	2025-05-13	completed	Коммерческое предложение на продажу #70
71	34	27	3872	340.83	2026-01-14	2026-02-23	cancelled	\N
72	37	52	1959	407.85	2024-05-21	2024-06-17	delivering	\N
73	40	28	3668	502.02	2026-01-09	2026-02-15	completed	\N
74	43	7	1674	812.05	2025-12-11	2025-12-17	completed	\N
75	46	67	3772	846.03	2025-02-06	2025-03-20	completed	Коммерческое предложение на продажу #75
76	49	23	245	641.36	2026-03-09	2026-04-06	completed	\N
77	52	34	34	348.09	2024-01-27	2024-03-15	delivering	\N
78	55	19	1952	224.01	2025-03-04	2025-04-25	draft	\N
79	58	42	2451	776.42	2025-03-15	2025-04-05	completed	\N
80	1	41	795	631.13	2026-01-25	2026-02-19	approved	Коммерческое предложение на продажу #80
81	4	8	3027	455.27	2025-09-01	2025-10-05	delivering	\N
82	7	44	1420	270.21	2025-03-22	2025-05-20	draft	\N
83	10	4	998	757.34	2025-06-23	2025-07-14	draft	\N
84	13	24	1963	232.50	2025-02-16	2025-04-09	completed	\N
85	\N	59	2994	229.27	2024-03-02	2024-04-21	approved	Коммерческое предложение на продажу #85
86	19	19	1984	142.28	2026-02-18	2026-02-27	completed	\N
87	22	48	2677	804.52	2025-03-12	2025-04-24	draft	\N
88	25	63	446	412.38	2024-02-28	2024-04-22	approved	\N
89	28	70	1533	151.35	2024-10-01	2024-11-28	delivering	\N
90	31	30	142	371.70	2024-04-16	2024-06-13	approved	Коммерческое предложение на продажу #90
91	34	2	859	548.91	2024-07-02	2024-07-18	draft	\N
92	37	24	3069	404.16	2025-08-05	2025-09-29	delivering	\N
93	40	13	3657	279.89	2025-11-25	2025-12-25	approved	\N
94	43	10	258	411.85	2024-01-26	2024-03-26	draft	\N
95	46	60	3628	804.56	2024-01-20	2024-02-07	completed	Коммерческое предложение на продажу #95
96	49	11	1648	835.65	2025-04-01	2025-05-25	draft	\N
97	52	44	2816	64.11	2024-02-03	2024-02-26	delivering	\N
98	55	27	766	404.21	2025-04-14	2025-04-30	approved	\N
99	58	36	1553	543.05	2024-04-25	2024-06-24	completed	\N
100	1	21	2316	290.45	2025-03-30	2025-05-01	cancelled	Коммерческое предложение на продажу #100
101	4	1	2221	242.84	2025-07-30	2025-08-06	completed	\N
102	\N	42	1075	106.43	2024-12-24	2025-02-06	completed	\N
103	10	42	455	645.55	2024-08-12	2024-10-03	delivering	\N
104	13	17	1902	647.10	2026-02-15	2026-03-23	approved	\N
105	16	12	665	811.11	2024-04-30	2024-06-26	approved	Коммерческое предложение на продажу #105
106	19	45	2681	95.36	2026-03-01	2026-04-11	completed	\N
107	22	8	523	756.73	2026-01-05	2026-02-05	delivering	\N
108	25	43	3385	200.04	2024-06-19	2024-07-29	completed	\N
109	28	46	851	551.95	2024-09-28	2024-11-11	completed	\N
110	31	48	105	437.48	2024-01-01	2024-02-07	completed	Коммерческое предложение на продажу #110
111	34	56	1244	744.57	2025-03-04	2025-04-25	delivering	\N
112	37	47	3096	403.80	2024-01-23	2024-02-07	draft	\N
113	40	2	2734	659.06	2024-04-15	2024-05-26	cancelled	\N
114	43	22	2447	94.26	2024-07-09	2024-09-01	draft	\N
115	46	48	3191	752.36	2024-11-02	2024-12-29	approved	Коммерческое предложение на продажу #115
116	49	5	1154	273.90	2025-08-09	2025-08-16	delivering	\N
117	52	23	3691	187.56	2024-07-16	2024-08-05	approved	\N
118	55	1	3931	196.77	2025-01-01	2025-01-10	draft	\N
119	\N	48	3720	605.92	2024-12-05	2025-01-01	completed	\N
120	1	8	474	577.37	2026-01-16	2026-02-04	cancelled	Коммерческое предложение на продажу #120
121	4	37	1355	727.80	2025-11-30	2025-12-07	approved	\N
122	7	52	2389	481.13	2025-07-21	2025-09-15	delivering	\N
123	10	27	2565	218.63	2026-03-05	2026-03-25	approved	\N
124	13	21	1216	773.08	2025-02-07	2025-02-22	delivering	\N
125	16	17	39	741.01	2024-01-28	2024-02-24	delivering	Коммерческое предложение на продажу #125
126	19	69	2652	249.77	2024-05-09	2024-07-06	cancelled	\N
127	22	35	3092	283.34	2024-06-14	2024-07-26	draft	\N
128	25	43	3087	560.69	2024-11-16	2025-01-15	completed	\N
129	28	18	3906	102.69	2024-05-26	2024-06-15	cancelled	\N
130	31	52	821	118.59	2025-04-15	2025-05-09	delivering	Коммерческое предложение на продажу #130
131	34	36	665	110.40	2024-01-16	2024-02-14	approved	\N
132	37	35	2635	133.51	2024-07-27	2024-08-29	approved	\N
133	40	13	2410	166.91	2025-02-09	2025-03-14	draft	\N
134	43	30	419	461.97	2024-08-25	2024-10-15	approved	\N
135	46	55	3511	858.42	2025-04-29	2025-05-24	approved	Коммерческое предложение на продажу #135
136	\N	66	2588	830.55	2024-09-14	2024-10-13	draft	\N
137	52	19	2882	76.47	2025-09-27	2025-10-29	draft	\N
138	55	30	118	58.81	2025-08-30	2025-09-14	completed	\N
139	58	69	3706	281.95	2024-05-20	2024-06-11	draft	\N
140	1	22	2898	144.48	2024-12-28	2025-02-15	approved	Коммерческое предложение на продажу #140
141	4	43	1462	63.39	2024-04-06	2024-04-28	approved	\N
142	7	18	421	546.69	2024-04-17	2024-05-11	approved	\N
143	10	7	2129	774.39	2025-05-11	2025-06-22	delivering	\N
144	13	18	1078	448.89	2024-05-09	2024-06-06	completed	\N
145	16	33	3532	165.94	2024-04-05	2024-05-12	delivering	Коммерческое предложение на продажу #145
146	19	33	2583	729.22	2024-11-25	2025-01-13	cancelled	\N
147	22	15	2429	207.38	2024-01-08	2024-02-01	completed	\N
148	25	62	2027	90.02	2025-05-19	2025-07-03	approved	\N
149	28	26	3948	231.58	2024-08-28	2024-10-20	approved	\N
150	31	56	1461	135.51	2025-09-24	2025-10-02	approved	Коммерческое предложение на продажу #150
151	34	60	2944	513.75	2024-10-23	2024-11-24	approved	\N
152	37	67	1376	592.88	2026-03-15	2026-05-09	approved	\N
153	\N	68	2182	217.33	2026-01-26	2026-03-06	approved	\N
154	43	42	2875	424.47	2025-05-04	2025-05-17	approved	\N
155	46	23	83	680.01	2024-09-29	2024-10-29	delivering	Коммерческое предложение на продажу #155
156	49	22	1494	312.85	2024-08-09	2024-10-08	completed	\N
157	52	41	3029	370.38	2026-03-15	2026-04-26	approved	\N
158	55	21	717	210.11	2024-12-24	2024-12-29	completed	\N
159	58	69	761	353.01	2025-11-10	2025-12-25	approved	\N
160	1	39	1841	200.43	2025-02-25	2025-03-14	delivering	Коммерческое предложение на продажу #160
161	4	33	3617	207.34	2024-01-14	2024-01-24	completed	\N
162	7	22	3257	192.33	2024-10-07	2024-10-17	approved	\N
163	10	23	2443	64.67	2024-01-14	2024-02-26	completed	\N
164	13	54	1625	198.85	2025-12-11	2026-01-13	draft	\N
165	16	53	1642	55.21	2024-12-16	2025-02-11	completed	Коммерческое предложение на продажу #165
166	19	47	1772	219.10	2024-03-09	2024-04-13	completed	\N
167	22	6	86	661.01	2025-09-21	2025-09-30	draft	\N
168	25	11	2364	240.57	2025-01-14	2025-03-08	approved	\N
169	28	23	560	190.47	2025-04-19	2025-04-29	draft	\N
170	\N	29	1575	775.09	2025-12-12	2025-12-19	approved	Коммерческое предложение на продажу #170
171	34	3	3826	501.90	2025-04-17	2025-05-23	completed	\N
172	37	42	1648	292.43	2024-04-23	2024-06-10	completed	\N
173	40	8	1951	879.40	2024-05-12	2024-06-11	approved	\N
174	43	68	2931	848.79	2024-01-11	2024-01-16	delivering	\N
175	46	30	1906	201.58	2024-10-04	2024-11-05	draft	Коммерческое предложение на продажу #175
176	49	55	2709	178.89	2024-08-16	2024-10-03	draft	\N
177	52	26	3231	401.76	2024-01-13	2024-02-28	cancelled	\N
178	55	34	1169	657.11	2025-03-21	2025-04-14	draft	\N
179	58	26	2940	138.77	2024-12-23	2025-01-14	cancelled	\N
180	1	37	486	828.51	2024-07-20	2024-08-23	approved	Коммерческое предложение на продажу #180
181	4	61	2707	811.07	2025-06-19	2025-07-17	delivering	\N
182	7	54	3673	831.21	2024-02-02	2024-03-05	completed	\N
183	10	27	3246	267.10	2024-10-26	2024-11-11	completed	\N
184	13	6	3363	289.44	2025-03-14	2025-05-09	draft	\N
185	16	42	3525	342.45	2025-06-07	2025-07-17	delivering	Коммерческое предложение на продажу #185
186	19	12	3433	770.07	2025-09-26	2025-11-09	delivering	\N
187	\N	23	1237	644.25	2024-02-12	2024-02-27	approved	\N
188	25	29	1833	314.97	2026-03-01	2026-03-23	draft	\N
189	28	64	699	240.08	2024-12-19	2025-01-20	completed	\N
190	31	26	2062	751.21	2025-08-09	2025-08-30	delivering	Коммерческое предложение на продажу #190
191	34	2	687	358.30	2024-10-27	2024-12-20	delivering	\N
192	37	29	1560	766.55	2024-05-20	2024-06-20	delivering	\N
193	40	6	1160	636.54	2024-06-26	2024-07-13	delivering	\N
194	43	35	268	606.65	2024-01-04	2024-02-16	approved	\N
195	46	65	3591	339.95	2024-11-16	2024-12-21	completed	Коммерческое предложение на продажу #195
196	49	35	724	132.21	2026-01-13	2026-02-04	approved	\N
197	52	3	3431	729.38	2025-08-18	2025-10-11	approved	\N
198	55	69	328	276.26	2025-03-13	2025-03-21	approved	\N
199	58	53	3028	134.31	2024-08-15	2024-09-18	delivering	\N
200	1	67	1082	288.06	2025-04-13	2025-04-21	approved	Коммерческое предложение на продажу #200
201	4	49	347	513.19	2025-04-07	2025-06-04	completed	\N
202	7	45	3608	406.21	2025-02-28	2025-04-28	delivering	\N
203	10	50	3102	252.59	2025-07-20	2025-09-14	cancelled	\N
204	\N	9	1516	611.96	2025-12-11	2025-12-21	delivering	\N
205	16	67	3067	569.54	2024-02-04	2024-02-12	draft	Коммерческое предложение на продажу #205
206	19	61	3796	642.93	2025-08-13	2025-10-10	draft	\N
207	22	67	1271	337.67	2025-12-15	2026-01-18	completed	\N
208	25	50	93	300.10	2024-08-29	2024-09-26	approved	\N
209	28	55	146	340.15	2024-02-07	2024-02-26	completed	\N
210	31	57	1103	828.05	2025-08-29	2025-10-05	approved	Коммерческое предложение на продажу #210
211	34	4	1633	463.17	2025-03-26	2025-05-12	approved	\N
212	37	52	1939	647.21	2024-05-03	2024-06-12	cancelled	\N
213	40	24	3020	209.98	2025-02-24	2025-04-24	delivering	\N
214	43	36	3188	123.68	2024-01-17	2024-03-11	delivering	\N
215	46	66	2660	483.47	2024-12-02	2024-12-20	delivering	Коммерческое предложение на продажу #215
216	49	43	2779	560.14	2024-02-17	2024-03-17	delivering	\N
217	52	27	2355	405.43	2024-11-07	2024-12-25	completed	\N
218	55	66	2850	577.14	2025-05-30	2025-07-22	delivering	\N
219	58	52	108	681.90	2024-11-18	2025-01-13	cancelled	\N
220	1	42	3484	73.51	2025-11-17	2026-01-01	approved	Коммерческое предложение на продажу #220
221	\N	18	579	755.03	2025-12-12	2026-01-15	draft	\N
222	7	55	3143	646.63	2024-03-20	2024-04-28	completed	\N
223	10	57	131	399.03	2024-07-29	2024-08-14	completed	\N
224	13	9	1657	699.74	2025-08-14	2025-10-01	cancelled	\N
225	16	2	1953	216.95	2025-08-17	2025-09-28	approved	Коммерческое предложение на продажу #225
226	19	66	3447	575.27	2025-11-20	2025-12-31	completed	\N
227	22	21	2019	156.40	2024-06-06	2024-07-07	completed	\N
228	25	27	3174	734.62	2025-04-19	2025-04-29	delivering	\N
229	28	1	2196	412.34	2024-11-11	2024-12-25	approved	\N
230	31	52	2415	301.69	2024-01-31	2024-02-18	completed	Коммерческое предложение на продажу #230
231	34	69	2823	860.86	2024-08-28	2024-09-07	completed	\N
232	37	31	3932	331.18	2025-12-04	2026-01-18	delivering	\N
233	40	3	472	293.52	2026-03-04	2026-04-14	completed	\N
234	43	54	1851	77.63	2025-01-24	2025-03-08	approved	\N
235	46	44	3366	510.12	2024-07-13	2024-07-18	approved	Коммерческое предложение на продажу #235
236	49	68	675	865.01	2024-10-05	2024-10-24	delivering	\N
237	52	66	3051	710.53	2025-11-05	2025-12-30	approved	\N
238	\N	7	2866	178.96	2025-12-11	2026-01-28	completed	\N
239	58	10	1918	667.35	2024-04-24	2024-05-25	delivering	\N
240	1	1	1592	590.74	2025-10-18	2025-11-09	completed	Коммерческое предложение на продажу #240
241	4	15	1417	801.16	2025-06-24	2025-08-05	completed	\N
242	7	7	431	375.77	2024-09-15	2024-11-08	delivering	\N
243	10	19	3701	773.18	2025-06-07	2025-06-17	approved	\N
244	13	26	3858	434.21	2024-02-17	2024-02-29	delivering	\N
245	16	20	1698	588.04	2024-02-13	2024-03-17	completed	Коммерческое предложение на продажу #245
246	19	27	137	488.63	2025-07-06	2025-07-13	delivering	\N
247	22	9	1870	189.64	2024-06-02	2024-07-10	approved	\N
248	25	54	2263	201.91	2026-02-27	2026-03-22	completed	\N
249	28	68	1346	690.66	2024-01-31	2024-03-18	completed	\N
250	31	59	613	491.31	2025-11-06	2025-12-18	cancelled	Коммерческое предложение на продажу #250
251	34	24	2637	601.58	2025-10-08	2025-10-30	draft	\N
252	37	54	2221	56.84	2025-08-29	2025-10-16	draft	\N
253	40	50	3526	148.10	2025-04-28	2025-05-28	approved	\N
254	43	2	2293	82.96	2024-05-16	2024-07-13	draft	\N
255	\N	16	1878	103.01	2026-02-14	2026-04-11	approved	Коммерческое предложение на продажу #255
256	49	54	807	623.28	2024-09-27	2024-10-19	approved	\N
257	52	42	3299	557.66	2025-10-07	2025-11-22	approved	\N
258	55	33	1378	898.58	2025-11-27	2026-01-23	completed	\N
259	58	1	342	205.95	2026-01-23	2026-03-16	approved	\N
260	1	19	1231	192.15	2025-01-17	2025-02-04	approved	Коммерческое предложение на продажу #260
261	4	3	2408	131.07	2024-06-08	2024-06-27	delivering	\N
262	7	32	46	209.33	2024-09-29	2024-11-09	delivering	\N
263	10	8	1033	570.49	2025-10-26	2025-12-18	delivering	\N
264	13	17	3771	692.31	2024-09-05	2024-10-04	approved	\N
265	16	40	182	352.38	2024-08-15	2024-10-03	completed	Коммерческое предложение на продажу #265
266	19	33	2063	692.04	2025-12-17	2026-01-25	completed	\N
267	22	48	3200	850.68	2025-11-26	2025-12-02	cancelled	\N
268	25	52	3397	647.92	2025-06-06	2025-07-15	draft	\N
269	28	46	3591	660.74	2025-04-07	2025-05-13	approved	\N
270	31	48	1142	848.51	2025-02-13	2025-03-16	approved	Коммерческое предложение на продажу #270
271	34	26	3945	162.46	2025-03-14	2025-03-21	approved	\N
272	\N	60	2485	710.27	2025-06-25	2025-07-26	approved	\N
273	40	10	539	370.04	2024-11-13	2024-12-24	completed	\N
274	43	23	562	687.48	2024-09-19	2024-10-05	delivering	\N
275	46	50	765	195.09	2025-02-24	2025-04-06	completed	Коммерческое предложение на продажу #275
276	49	2	1582	407.33	2024-03-15	2024-04-18	cancelled	\N
277	52	53	734	214.83	2024-12-19	2025-01-17	completed	\N
278	55	20	2418	828.02	2025-09-09	2025-11-03	cancelled	\N
279	58	32	892	325.92	2025-07-20	2025-09-10	delivering	\N
280	1	52	2287	489.98	2025-12-28	2026-02-02	delivering	Коммерческое предложение на продажу #280
281	4	24	1534	829.33	2025-03-20	2025-03-25	approved	\N
282	7	32	1053	498.79	2024-02-20	2024-03-29	cancelled	\N
283	10	33	3676	568.77	2024-09-09	2024-09-20	approved	\N
284	13	68	3065	215.21	2026-02-21	2026-04-05	draft	\N
285	16	17	1658	63.32	2024-01-05	2024-01-14	approved	Коммерческое предложение на продажу #285
286	19	7	2682	882.16	2025-12-16	2026-01-19	cancelled	\N
287	22	19	3450	576.31	2026-02-03	2026-02-12	cancelled	\N
288	25	40	3045	605.99	2024-08-20	2024-10-18	draft	\N
289	\N	37	3004	845.74	2024-03-13	2024-03-31	approved	\N
290	31	3	846	491.52	2025-08-11	2025-09-16	approved	Коммерческое предложение на продажу #290
291	34	70	1935	166.87	2025-08-28	2025-10-11	delivering	\N
292	37	42	3421	622.30	2024-10-31	2024-11-15	completed	\N
293	40	18	1225	822.76	2024-12-03	2025-01-16	cancelled	\N
294	43	20	237	493.21	2024-08-27	2024-09-22	completed	\N
295	46	4	2345	842.95	2025-03-12	2025-04-13	delivering	Коммерческое предложение на продажу #295
296	49	14	2559	852.07	2025-07-26	2025-09-08	approved	\N
297	52	70	3783	877.00	2024-02-04	2024-03-21	cancelled	\N
298	55	48	1657	506.19	2024-01-05	2024-01-15	delivering	\N
299	58	47	1017	585.62	2024-01-03	2024-02-05	completed	\N
300	1	53	421	846.30	2025-03-16	2025-04-26	approved	Коммерческое предложение на продажу #300
301	4	62	636	695.25	2025-10-29	2025-12-19	completed	\N
302	7	38	3489	770.40	2024-11-13	2024-12-03	completed	\N
303	10	40	2020	272.70	2025-12-28	2026-01-12	approved	\N
304	13	10	2699	219.07	2025-11-07	2025-12-21	approved	\N
305	16	52	407	65.76	2025-05-23	2025-06-28	delivering	Коммерческое предложение на продажу #305
306	\N	22	310	474.40	2024-12-24	2025-01-01	approved	\N
307	22	12	3113	541.38	2025-11-10	2025-11-26	cancelled	\N
308	25	19	2856	336.83	2025-09-28	2025-11-19	completed	\N
309	28	10	2429	222.54	2025-05-10	2025-05-27	approved	\N
310	31	6	787	307.04	2024-07-07	2024-09-01	approved	Коммерческое предложение на продажу #310
311	34	40	1136	770.40	2024-10-20	2024-11-29	delivering	\N
312	37	35	2030	419.41	2025-03-30	2025-05-21	cancelled	\N
313	40	54	3557	163.73	2024-08-18	2024-09-12	cancelled	\N
314	43	1	2843	762.21	2025-02-27	2025-03-24	draft	\N
315	46	10	3231	453.18	2024-02-04	2024-03-24	delivering	Коммерческое предложение на продажу #315
316	49	46	377	463.41	2024-09-01	2024-10-18	approved	\N
317	52	69	2439	348.31	2024-10-01	2024-10-28	approved	\N
318	55	33	161	566.24	2024-03-16	2024-04-25	completed	\N
319	58	66	15	345.94	2025-07-11	2025-09-01	draft	\N
320	1	13	1324	811.44	2025-04-07	2025-04-28	approved	Коммерческое предложение на продажу #320
321	4	51	2750	364.05	2025-09-08	2025-10-25	draft	\N
322	7	51	202	601.39	2025-04-29	2025-05-04	draft	\N
323	\N	2	2417	262.73	2024-01-13	2024-03-03	approved	\N
324	13	40	490	510.94	2024-01-19	2024-03-12	approved	\N
325	16	70	622	205.59	2025-07-06	2025-07-12	completed	Коммерческое предложение на продажу #325
326	19	49	1622	704.76	2026-02-24	2026-04-17	completed	\N
327	22	31	2114	369.13	2025-07-11	2025-08-20	approved	\N
328	25	36	2583	876.43	2026-01-31	2026-02-06	completed	\N
329	28	69	2223	65.52	2025-02-05	2025-03-02	approved	\N
330	31	44	380	875.35	2025-01-26	2025-02-18	completed	Коммерческое предложение на продажу #330
331	34	27	3561	227.15	2024-08-30	2024-09-25	completed	\N
332	37	17	829	101.89	2024-07-13	2024-08-25	cancelled	\N
333	40	39	1089	773.25	2025-05-30	2025-07-06	cancelled	\N
334	43	56	2006	90.97	2026-01-07	2026-02-19	delivering	\N
335	46	6	2453	197.12	2025-11-26	2026-01-15	draft	Коммерческое предложение на продажу #335
336	49	12	2225	764.04	2025-12-10	2026-01-24	cancelled	\N
337	52	37	3555	242.89	2024-04-04	2024-04-21	completed	\N
338	55	19	3330	384.38	2024-04-12	2024-04-23	approved	\N
339	58	36	2277	341.04	2024-02-18	2024-03-15	approved	\N
340	\N	52	997	212.57	2024-05-02	2024-05-14	delivering	Коммерческое предложение на продажу #340
341	4	52	142	304.50	2025-04-10	2025-06-05	completed	\N
342	7	21	611	70.29	2024-08-19	2024-10-01	cancelled	\N
343	10	1	1959	458.41	2024-10-06	2024-11-03	completed	\N
344	13	38	1977	668.82	2024-02-28	2024-04-28	delivering	\N
345	16	18	3772	596.28	2024-07-20	2024-07-30	delivering	Коммерческое предложение на продажу #345
346	19	9	1194	384.89	2024-03-16	2024-05-12	completed	\N
347	22	44	2996	261.08	2024-10-05	2024-11-05	completed	\N
348	25	63	3302	382.43	2025-02-21	2025-03-06	completed	\N
349	28	24	1026	331.08	2026-02-16	2026-03-29	approved	\N
350	31	1	1498	251.12	2024-01-14	2024-01-27	completed	Коммерческое предложение на продажу #350
351	34	10	3040	506.75	2024-01-17	2024-01-30	approved	\N
352	37	58	34	237.59	2025-02-01	2025-03-11	approved	\N
353	40	43	3811	741.65	2025-02-06	2025-02-26	draft	\N
354	43	51	987	421.71	2024-01-09	2024-02-06	draft	\N
355	46	68	3643	596.71	2025-10-24	2025-12-08	approved	Коммерческое предложение на продажу #355
356	49	2	2565	515.54	2024-05-03	2024-05-08	completed	\N
357	\N	6	2436	154.93	2024-06-17	2024-06-25	completed	\N
358	55	31	2627	79.35	2025-01-08	2025-01-21	draft	\N
359	58	34	1398	188.58	2024-07-23	2024-09-15	cancelled	\N
360	1	48	2338	450.03	2025-09-03	2025-10-01	approved	Коммерческое предложение на продажу #360
361	4	15	2363	603.63	2024-07-10	2024-08-09	draft	\N
362	7	69	1772	91.33	2025-04-19	2025-05-16	cancelled	\N
363	10	64	3837	304.51	2024-11-25	2024-12-16	completed	\N
364	13	18	189	324.57	2026-02-04	2026-03-16	cancelled	\N
365	16	37	2850	296.09	2024-12-03	2024-12-22	delivering	Коммерческое предложение на продажу #365
366	19	16	2293	68.77	2025-07-28	2025-08-09	completed	\N
367	22	50	1434	586.72	2025-06-26	2025-07-14	approved	\N
368	25	37	3131	333.99	2025-02-19	2025-03-06	completed	\N
369	28	48	621	159.23	2025-09-23	2025-10-29	draft	\N
370	31	53	1566	65.20	2025-02-22	2025-03-05	completed	Коммерческое предложение на продажу #370
371	34	18	1278	512.79	2024-02-01	2024-03-05	draft	\N
372	37	58	3547	596.09	2026-02-14	2026-03-17	cancelled	\N
373	40	52	75	165.76	2024-12-27	2025-02-21	cancelled	\N
374	\N	47	2074	363.57	2024-03-13	2024-04-10	cancelled	\N
375	46	1	287	427.83	2025-11-26	2026-01-16	completed	Коммерческое предложение на продажу #375
376	49	12	1743	475.01	2024-04-06	2024-05-01	approved	\N
377	52	45	2166	426.83	2024-03-04	2024-04-01	draft	\N
378	55	33	1247	118.42	2024-09-08	2024-10-20	approved	\N
379	58	57	2333	279.22	2025-12-27	2026-01-25	cancelled	\N
380	1	47	885	137.03	2024-11-23	2025-01-11	cancelled	Коммерческое предложение на продажу #380
381	4	60	2400	100.98	2026-01-10	2026-02-13	draft	\N
382	7	11	3116	598.32	2025-06-21	2025-08-05	delivering	\N
383	10	51	1790	869.94	2024-11-03	2025-01-01	completed	\N
384	13	28	1769	333.93	2024-05-06	2024-06-15	approved	\N
385	16	42	2932	568.15	2024-09-27	2024-11-11	completed	Коммерческое предложение на продажу #385
386	19	59	1631	517.16	2024-01-08	2024-02-05	completed	\N
387	22	7	3172	211.95	2024-09-05	2024-10-27	completed	\N
388	25	61	3292	672.73	2025-07-09	2025-07-30	approved	\N
389	28	1	3310	597.82	2025-07-31	2025-08-16	approved	\N
390	31	47	1781	468.09	2024-08-10	2024-08-27	completed	Коммерческое предложение на продажу #390
391	\N	19	2837	651.97	2025-12-16	2026-01-20	approved	\N
392	37	18	871	532.87	2025-02-18	2025-03-13	approved	\N
393	40	24	3567	528.77	2024-05-16	2024-06-10	approved	\N
394	43	21	3350	388.59	2025-04-16	2025-05-15	completed	\N
395	46	24	1067	194.71	2025-04-29	2025-05-19	completed	Коммерческое предложение на продажу #395
396	49	21	926	664.19	2024-12-12	2025-01-27	draft	\N
397	52	18	986	634.68	2024-01-23	2024-03-04	draft	\N
398	55	1	277	412.67	2024-07-12	2024-07-31	completed	\N
399	58	23	2124	290.67	2024-05-21	2024-06-08	cancelled	\N
400	1	64	2712	114.66	2025-08-28	2025-10-05	draft	Коммерческое предложение на продажу #400
401	4	2	3285	623.40	2024-12-05	2025-01-31	approved	\N
402	7	11	1354	767.12	2025-12-07	2026-01-02	draft	\N
403	10	50	245	828.04	2025-03-29	2025-04-23	delivering	\N
404	13	9	836	520.69	2024-08-14	2024-08-26	draft	\N
405	16	51	3240	295.27	2024-09-08	2024-09-26	completed	Коммерческое предложение на продажу #405
406	19	24	3178	695.82	2025-10-02	2025-11-22	completed	\N
407	22	17	2028	434.74	2025-04-23	2025-05-11	approved	\N
408	\N	10	1706	804.26	2025-06-02	2025-07-21	cancelled	\N
409	28	51	1655	839.65	2026-01-17	2026-02-21	draft	\N
410	31	57	24	230.20	2025-01-05	2025-01-13	approved	Коммерческое предложение на продажу #410
411	34	5	1382	574.14	2024-06-22	2024-08-05	delivering	\N
412	37	39	564	587.50	2024-03-27	2024-05-17	completed	\N
413	40	61	3250	653.31	2026-01-15	2026-01-23	draft	\N
414	43	64	1116	70.63	2025-10-25	2025-12-24	delivering	\N
415	46	47	2083	196.12	2025-04-09	2025-05-10	draft	Коммерческое предложение на продажу #415
416	49	1	3458	837.42	2024-06-12	2024-06-17	completed	\N
417	52	32	2904	397.09	2025-06-04	2025-07-21	cancelled	\N
418	55	52	3626	862.14	2024-09-25	2024-10-26	completed	\N
419	58	37	2359	433.41	2024-11-18	2024-12-31	delivering	\N
420	1	27	437	496.70	2024-08-10	2024-09-21	completed	Коммерческое предложение на продажу #420
\.


--
-- TOC entry 3681 (class 0 OID 24613)
-- Dependencies: 224
-- Data for Name: supplier; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.supplier (id, name, legal_address, status) FROM stdin;
1	АгроТрейд-01	г. Белгород, ул. Степная, д. 51	active
2	ЗерноСнаб-02	г. Тамбов, ул. Луговая, д. 52	inactive
3	ФермерМаркет-03	г. Ставрополь, ул. Садовая, д. 53	active
4	ТехноПоставка-04	г. Волгоград, ул. Школьная, д. 54	active
5	СеменаЮг-05	г. Краснодар, ул. Центральная, д. 55	active
6	УдобренияПлюс-06	г. Белгород, ул. Степная, д. 56	active
7	КомбикормСервис-07	г. Тамбов, ул. Луговая, д. 57	active
8	ЛогистикАгро-08	г. Ставрополь, ул. Садовая, д. 58	active
9	СкладРесурс-09	г. Волгоград, ул. Школьная, д. 59	suspended
10	ПолевойПартнер-10	г. Краснодар, ул. Центральная, д. 60	active
11	АгроТрейд-11	г. Белгород, ул. Степная, д. 61	active
12	ЗерноСнаб-12	г. Тамбов, ул. Луговая, д. 62	active
13	ФермерМаркет-13	г. Ставрополь, ул. Садовая, д. 63	active
14	ТехноПоставка-14	г. Волгоград, ул. Школьная, д. 64	inactive
15	СеменаЮг-15	г. Краснодар, ул. Центральная, д. 65	active
16	УдобренияПлюс-16	г. Белгород, ул. Степная, д. 66	active
17	КомбикормСервис-17	г. Тамбов, ул. Луговая, д. 67	suspended
18	ЛогистикАгро-18	г. Ставрополь, ул. Садовая, д. 68	inactive
19	СкладРесурс-19	г. Волгоград, ул. Школьная, д. 69	active
20	ПолевойПартнер-20	г. Краснодар, ул. Центральная, д. 70	active
21	АгроТрейд-21	г. Белгород, ул. Степная, д. 71	inactive
22	ЗерноСнаб-22	г. Тамбов, ул. Луговая, д. 72	active
23	ФермерМаркет-23	г. Ставрополь, ул. Садовая, д. 73	active
24	ТехноПоставка-24	г. Волгоград, ул. Школьная, д. 74	active
25	СеменаЮг-25	г. Краснодар, ул. Центральная, д. 75	archived
\.


--
-- TOC entry 3698 (class 0 OID 24674)
-- Dependencies: 241
-- Data for Name: supplier_product_price; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.supplier_product_price (id, supplier_id, product_id, purchase_price, valid_from, valid_until) FROM stdin;
1	1	51	347.72	2024-08-23	\N
2	1	30	90.50	2024-02-04	\N
3	1	38	228.58	2025-01-21	\N
4	1	21	178.82	2024-10-03	2025-01-19
5	1	48	311.73	2025-03-01	\N
6	1	43	319.26	2025-07-08	\N
7	1	53	421.15	2025-08-25	2025-11-12
8	1	14	435.40	2024-11-15	2025-07-20
9	1	36	284.70	2024-01-24	\N
10	1	47	270.91	2024-04-12	\N
11	1	16	191.62	2025-04-27	\N
12	1	1	206.42	2024-08-15	2025-06-26
13	1	3	212.66	2025-06-30	\N
14	1	55	436.24	2025-05-23	2025-12-29
15	1	2	249.42	2024-04-20	\N
16	1	49	188.33	2024-01-26	\N
17	1	34	154.46	2025-01-13	\N
18	1	7	398.16	2024-11-14	\N
19	1	33	154.05	2024-03-26	\N
20	1	26	266.57	2025-05-16	\N
21	1	32	220.72	2025-02-16	\N
22	1	58	488.54	2025-08-03	2025-09-17
23	1	11	447.14	2024-09-15	\N
24	1	19	313.97	2024-06-03	\N
25	1	20	331.36	2025-02-20	2025-08-06
26	1	56	380.95	2025-07-29	2026-01-14
27	2	31	315.41	2025-05-13	\N
28	2	42	343.61	2025-03-04	\N
29	2	60	201.68	2025-03-21	2025-09-05
30	2	62	137.64	2025-01-15	\N
31	2	66	216.23	2024-10-24	\N
32	2	11	314.09	2025-01-15	\N
33	2	14	386.80	2025-04-18	\N
34	2	28	475.65	2024-05-18	\N
35	2	45	172.48	2025-04-30	\N
36	2	5	416.16	2024-06-02	\N
37	2	25	279.61	2024-01-06	\N
38	2	9	302.03	2024-04-06	\N
39	2	49	289.47	2024-03-03	\N
40	2	1	228.77	2024-03-09	\N
41	2	65	146.89	2024-08-19	\N
42	2	34	351.71	2024-07-16	\N
43	2	48	193.08	2025-03-12	\N
44	2	8	342.78	2025-02-16	2025-12-30
45	2	56	286.92	2024-07-26	\N
46	2	13	351.31	2024-08-09	\N
47	2	29	439.64	2024-04-14	\N
48	2	53	406.29	2025-01-27	\N
49	2	3	374.83	2025-08-29	\N
50	2	63	109.73	2025-04-29	\N
51	2	32	347.63	2024-08-25	2024-12-26
52	2	52	307.75	2024-05-30	\N
53	2	41	491.39	2024-01-01	\N
54	2	55	449.99	2024-02-23	\N
55	3	61	197.95	2024-05-26	2026-01-19
56	3	42	416.92	2024-09-13	\N
57	3	39	261.45	2025-03-21	\N
58	3	65	328.76	2025-01-09	2026-03-09
59	3	16	230.44	2025-03-10	\N
60	3	31	125.73	2024-07-14	\N
61	3	51	260.77	2024-01-11	\N
62	3	25	314.86	2024-07-18	\N
63	3	50	282.77	2024-04-06	2025-01-20
64	3	63	119.72	2024-01-14	\N
65	3	46	186.03	2024-10-08	\N
66	3	67	194.24	2024-09-16	\N
67	3	23	450.58	2025-08-08	2026-03-09
68	3	20	274.70	2025-04-10	\N
69	3	54	314.12	2025-07-12	2025-08-20
70	3	34	367.00	2024-06-05	2025-01-02
71	3	21	390.87	2024-05-14	\N
72	3	55	404.81	2024-05-02	\N
73	3	6	397.42	2024-03-04	\N
74	3	68	375.65	2024-10-11	2025-02-17
75	3	14	482.30	2024-01-18	\N
76	3	11	495.57	2024-11-20	2025-03-16
77	3	41	388.49	2025-08-23	2025-12-24
78	3	18	294.12	2025-02-16	\N
79	4	9	368.15	2024-01-04	\N
80	4	25	387.25	2024-04-30	\N
81	4	65	273.33	2024-03-29	\N
82	4	19	329.63	2024-04-29	2024-12-01
83	4	35	318.32	2024-07-09	\N
84	4	11	307.82	2024-01-21	2024-03-01
85	4	23	438.23	2025-01-18	\N
86	4	29	486.24	2024-09-09	\N
87	4	3	197.38	2024-07-14	2024-12-26
88	4	18	188.71	2025-01-23	2025-05-24
89	4	39	276.14	2025-08-01	\N
90	4	13	336.79	2024-07-11	\N
91	4	45	181.89	2024-10-29	\N
92	4	33	271.14	2024-04-04	\N
93	4	27	388.66	2024-12-23	2025-10-16
94	4	12	494.55	2024-06-23	\N
95	4	41	404.40	2025-01-29	\N
96	4	48	335.00	2025-03-20	\N
97	5	27	428.31	2025-07-31	\N
98	5	44	329.27	2024-02-04	\N
99	5	65	191.16	2025-01-02	2025-02-05
100	5	35	325.67	2025-05-01	\N
101	5	12	364.95	2024-04-12	\N
102	5	70	283.69	2024-06-15	\N
103	5	34	300.07	2024-01-23	\N
104	5	14	420.12	2025-08-30	\N
105	5	30	129.49	2025-02-12	\N
106	5	17	229.20	2025-01-17	\N
107	5	3	380.46	2025-02-14	\N
108	5	8	228.16	2024-08-27	2026-02-27
109	5	64	240.71	2025-03-12	2026-01-31
110	5	43	329.26	2024-01-30	\N
111	5	41	508.78	2024-11-03	2025-11-25
112	5	66	371.26	2024-07-22	2026-03-25
113	5	31	296.36	2024-05-13	\N
114	5	9	433.54	2024-02-25	2025-02-05
115	5	62	205.40	2024-05-24	2025-10-05
116	5	38	239.06	2025-04-16	\N
117	5	21	186.60	2024-06-25	\N
118	5	24	240.30	2024-08-29	2025-12-19
119	6	20	236.55	2024-04-08	\N
120	6	48	204.96	2024-05-16	\N
121	6	1	320.99	2024-10-16	2025-11-23
122	6	62	193.39	2025-02-07	\N
123	6	21	409.54	2025-08-23	\N
124	6	45	163.63	2024-07-01	2025-08-12
125	6	13	536.51	2025-05-28	2026-01-16
126	6	31	132.69	2024-08-23	\N
127	6	36	191.53	2024-08-04	\N
128	6	44	419.07	2025-01-28	\N
129	6	26	436.70	2024-10-18	\N
130	6	65	412.59	2025-05-07	\N
131	6	14	411.51	2025-03-07	\N
132	6	63	336.71	2025-06-13	2026-03-08
133	6	43	312.89	2024-12-20	\N
134	6	25	278.24	2025-02-18	2025-03-26
135	6	54	322.60	2024-05-11	\N
136	6	30	243.80	2025-04-10	\N
137	6	46	95.99	2025-02-15	\N
138	6	16	255.38	2025-03-11	\N
139	6	49	346.78	2025-04-06	\N
140	7	69	457.17	2024-10-15	\N
141	7	47	344.02	2025-04-16	\N
142	7	15	158.30	2025-01-13	\N
143	7	23	350.76	2024-10-30	2024-12-11
144	7	55	350.19	2024-07-02	\N
145	7	50	325.40	2025-04-02	\N
146	7	11	262.57	2024-04-24	\N
147	7	10	258.61	2025-02-09	\N
148	7	1	122.28	2024-04-06	\N
149	7	22	260.04	2025-05-20	\N
150	7	60	132.63	2025-04-14	\N
151	7	46	93.68	2025-06-20	\N
152	7	38	241.86	2024-11-30	\N
153	7	17	369.07	2024-04-13	\N
154	7	13	461.60	2024-03-21	\N
155	7	33	122.66	2024-11-05	\N
156	7	53	431.63	2024-09-20	2025-01-09
157	7	40	392.50	2025-03-19	2025-07-25
158	7	70	383.77	2024-11-06	\N
159	7	30	78.93	2024-09-06	\N
160	7	28	495.70	2025-08-10	\N
161	7	4	322.94	2025-03-23	\N
162	7	51	352.15	2024-07-09	\N
163	8	52	413.42	2024-10-21	2025-09-27
164	8	24	383.69	2025-08-25	\N
165	8	37	441.71	2025-07-08	\N
166	8	35	239.90	2024-01-13	\N
167	8	1	318.95	2025-03-31	\N
168	8	53	394.87	2025-05-25	\N
169	8	27	279.87	2025-02-03	\N
170	8	2	234.80	2025-08-08	2026-02-15
171	8	51	374.70	2024-12-19	\N
172	8	64	215.25	2025-06-26	\N
173	8	43	416.53	2024-11-18	\N
174	8	9	265.85	2024-06-08	\N
175	8	18	232.23	2024-12-26	\N
176	8	58	441.02	2024-01-24	2024-12-09
177	8	30	320.32	2025-05-17	2025-12-25
178	8	39	480.93	2024-05-14	\N
179	8	62	161.03	2024-05-06	\N
180	8	41	303.12	2024-04-23	\N
181	8	21	320.27	2024-06-11	2025-08-25
182	8	7	433.63	2024-12-26	2025-08-27
183	8	15	166.87	2024-10-12	\N
184	8	50	340.88	2024-04-18	\N
185	8	63	264.44	2024-11-21	\N
186	8	46	292.13	2024-08-18	2026-02-05
187	8	70	280.17	2024-10-20	2025-07-07
188	8	49	135.92	2024-06-30	\N
189	8	5	199.37	2024-12-08	2025-12-08
190	9	50	325.51	2024-09-12	\N
191	9	54	425.82	2024-04-03	2024-05-26
192	9	69	458.95	2025-02-02	\N
193	9	53	403.34	2024-06-14	\N
194	9	25	496.21	2024-09-02	\N
195	9	4	217.07	2024-07-23	\N
196	9	29	433.32	2024-02-01	\N
197	9	9	343.50	2024-09-07	\N
198	9	40	376.06	2024-09-04	2026-01-19
199	9	62	342.32	2024-04-23	2025-04-27
200	9	33	318.06	2024-06-12	\N
201	9	6	179.39	2024-03-12	\N
202	9	39	458.60	2025-02-20	2025-11-17
203	9	37	430.42	2024-03-13	2025-01-03
204	9	47	316.06	2025-07-04	\N
205	9	27	404.72	2025-06-30	2025-11-19
206	9	2	212.06	2024-03-02	\N
207	9	21	367.57	2024-02-10	\N
208	9	61	206.51	2025-06-07	\N
209	10	14	503.00	2024-03-16	\N
210	10	19	219.21	2024-09-07	2025-09-22
211	10	34	261.85	2025-03-31	\N
212	10	15	233.47	2024-09-21	\N
213	10	22	268.32	2024-05-23	\N
214	10	47	300.30	2024-08-14	2025-09-19
215	10	53	362.10	2024-11-10	2025-03-01
216	10	43	411.96	2024-11-08	\N
217	10	66	393.43	2024-10-28	\N
218	10	10	320.39	2024-05-11	\N
219	10	59	370.68	2025-01-28	\N
220	10	16	127.80	2024-08-04	\N
221	10	51	370.31	2024-01-13	\N
222	10	31	257.08	2025-06-13	\N
223	10	70	401.46	2024-06-23	2024-12-29
224	10	17	206.95	2025-05-28	\N
225	10	7	428.47	2025-05-11	\N
226	10	62	350.71	2025-05-08	\N
227	10	42	291.81	2024-01-15	\N
228	11	40	250.53	2024-07-26	\N
229	11	27	293.98	2024-04-11	2025-10-12
230	11	29	513.35	2025-07-20	2026-03-23
231	11	20	279.32	2024-03-22	2024-06-17
232	11	36	243.92	2025-06-19	\N
233	11	16	267.47	2024-07-06	\N
234	11	26	455.78	2024-09-07	\N
235	11	23	375.01	2024-06-30	\N
236	11	32	309.86	2025-07-19	\N
237	11	30	215.20	2024-03-06	2024-08-18
238	11	37	257.58	2024-03-03	\N
239	11	59	336.52	2025-08-18	\N
240	11	41	432.82	2024-02-22	\N
241	11	1	210.02	2024-07-20	\N
242	11	54	373.65	2025-05-02	\N
243	11	48	204.99	2024-08-08	\N
244	11	53	444.31	2024-01-10	\N
245	11	51	387.28	2024-04-24	2025-08-20
246	11	56	433.11	2024-07-18	\N
247	11	34	347.00	2025-04-12	2025-09-17
248	11	15	120.05	2024-10-04	\N
249	12	62	133.01	2025-03-18	\N
250	12	66	363.91	2025-03-29	\N
251	12	37	221.90	2024-07-06	\N
252	12	39	363.99	2024-08-25	\N
253	12	34	169.30	2024-11-23	2025-08-21
254	12	32	264.03	2024-11-03	2025-03-20
255	12	44	363.77	2024-06-07	\N
256	12	18	291.78	2025-02-09	2025-05-30
257	12	64	283.03	2024-06-09	\N
258	12	36	391.91	2025-04-30	\N
259	12	33	196.29	2024-10-02	\N
260	12	20	242.21	2024-11-06	\N
261	12	41	383.67	2024-12-28	2025-07-07
262	12	29	520.74	2025-03-11	\N
263	12	8	224.21	2024-01-06	2025-03-24
264	12	49	362.48	2025-02-22	\N
265	12	12	502.10	2025-04-12	2025-05-21
266	12	65	355.08	2024-07-26	\N
267	12	1	178.51	2024-10-28	2024-12-09
268	12	21	328.99	2025-01-26	\N
269	12	60	194.03	2024-07-23	2026-01-19
270	12	31	156.86	2024-10-07	\N
271	12	53	372.06	2024-11-29	2026-01-02
272	12	11	449.15	2024-02-17	2025-04-21
273	12	54	497.41	2025-05-13	\N
274	12	5	185.90	2025-08-23	\N
275	13	44	465.59	2025-07-13	2025-10-17
276	13	36	215.13	2024-10-25	\N
277	13	48	321.14	2024-03-10	2026-03-26
278	13	57	527.40	2024-11-17	2025-06-19
279	13	37	244.14	2025-04-01	2025-08-18
280	13	15	174.11	2025-04-24	\N
281	13	61	285.12	2024-07-22	\N
282	13	9	483.99	2024-09-18	\N
283	13	67	306.46	2024-05-23	\N
284	13	68	439.21	2025-02-09	2025-04-04
285	13	5	262.21	2024-03-02	2025-09-07
286	13	21	185.32	2025-08-02	\N
287	13	50	376.79	2025-05-31	2025-12-14
288	13	47	200.28	2025-05-18	\N
289	13	27	303.84	2024-03-09	2025-11-07
290	13	53	254.01	2025-07-25	2026-01-07
291	13	10	402.33	2025-06-09	2025-08-22
292	13	54	500.56	2025-04-24	\N
293	13	38	435.38	2024-03-15	2025-06-29
294	13	20	401.65	2025-04-21	\N
295	13	66	196.58	2024-06-04	\N
296	13	13	440.62	2025-08-12	\N
297	13	69	385.55	2025-03-18	2025-11-08
298	13	6	283.74	2024-01-07	\N
299	13	29	429.97	2024-01-09	\N
300	13	16	237.90	2024-09-22	2025-09-21
301	13	59	558.14	2024-07-24	2025-08-21
302	13	58	508.07	2025-08-13	\N
303	14	40	283.76	2024-08-10	2024-10-06
304	14	24	489.70	2025-03-26	\N
305	14	13	501.58	2024-01-27	\N
306	14	4	266.87	2024-08-01	\N
307	14	6	390.02	2025-02-12	\N
308	14	17	376.54	2025-08-27	2025-10-19
309	14	45	109.47	2025-06-02	2026-03-23
310	14	62	244.50	2024-08-02	\N
311	14	5	275.16	2025-01-01	2025-07-12
312	14	21	276.96	2024-08-03	2026-02-03
313	14	56	522.85	2024-09-18	\N
314	14	61	313.94	2024-01-17	\N
315	14	15	173.54	2025-02-13	2025-05-05
316	14	43	557.13	2024-11-13	2025-07-23
317	14	14	506.83	2024-11-03	\N
318	14	68	473.61	2025-02-03	2025-03-13
319	14	20	249.90	2024-10-08	2026-02-09
320	14	11	418.26	2024-05-24	2024-12-21
321	14	28	335.56	2024-05-11	\N
322	14	18	290.96	2025-04-26	2025-06-26
323	14	58	399.22	2024-08-02	2025-06-28
324	15	21	308.89	2024-12-05	\N
325	15	54	378.86	2024-05-27	2024-08-12
326	15	23	241.03	2025-06-21	\N
327	15	68	328.97	2024-12-23	2025-03-10
328	15	20	293.31	2024-03-08	\N
329	15	24	351.61	2025-03-01	2025-12-30
330	15	47	240.61	2024-01-01	\N
331	15	57	462.00	2024-09-20	\N
332	15	60	274.49	2025-01-09	2026-01-16
333	15	8	442.05	2025-05-04	\N
334	15	51	239.81	2025-08-07	\N
335	15	11	326.53	2024-05-12	2024-11-04
336	15	2	189.13	2024-03-25	2025-06-11
337	15	32	180.43	2024-04-17	\N
338	15	14	583.34	2024-11-04	\N
339	15	66	338.22	2025-08-27	\N
340	15	26	354.68	2024-09-30	2025-03-13
341	15	41	423.83	2025-01-18	\N
342	15	65	336.27	2024-02-05	2025-04-04
343	15	17	150.81	2024-07-21	2024-09-03
344	15	40	292.02	2024-02-04	2024-11-03
345	15	70	379.71	2024-11-26	\N
346	15	58	537.62	2024-12-18	\N
347	15	48	401.49	2025-08-14	\N
348	16	34	153.99	2025-01-09	\N
349	16	57	345.46	2025-07-10	\N
350	16	16	247.38	2025-04-13	2025-07-13
351	16	36	276.60	2024-10-29	\N
352	16	66	451.69	2025-04-08	\N
353	16	44	367.22	2024-03-25	2025-09-22
354	16	65	395.00	2025-06-07	\N
355	16	58	501.69	2024-08-15	2025-04-05
356	16	12	497.48	2024-12-28	\N
357	16	17	125.92	2025-07-30	2026-01-10
358	16	22	261.37	2025-06-29	\N
359	16	52	241.66	2025-06-27	\N
360	16	56	502.21	2024-06-18	\N
361	16	32	261.73	2024-08-19	2025-02-20
362	16	50	177.45	2025-03-10	2025-08-29
363	16	67	445.76	2025-02-14	\N
364	16	30	167.53	2025-03-07	2025-12-01
365	16	43	369.12	2024-11-03	\N
366	16	7	255.41	2025-07-23	\N
367	16	53	422.67	2025-08-31	2025-10-15
368	16	60	305.17	2024-05-23	\N
369	16	49	190.85	2024-09-05	\N
370	16	70	302.18	2025-03-05	\N
371	16	61	360.92	2024-12-24	\N
372	16	5	274.40	2024-10-09	\N
373	17	20	270.41	2024-12-15	2025-02-03
374	17	27	412.79	2024-09-18	\N
375	17	57	479.68	2025-08-22	\N
376	17	19	292.28	2025-05-13	2025-10-05
377	17	36	446.43	2024-03-31	2026-01-07
378	17	42	518.24	2025-01-06	\N
379	17	2	230.46	2025-05-21	2026-01-27
380	17	41	499.14	2024-04-17	2025-02-27
381	17	8	356.15	2024-10-09	2025-10-01
382	17	34	217.59	2025-03-05	\N
383	17	70	278.31	2024-12-31	\N
384	17	61	233.62	2025-05-12	\N
385	17	7	272.58	2025-08-28	\N
386	17	50	395.79	2025-05-14	\N
387	17	63	370.86	2025-01-19	\N
388	17	39	263.14	2025-01-26	\N
389	17	31	129.81	2024-04-10	\N
390	17	15	265.47	2024-09-01	\N
391	17	43	397.01	2025-03-07	2025-04-26
392	17	29	555.94	2025-03-26	2026-02-12
393	17	60	285.21	2024-02-08	2025-12-12
394	17	62	121.55	2025-03-18	\N
395	17	35	338.89	2025-08-03	\N
396	17	40	503.68	2024-12-02	\N
397	17	30	179.11	2024-01-30	2024-11-14
398	17	10	516.04	2024-12-18	\N
399	17	5	178.64	2024-10-10	\N
400	18	34	325.86	2024-11-16	\N
401	18	21	419.39	2025-01-26	\N
402	18	27	507.62	2025-05-12	\N
403	18	42	374.76	2025-01-05	2025-05-01
404	18	52	286.40	2024-04-24	\N
405	18	45	290.11	2025-06-02	2025-11-30
406	18	23	388.08	2025-07-05	\N
407	18	55	276.90	2024-03-18	\N
408	18	32	312.87	2025-02-18	2025-06-02
409	18	63	169.58	2025-06-29	\N
410	18	6	446.92	2025-01-29	\N
411	18	22	467.95	2024-10-27	\N
412	18	64	377.19	2024-11-27	2025-08-10
413	18	40	435.57	2024-09-08	\N
414	18	35	228.73	2024-10-07	\N
415	18	48	195.10	2024-08-12	\N
416	18	66	265.62	2024-08-09	\N
417	18	15	312.43	2024-01-22	2025-06-29
418	18	69	509.55	2025-02-28	\N
419	18	20	400.17	2024-07-15	\N
420	18	7	359.25	2025-08-17	\N
421	18	47	335.12	2025-01-31	2026-02-26
422	18	60	200.61	2025-01-07	2025-06-20
423	18	41	357.04	2025-01-08	2025-06-07
424	18	26	459.63	2024-12-31	\N
425	19	32	254.21	2025-04-16	\N
426	19	69	510.34	2025-03-22	\N
427	19	17	271.22	2024-01-23	\N
428	19	13	543.08	2024-02-28	2025-07-30
429	19	65	265.96	2024-05-31	\N
430	19	61	361.02	2024-10-09	\N
431	19	41	388.98	2024-01-29	2025-07-14
432	19	7	384.50	2024-05-16	2025-01-21
433	19	52	388.03	2024-11-13	\N
434	19	53	439.06	2024-12-04	\N
435	19	39	447.23	2025-06-04	2025-09-15
436	19	15	85.36	2025-08-23	\N
437	19	47	279.05	2024-02-24	2025-03-26
438	19	23	365.19	2024-03-13	\N
439	19	46	369.94	2024-06-14	2025-03-25
440	19	45	262.09	2024-07-02	2024-08-31
441	19	42	312.00	2025-05-21	2025-12-06
442	19	36	446.48	2025-02-17	\N
443	19	59	513.01	2025-01-31	2025-09-13
444	19	30	109.86	2024-05-21	2024-12-24
445	19	18	252.85	2024-12-29	\N
446	19	37	253.26	2024-09-15	\N
447	19	4	248.08	2025-02-15	\N
448	20	11	332.05	2025-05-25	\N
449	20	44	510.87	2024-12-19	\N
450	20	58	463.85	2024-06-07	\N
451	20	13	445.52	2025-03-27	2025-07-25
452	20	33	146.66	2025-03-31	\N
453	20	3	394.69	2025-07-04	2026-02-22
454	20	8	316.99	2024-11-15	2025-02-08
455	20	63	182.03	2024-07-03	\N
456	20	36	352.96	2024-04-12	\N
457	20	4	303.69	2024-12-11	2025-12-13
458	20	69	421.89	2025-02-02	2025-05-02
459	20	24	338.57	2024-03-08	\N
460	20	7	255.79	2024-09-02	2026-02-16
461	20	70	271.12	2025-02-17	\N
462	20	1	335.14	2025-05-06	2025-07-25
463	20	34	241.15	2024-04-03	\N
464	20	56	465.11	2025-06-23	\N
465	20	54	388.25	2025-06-18	\N
466	20	21	224.26	2024-07-24	\N
467	20	25	427.24	2024-07-21	\N
468	20	20	192.66	2025-02-17	\N
469	20	2	207.42	2024-01-25	\N
470	20	37	288.12	2025-05-10	\N
471	20	48	151.16	2024-10-21	2025-11-12
472	20	59	347.53	2024-02-12	\N
473	20	5	319.27	2025-08-24	2026-02-16
474	20	12	296.69	2024-11-17	2025-09-16
475	20	67	300.74	2024-11-06	\N
476	21	7	395.08	2025-01-25	\N
477	21	56	339.61	2024-10-24	\N
478	21	66	415.81	2024-12-04	2025-07-21
479	21	23	234.81	2025-01-15	2025-09-07
480	21	68	393.49	2024-05-25	\N
481	21	48	324.61	2024-09-21	2025-12-27
482	21	6	430.14	2024-11-13	\N
483	21	59	327.31	2024-10-06	\N
484	21	14	556.22	2025-02-27	\N
485	21	70	326.19	2024-07-13	2025-12-24
486	21	30	195.25	2024-01-11	\N
487	21	12	480.11	2025-04-04	\N
488	21	58	339.20	2024-03-13	\N
489	21	15	234.16	2025-05-15	\N
490	21	46	366.21	2024-06-20	2025-04-08
491	21	21	267.27	2025-02-11	\N
492	21	5	380.77	2025-08-06	\N
493	21	69	500.81	2025-02-08	2025-06-21
494	21	62	155.46	2025-07-22	2025-10-11
495	21	32	150.57	2024-02-06	\N
496	21	52	312.22	2024-08-22	\N
497	21	37	271.98	2024-03-26	2024-07-03
498	21	41	312.76	2025-01-14	\N
499	21	40	395.00	2024-02-12	\N
500	21	1	115.53	2024-07-12	2025-07-02
501	21	3	373.86	2024-01-15	2025-01-28
502	22	23	369.35	2024-03-12	2025-12-18
503	22	45	238.46	2025-07-24	\N
504	22	60	276.31	2024-11-21	\N
505	22	26	279.80	2025-02-23	\N
506	22	8	325.85	2024-06-22	2025-06-16
507	22	51	191.62	2024-05-26	2025-02-19
508	22	49	379.77	2024-01-04	\N
509	22	10	319.74	2025-08-03	\N
510	22	61	125.33	2024-02-06	\N
511	22	19	233.68	2025-05-02	2025-12-15
512	22	33	344.72	2025-01-31	\N
513	22	5	267.92	2025-05-17	\N
514	22	15	335.11	2025-04-10	\N
515	22	12	391.04	2025-04-10	\N
516	22	41	476.02	2024-05-27	\N
517	22	13	410.22	2025-02-01	2025-05-24
518	22	22	289.46	2024-01-22	\N
519	22	43	504.44	2024-02-13	\N
520	22	47	156.38	2025-07-08	2025-11-17
521	22	29	437.59	2024-12-27	\N
522	22	2	246.39	2025-03-03	\N
523	22	65	265.90	2025-02-01	\N
524	23	52	332.89	2024-04-21	\N
525	23	18	200.22	2025-02-26	\N
526	23	68	331.18	2025-07-18	2026-02-01
527	23	27	552.88	2025-06-13	\N
528	23	32	229.64	2024-01-25	\N
529	23	28	515.64	2024-06-25	\N
530	23	25	370.02	2025-07-30	2026-02-09
531	23	37	295.53	2024-07-21	\N
532	23	13	520.22	2024-09-08	2025-02-18
533	23	61	363.08	2025-05-13	\N
534	23	56	286.21	2025-06-27	2026-03-08
535	23	47	129.40	2024-04-09	2024-08-04
536	23	43	335.24	2024-02-24	\N
537	23	51	232.92	2024-12-31	\N
538	23	40	277.66	2024-08-06	\N
539	23	24	470.27	2024-11-19	\N
540	23	30	137.68	2025-05-01	\N
541	23	36	346.02	2024-11-06	\N
542	23	49	190.07	2024-06-04	\N
543	23	57	445.39	2025-02-23	\N
544	23	67	285.84	2025-04-25	\N
545	23	55	509.53	2024-09-12	2025-03-11
546	23	42	535.56	2024-04-09	2024-09-24
547	23	20	221.38	2025-04-29	\N
548	23	14	599.14	2024-01-21	\N
549	23	26	433.78	2025-01-06	2025-12-01
550	23	10	519.82	2025-04-03	2026-01-09
551	23	31	199.94	2025-08-03	\N
552	24	48	361.81	2024-12-29	\N
553	24	9	406.34	2024-01-17	\N
554	24	23	296.50	2024-06-22	\N
555	24	18	285.12	2025-06-09	\N
556	24	39	493.10	2024-01-25	\N
557	24	51	385.64	2024-02-23	\N
558	24	20	295.06	2024-09-26	\N
559	24	8	500.65	2024-02-11	\N
560	24	60	182.55	2025-04-14	\N
561	24	65	445.02	2024-02-02	\N
562	24	43	395.25	2024-01-11	2024-02-29
563	24	13	418.81	2024-07-23	2025-09-15
564	24	41	370.30	2024-10-03	\N
565	24	21	304.98	2025-06-08	2026-02-10
566	24	19	254.22	2024-12-16	\N
567	24	46	242.43	2024-09-24	\N
568	24	61	231.72	2024-09-30	\N
569	24	54	480.91	2024-03-27	2025-03-18
570	24	6	228.34	2024-04-26	\N
571	25	32	392.05	2024-06-08	\N
572	25	10	353.50	2025-04-16	\N
573	25	23	243.95	2024-04-18	\N
574	25	48	348.67	2024-05-22	\N
575	25	9	443.02	2024-03-24	\N
576	25	20	395.19	2024-04-03	\N
577	25	31	306.67	2024-08-26	\N
578	25	50	323.07	2025-01-31	\N
579	25	6	361.00	2025-05-02	2026-02-09
580	25	56	480.68	2024-07-09	\N
581	25	5	299.19	2025-08-29	2025-10-13
582	25	1	157.86	2025-06-03	\N
583	25	47	368.00	2024-07-21	\N
584	25	59	364.60	2025-08-12	2026-03-09
585	25	67	444.87	2024-03-18	\N
586	25	52	386.21	2024-08-21	\N
587	25	22	324.88	2025-08-29	\N
588	25	40	331.51	2024-10-19	\N
589	25	60	94.42	2024-12-19	2025-10-16
\.


--
-- TOC entry 3682 (class 0 OID 24616)
-- Dependencies: 225
-- Data for Name: unit; Type: TABLE DATA; Schema: farm_association; Owner: admin
--

COPY public.unit (id, code, name) FROM stdin;
1	kg	Килограмм
2	t	Тонна
3	l	Литр
4	pcs	Штука
5	box	Коробка
6	bag	Мешок
7	m	Метр
8	m2	Квадратный метр
9	ha	Гектар
10	head	Голова
11	doz	Дюжина
12	pack	Упаковка
\.


--
-- TOC entry 3730 (class 0 OID 0)
-- Dependencies: 255
-- Name: association_employee_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.association_employee_id_seq', 35, true);


--
-- TOC entry 3731 (class 0 OID 0)
-- Dependencies: 250
-- Name: association_member_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.association_member_id_seq', 180, true);


--
-- TOC entry 3732 (class 0 OID 0)
-- Dependencies: 259
-- Name: contract_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.contract_id_seq', 140, true);


--
-- TOC entry 3733 (class 0 OID 0)
-- Dependencies: 256
-- Name: employee_plot_assignment_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.employee_plot_assignment_id_seq', 558, true);


--
-- TOC entry 3734 (class 0 OID 0)
-- Dependencies: 246
-- Name: farm_association_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_association_id_seq', 8, true);


--
-- TOC entry 3735 (class 0 OID 0)
-- Dependencies: 254
-- Name: farm_employee_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_employee_id_seq', 320, true);


--
-- TOC entry 3736 (class 0 OID 0)
-- Dependencies: 247
-- Name: farm_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_id_seq', 60, true);


--
-- TOC entry 3737 (class 0 OID 0)
-- Dependencies: 266
-- Name: farm_inspection_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_inspection_id_seq', 1, false);


--
-- TOC entry 3738 (class 0 OID 0)
-- Dependencies: 251
-- Name: farm_owner_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_owner_id_seq', 80, true);


--
-- TOC entry 3739 (class 0 OID 0)
-- Dependencies: 257
-- Name: farm_ownership_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_ownership_id_seq', 123, true);


--
-- TOC entry 3740 (class 0 OID 0)
-- Dependencies: 253
-- Name: farm_plot_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.farm_plot_id_seq', 220, true);


--
-- TOC entry 3741 (class 0 OID 0)
-- Dependencies: 265
-- Name: inspection_result_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.inspection_result_id_seq', 1, false);


--
-- TOC entry 3742 (class 0 OID 0)
-- Dependencies: 264
-- Name: inspection_type_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.inspection_type_id_seq', 1, false);


--
-- TOC entry 3743 (class 0 OID 0)
-- Dependencies: 249
-- Name: person_document_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.person_document_id_seq', 300, true);


--
-- TOC entry 3744 (class 0 OID 0)
-- Dependencies: 245
-- Name: person_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.person_id_seq', 250, true);


--
-- TOC entry 3745 (class 0 OID 0)
-- Dependencies: 252
-- Name: product_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.product_id_seq', 70, true);


--
-- TOC entry 3746 (class 0 OID 0)
-- Dependencies: 260
-- Name: purchase_order_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.purchase_order_id_seq', 220, true);


--
-- TOC entry 3747 (class 0 OID 0)
-- Dependencies: 261
-- Name: purchase_order_item_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.purchase_order_item_id_seq', 888, true);


--
-- TOC entry 3748 (class 0 OID 0)
-- Dependencies: 262
-- Name: purchase_requisition_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.purchase_requisition_id_seq', 450, true);


--
-- TOC entry 3749 (class 0 OID 0)
-- Dependencies: 263
-- Name: sales_requisition_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.sales_requisition_id_seq', 420, true);


--
-- TOC entry 3750 (class 0 OID 0)
-- Dependencies: 248
-- Name: supplier_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.supplier_id_seq', 25, true);


--
-- TOC entry 3751 (class 0 OID 0)
-- Dependencies: 258
-- Name: supplier_product_price_id_seq; Type: SEQUENCE SET; Schema: farm_association; Owner: admin
--

SELECT pg_catalog.setval('public.supplier_product_price_id_seq', 589, true);


--
-- TOC entry 3448 (class 2606 OID 25596)
-- Name: association_employee association_employee_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_employee
    ADD CONSTRAINT association_employee_pkey PRIMARY KEY (id);


--
-- TOC entry 3452 (class 2606 OID 25594)
-- Name: association_member association_member_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_member
    ADD CONSTRAINT association_member_pkey PRIMARY KEY (id);


--
-- TOC entry 3424 (class 2606 OID 25060)
-- Name: association_role association_role_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_role
    ADD CONSTRAINT association_role_pkey PRIMARY KEY (id);


--
-- TOC entry 3454 (class 2606 OID 25612)
-- Name: contract contract_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.contract
    ADD CONSTRAINT contract_pkey PRIMARY KEY (id);


--
-- TOC entry 3480 (class 2606 OID 25598)
-- Name: employee_plot_assignment employee_plot_assignment_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.employee_plot_assignment
    ADD CONSTRAINT employee_plot_assignment_pkey PRIMARY KEY (id);


--
-- TOC entry 3426 (class 2606 OID 25074)
-- Name: employment_status employment_status_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.employment_status
    ADD CONSTRAINT employment_status_pkey PRIMARY KEY (id);


--
-- TOC entry 3430 (class 2606 OID 25154)
-- Name: farm_association farm_association_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_association
    ADD CONSTRAINT farm_association_pkey PRIMARY KEY (id);


--
-- TOC entry 3456 (class 2606 OID 25348)
-- Name: farm_employee farm_employee_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_employee
    ADD CONSTRAINT farm_employee_pkey PRIMARY KEY (id);


--
-- TOC entry 3458 (class 2606 OID 25264)
-- Name: farm_owner farm_owner_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_owner
    ADD CONSTRAINT farm_owner_pkey PRIMARY KEY (id);


--
-- TOC entry 3460 (class 2606 OID 25604)
-- Name: farm_ownership farm_ownership_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_ownership
    ADD CONSTRAINT farm_ownership_pkey PRIMARY KEY (id);


--
-- TOC entry 3428 (class 2606 OID 25182)
-- Name: farm farm_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm
    ADD CONSTRAINT farm_pkey PRIMARY KEY (id);


--
-- TOC entry 3462 (class 2606 OID 25320)
-- Name: farm_plot farm_plot_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot
    ADD CONSTRAINT farm_plot_pkey PRIMARY KEY (id);


--
-- TOC entry 3432 (class 2606 OID 25103)
-- Name: farm_plot_type farm_plot_type_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_type
    ADD CONSTRAINT farm_plot_type_pkey PRIMARY KEY (id);


--
-- TOC entry 3434 (class 2606 OID 25067)
-- Name: farm_role farm_role_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_role
    ADD CONSTRAINT farm_role_pkey PRIMARY KEY (id);


--
-- TOC entry 3436 (class 2606 OID 25053)
-- Name: identity_document_type identity_document_type_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.identity_document_type
    ADD CONSTRAINT identity_document_type_pkey PRIMARY KEY (id);


--
-- TOC entry 3466 (class 2606 OID 25592)
-- Name: person_document person_document_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.person_document
    ADD CONSTRAINT person_document_pkey PRIMARY KEY (id);


--
-- TOC entry 3438 (class 2606 OID 25121)
-- Name: person person_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.person
    ADD CONSTRAINT person_pkey PRIMARY KEY (id);


--
-- TOC entry 3450 (class 2606 OID 25602)
-- Name: association_farms pk_association_farm; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_farms
    ADD CONSTRAINT pk_association_farm PRIMARY KEY (farm_id, association_id);


--
-- TOC entry 3482 (class 2606 OID 25608)
-- Name: farm_plot_consumption_product pk_consumption; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_consumption_product
    ADD CONSTRAINT pk_consumption PRIMARY KEY (product_id, farm_plot_id);


--
-- TOC entry 3464 (class 2606 OID 25600)
-- Name: farm_plot_assignment pk_farm_plot; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_assignment
    ADD CONSTRAINT pk_farm_plot PRIMARY KEY (farm_id, farm_plot_id);


--
-- TOC entry 3484 (class 2606 OID 25606)
-- Name: farm_plot_production_product pk_production; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_production_product
    ADD CONSTRAINT pk_production PRIMARY KEY (product_id, farm_plot_id);


--
-- TOC entry 3468 (class 2606 OID 25277)
-- Name: product product_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.product
    ADD CONSTRAINT product_pkey PRIMARY KEY (id);


--
-- TOC entry 3440 (class 2606 OID 25086)
-- Name: product_type product_type_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.product_type
    ADD CONSTRAINT product_type_pkey PRIMARY KEY (id);


--
-- TOC entry 3442 (class 2606 OID 25620)
-- Name: product_type product_type_sku_key; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.product_type
    ADD CONSTRAINT product_type_sku_key UNIQUE (sku);


--
-- TOC entry 3472 (class 2606 OID 25614)
-- Name: purchase_order_item purchase_order_item_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order_item
    ADD CONSTRAINT purchase_order_item_pkey PRIMARY KEY (id);


--
-- TOC entry 3470 (class 2606 OID 25512)
-- Name: purchase_order purchase_order_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order
    ADD CONSTRAINT purchase_order_pkey PRIMARY KEY (id);


--
-- TOC entry 3474 (class 2606 OID 25616)
-- Name: purchase_requisition purchase_requisition_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_requisition
    ADD CONSTRAINT purchase_requisition_pkey PRIMARY KEY (id);


--
-- TOC entry 3476 (class 2606 OID 25618)
-- Name: sales_requisition sales_requisition_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.sales_requisition
    ADD CONSTRAINT sales_requisition_pkey PRIMARY KEY (id);


--
-- TOC entry 3444 (class 2606 OID 25220)
-- Name: supplier supplier_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.supplier
    ADD CONSTRAINT supplier_pkey PRIMARY KEY (id);


--
-- TOC entry 3478 (class 2606 OID 25610)
-- Name: supplier_product_price supplier_product_price_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.supplier_product_price
    ADD CONSTRAINT supplier_product_price_pkey PRIMARY KEY (id);


--
-- TOC entry 3446 (class 2606 OID 25046)
-- Name: unit unit_pkey; Type: CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.unit
    ADD CONSTRAINT unit_pkey PRIMARY KEY (id);


--
-- TOC entry 3523 (class 2606 OID 25711)
-- Name: employee_plot_assignment fk_assignment_employee; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.employee_plot_assignment
    ADD CONSTRAINT fk_assignment_employee FOREIGN KEY (farm_employee_id) REFERENCES public.farm_employee(id) ON DELETE CASCADE;


--
-- TOC entry 3524 (class 2606 OID 25716)
-- Name: employee_plot_assignment fk_assignment_plot; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.employee_plot_assignment
    ADD CONSTRAINT fk_assignment_plot FOREIGN KEY (farm_plot_id) REFERENCES public.farm_plot(id) ON DELETE CASCADE;


--
-- TOC entry 3487 (class 2606 OID 25696)
-- Name: association_employee fk_assoc_emp_association; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_employee
    ADD CONSTRAINT fk_assoc_emp_association FOREIGN KEY (association_id) REFERENCES public.farm_association(id) ON DELETE CASCADE;


--
-- TOC entry 3488 (class 2606 OID 25691)
-- Name: association_employee fk_assoc_emp_person; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_employee
    ADD CONSTRAINT fk_assoc_emp_person FOREIGN KEY (person_id) REFERENCES public.person(id);


--
-- TOC entry 3489 (class 2606 OID 25701)
-- Name: association_employee fk_assoc_emp_role; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_employee
    ADD CONSTRAINT fk_assoc_emp_role FOREIGN KEY (role_id) REFERENCES public.association_role(id);


--
-- TOC entry 3490 (class 2606 OID 25706)
-- Name: association_employee fk_assoc_emp_status; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_employee
    ADD CONSTRAINT fk_assoc_emp_status FOREIGN KEY (employment_status_id) REFERENCES public.employment_status(id);


--
-- TOC entry 3491 (class 2606 OID 25731)
-- Name: association_farms fk_association_af; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_farms
    ADD CONSTRAINT fk_association_af FOREIGN KEY (association_id) REFERENCES public.farm_association(id) ON DELETE CASCADE;


--
-- TOC entry 3512 (class 2606 OID 25796)
-- Name: purchase_order fk_association_order; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order
    ADD CONSTRAINT fk_association_order FOREIGN KEY (association_id) REFERENCES public.farm_association(id);


--
-- TOC entry 3525 (class 2606 OID 25766)
-- Name: farm_plot_consumption_product fk_consumption_plot; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_consumption_product
    ADD CONSTRAINT fk_consumption_plot FOREIGN KEY (farm_plot_id) REFERENCES public.farm_plot(id) ON DELETE CASCADE;


--
-- TOC entry 3526 (class 2606 OID 25761)
-- Name: farm_plot_consumption_product fk_consumption_product; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_consumption_product
    ADD CONSTRAINT fk_consumption_product FOREIGN KEY (product_id) REFERENCES public.product(id) ON DELETE CASCADE;


--
-- TOC entry 3495 (class 2606 OID 25791)
-- Name: contract fk_contract_association; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.contract
    ADD CONSTRAINT fk_contract_association FOREIGN KEY (association_id) REFERENCES public.farm_association(id) ON DELETE CASCADE;


--
-- TOC entry 3496 (class 2606 OID 25786)
-- Name: contract fk_contract_farm; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.contract
    ADD CONSTRAINT fk_contract_farm FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE SET NULL;


--
-- TOC entry 3497 (class 2606 OID 25781)
-- Name: contract fk_contract_supplier; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.contract
    ADD CONSTRAINT fk_contract_supplier FOREIGN KEY (supplier_id) REFERENCES public.supplier(id) ON DELETE SET NULL;


--
-- TOC entry 3498 (class 2606 OID 25676)
-- Name: farm_employee fk_employee_farm; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_employee
    ADD CONSTRAINT fk_employee_farm FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE CASCADE;


--
-- TOC entry 3499 (class 2606 OID 25671)
-- Name: farm_employee fk_employee_person; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_employee
    ADD CONSTRAINT fk_employee_person FOREIGN KEY (person_id) REFERENCES public.person(id);


--
-- TOC entry 3500 (class 2606 OID 25681)
-- Name: farm_employee fk_employee_role; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_employee
    ADD CONSTRAINT fk_employee_role FOREIGN KEY (role_id) REFERENCES public.farm_role(id);


--
-- TOC entry 3501 (class 2606 OID 25686)
-- Name: farm_employee fk_employee_status; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_employee
    ADD CONSTRAINT fk_employee_status FOREIGN KEY (employment_status_id) REFERENCES public.employment_status(id);


--
-- TOC entry 3492 (class 2606 OID 25736)
-- Name: association_farms fk_farm_af; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_farms
    ADD CONSTRAINT fk_farm_af FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE CASCADE;


--
-- TOC entry 3506 (class 2606 OID 25721)
-- Name: farm_plot_assignment fk_farm_plot_farm; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_assignment
    ADD CONSTRAINT fk_farm_plot_farm FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE CASCADE;


--
-- TOC entry 3507 (class 2606 OID 25726)
-- Name: farm_plot_assignment fk_farm_plot_plot; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_assignment
    ADD CONSTRAINT fk_farm_plot_plot FOREIGN KEY (farm_plot_id) REFERENCES public.farm_plot(id) ON DELETE CASCADE;


--
-- TOC entry 3505 (class 2606 OID 25666)
-- Name: farm_plot fk_farm_plot_type; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot
    ADD CONSTRAINT fk_farm_plot_type FOREIGN KEY (farm_plot_type_id) REFERENCES public.farm_plot_type(id);


--
-- TOC entry 3485 (class 2606 OID 25626)
-- Name: farm_plot_type fk_farm_plot_type_parent; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_type
    ADD CONSTRAINT fk_farm_plot_type_parent FOREIGN KEY (parent_id) REFERENCES public.farm_plot_type(id);


--
-- TOC entry 3493 (class 2606 OID 25641)
-- Name: association_member fk_member_association; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_member
    ADD CONSTRAINT fk_member_association FOREIGN KEY (association_id) REFERENCES public.farm_association(id) ON DELETE CASCADE;


--
-- TOC entry 3494 (class 2606 OID 25646)
-- Name: association_member fk_member_person; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.association_member
    ADD CONSTRAINT fk_member_person FOREIGN KEY (person_id) REFERENCES public.person(id);


--
-- TOC entry 3513 (class 2606 OID 25806)
-- Name: purchase_order fk_order_creator; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order
    ADD CONSTRAINT fk_order_creator FOREIGN KEY (created_by) REFERENCES public.person(id) ON DELETE SET NULL;


--
-- TOC entry 3502 (class 2606 OID 25651)
-- Name: farm_owner fk_owner_person; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_owner
    ADD CONSTRAINT fk_owner_person FOREIGN KEY (person_id) REFERENCES public.person(id);


--
-- TOC entry 3503 (class 2606 OID 25741)
-- Name: farm_ownership fk_ownership_farm; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_ownership
    ADD CONSTRAINT fk_ownership_farm FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE CASCADE;


--
-- TOC entry 3504 (class 2606 OID 25746)
-- Name: farm_ownership fk_ownership_owner; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_ownership
    ADD CONSTRAINT fk_ownership_owner FOREIGN KEY (farm_owner_id) REFERENCES public.farm_owner(id);


--
-- TOC entry 3508 (class 2606 OID 25631)
-- Name: person_document fk_person_document_person; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.person_document
    ADD CONSTRAINT fk_person_document_person FOREIGN KEY (person_id) REFERENCES public.person(id) ON DELETE CASCADE;


--
-- TOC entry 3509 (class 2606 OID 25636)
-- Name: person_document fk_person_document_type; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.person_document
    ADD CONSTRAINT fk_person_document_type FOREIGN KEY (document_type_id) REFERENCES public.identity_document_type(id);


--
-- TOC entry 3515 (class 2606 OID 25816)
-- Name: purchase_order_item fk_product_item; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order_item
    ADD CONSTRAINT fk_product_item FOREIGN KEY (product_id) REFERENCES public.product(id);


--
-- TOC entry 3521 (class 2606 OID 25776)
-- Name: supplier_product_price fk_product_spp; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.supplier_product_price
    ADD CONSTRAINT fk_product_spp FOREIGN KEY (product_id) REFERENCES public.product(id) ON DELETE CASCADE;


--
-- TOC entry 3510 (class 2606 OID 25656)
-- Name: product fk_product_type; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.product
    ADD CONSTRAINT fk_product_type FOREIGN KEY (type_id) REFERENCES public.product_type(id);


--
-- TOC entry 3486 (class 2606 OID 25621)
-- Name: product_type fk_product_type_parent; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.product_type
    ADD CONSTRAINT fk_product_type_parent FOREIGN KEY (parent_id) REFERENCES public.product_type(id);


--
-- TOC entry 3511 (class 2606 OID 25661)
-- Name: product fk_product_unit; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.product
    ADD CONSTRAINT fk_product_unit FOREIGN KEY (unit_id) REFERENCES public.unit(id) ON DELETE SET NULL;


--
-- TOC entry 3527 (class 2606 OID 25756)
-- Name: farm_plot_production_product fk_production_plot; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_production_product
    ADD CONSTRAINT fk_production_plot FOREIGN KEY (farm_plot_id) REFERENCES public.farm_plot(id) ON DELETE CASCADE;


--
-- TOC entry 3528 (class 2606 OID 25751)
-- Name: farm_plot_production_product fk_production_product; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.farm_plot_production_product
    ADD CONSTRAINT fk_production_product FOREIGN KEY (product_id) REFERENCES public.product(id) ON DELETE CASCADE;


--
-- TOC entry 3516 (class 2606 OID 25811)
-- Name: purchase_order_item fk_purchase_order_item; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order_item
    ADD CONSTRAINT fk_purchase_order_item FOREIGN KEY (purchase_order_id) REFERENCES public.purchase_order(id) ON DELETE CASCADE;


--
-- TOC entry 3517 (class 2606 OID 25821)
-- Name: purchase_requisition fk_requisition_farm; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_requisition
    ADD CONSTRAINT fk_requisition_farm FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE CASCADE;


--
-- TOC entry 3518 (class 2606 OID 25826)
-- Name: purchase_requisition fk_requisition_product; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_requisition
    ADD CONSTRAINT fk_requisition_product FOREIGN KEY (product_id) REFERENCES public.product(id);


--
-- TOC entry 3519 (class 2606 OID 25831)
-- Name: sales_requisition fk_sales_farm; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.sales_requisition
    ADD CONSTRAINT fk_sales_farm FOREIGN KEY (farm_id) REFERENCES public.farm(id) ON DELETE CASCADE;


--
-- TOC entry 3520 (class 2606 OID 25836)
-- Name: sales_requisition fk_sales_product; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.sales_requisition
    ADD CONSTRAINT fk_sales_product FOREIGN KEY (product_id) REFERENCES public.product(id);


--
-- TOC entry 3514 (class 2606 OID 25801)
-- Name: purchase_order fk_supplier_order; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.purchase_order
    ADD CONSTRAINT fk_supplier_order FOREIGN KEY (supplier_id) REFERENCES public.supplier(id);


--
-- TOC entry 3522 (class 2606 OID 25771)
-- Name: supplier_product_price fk_supplier_spp; Type: FK CONSTRAINT; Schema: farm_association; Owner: admin
--

ALTER TABLE ONLY public.supplier_product_price
    ADD CONSTRAINT fk_supplier_spp FOREIGN KEY (supplier_id) REFERENCES public.supplier(id) ON DELETE CASCADE;


-- Completed on 2026-04-21 22:04:27

--
-- PostgreSQL database dump complete
--

--
-- Database "postgres" dump
--

